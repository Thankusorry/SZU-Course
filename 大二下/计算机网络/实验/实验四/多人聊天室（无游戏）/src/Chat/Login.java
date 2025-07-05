package Chat;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.util.HashMap;
import java.util.Map;

public class Login {
    private JPanel panel1;
    private JButton Login_Button;
    private JTextField account_Filed;
    private JTextField password_Filed;
    private JLabel welcome_titile;
    private JFrame frame;
    private static final String ACCOUNT_FILE = "./src/Chat/account.txt";
    private static final Map<String, String> accounts = new HashMap<>();
    private String IP = "127.0.0.1";
    private int port = 12345;



    public Login() {

        loadAccounts();
        setComponents(panel1);
        Login_Button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String account = account_Filed.getText();
                String password = password_Filed.getText();
                //防止未输入，或输入制表符
                if(account == null || account.trim().equals("")){
                    JOptionPane.showMessageDialog(null,"账号不能为空");
                    return;
                }
                if(password == null || password.trim().equals("")){
                    JOptionPane.showMessageDialog(null,"密码不能为空");
                    return;
                }
                if (login(account, password)) {
                    SwingUtilities.invokeLater(new Runnable() {
                        @Override
                        public void run() {
                            Client client = new Client(account);
                            try {
                                client.run();
                            } catch (Exception ex) {
                                throw new RuntimeException(ex);
                            }
                        }
                    });
                    frame.dispose();
                } else {
                    JOptionPane.showMessageDialog(null,"账户名或密码错误！");
                }
            }
        });
    }
    private void setComponents(JPanel panel1){}

    // 登录识别
    private static boolean login(String account, String password) {
        //如果有——登录
        if (accounts.containsKey(account)) {
            String storedPassword = accounts.get(account);
            if (storedPassword.equals(password)) {
                return true;
            }

        }
        // 如果没有——注册
        else {
            accounts.put(account, password);
            saveAccounts();
            return true;
        }

        return false;
    }

    // 将文件的内容读取到accounts（Map)
    private static void loadAccounts() {
        try (BufferedReader reader = new BufferedReader(new FileReader(ACCOUNT_FILE))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(":");
                if (parts.length == 2) {
                    String username = parts[0];
                    String password = parts[1];
                    accounts.put(username, password);
                }
            }
        } catch (IOException e) {
            // 处理文件读取异常
            e.printStackTrace();
        }
    }


    private static void saveAccounts() {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(ACCOUNT_FILE))) {
            for (Map.Entry<String, String> entry : accounts.entrySet()) {
                String username = entry.getKey();
                String password = entry.getValue();
                String line = username + ":" + password;
                writer.write(line);
                writer.newLine();
            }
        } catch (IOException e) {
            // 处理文件写入异常
            e.printStackTrace();
        }
    }

    public void run(){
        frame = new JFrame("登录页面");
        frame.setContentPane(this.panel1);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);
    }

    public static void main(String[] args) {
        new Login().run();
    }

}
