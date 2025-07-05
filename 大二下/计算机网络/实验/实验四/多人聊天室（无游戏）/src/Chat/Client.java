package Chat;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.time.format.DateTimeFormatter;
import java.util.*;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.time.LocalDateTime;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.atomic.AtomicBoolean;

import static java.lang.Thread.sleep;

public class Client {
    private String IP = "127.0.0.1";
    private int port = 5000;
    public DatagramSocket socket = null;
    public String send;
    public String account;
    public String receiver = "公共大厅";
    private JPanel panel1;
    private JTextField message_Field;
    private JButton send_Button;
    private JButton end_Button;
    private JButton game_Button;
    private JList message_List;
    private JList friendList;
    private JLabel ReceiverName;
    private JButton createGroupButton;
    private JButton addMemberButton;
    private JButton removeMemberButton;
    private List<String> onlineUser = new ArrayList<>();
    private DefaultListModel messageListModel;
    private DefaultListModel friendListModel;
    private Map<String, DefaultListModel> chatRecord = new HashMap<>();
    private DefaultListModel chatHallRecord = new DefaultListModel();
    private DefaultListModel systemRecord = new DefaultListModel();
    private Set<String> highlightFriend = new HashSet<>();
    private List<String> groups = new ArrayList<String>();
    public static void main(String[] args) throws IOException {
        Client client = new Client("张三");
        client.run();
    }

    public Client() {
        this.account = "张三";
        init();
    }

    public Client(String account) {
        this.account = account;
        init();
    }

    private void init(){
        chatRecord.put("系统消息",systemRecord);
        chatRecord.put("聊天大厅",chatHallRecord);
        ReceiverName.setText(receiver);
        message_List.setModel(chatHallRecord);
        message_List.ensureIndexIsVisible(chatHallRecord.size() - 1);

        friendList.setCellRenderer(new HighlightedListCellRenderer());



        send_Button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if(receiver == "系统消息"){
                    JOptionPane.showMessageDialog(null,"无法向系统发送消息！");
                }else{
                    refresh_message();
                }
            }
        });

        end_Button.addActionListener(new ActionListener()  {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    // 发送退出指令
                    String logoutMsg = "9999&" + account;
                    byte[] data = logoutMsg.getBytes(StandardCharsets.UTF_8);
                    DatagramPacket packet = new DatagramPacket(
                            data,
                            data.length,
                            InetAddress.getByName(IP),
                            port
                    );
                    socket.send(packet);
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
            }
        });
        message_Field.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyChar() == KeyEvent.VK_ENTER) {
                    if(receiver == "系统消息"){
                        JOptionPane.showMessageDialog(null,"无法向系统发送消息！");
                    }else{
                        refresh_message();
                    }
                }
            }
        });

        friendList.addListSelectionListener(new ListSelectionListener() {
            @Override
            public void valueChanged(ListSelectionEvent e) {
                String selectFriend = (String) friendList.getSelectedValue();
                if (selectFriend!=receiver){
                    message_Field.setText("");
                    if(selectFriend == null || selectFriend == "聊天大厅"){
                        receiver = "聊天大厅";
                    }else if(chatRecord.containsKey(selectFriend)){
                        receiver = selectFriend;
                    }else{
                        receiver = selectFriend;
                        chatRecord.put(receiver, new DefaultListModel());
                    }
                    ReceiverName.setText(receiver);
                    DefaultListModel receiveChatHistory = chatRecord.get(receiver);
                    message_List.setModel(receiveChatHistory);
                    message_List.ensureIndexIsVisible(receiveChatHistory.size() - 1);

                    if (highlightFriend.contains(receiver)) {
                        highlightFriend.remove(receiver);
                    }
                    friendList.repaint();
                    friendList.revalidate();
                }

                for (Map.Entry<String,DefaultListModel> entry : chatRecord.entrySet()) {
                    String key = entry.getKey();
                    DefaultListModel value = entry.getValue();
                    System.out.println("Key = " + key + ", Value = " + value);
                }
            }
        });

        //创建小组
        createGroupButton.addActionListener(e -> {
            String groupName = JOptionPane.showInputDialog("请输入小组名：");
            if (groupName != null && !groupName.isEmpty()) {
                try {
                    // 明文消息构造
                    String message = "5555&" + groupName + "&" + account;
                    // UDP数据包封装
                    byte[] data = message.getBytes(StandardCharsets.UTF_8);
                    DatagramPacket packet = new DatagramPacket(
                            data,
                            data.length,
                            InetAddress.getByName(IP),
                            port
                    );
                    socket.send(packet);

                    friendListModel.addElement(groupName);
                    groups.add(groupName);
                } catch (IOException ex) {
                    JOptionPane.showMessageDialog(null, "群组创建失败: " + ex.getMessage());
                }
            }
        });

        // 添加成员
        addMemberButton.addActionListener(e -> {
            // 创建一个列表模型，并添加所有在线用户
            DefaultListModel<String> listModel = new DefaultListModel<>();
            for (String user : onlineUser) {
                listModel.addElement(user);
            }

            JList<String> list = new JList<>(listModel);
            list.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION); // 允许选择多个用户

            int response = JOptionPane.showConfirmDialog(null, new JScrollPane(list), "请选择要添加的成员", JOptionPane.OK_CANCEL_OPTION);

            if (response == JOptionPane.OK_OPTION) {
                List<String> selectedUsers = list.getSelectedValuesList();
                for (String user : selectedUsers) {
                    try {
                        // 明文协议格式
                        String msg = "6666&" + receiver + "&" + user;
                        // UDP直发
                        byte[] data = msg.getBytes(StandardCharsets.UTF_8);
                        DatagramPacket packet = new DatagramPacket(
                                data,
                                data.length,
                                InetAddress.getByName(IP),
                                port
                        );
                        socket.send(packet);
                    } catch (IOException ex) {
                        JOptionPane.showMessageDialog(null, "添加成员失败: " + ex.getMessage());
                    }
                }
            }
        });

        removeMemberButton.addActionListener(e -> {
            // 创建一个列表模型，并添加所有在线用户
            DefaultListModel<String> listModel = new DefaultListModel<>();
            for (String user : onlineUser) {
                listModel.addElement(user);
            }

            JList<String> list = new JList<>(listModel);
            list.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION); // 允许选择多个用户

            int response = JOptionPane.showConfirmDialog(null, new JScrollPane(list), "请选择要删除的成员", JOptionPane.OK_CANCEL_OPTION);

            if (response == JOptionPane.OK_OPTION) {
                List<String> selectedUsers = list.getSelectedValuesList();
                for (String user : selectedUsers) {
                    try {
                        // 明文协议格式
                        String msg = "7777&" + receiver + "&" + user;
                        // UDP直发
                        byte[] data = msg.getBytes(StandardCharsets.UTF_8);
                        DatagramPacket packet = new DatagramPacket(
                                data,
                                data.length,
                                InetAddress.getByName(IP),
                                port
                        );
                        socket.send(packet);
                    } catch (IOException ex) {
                        JOptionPane.showMessageDialog(null, "添加成员失败: " + ex.getMessage());
                    }
                }
            }
        });
    }

    private synchronized void refresh_message() {
        // 1. 获取输入并清空
        String message = message_Field.getText();
        message_Field.setText("");

        // 2. 协议构造
        String send2 = "";
        if (groups.contains(receiver)) { // 群组消息
            send2 = "8888&" + receiver + "&" + account + "(" + nowDate() + "):" + message;
        } else if (!receiver.equals("聊天大厅")) { // 私聊
            send2 = "1333&" + account + "&" + receiver + "&(" + nowDate() + "):" + message;
        } else { // 公共大厅
            send2 = "2333&" + account + "&(" + nowDate() + "):" + message;
        }

        // 3. 发送消息
        try {
            byte[] sendData = send2.getBytes();
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, InetAddress.getByName(IP), port);
            socket.send(sendPacket);
        } catch (IOException ex) {
            ex.printStackTrace();
        }

        // 4. 更新本地记录
        if (chatRecord.containsKey(receiver)) {
            DefaultListModel receiveChatHistory = chatRecord.get(receiver);
            if (!receiver.equals("聊天大厅") && !groups.contains(receiver)) { // 私聊消息本地存储
                receiveChatHistory.addElement(account + "(" + nowDate() + "):" + message);
            }
            message_List.ensureIndexIsVisible(receiveChatHistory.size() - 1);
        } else { // 新会话初始化
            DefaultListModel newModel = new DefaultListModel();
            newModel.addElement(account + "(" + nowDate() + "):" + message);
            chatRecord.put(receiver, newModel);
            message_List.ensureIndexIsVisible(newModel.size() - 1);
        }
    }

    public synchronized void run() throws IOException {
        try {

            JFrame frame = new JFrame(account);
            frame.setContentPane(this.panel1);
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.pack();
            frame.setVisible(true);
            frame.toFront();
            frame.requestFocus();


            socket = new DatagramSocket();
            new Thread(() -> {
                //聊天信息、好友列表绑定模型
                messageListModel = new DefaultListModel();
                message_List.setModel(messageListModel);
                message_List.updateUI();
                message_List.ensureIndexIsVisible(messageListModel.size() - 1);

                friendListModel = new DefaultListModel();
                friendList.setModel(friendListModel);
                friendListModel.addElement("系统消息");
                friendListModel.addElement("聊天大厅");
                receiver = "聊天大厅";
                message_List.setModel(chatHallRecord);
                message_List.ensureIndexIsVisible(chatHallRecord.size() - 1);
                // 发送初始连接消息
                send = "1111&" + account;
                byte[] connectData = send.getBytes();
                DatagramPacket connectPacket = null;
                try {
                    connectPacket = new DatagramPacket(
                            connectData, connectData.length, InetAddress.getByName(IP), port);
                } catch (UnknownHostException e) {
                    throw new RuntimeException(e);
                }
                try {
                    socket.send(connectPacket);
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }

                byte[] receiveData = new byte[1024];

                while (true) {
                    try {
                        DatagramPacket receivePacket =
                                new DatagramPacket(receiveData, receiveData.length);
                        socket.receive(receivePacket);
                        String receive = new String(
                                receivePacket.getData(), 0, receivePacket.getLength());

                        System.out.println("客户端收到信息:"+receive);

                        if ("3333".equals(receive)) {
                            JOptionPane.showMessageDialog(frame,"你已被强制下线！");
                            frame.dispose();
                            try {
                                socket.close();
                            } catch (Exception e) {
                                throw new RuntimeException(e);
                            }
                        }
                        if ("exit".equals(receive)) {
                            JOptionPane.showMessageDialog(frame,"已退出");
                            frame.dispose();
                            try {
                                socket.close();
                            } catch (Exception e) {
                                throw new RuntimeException(e);
                            }
                        }
                        // 处理接收到的消息
                        String[] parts = receive.split("&");
                        System.out.println("parts[0]= "+parts[0]);
                        System.out.println("parts[1]= "+parts[1]);

                        SwingUtilities.invokeLater(() -> {
                            if (parts[0].equals("1111")) {
                                if (!parts[1].equals(account)) {//添加新进入的客户端
                                    friendListModel.addElement(parts[1]);
                                    onlineUser.add(parts[1]);
                                }
                            }
                            else if (parts[0].equals("2222")) {//添加之前进入的客户端
                                String[] onlineFriend = parts[1].split(",");
                                for (String friend : onlineFriend) {
                                    if(!friend.equals(account)) {
                                        friendListModel.addElement(friend);
                                        onlineUser.add(friend);
                                    }
                                }
                            }
                            else if (parts[0].equals("4444")) {                  //离线
                                friendListModel.removeElement(parts[1]);
                                onlineUser.remove(parts[1]);
                            }
                            else if (parts[0].equals("0000")) {               //私聊
                                String sender = parts[1].split(";")[0];
                                System.out.println(sender);
                                String message = sender+parts[1].split(";")[1];
                                if (!chatRecord.containsKey(sender)) {
                                    chatRecord.put(sender, new DefaultListModel());
                                }
                                DefaultListModel senderChatHistory = chatRecord.get(sender);
                                senderChatHistory.addElement(message);
                                System.out.println("receiver:"+receiver);
                                // 如果这是当前显示的聊天窗口，立即在聊天窗口中显示这条消息
                                if (sender.equals(receiver)) {
                                    message_List.setModel(senderChatHistory);
                                    message_List.ensureIndexIsVisible(senderChatHistory.size() - 1);
                                }else{
                                    highlightFriend.add(sender);
                                    friendList.repaint();
                                    friendList.revalidate();
                                }
                            }else if (parts[0].equals("8888")) { // 如果是小组消息
                                String groupName = parts[1];
                                String message = parts[2];
                                DefaultListModel groupChatHistory = chatRecord.get(groupName);
                                groupChatHistory.addElement(message);
                                if (groupName.equals(receiver)) { // 如果这是当前显示的小组
                                    message_List.setModel(groupChatHistory);
                                    message_List.ensureIndexIsVisible(groupChatHistory.size() - 1);
                                } else {
                                    highlightFriend.add(groupName);
                                    friendList.repaint();
                                    friendList.revalidate();
                                }
                            }
                            else if(parts[0].equals("6666")){
                                friendListModel.addElement(parts[1]);
                                groups.add(parts[1]);
                            }
                            else if(parts[0].equals("7777"))
                            {
                                friendListModel.removeElement(parts[1]);
                                groups.remove(parts[1]);
                            }
                            else if(parts[0].equals("5555")){
                                //更新群聊聊天记录
                                String [] part = parts[1].split(";");
                                String message = part[0]+part[1];
                                chatHallRecord.addElement(message);
                                if ("聊天大厅".equals(receiver)) {
                                    message_List.setModel(chatHallRecord);
                                    message_List.ensureIndexIsVisible(chatHallRecord.size() - 1);
                                }else{
                                    highlightFriend.add("聊天大厅");
                                    friendList.repaint();
                                    friendList.revalidate();
                                }
                            }


                            else{
                                System.out.println("非法字符！");
                            }
                        });

                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }).start();



        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public String nowDate(){
        LocalDateTime now = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        String formatDateTime = now.format(formatter);
        return formatDateTime;
    }


    //    点亮用户
    public class HighlightedListCellRenderer extends DefaultListCellRenderer {
        @Override
        public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
            Component component = super.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);
            if (highlightFriend.contains(value)) {
                component.setBackground(Color.ORANGE);
            } else {
                component.setBackground(Color.white);
            }
            return component;
        }
    }
}