//import javax.swing.*;
//import java.awt.*;
//import java.awt.event.*;
//import java.io.*;
//import java.net.*;
//
//public class Client {
//    private String username;   // 保存用户名
//    private BufferedReader in;
//    private PrintWriter out;
//    private JFrame frame = new JFrame("Chat Room");  // 初始窗口标题，后续将设置为 "Chat Room - <username>"
//    private JPanel messagePanel = new JPanel();  // 用于显示消息的面板
//    private JScrollPane scrollPane;
//    private JTextField textField = new JTextField(50);
//    private Socket socket;
//
//    public Client() {
//        messagePanel.setLayout(new BoxLayout(messagePanel, BoxLayout.Y_AXIS));
//        scrollPane = new JScrollPane(messagePanel);
//        scrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
//
//        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
//        frame.getContentPane().add(scrollPane, BorderLayout.CENTER);
//        frame.getContentPane().add(textField, BorderLayout.SOUTH);
//        frame.setSize(600, 400);  // 设置窗口初始大小
//        frame.setLocationRelativeTo(null);  // 窗口居中显示
//
//        textField.addActionListener(new ActionListener() {
//            public void actionPerformed(ActionEvent e) {
//                sendMessage();  // 发送消息
//            }
//        });
//    }
//
//    // 发送消息
//    private void sendMessage() {
//        String message = textField.getText();
//        if (message.equalsIgnoreCase("exit")) {
//            try {
//                out.println("exit");
//                socket.close();
//                System.exit(0);
//            } catch (IOException e) {
//                e.printStackTrace();
//            }
//        } else {
//            out.println(message);   // 发送聊天消息
//            addMessage(message, true);   // 添加自己的消息
//            textField.setText("");   // 清空输入框
//        }
//    }
//
//    // 添加消息到聊天框
//    private void addMessage(String message, boolean isMine) {
//        JPanel messagePanelItem = new JPanel();
//        messagePanelItem.setLayout(new FlowLayout(isMine ? FlowLayout.RIGHT : FlowLayout.LEFT));
//
//        // 显示消息内容，根据发送者的不同显示不同的背景色
//        JTextArea messageText = new JTextArea(isMine ? message : username + ": " + message);
//        messageText.setEditable(false);
//        messageText.setWrapStyleWord(true);
//        messageText.setLineWrap(true);
//        messageText.setFont(new Font("微软雅黑", Font.PLAIN, 14));  // 设置字体为支持中文
//        messageText.setBackground(isMine ? new Color(0xD6F7FF) : new Color(0xFFFFFF));  // 根据发送者设置不同的背景色
//        messageText.setBorder(BorderFactory.createEmptyBorder(5, 20, 5, 10));
//
//        messagePanelItem.add(messageText);
//        messagePanel.add(messagePanelItem);
//        messagePanel.revalidate();
//        messagePanel.repaint();
//
//        // 滚动条自动滚动到最底部
//        SwingUtilities.invokeLater(new Runnable() {
//            public void run() {
//                JScrollBar verticalScrollBar = scrollPane.getVerticalScrollBar();
//                verticalScrollBar.setValue(verticalScrollBar.getMaximum());
//            }
//        });
//    }
//
//    // 运行客户端，连接到服务器
//    public void run() throws IOException {
//        socket = new Socket("localhost", 12345);   // 连接到服务器
//        in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
//        out = new PrintWriter(socket.getOutputStream(), true);
//
//        // 读取服务器发来的提示信息，并要求输入用户名
//        String prompt = in.readLine();
//        String response = JOptionPane.showInputDialog(frame, prompt, "Enter your username", JOptionPane.PLAIN_MESSAGE);
//
//        // 确保输入了用户名
//        if (response != null && !response.trim().isEmpty()) {
//            username = response.trim();  // 保存用户名
//        } else {
//            username = "User" + Math.random();  // 如果未输入，给一个默认用户名
//        }
//
//        // 设置窗口标题为 "Chat Room - 用户名"
//        frame.setTitle("Chat Room - " + username);
//
//        // 将用户名发送给服务器
//        out.println(username);
//
//        // 监听来自服务器的消息并显示
//        new Thread(new Runnable() {
//            public void run() {
//                try {
//                    String message;
//                    while ((message = in.readLine()) != null) {
//                        if (!message.startsWith(username + ":")) {  // 不显示自己的消息
//                            addMessage(message, false);  // 处理接收到的消息（别人发来的）
//                        }
//                    }
//                } catch (IOException e) {
//                    e.printStackTrace();
//                }
//            }
//        }).start();
//
//        frame.setVisible(true);
//    }
//
//    public static void main(String[] args) throws IOException {
//        Client client1 = new Client();
//        client1.run();
//        Client client2 = new Client();
//        client2.run();
//        Client client3 = new Client();
//        client3.run();
//    }
//}
