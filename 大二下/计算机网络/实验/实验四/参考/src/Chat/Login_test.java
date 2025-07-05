package Chat;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class Login_test {
    private JTextField usernameField;
    private JPasswordField passwordField;
    private JButton loginButton;
    private JFrame frame;
    private boolean isLogged;

    public Login_test() {
        frame = new JFrame("登录");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Container container = frame.getContentPane();
        container.setLayout(new GridLayout(3, 2));

        container.add(new JLabel("用户名:"));
        usernameField = new JTextField();
        container.add(usernameField);

        container.add(new JLabel("密码:"));
        passwordField = new JPasswordField();
        container.add(passwordField);

        loginButton = new JButton("确定");
        container.add(loginButton);

        loginButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                // TODO: Implement your login logic here. In this simple example, we just check if the username and password are not empty.
                if (!usernameField.getText().isEmpty() && passwordField.getPassword().length != 0) {
                    isLogged = true;
                    frame.setVisible(false);  // hide login window
                } else {
                    JOptionPane.showMessageDialog(null,"用户名/密码不能为空");
                }
            }
        });

        frame.pack();
    }

    public boolean display() {
        frame.setVisible(true);
        return isLogged;
    }

    public String getUsername() {
        return usernameField.getText();
    }
}
