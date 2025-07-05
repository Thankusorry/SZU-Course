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

public class Clienttt {
    private String IP = "127.0.0.1";
    private int port = 5000;
    public DatagramSocket socket = null;
    public String send;
    public String account;
    public String receiver = "公共大厅2";
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
    private int[][] chessBoard = new int[15][15]; // 15x15棋盘
    private String opponentAddress;
    private int gamePort = 8888; // 游戏专用端口
    private DatagramSocket gameSocket;
    private JFrame gameFrame;
    private String currentRoomId;
    private int playerNumber; // 1-先手玩家 2-后手玩家
    private AtomicBoolean myTurn = new AtomicBoolean(false);
    private int num=1;
    public static void main(String[] args) throws IOException {
        Clienttt client = new Clienttt("王五");
        client.run();
    }

    public Clienttt() {
        this.account = "王五";
        init();
    }

    public Clienttt(String account) {
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
//        game_Button.addActionListener(new ActionListener()  {
//            @Override
//            public void actionPerformed(ActionEvent e) {
//                try {
//                    // 发送邀请游戏指令
//                    String logoutMsg = "1234&" + account+"&"+receiver;
//                    byte[] data = logoutMsg.getBytes(StandardCharsets.UTF_8);
//                    DatagramPacket packet = new DatagramPacket(
//                            data,
//                            data.length,
//                            InetAddress.getByName(IP),
//                            port
//                    );
//                    socket.send(packet);
//                } catch (IOException ex) {
//                    ex.printStackTrace();
//                }
//
//            }
//        });
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

    private synchronized  void refresh_message() {
        String message = message_Field.getText();
        message_Field.setText("");
        String send2 = "";

        if (groups.contains(receiver)) {
            send2 = "8888&" + receiver + "&" + account + "(" + nowDate() + "):" + message;
        } else if (!receiver.equals("聊天大厅")) {
            send2 = "1333&" + account + "&" + receiver + "&(" + nowDate() + "):" + message;
        } else {
            send2 = "2333&" + account + "&(" + nowDate() + "):" + message;
        }

        try {
            byte[] sendData = send2.getBytes();
            DatagramPacket sendPacket = new DatagramPacket(
                    sendData, sendData.length, InetAddress.getByName(IP), port);
            socket.send(sendPacket);
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        System.out.println(send2);

        if(chatRecord.containsKey(receiver)){
            DefaultListModel receiveChatHistory = chatRecord.get(receiver);
            message_List.setModel(receiveChatHistory);
            if(!receiver.equals("聊天大厅")&&!groups.contains(receiver))
                receiveChatHistory.addElement(account+"("+nowDate()+"):"+message);
            message_List.ensureIndexIsVisible(receiveChatHistory.size() - 1);
        }else{
            messageListModel = new DefaultListModel();
            chatRecord.put(receiver, messageListModel);
            DefaultListModel receiveChatHistory = chatRecord.get(receiver);
            message_List.setModel(receiveChatHistory);
            receiveChatHistory.addElement(account+"("+nowDate()+"):"+message);
            message_List.ensureIndexIsVisible(receiveChatHistory.size() - 1);
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
                            else if (parts[0].equals("4321")) { // 1开始游戏
                                currentRoomId = parts[1];
                                playerNumber =1;
                                initGameUI();
                                myTurn.set(true);
                                System.out.println("playerNumber: "+playerNumber);
                                SwingUtilities.invokeLater(() -> {
                                    gameFrame.setTitle("五子棋 - 房间号：" + currentRoomId
                                            + " (玩家" + playerNumber + ")");
                                    System.out.println("Myturn "+ myTurn.get());
                                    if(myTurn.get()) JOptionPane.showMessageDialog(gameFrame, "轮到您下棋！");
                                });

                            }
                            else if (parts[0].equals("4322")) { // 1开始游戏
                                currentRoomId = parts[1];
                                playerNumber =2;
                                initGameUI();
                                myTurn.set(true);
                                System.out.println("playerNumber: "+playerNumber);
                                SwingUtilities.invokeLater(() -> {
                                    gameFrame.setTitle("五子棋 - 房间号：" + currentRoomId
                                            + " (玩家" + playerNumber + ")");
                                    System.out.println("Myturn "+ myTurn.get());
                                    if(myTurn.get()) JOptionPane.showMessageDialog(gameFrame, "轮到您下棋！");
                                });
                            }
                            else if (parts[0].equals("MOVE")) {
                                int x = Integer.parseInt(parts[1]);
                                int y = Integer.parseInt(parts[2]);
                                placePiece(x, y, 2); // 对手棋子
                                gameFrame.repaint();
                                checkWinner(x, y);
                            }
//                            else if (parts[0].equals("TURN")) {
//
//                                if (parts[1].equals("YOUR_TURN")) {
//                                    myTurn.set(true);
//                                    SwingUtilities.invokeLater(() -> {
//                                        gameFrame.setTitle("轮到您下棋 - 房间号：" + currentRoomId);
//                                    });
//                                } else {
//                                    myTurn.set(false);
//                                    SwingUtilities.invokeLater(() -> {
//                                        gameFrame.setTitle("等待对手... - 房间号：" + currentRoomId);
//                                    });
//                                }
//                            }
                            else if (parts[0].equals("WIN")) {
                                SwingUtilities.invokeLater(() -> {
                                    JOptionPane.showMessageDialog(gameFrame, parts[1]);
                                    gameFrame.dispose();
                                    resetGameState();
                                });
                            }
                            else if (parts[0].equals("1234")){
                                String inviter=parts[1];
                                String opponet=parts[2];
                                System.out.println(inviter+"invite"+opponet);
                                Object[] options = {"开就开", "算了算了"};
                                int choice = JOptionPane.showOptionDialog(
                                        null,
                                        "来自 " + inviter + " 的对战邀请",
                                        "开一局吗?",
                                        JOptionPane.YES_NO_OPTION,
                                        JOptionPane.QUESTION_MESSAGE,
                                        null,
                                        options,
                                        options[0]
                                );

                                if (choice == 0) {
                                    System.out.println(opponet+"接受邀请");
                                    try {
                                        // 明文协议格式
                                        String msg = "4321&" + inviter + "&" + opponet;
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
                                        JOptionPane.showMessageDialog(null, "邀请失败: " + ex.getMessage());
                                    }
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

    private void createUIComponents() {
        // TODO: place custom component creation code here
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
//    // 连接到游戏服务器
//    private void connectToGameServer(String roomId) {
//        try {
//            // 复用主Socket，不需要新建
//            // 初始化游戏界面
//            initGameUI();
//
//            // 启动接收线程（合并到主消息循环）
//
//            // 发送准备就绪消息
//            String readyMsg = "1&" + roomId;
//            byte[] data = readyMsg.getBytes();
//            DatagramPacket packet = new DatagramPacket(data, data.length,
//                    InetAddress.getByName(IP), port); // 使用主端口
//            socket.send(packet);
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
//    }

    // 初始化游戏界面
    private synchronized void initGameUI() {
        gameFrame = new JFrame("五子棋对战 - 房间号：" + currentRoomId + " (玩家" + playerNumber + ")");
        gameFrame.setSize(600, 600);
        chessBoard = new int[15][15];

        JPanel boardPanel = new JPanel() {
            @Override
            protected void paintComponent(Graphics g) {
                super.paintComponent(g);
                // 绘制棋盘
                drawChessBoard(g);
            }
        };



        // 窗口关闭监听器
        gameFrame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                if(currentRoomId != null) {
                    String msg = "5558&" + currentRoomId;
                    byte[] data = msg.getBytes();
                    try {
                        DatagramPacket packet = new DatagramPacket(data, data.length,
                                InetAddress.getByName(IP), port);
                        socket.send(packet);
                    } catch (Exception ex) {
                        ex.printStackTrace();
                    }
                    resetGameState();
                }
            }
        });
        // 修改鼠标监听器的点击处理
        boardPanel.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                System.out.println("MyTurn="+myTurn.get());
                if (!myTurn.get()) {
                    System.out.println("[EVENT] 拒绝非回合操作");
                    return;
                }

                // 同步块保证原子操作
                synchronized(myTurn) {
                    //第二层状态验证

                    if (!myTurn.get()) {
                        System.out.println("[EVENT] 二次状态检查失败");
                        return;
                    }

                    // 精确坐标转换
                    int margin = 40;
                    int gridSize = 40;
                    int rawX = e.getX() - margin;
                    int rawY = e.getY() - margin;

                    if(rawX < 0 || rawY < 0) return;

                    int x = Math.round(rawX / (float)gridSize);
                    int y = Math.round(rawY / (float)gridSize);

                    if(x < 0 || x >= 15 || y < 0 || y >= 15) return;

                    // 落子并更新状态
                    if (placePiece(x, y, 1)) {
                        myTurn.set(false);
                        System.out.println("[ACTION] 成功落子 (" + x + "," + y + ")");
                        gameFrame.repaint();
                        checkWinner(x, y);
                    }
                }
            }
        });

        gameFrame.add(boardPanel);
        gameFrame.setVisible(true);
    }

    // 绘制棋盘逻辑
    private synchronized void drawChessBoard(Graphics g) {
        g.setColor(new Color(200, 170, 100));
        g.fillRect(0, 0, 600, 600);

        g.setColor(Color.BLACK);
        for (int i = 0; i < 15; i++) {
            g.drawLine(40, 40 + i * 40, 600 - 40, 40 + i * 40);
            g.drawLine(40 + i * 40, 40, 40 + i * 40, 600 - 40);
        }

        // 绘制棋子
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                if (chessBoard[i][j] == 1) {
                    drawChessPiece(g, i, j, Color.BLACK);
                } else if (chessBoard[i][j] == 2) {
                    drawChessPiece(g, i, j, Color.WHITE);
                }
            }
        }
    }


    // 添加游戏状态重置方法
    private synchronized void resetGameState() {
        chessBoard = new int[15][15];
        myTurn.set(false);
        currentRoomId = null;
        playerNumber = 0;
    }

    // 发送移动信息
    private synchronized void sendMove(int x, int y) {
        String moveMsg = "5556&" + currentRoomId + "&" + x + "&" + y;
        byte[] data = moveMsg.getBytes();
        try {
            DatagramPacket packet = new DatagramPacket(data, data.length,
                    InetAddress.getByName(IP), port);
            socket.send(packet);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // 落子逻辑
    private synchronized boolean placePiece(int x, int y, int player) {
        if (x < 0 || x >= 15 || y < 0 || y >= 15) return false;
        if (chessBoard[x][y] != 0) return false;

        chessBoard[x][y] = player;
        return true;
    }

    // 胜利检测
    private synchronized void checkWinner(int x, int y) {
        int player = chessBoard[x][y];
        int[][] directions = {{1,0}, {0,1}, {1,1}, {1,-1}};

        for (int[] dir : directions) {
            int count = 1;
            for (int i = -4; i <= 4; i++) {
                if (i == 0) continue;
                int nx = x + dir[0] * i;
                int ny = y + dir[1] * i;
                if (nx >= 0 && nx < 15 && ny >= 0 && ny < 15 &&
                        chessBoard[nx][ny] == player) {
                    count++;
                    if (count >= 5) {
                        String winMsg = "5557&" + currentRoomId + "&" + playerNumber;
                        byte[] data = winMsg.getBytes();
                        try {
                            DatagramPacket packet = new DatagramPacket(data, data.length,
                                    InetAddress.getByName(IP), port);
                            socket.send(packet);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                } else {
                    break;
                }
            }
        }
    }

    // 在Client2类中添加这个方法
    private synchronized void drawChessPiece(Graphics g, int x, int y, Color color) {
        int px = 40 + x * 40 - 15;
        int py = 40 + y * 40 - 15;

        Graphics2D g2d = (Graphics2D)g;
        g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                RenderingHints.VALUE_ANTIALIAS_ON);

        // 3D渐变效果
        GradientPaint gradient = new GradientPaint(
                px+5, py+5, color.brighter(),
                px+25, py+25, color.darker());
        g2d.setPaint(gradient);
        g2d.fillOval(px, py, 30, 30);

        // 高光效果
        g2d.setColor(new Color(255, 255, 255, 100));
        g2d.fillArc(px+2, py+2, 26, 26, 45, 180);

        // 边框
        g2d.setColor(color.darker());
        g2d.drawOval(px, py, 30, 30);
    }
}