//import javax.swing.*;
//import javax.swing.event.ListSelectionEvent;
//import javax.swing.event.ListSelectionListener;
//import java.awt.*;
//import java.awt.event.ActionEvent;
//import java.awt.event.ActionListener;
//import java.awt.event.KeyAdapter;
//import java.awt.event.KeyEvent;
//import java.io.*;
//import java.net.Socket;
//import java.time.format.DateTimeFormatter;
//import java.util.*;
//import java.util.HashSet;
//import java.util.List;
//import java.util.Map;
//import java.util.Set;
//import java.util.concurrent.TimeUnit;
//import java.time.LocalDateTime;
//import java.lang.Object;
//
////public class Client {
//    private String IP = "127.0.0.1";
//    private int port = 5000;
//    public Socket socket = null;
//    public DataOutputStream output = null;
//    public DataInputStream input = null;
//    public String send;
//    public String account ;
//    public String receiver = "公共大厅";
//    private JPanel panel1;
//    private JTextField message_Field;
//    private JButton send_Button;
//    private JList message_List;
//    private JList friendList;
//    private JLabel ReceiverName;
//    private JButton createGroupButton;
//    private JButton addMemberButton;
//    private JButton removeMemberButton;
//    private List<String> onlineUser = new ArrayList<>();
//    private DefaultListModel messageListModel;
//    private DefaultListModel friendListModel;
//    private Map<String,DefaultListModel> chatRecord = new HashMap<>();
//    private DefaultListModel chatHallRecord = new DefaultListModel();
//    private DefaultListModel systemRecord = new DefaultListModel();
//    private Set<String> highlightFriend = new HashSet<>();
//
//    private List<String> groups = new ArrayList<String>();
//
//
//    public static void main(String[] args) throws IOException {
//        Client client = new Client("哈哈哈");
//        client.run();
//    }
//
//    public Client() {
//        this.account = "momo";
//        init();
//    }
//    public Client(String account) {
//        this.account = account;
//        init();
//    }
//
//    private void init(){
//        chatRecord.put("系统消息",systemRecord);
//        chatRecord.put("聊天大厅",chatHallRecord);
//        ReceiverName.setText(receiver);
//        message_List.setModel(chatHallRecord);
//        message_List.ensureIndexIsVisible(chatHallRecord.size() - 1);
//
//        friendList.setCellRenderer(new HighlightedListCellRenderer());
//
//        send_Button.addActionListener(new ActionListener() {
//            @Override
//            public void actionPerformed(ActionEvent e) {
//                if(receiver == "系统消息"){
//                    JOptionPane.showMessageDialog(null,"无法向系统发送消息！");
//                }else{
//                    refresh_message();
//                }
//            }
//        });
//
//        message_Field.addKeyListener(new KeyAdapter() {
//            @Override
//            public void keyPressed(KeyEvent e) {
//                if (e.getKeyChar() == KeyEvent.VK_ENTER) {
//                    if(receiver == "系统消息"){
//                        JOptionPane.showMessageDialog(null,"无法向系统发送消息！");
//                    }else{
//                        refresh_message();
//                    }
//                }
//            }
//        });
//
//        friendList.addListSelectionListener(new ListSelectionListener() {
//            @Override
//            public void valueChanged(ListSelectionEvent e) {
//                String selectFriend = (String) friendList.getSelectedValue();
//                if (selectFriend!=receiver){
//                    message_Field.setText("");
//                    if(selectFriend == null || selectFriend == "聊天大厅"){
//                        receiver = "聊天大厅";
//                    }else if(chatRecord.containsKey(selectFriend)){
//                        receiver = selectFriend;
//                    }else{
//                        receiver = selectFriend;
//                        chatRecord.put(receiver, new DefaultListModel());
//                    }
//                    ReceiverName.setText(receiver);
//                    DefaultListModel receiveChatHistory = chatRecord.get(receiver);
//                    message_List.setModel(receiveChatHistory);
//                    message_List.ensureIndexIsVisible(receiveChatHistory.size() - 1);
//
//                    if (highlightFriend.contains(receiver)) {
//                        highlightFriend.remove(receiver);
//                    }
//                    friendList.repaint();
//                    friendList.revalidate();
//                }
//
//                for (Map.Entry<String,DefaultListModel> entry : chatRecord.entrySet()) {
//                    String key = entry.getKey();
//                    DefaultListModel value = entry.getValue();
//                    System.out.println("Key = " + key + ", Value = " + value);
//                }
//            }
//        });
//
//        createGroupButton.addActionListener(e -> {
//            String groupName = JOptionPane.showInputDialog("请输入小组名：");
//            if (groupName != null && !groupName.isEmpty()) {
//                try {
//                    encryptWrite("5555&" + groupName+"&"+account, output);
//                    friendListModel.addElement(groupName);
//                    groups.add(groupName);
//                } catch (IOException ex) {
//                    ex.printStackTrace();
//                }
//            }
//        });
//
//        addMemberButton.addActionListener(e -> {
//            // 创建一个列表模型，并添加所有在线用户
//            DefaultListModel<String> listModel = new DefaultListModel<>();
//            for (String user : onlineUser) {
//                listModel.addElement(user);
//            }
//
//            JList<String> list = new JList<>(listModel);
//            list.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION); // 允许选择多个用户
//
//            int response = JOptionPane.showConfirmDialog(null, new JScrollPane(list), "请选择要添加的成员", JOptionPane.OK_CANCEL_OPTION);
//
//            if (response == JOptionPane.OK_OPTION) {
//                List<String> selectedUsers = list.getSelectedValuesList();
//                for (String user : selectedUsers) {
//                    try {
//                        encryptWrite("6666&" + receiver + "&" + user, output);
//                    } catch (IOException ex) {
//                        ex.printStackTrace();
//                    }
//                }
//            }
//        });
//
//        removeMemberButton.addActionListener(e -> {
//            // 创建一个列表模型，并添加所有在线用户
//            DefaultListModel<String> listModel = new DefaultListModel<>();
//            for (String user : onlineUser) {
//                listModel.addElement(user);
//            }
//
//            JList<String> list = new JList<>(listModel);
//            list.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION); // 允许选择多个用户
//
//            int response = JOptionPane.showConfirmDialog(null, new JScrollPane(list), "请选择要删除的成员", JOptionPane.OK_CANCEL_OPTION);
//
//            if (response == JOptionPane.OK_OPTION) {
//                List<String> selectedUsers = list.getSelectedValuesList();
//                for (String user : selectedUsers) {
//                    try {
//                        encryptWrite("7777&" + receiver + "&" + user, output);
//                    } catch (IOException ex) {
//                        ex.printStackTrace();
//                    }
//                }
//            }
//        });
//    }
//    //发送信息
//    // 刷新消息列表
//    private void refresh_message() {
//        String message = message_Field.getText();
//        message_Field.setText("");
//        String send2 ="";
//        //
//        if (groups.contains(receiver)) { // 如果接收者是一个小组
//            send2 = "8888&" + receiver + "&" + account +"(" + nowDate()+"):"+message; // 使用新的消息类型8888来发送小组消息
//        } else if(!receiver.equals("聊天大厅")){//私聊
//            send2 = "1333&" + account + "&" + receiver + "&(" + nowDate()+"):"+message;
//
//        }else{//群聊
//            send2 = "2333&" + account + "&(" + nowDate()+"):"+message;
//        }
//        try {
//            encryptWrite(send2, output);
//        } catch (IOException ex) {
//            ex.printStackTrace();
//        }
//
//        System.out.println(send2);
//
//        if(chatRecord.containsKey(receiver)){
//            DefaultListModel receiveChatHistory = chatRecord.get(receiver);
//            message_List.setModel(receiveChatHistory);
//            if(!receiver.equals("聊天大厅")&&!groups.contains(receiver))
//                receiveChatHistory.addElement(account+"("+nowDate()+"):"+message);
//            message_List.ensureIndexIsVisible(receiveChatHistory.size() - 1);
//        }else{
//            messageListModel = new DefaultListModel();
//            chatRecord.put(receiver, messageListModel);
//            DefaultListModel receiveChatHistory = chatRecord.get(receiver);
//            message_List.setModel(receiveChatHistory);
//            receiveChatHistory.addElement(account+"("+nowDate()+"):"+message);
//            message_List.ensureIndexIsVisible(receiveChatHistory.size() - 1);
//        }
//    }
//    public void run() throws IOException {
//        try {
//            JFrame frame = new JFrame(account);
//            frame.setContentPane(this.panel1);
//            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
//            frame.pack();
//            frame.setVisible(true);
//            frame.toFront();
//            frame.requestFocus();
//
//
//            // 线程，每秒更新聊天记录
//            new Thread(() -> {
//                while (true) {
//                    try {
//                        TimeUnit.SECONDS.sleep(1);
//                        // 这里添加更新聊天记录或好友列表的代码
//                    } catch (InterruptedException e) {
//                        e.printStackTrace();
//                    }
//                }
//            }).start();
//
//            Thread.sleep(250);
//
//            new Thread(() -> {
//                //        聊天信息、好友列表绑定模型
//                messageListModel = new DefaultListModel();
//                message_List.setModel(messageListModel);
//                message_List.updateUI();
//                message_List.ensureIndexIsVisible(messageListModel.size() - 1);
//
//                friendListModel = new DefaultListModel();
//                friendList.setModel(friendListModel);
//                friendListModel.addElement("系统消息");
//                friendListModel.addElement("聊天大厅");
//                receiver = "聊天大厅";
//                message_List.setModel(chatHallRecord);
//                message_List.ensureIndexIsVisible(chatHallRecord.size() - 1);
//
//                try {
//                    socket = new Socket(IP, port);
//                } catch (IOException e) {
//                    throw new RuntimeException(e);
//                }
//                try {
//                    output = new DataOutputStream(socket.getOutputStream());
//                } catch (IOException e) {
//                    throw new RuntimeException(e);
//                }
//                try {
//                    input = new DataInputStream(socket.getInputStream());
//                } catch (IOException e) {
//                    throw new RuntimeException(e);
//                }
//                //建立连接，上线
//                //把昵称发送给server，以便告知其加入聊天室
//                send = "1111&"+account;
//                try {
//                    encryptWrite(send, output);
//                } catch (IOException e) {
//                    throw new RuntimeException(e);
//                }
//                //接收消息
//                while (true) {
//                    System.out.println(input);
//                    String receive = null;
//                    try {
//                        receive = readDecrypt(input);
//                    } catch (IOException e) {
//                        throw new RuntimeException(e);
//                    }
//                    //如果收到 3333 则下线
//                    if ("3333".equals(receive)) {
//                        JOptionPane.showMessageDialog(null,"你已被强制下线！");
//                        frame.dispose();
//                        try {
//                            input.close();
//                        } catch (IOException e) {
//                            throw new RuntimeException(e);
//                        }
//                        try {
//                            output.close();
//                        } catch (IOException e) {
//                            throw new RuntimeException(e);
//                        }
//                        try {
//                            socket.close();
//                        } catch (IOException e) {
//                            throw new RuntimeException(e);
//                        }
//                        System.exit(0);
//                    }
//                    String []parts = receive.split("&");
//                    System.out.println(parts[0]);
//                    System.out.println(parts[1]);
//                    SwingUtilities.invokeLater(()-> {
//                        if (parts[0].equals("1111")) {
//                            if (!parts[1].equals(account)) {//添加新进入的客户端
//                                friendListModel.addElement(parts[1]);
//                                onlineUser.add(parts[1]);
//
//                            }
//                        } else if (parts[0].equals("2222")) {//添加之前进入的客户端
//                            String[] onlineFriend = parts[1].split(",");
//                            for (String friend : onlineFriend) {
//                                if(!friend.equals(account)) {
//                                    friendListModel.addElement(friend);
//                                    onlineUser.add(friend);
//                                }
//                            }
//                        } else if (parts[0].equals("4444")) {//离线
//                            friendListModel.removeElement(parts[1]);
//                            onlineUser.remove(parts[1]);
//                        } else if (parts[0].equals("0000")) {//私聊
//                            String sender = parts[1].split(";")[0];
//                            System.out.println(sender);
////                    messageListModel.addElement(parts[1]);
//                            String message = sender+parts[1].split(";")[1];
//                            if (!chatRecord.containsKey(sender)) {
//                                chatRecord.put(sender, new DefaultListModel());
//                            }
//                            DefaultListModel senderChatHistory = chatRecord.get(sender);
//                            senderChatHistory.addElement(message);
//                            System.out.println("receiver:"+receiver);
//                            // 如果这是当前显示的聊天窗口，立即在聊天窗口中显示这条消息
//                            if (sender.equals(receiver)) {
//                                message_List.setModel(senderChatHistory);
//                                message_List.ensureIndexIsVisible(senderChatHistory.size() - 1);
//                            }else{
//                                highlightFriend.add(sender);
//                                friendList.repaint();
//                                friendList.revalidate();
//                            }
//                        }else if (parts[0].equals("8888")) { // 如果是小组消息
//                            String groupName = parts[1];
//                            String message = parts[2];
//                            DefaultListModel groupChatHistory = chatRecord.get(groupName);
//                            groupChatHistory.addElement(message);
//                            if (groupName.equals(receiver)) { // 如果这是当前显示的小组
//                                message_List.setModel(groupChatHistory);
//                                message_List.ensureIndexIsVisible(groupChatHistory.size() - 1);
//                            } else {
//                                highlightFriend.add(groupName);
//                                friendList.repaint();
//                                friendList.revalidate();
//                            }
//                        } else if(parts[0].equals("6666")){
//                            friendListModel.addElement(parts[1]);
//                            groups.add(parts[1]);
//                        }else if(parts[0].equals("7777"))
//                        {
//                            friendListModel.removeElement(parts[1]);
//                            groups.remove(parts[1]);
//                        }else if(parts[0].equals("5555")){
//                            //更新群聊聊天记录
//                            String [] part = parts[1].split(";");
//                            String message = part[0]+part[1];
//                            chatHallRecord.addElement(message);
//                            if ("聊天大厅".equals(receiver)) {
//                                message_List.setModel(chatHallRecord);
//                                message_List.ensureIndexIsVisible(chatHallRecord.size() - 1);
//                            }else{
//                                highlightFriend.add("聊天大厅");
//                                friendList.repaint();
//                                friendList.revalidate();
//
//                            }
//                        }else{
//                            System.out.println("非法字符！");
//                        }
//                    });
//                }
//            }).start();
//
//        }catch(Exception e){
//            e.printStackTrace();
//        } finally{
//            try {
//                if (socket != null)
//                    socket.close();
//                input.close();
//                output.close();
//            } catch (Exception e) {
//                e.printStackTrace();
//            }
//        }
//    }
//    public void encryptWrite(String src,DataOutputStream output)throws IOException {
//        //将一个字符串转化为字符数组
//        char[] char_arr = src.toCharArray();
//        //加密操作
//        for(int i = 0;i<char_arr.length;i++){
//            output.writeChar(char_arr[i]+13);
//        }
//        //用作结束标志符
//        output.writeChar(2333);
//        output.flush();
//    }
//    //读取并解密
//    public String readDecrypt(DataInputStream input)throws IOException{
//        String rtn="";
//        while(true){
//            int char_src =input.readChar();
//            if(char_src!=2333 && char_src!=1333 && char_src!=1111 &&char_src!=3333  ){
//                rtn=rtn+(char)(char_src-13);
//            }else{
//                break;
//            }
//        }
//        System.out.println(rtn);
//        return rtn;
//    }
//    public String nowDate(){
//        LocalDateTime now = LocalDateTime.now();
//        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
//        String formatDateTime = now.format(formatter);
//        return formatDateTime;
//    }
//
//    //    点亮用户
//    public class HighlightedListCellRenderer extends DefaultListCellRenderer {
//        @Override
//        public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
//            Component component = super.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);
//            if (highlightFriend.contains(value)) {
//                component.setBackground(Color.ORANGE);
//            } else {
//                component.setBackground(Color.white);
//            }
//            return component;
//        }
//    }
//
//}