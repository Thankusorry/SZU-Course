import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Client1 {
    private String username;
    private BufferedReader in;
    private PrintWriter out;
    private Socket socket;

    private JFrame frame = new JFrame("Chat Room");
    private JPanel messagePanel = new JPanel();
    private JScrollPane scrollPane;
    private JTextField textField = new JTextField(30);
    private SimpleDateFormat timeFormat = new SimpleDateFormat("HH:mm");
    private Date lastMsgTime;

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new Client1().initialize());
        SwingUtilities.invokeLater(() -> new Client1().initialize());

    }

    private void initialize() {
        // GUI初始化
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(600, 400);
        frame.setLayout(new BorderLayout());

        // 顶部标题栏
        JPanel header = new JPanel();
        header.setBackground(Color.BLACK);
        header.setPreferredSize(new Dimension(frame.getWidth(), 40));
        JLabel title = new JLabel("Chat Room", SwingConstants.CENTER);
        title.setForeground(Color.WHITE);
        title.setFont(new Font("微软雅黑", Font.BOLD, 18));
        header.add(title);

        // 消息显示区域
        messagePanel.setLayout(new BoxLayout(messagePanel, BoxLayout.Y_AXIS));
        scrollPane = new JScrollPane(messagePanel);
        scrollPane.getVerticalScrollBar().setUnitIncrement(16);

        // 底部操作面板
        JPanel bottomPanel = new JPanel(new BorderLayout(5,5));
        JPanel buttonPanel = new JPanel();
        JButton sendBtn = new JButton("发送");
        JButton exitBtn = new JButton("退出");

        sendBtn.addActionListener(e -> sendMessage());
        exitBtn.addActionListener(e -> exitApp());

        buttonPanel.add(sendBtn);
        buttonPanel.add(exitBtn);
        bottomPanel.add(textField, BorderLayout.CENTER);
        bottomPanel.add(buttonPanel, BorderLayout.EAST);

        frame.add(header, BorderLayout.NORTH);
        frame.add(scrollPane, BorderLayout.CENTER);
        frame.add(bottomPanel, BorderLayout.SOUTH);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);

        // 网络连接
        connectToServer();
    }

    private void connectToServer() {
        new SwingWorker<Void, Void>() {
            @Override
            protected Void doInBackground() {
                try {
                    socket = new Socket("localhost", 12345);
                    in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                    out = new PrintWriter(socket.getOutputStream(), true);

                    // 获取用户名
                    String serverPrompt = in.readLine();
                    String name = JOptionPane.showInputDialog(frame, serverPrompt, "用户登录", JOptionPane.PLAIN_MESSAGE);
                    username = (name != null && !name.trim().isEmpty()) ? name : "用户" + (int)(Math.random()*1000);
                    out.println(username);

                    // 启动消息监听
                    new MessageReceiver().start();

                } catch (ConnectException e) {
                    showError("无法连接服务器");
                } catch (IOException e) {
                    showError("网络错误: " + e.getMessage());
                }
                return null;
            }
        }.execute();
    }

    private class MessageReceiver extends Thread {
        @Override
        public void run() {
            try {
                String message;
                while ((message = in.readLine()) != null) {
                    processMessage(message);
                }
            } catch (IOException e) {
                if (!socket.isClosed()) {
                    showError("连接已断开");
                }
            }
        }
    }

    private void processMessage(String rawMsg) {
        String[] parts = rawMsg.split("‖", 3);
        if (parts.length != 3) return;

        String sender = parts[0];
        String time = parts[1];
        String content = parts[2];

        SwingUtilities.invokeLater(() -> {
            boolean isSelf = sender.equals(username);
            addMessageBubble(sender, time, content, isSelf);
        });
    }

    private void addMessageBubble(String sender, String time, String content, boolean isSelf) {
        // 时间间隔显示
        try {
            Date currentTime = timeFormat.parse(time);
            if (lastMsgTime == null || (currentTime.getTime() - lastMsgTime.getTime() > 60000)) {
                addTimeLabel(time);
                lastMsgTime = currentTime;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        // 消息容器
        JPanel msgContainer = new JPanel();
        msgContainer.setLayout(new BoxLayout(msgContainer, BoxLayout.Y_AXIS));
        msgContainer.setAlignmentX(isSelf ? Component.RIGHT_ALIGNMENT : Component.LEFT_ALIGNMENT);

        // 发送者标签
        if (!isSelf) {
            JLabel nameLabel = new JLabel(sender);
            nameLabel.setForeground(Color.GRAY);
            nameLabel.setFont(new Font("微软雅黑", Font.PLAIN, 10));
            msgContainer.add(nameLabel);
        }

        // 消息气泡
        JTextArea bubble = new JTextArea(content);
        bubble.setLineWrap(true);
        bubble.setWrapStyleWord(true);
        bubble.setEditable(false);
        bubble.setBackground(isSelf ? new Color(0, 153, 255) : new Color(240, 240, 240));
        bubble.setForeground(isSelf ? Color.WHITE : Color.BLACK);
        bubble.setBorder(BorderFactory.createEmptyBorder(8, 12, 8, 12));

        msgContainer.add(bubble);
        messagePanel.add(msgContainer);
        messagePanel.revalidate();

        // 自动滚动
        SwingUtilities.invokeLater(() -> {
            JScrollBar vertical = scrollPane.getVerticalScrollBar();
            vertical.setValue(vertical.getMaximum());
        });
    }

    private void addTimeLabel(String time) {
        JLabel timeLabel = new JLabel(time);
        timeLabel.setHorizontalAlignment(SwingConstants.CENTER);
        timeLabel.setForeground(Color.GRAY);
        timeLabel.setFont(new Font("微软雅黑", Font.PLAIN, 10));

        messagePanel.add(timeLabel);
        messagePanel.revalidate();
    }

    private void sendMessage() {
        String msg = textField.getText().trim();
        if (msg.isEmpty()) {
            JOptionPane.showMessageDialog(frame, "不能发送空消息", "错误", JOptionPane.ERROR_MESSAGE);
            return;
        }

        String timestamp = timeFormat.format(new Date());
        out.println(username + "‖" + timestamp + "‖" + msg);
        textField.setText("");
    }

    private void exitApp() {
        int confirm = JOptionPane.showConfirmDialog(frame,
                "确定要退出聊天室吗？", "退出确认",
                JOptionPane.YES_NO_OPTION);

        if (confirm == JOptionPane.YES_OPTION) {
            try {
                if (out != null) out.println("EXIT‖" + username);
                if (socket != null) socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            System.exit(0);
        }
    }

    private void showError(String message) {
        SwingUtilities.invokeLater(() -> {
            JOptionPane.showMessageDialog(frame, message, "系统错误", JOptionPane.ERROR_MESSAGE);
            System.exit(1);
        });
    }
}