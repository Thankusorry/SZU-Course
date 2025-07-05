import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.Socket;
import java.nio.file.*;

public class ClientGUI extends JFrame {
    // UI组件
    private JTextField hostField = new JTextField("localhost", 15);
    private JTextField portField = new JTextField("12345", 5);
    private JButton connectBtn = new JButton("连接");
    private JTextArea logArea = new JTextArea(10, 40);

    // 网络相关
    private Socket socket;
    private DataInputStream dataIn;
    private DataOutputStream dataOut;

    public ClientGUI() {
        super("TCP客户端");
        setupUI();
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        pack();
        setLocationRelativeTo(null);
    }

    private void setupUI() {
        // 连接面板
        JPanel connPanel = new JPanel();
        connPanel.add(new JLabel("服务器:"));
        connPanel.add(hostField);
        connPanel.add(new JLabel("端口:"));
        connPanel.add(portField);
        connPanel.add(connectBtn);

        // 功能按钮
        JPanel btnPanel = new JPanel(new GridLayout(1, 4));
        String[] commands = {"Time", "Dir", "GetFile", "Query","Exit"};
        for (String cmd : commands) {
            JButton btn = new JButton(cmd);
            btn.addActionListener(this::handleCommand);
            btnPanel.add(btn);
        }

        // 日志区域
        logArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(logArea);

        // 整体布局
        setLayout(new BorderLayout(5, 5));
        add(connPanel, BorderLayout.NORTH);
        add(btnPanel, BorderLayout.CENTER);
        add(scrollPane, BorderLayout.SOUTH);

        // 连接事件
        connectBtn.addActionListener(e -> connectToServer());
    }

    private void connectToServer() {
        new SwingWorker<Void, Void>() {
            @Override
            protected Void doInBackground() throws Exception {
                String host = hostField.getText();
                int port = Integer.parseInt(portField.getText());
                try {
                    socket = new Socket(host, port);
                    dataIn = new DataInputStream(socket.getInputStream());
                    dataOut = new DataOutputStream(socket.getOutputStream());
                    log("成功连接到服务器");
                    enableCommands(true);
                } catch (IOException e) {
                    log("连接失败: " + e.getMessage());
                }
                return null;
            }
        }.execute();
    }

    private void handleCommand(ActionEvent e) {
        String command = ((JButton) e.getSource()).getText();
        new SwingWorker<Void, String>() {
            @Override
            protected Void doInBackground() throws Exception {
                try {
                    if (command.equals("GetFile")) {
                        // 获取文件名
                        handleFileTransfer();
                    }
                    else if (command.equals("Query")) {
                        handleQuery();
                    }
                    else if (command.equals("Exit")){
                        dataOut.writeUTF("Exit");
                        dataOut.flush();
                        dataOut.close();
                        socket.close();
                        log("关闭连接");
                        enableCommands(false);
                    }
                    else {
                        dataOut.writeUTF(command);
                        dataOut.flush();
                        log("发送命令: " + command);
                        String response = dataIn.readUTF();
                        publish(response);
                    }

                } catch (IOException ex) {
                    log("命令执行错误: " + ex.getMessage());
                }
                return null;
            }

            @Override
            protected void process(java.util.List<String> chunks) {
                chunks.forEach(response -> {
                    if (command.equals("Dir")) {
                        log("文件列表:\n" + response);
                    } else {
                        log("服务器响应: " + response);
                    }
                });
            }
        }.execute();
    }
    private void handleQuery() throws IOException {
        String studentId = JOptionPane.showInputDialog(this, "请输入学号:");
        if (studentId != null && !studentId.isEmpty()) {
            dataOut.writeUTF("Query "+studentId.trim());
            dataOut.flush();
            log("发送命令: 查询学号--" +studentId);
            String response = dataIn.readUTF();
            log(response);
        }
    }
    private void handleFileTransfer() throws IOException {
        String fileName = JOptionPane.showInputDialog(this, "输入文件名:");
        if (fileName == null || fileName.isEmpty()) return;
        String command = "GetFile "+fileName;
        dataOut.writeUTF(command);
        dataOut.flush();
        log("发送命令: " + command);
        String response = dataIn.readUTF();
        if (response.startsWith("FileName:")) {
            // 确认下载
            int confirm = JOptionPane.showConfirmDialog(this,
                    "是否下载文件: " + response.split(":")[1], "确认下载",
                    JOptionPane.YES_NO_OPTION);
            if(confirm == JOptionPane.YES_OPTION) {
                dataOut.writeUTF("Yes");
            }
            else{
                dataOut.writeUTF("No");
            }

            // 接收文件
            long fileSize = dataIn.readLong();
            byte[] content = new byte[(int) fileSize];
            dataIn.readFully(content);

            // 保存文件
            JFileChooser saver = new JFileChooser();
            saver.setSelectedFile(new File("received_" + fileName));
            if (saver.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
                Files.write(saver.getSelectedFile().toPath(), content);
                log("文件保存成功: " + saver.getSelectedFile());
            }
        }
        else{
            log("错误: " + response);
        }
    }

    private void log(String message) {
        SwingUtilities.invokeLater(() ->
                logArea.append(message + "\n")
        );
    }

    private void enableCommands(boolean enabled) {
        for (Component comp : getContentPane().getComponents()) {
            if (comp instanceof JPanel) {
                for (Component btn : ((JPanel)comp).getComponents()) {
                    if (btn instanceof JButton && !btn.equals(connectBtn)) {
                        btn.setEnabled(enabled);
                    }
                }
            }
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new ClientGUI().setVisible(true));
    }
}