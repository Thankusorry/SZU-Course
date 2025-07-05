package Chat;

import javax.swing.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;

public class Server {
    private int port = 5000;
    private DatagramSocket udpSocket;
    private byte[] buffer = new byte[1024];

    private Map<String, ClientInfo> clients = new ConcurrentHashMap<>();
    private List<String> onlineClient = new ArrayList<>();
    private JPanel panel1;
    private JList userList;
    private JList sysMessageList;
    private JTextField sysMessageFiled;
    private JButton send_Button;
    private DefaultListModel<String> userListModel;
    private DefaultListModel<String> sysMessageListModel;
    private Map<String, List<String>> groups = new ConcurrentHashMap<>();
    // 在Server类中添加以下成员变量
    public void start() throws Exception {
        JFrame frame = new JFrame("服务器");
        frame.setContentPane(this.panel1);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);

//        聊天信息、好友列表绑定模型
        userListModel = new DefaultListModel<>();
        userList.setModel(userListModel);
        sysMessageListModel = new DefaultListModel<>();
        sysMessageList.setModel(sysMessageListModel);

        udpSocket = new DatagramSocket(port);
        System.out.println("UDP Server started, listening on port " + port);


        while (true) {
            DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
            udpSocket.receive(packet);
            new Thread(new ClientHandler(packet)).start();
        }
    }

    class ClientInfo {
        InetAddress address;
        int port;
        long lastActive;

        public ClientInfo(InetAddress address, int port) {
            this.address = address;
            this.port = port;
            this.lastActive = System.currentTimeMillis();
        }
    }

    class ClientHandler implements Runnable {
        private DatagramPacket packet;
        private String account;

        ClientHandler(DatagramPacket packet) {
            this.packet = packet;
        }

        @Override
        public synchronized void run() {
            try {

                String message = new String(packet.getData(), 0, packet.getLength(), "UTF-8");
                System.out.println("Received: " + message);
                String[] parts = message.split("&", 5);
                int code = Integer.parseInt(parts[0]);
                account = parts[1];
                System.out.println("code="+code);
                System.out.println("account="+account);
                List<String> groupMembers = new ArrayList<>(); // 初始化为一个空列表
                String groupName = parts[1];
                groupMembers = groups.get(groupName);
                // 更新客户端地址信息
                clients.put(account, new ClientInfo(packet.getAddress(), packet.getPort()));

                switch (code) {
                    case 1111: // 登录
                        if (!onlineClient.contains(account)) {
                            onlineClient.add(account);
                            SwingUtilities.invokeLater(() -> userListModel.addElement(account));
                            broadcast("1111&" + account);
                        }
                        sendOnlineList(account);
                        break;

                    case 2333: // 广播消息
                        broadcast("5555&" + account + ";" + parts[2]);
                        break;

                    case 1333: // 私聊
                        String recipient = parts[2];
                        String privateMsg = parts[3];
                        sendTo(recipient, "0000&" + account + ";" + privateMsg);
                        break;

                    case 5555: // 创建群组
                        groupName = parts[1];
                        groups.putIfAbsent(groupName, new ArrayList<>());
                        account=parts[2];
                        groupMembers = groups.get(groupName);
                        groupMembers.add(account);
                        break;

                    case 6666: // 添加群成员
                        String addGroup = parts[1];
                        String newMember = parts[2];
                        if (groupMembers !=null){
                            groupMembers.add(newMember);
                            sendTo(newMember, "6666&" + addGroup);
                        }
                        break;

                    case 7777: // 移除群成员
                        String removeGroup = parts[1];
                        String removeMember = parts[2];
                        if (groupMembers != null) {
                            groupMembers.remove(removeMember);
                            sendTo(removeMember, "7777&" + removeGroup);
                        }
                        break;

                    case 8888: // 群组消息
                        String targetGroup = parts[1];
                        String groupMsg = parts[2];
                        groupMembers = groups.get(targetGroup);
                        if (groupMembers != null) {
                            //群组消息==逐个私聊
                            for (String member : groupMembers) {
                                sendTo(member, "8888&" + targetGroup + "&" + groupMsg);
                            }
                        }
                        break;

                    case 9999: // 主动退出
                        ClientInfo target = clients.get(account);
                        if(target!=null) {
                            sendUDP("exit", target.address, target.port);
                            clients.remove(account);
                            onlineClient.remove(account);
                            SwingUtilities.invokeLater(() -> {
                                userListModel.removeElement(account);
                                sysMessageListModel.addElement("系统检测到 " + account + " 下线");
                            });
                            broadcast("4444&" + account);
                        }
                        break;
                    default:
                        System.out.println("Unknown message type: " + code);
                        break;
                }
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                // 不自动移除，依靠心跳检测
            }
        }
    }



    // 消息发送方法
    private void sendUDP(String message, InetAddress address, int port) {
        try {
            byte[] data = message.getBytes("UTF-8");
            DatagramPacket packet = new DatagramPacket(data, data.length, address, port);
            udpSocket.send(packet);
        } catch (IOException e) {
            System.err.println("发送消息失败: " + e.getMessage());
        }
    }

    private void broadcast(String message) {
        clients.forEach((acc, info) -> {
            info.lastActive = System.currentTimeMillis();
            sendUDP(message, info.address, info.port);
        });
    }

    //私聊
    private synchronized void sendTo(String recipient, String message) {
        ClientInfo info = clients.get(recipient);
        if (info != null) {
            info.lastActive = System.currentTimeMillis();
            sendUDP(message, info.address, info.port);
        }
        else {
            System.out.println("Recipient not found: " + recipient);
        }
    }
    //更新用户列表
    private void sendOnlineList(String account) {
        ClientInfo info = clients.get(account);
        if (info != null) {
            String list = "2222&" + String.join(",", onlineClient);
            sendUDP(list, info.address, info.port);
        }
        else {
            System.out.println("Recipient not found: " + account);
        }
    }

    // GUI部分保持不变
    public Server() {
        send_Button.addActionListener(e -> refreshMessage());
        sysMessageFiled.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyChar() == KeyEvent.VK_ENTER) refreshMessage();
            }
        });
        userList.addMouseListener(new MouseAdapter() {
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 2) {
                    String selected = userList.getSelectedValue().toString();
                    int confirm = JOptionPane.showConfirmDialog(null,
                            "强制下线用户：" + selected, "确认", JOptionPane.YES_NO_OPTION);
                    if (confirm == JOptionPane.YES_OPTION) {
                        forceDisconnect(selected);
                    }
                }
            }
        });
    }

    //给所有用户、群聊发送系统消息
    private void refreshMessage() {
        String message = sysMessageFiled.getText();
        broadcast("0000&系统消息;(" + nowDate() + "):" + message);
        sysMessageListModel.addElement("系统消息 (" + nowDate() + "): " + message);
        sysMessageFiled.setText("");
        sysMessageList.ensureIndexIsVisible(sysMessageListModel.size() - 1);
    }

    // 强制下线用户
    private void forceDisconnect(String account) {
        ClientInfo info = clients.get(account);
        if (info != null) {
            sendUDP("3333", info.address, info.port);
            clients.remove(account);
            userListModel.removeElement(account);
            broadcast("4444&" + account);
        }
    }

    private String nowDate() {
        return LocalDateTime.now()
                .format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"));
    }
    public static void main(String[] args) {
        try {
            Server server = new Server();
            server.start();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}