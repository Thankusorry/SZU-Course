import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Client3 {
    private String username;
    private BufferedReader in;
    private PrintWriter out;
    private Socket socket;

    private JFrame frame = new JFrame("Chat Room");
    private JPanel messagePanel = new JPanel();
    private JScrollPane scrollPane;
    private JTextField textField = new JTextField(40);
    private SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy年MM月dd日 HH:mm");
    private Date lastMessageTime; // 新增：记录上条消息时间

    public Client3() {
        // 顶部标题栏
        JPanel header = new JPanel();
        header.setBackground(Color.BLACK);
        header.setPreferredSize(new Dimension(600, 30));
        JLabel title = new JLabel("Chat Room", SwingConstants.CENTER);
        title.setForeground(Color.WHITE);
        title.setFont(new Font("微软雅黑", Font.BOLD, 16));
        header.add(title);
        frame.add(header, BorderLayout.NORTH);

        messagePanel.setLayout(new BoxLayout(messagePanel, BoxLayout.Y_AXIS));
        scrollPane = new JScrollPane(messagePanel);
        scrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().add(scrollPane, BorderLayout.CENTER);

        // 输入面板包含文本框和发送按钮
        JPanel inputPanel = new JPanel(new BorderLayout(5, 5));
        inputPanel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));

        JButton sendButton = new JButton("发送");
        sendButton.addActionListener(e -> sendMessage());

        inputPanel.add(textField, BorderLayout.CENTER);
        inputPanel.add(sendButton, BorderLayout.EAST);
        frame.getContentPane().add(inputPanel, BorderLayout.SOUTH);

        textField.addActionListener(e -> sendMessage());

        frame.setSize(600, 400);
        frame.setLocationRelativeTo(null);
    }

    private void sendMessage() {
        String message = textField.getText().trim();
        if (message.isEmpty()) {
            JOptionPane.showMessageDialog(frame, "不能发送空白消息", "错误", JOptionPane.ERROR_MESSAGE);
            return;
        }
        SimpleDateFormat timeFormat = new SimpleDateFormat("HH:mm");
        String currentTime = timeFormat.format(new Date());

        // 发送带时间的消息
        out.println(currentTime + "|" + message);
        addMessage(username, currentTime, message, true);

        if (message.equalsIgnoreCase("exit")) {
            try {
                out.println("exit");
                socket.close();
                frame.dispose();
            } catch (IOException e) {
                e.printStackTrace();
            }
        } else {
            String time = dateFormat.format(new Date());
            out.println(time + "|" + message);
            addMessage(username, time, message, true);
            textField.setText("");
        }
    }


    private void addMessage(String sender, String timeStr, String content, boolean isMine) {
        Date currentTime = new Date();

        // 时间显示逻辑
        if (lastMessageTime == null ||
                (currentTime.getTime() - lastMessageTime.getTime() > 60000)) {
            addTimeLabel(timeStr);
            lastMessageTime = currentTime;
        }

        // 消息容器
        JPanel messageContainer = new JPanel(new BorderLayout());
        messageContainer.setMaximumSize(new Dimension(500, Integer.MAX_VALUE));

        // 发送者标签（仅显示对方消息）
        if (!isMine) {
            JLabel nameLabel = new JLabel(sender);
            nameLabel.setForeground(Color.GRAY);
            nameLabel.setFont(new Font("微软雅黑", Font.PLAIN, 12));
            messageContainer.add(nameLabel, BorderLayout.NORTH);
        }

        // 消息气泡
        JPanel bubble = new JPanel(new BorderLayout());
        bubble.setBackground(isMine ? new Color(0x0099FF) : new Color(0xEEEEEE));
        bubble.setBorder(BorderFactory.createEmptyBorder(8, 12, 8, 12));
        bubble.setMaximumSize(new Dimension(400, Integer.MAX_VALUE));

        // 消息内容
        JTextArea contentArea = new JTextArea(content);
        contentArea.setEditable(false);
        contentArea.setLineWrap(true);
        contentArea.setWrapStyleWord(true);
        contentArea.setFont(new Font("微软雅黑", Font.PLAIN, 14));
        contentArea.setForeground(isMine ? Color.WHITE : Color.BLACK);
        contentArea.setBackground(new Color(0x00000000, true));

        bubble.add(contentArea, BorderLayout.CENTER);

        // 对齐方式
        JPanel alignPanel = new JPanel(new FlowLayout(
                isMine ? FlowLayout.RIGHT : FlowLayout.LEFT, 10, 5));
        alignPanel.add(bubble);

        messageContainer.add(alignPanel, BorderLayout.CENTER);
        messagePanel.add(messageContainer);

        // 更新界面
        messagePanel.revalidate();
        messagePanel.repaint();
        scrollToBottom();
    }
    // 新增滚动到底部的方法
    private void scrollToBottom() {
        SwingUtilities.invokeLater(() -> {
            JScrollBar vertical = scrollPane.getVerticalScrollBar();
            vertical.setValue(vertical.getMaximum());
        });
    }

    // 添加时间分隔线
    private void addTimeLabel(String timeStr) {
        JPanel timePanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        JLabel timeLabel = new JLabel(timeStr);
        timeLabel.setForeground(Color.GRAY);
        timeLabel.setFont(new Font("微软雅黑", Font.PLAIN, 10));

        timePanel.add(timeLabel);
        messagePanel.add(timePanel);
    }

    public void run() throws IOException {
        socket = new Socket("127.0.0.1", 12345);
        in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        out = new PrintWriter(socket.getOutputStream(), true);

        String prompt = in.readLine();
        String response = JOptionPane.showInputDialog(frame, prompt, "", JOptionPane.PLAIN_MESSAGE);

        if (response != null && !response.trim().isEmpty()) {
            username = response.trim();
        } else {
            username = "用户" + (int)(Math.random()*1000);
        }

        out.println(username);
        frame.setTitle(username + "的聊天栏");

        new Thread(() -> {
            try {
                String message;
                while ((message = in.readLine()) != null) {
                    String[] parts = message.split("\\|", 2);
                    if (parts.length != 2) continue;

                    String time = parts[0];
//                    String[] timeContent = parts[1].split("\\|", 2);
                    String content = parts[1];
//                    if (timeContent.length != 2) continue;
//
//                    final String time = timeContent[0];
//                    final String content = timeContent[1];
//                    final boolean isMine = sender.equals(username);

                    SwingUtilities.invokeLater(() ->
                            addMessage("对方", time, content, false));
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }).start();

        frame.setVisible(true);
    }

    public static void main(String[] args) throws IOException {
        Client3 client = new Client3();
        client.run();
    }
}