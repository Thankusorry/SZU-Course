//package Chat;

import javax.swing.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.time.format.DateTimeFormatter;
import java.util.*;
import java.time.LocalDateTime;

public class tempServer {
    private int port = 5000;
    private ServerSocket serverSocket = null;

    private Map<String, DataOutputStream> clients = new HashMap<>();
    private List<String> onlineClient = new ArrayList<>();
    private JPanel panel1;
    private JList userList;
    private JList sysMessageList;
    private JTextField sysMessageFiled;
    private JButton send_Button;
    private DefaultListModel userListModel;
    private DefaultListModel sysMessageListModel;
    private Map<String, List<String>> groups = new HashMap<>();

    public void start() throws Exception {
        JFrame frame = new JFrame("服务器");
        frame.setContentPane(this.panel1);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);

//        聊天信息、好友列表绑定模型
        userListModel = new DefaultListModel();
        userList.setModel(userListModel);
        sysMessageListModel = new DefaultListModel();
        sysMessageList.setModel(sysMessageListModel);

        serverSocket = new ServerSocket(port);
        System.out.println("Server started, listening on port " + port);

        while (true) {
            Socket clientSocket = serverSocket.accept();
            new Thread(new ClientHandler(clientSocket)).start();
        }
    }

    class ClientHandler implements Runnable {
        private Socket clientSocket;
        private DataInputStream input;
        private DataOutputStream output;
        private String account;

        ClientHandler(Socket clientSocket) throws Exception {
            this.clientSocket = clientSocket;
            this.input = new DataInputStream(clientSocket.getInputStream());
            this.output = new DataOutputStream(clientSocket.getOutputStream());
        }

        @Override
        public void run() {
            try {
                while (true) {
                    String message = readDecrypt(input);
                    System.out.println("Received: " + message);
                    String[] parts = message.split("&", 4);
                    int code = Integer.parseInt(parts[0]);
                    account = parts[1];

                    List<String> groupMembers = new ArrayList<>(); // 初始化为一个空列表
                    String groupName = parts[1];
                    groupMembers = groups.get(groupName);

                    switch (code) {
                        case 1111:
                            clients.put(account, output);
                            onlineClient.add(account);
                            userListModel.addElement(account);
                            sendOnlineList(account);
                            broadcast("1111&"+account);
                            break;

                        case 2333:
                            broadcast("5555&"+account + ";" + parts[2]);
                            break;

                        case 1333:

                            String recipient = parts[2];
                            String privateMessage = parts[3];
                            sendTo(recipient, account +";"+ privateMessage);
                            break;

                        case 5555: // 创建小组
                            groupName = parts[1];
                            groups.put(groupName, new ArrayList<>());
                            String account = parts[2];
                            groupMembers = groups.get(groupName);
                            groupMembers.add(account);
                            break;

                        case 6666: // 添加成员到小组
                            String newMember = parts[2];
                            if (groupMembers != null) {
                                groupMembers.add(newMember);
                                DataOutputStream output = clients.get(newMember);
                                encryptWrite("6666&"+groupName, output);
                            }
                            break;
                        case 7777: // 从小组中移除成员
                            String memberToRemove = parts[2];
                            if (groupMembers != null) {
                                groupMembers.remove(memberToRemove);
                                DataOutputStream output = clients.get(memberToRemove);
                                encryptWrite("7777&"+groupName, output);
                            }
                            break;
                        case 8888://小组消息
                            groupName = parts[1];
                            groupMembers = groups.get(groupName);
                            if (groupMembers != null) {
                                broadcastToGroup(groupName,parts[2]);
                            }
                            break;

                        default:
                            System.out.println("Unknown message type: " + code);
                            break;
                    }
                }
            } catch (EOFException | SocketException e) {
                System.err.println("Connection was closed by client or network error occurred.");
            } catch (Exception e) {
                e.printStackTrace();

            } finally {
                clients.remove(account);
                onlineClient.remove(account);
                userListModel.removeElement(account);
                System.out.println(account+"下线");
                try {
                    broadcast("4444&"+account);
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
                try {
                    if (clientSocket != null) {
                        clientSocket.close();
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
        //广播
        public void broadcast(String message) throws Exception {
            for (DataOutputStream output : clients.values()) {
                encryptWrite(message, output);
            }
        }
        //小组广播
        public void broadcastToGroup(String groupName, String message) throws Exception {
            List<String> groupMembers = groups.get(groupName);
            if (groupMembers != null) {
                for (String member : groupMembers) {
                    DataOutputStream output = clients.get(member);
                    if (output != null) {
                        encryptWrite("8888&"+groupName+"&"+message, output);
                    }
                }
            }
        }

        //私聊
        private void sendTo(String recipient, String message) throws Exception {
            DataOutputStream output = clients.get(recipient);
            if (output != null) {
//                private_encryptWrite(message,output);
                encryptWrite("0000&"+message, output);
            } else {
                System.out.println("Recipient not found: " + recipient);
            }
        }
        //更新用户列表
        private void sendOnlineList(String recipient)throws Exception{
            DataOutputStream output = clients.get(recipient);
            if (output != null) {
                encryptWrite("2222&" + String.join(",", onlineClient), output);
            } else {
                System.out.println("Recipient not found: " + recipient);
            }
        }

    }

    public tempServer() {
        send_Button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                refresh_message();
            }
        });

        sysMessageFiled.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyChar() == KeyEvent.VK_ENTER)
                    refresh_message();
            }
        });
        userList.addMouseListener(new MouseAdapter() {
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 2) { // 双击事件
                    String selectedUser = userList.getSelectedValue().toString();
                    int confirm = JOptionPane.showConfirmDialog(null, "是否要强制下线用户：" + selectedUser, "强制下线", JOptionPane.YES_NO_OPTION);
                    if (confirm == JOptionPane.YES_OPTION) {
                        forceDisconnect(selectedUser);
                    }
                }
            }
        });

    }
    ////输入
    //给所有用户、群聊发送系统消息
    private void refresh_message() {
        String message = sysMessageFiled.getText();
        for (DataOutputStream output : clients.values()) {
            try {
                encryptWrite("0000&系统消息"+";("+nowDate()+"):"+message, output);
            } catch (IOException ex) {
                throw new RuntimeException(ex);
            }
        }
        sysMessageFiled.setText("");

        sysMessageListModel.addElement("系统消息"+"("+nowDate()+"):"+message);
//        自动往下滚
        sysMessageList.ensureIndexIsVisible(sysMessageListModel.size() - 1);
    }

    // 强制下线用户
    private void forceDisconnect(String account) {
        DataOutputStream output = clients.get(account);
        if (output != null) {
            try {
                encryptWrite("3333",output); // 发送下线信号给客户端
                output.close();
                clients.remove(account); // 在服务器端移除该客户端
                broadcast("4444&" + account); // 广播下线信息
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void broadcast(String message) {
        for (DataOutputStream output : clients.values()) {
            try {
                encryptWrite(message, output);
            } catch (IOException ex) {
                throw new RuntimeException(ex);
            }
        }
    }


    public void encryptWrite(String src,DataOutputStream output)throws IOException{
        char[] char_arr = src.toCharArray();
        //加密
        for(int i=0;i<char_arr.length;i++){
            output.writeChar(char_arr[i]+13);
        }
        //结束标识符
        output.writeChar(2333);
        System.out.println(output);
        output.flush();
    }

    public String readDecrypt(DataInputStream input)throws IOException{
        String rtn = "";
        while(true){
            int char_src = input.readChar();
            if(char_src!=2333){
                rtn += (char)(char_src-13);
            }else{
                break;
            }
        }
        return rtn;
    }
    public String nowDate(){
        LocalDateTime now = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        String formatDateTime = now.format(formatter);
        return formatDateTime;
    }

    public static void main(String[] args) {
        try {
            tempServer server = new tempServer(); // 创建Server对象
            server.start(); // 运行服务器
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

}