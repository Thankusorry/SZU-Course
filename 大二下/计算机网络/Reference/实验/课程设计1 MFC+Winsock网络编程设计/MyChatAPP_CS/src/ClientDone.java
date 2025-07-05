import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.Scanner;

public class ClientDone {
    public static void main(String[] args) {
    }
}

class ClientSendGUI implements Runnable {
    private Socket socket;
    private ClientGUI clientGUI;

    ClientSendGUI(Socket socket, ClientGUI clientGUI) {
        this.socket = socket;
        this.clientGUI = clientGUI;
    }

    @Override
    public void run() {
        try {
            DataOutputStream output = new DataOutputStream(socket.getOutputStream());
//            while (true) {
//                clientGUI.TextToSend(output);
//            }
            clientGUI.TextToSend(output);
        } catch (IOException error) {
            error.printStackTrace();
        }
    }
}

class ClientReceiveGUI implements Runnable {
    private Socket socket;
    private ClientGUI clientGUI;

    ClientReceiveGUI(Socket socket, ClientGUI clientGUI) {

        this.socket = socket;
        this.clientGUI = clientGUI;
    }

    @Override
    public void run() {
        try {
            DataInputStream input = new DataInputStream(socket.getInputStream());
            while (true) {
                clientGUI.TextFromReceive(input.readUTF());
            }
        } catch (IOException error) {
            error.printStackTrace();
        }
    }
}

class ClientGUI {
    private JFrame frame;
    private JPanel panel1;
    private JPanel panel2;
    private JPanel panel3;
    private JPanel panel4;
    private JLabel labelServer;
    private JLabel labelServerPort;
    private JLabel labelSend;
    private JLabel labelMessage;
    private JTextField textServer;
    private JTextField textServerPort;
    private JButton buttonListen;
    private JButton buttonStop;
    private JButton buttonSend;
    private JTextField textSend;
    private JTextArea textMessage;


    ClientGUI() {
        frame = new JFrame("客户端");
        panel1 = new JPanel();
        labelServer = new JLabel("客户端名称：");
        textServer = new JTextField(14);
        labelServerPort = new JLabel("客户端端口：");
        textServerPort = new JTextField(6);
        buttonListen = new JButton("连接");
        buttonStop = new JButton("断开");
        panel1.add(labelServer);
        panel1.add(textServer);
        panel1.add(labelServerPort);
        panel1.add(textServerPort);
        panel1.add(buttonListen);
        panel1.add(buttonStop);

        panel2 = new JPanel();
        labelSend = new JLabel("消息：");
        textSend = new JTextField(39);
        buttonSend = new JButton("发送");
        panel2.add(labelSend);
        panel2.add(textSend);
        panel2.add(buttonSend);

        panel3 = new JPanel();
        labelMessage = new JLabel("消息记录");
        panel3.add(labelMessage);

        panel4 = new JPanel();
        textMessage = new JTextArea(14, 50);
        panel4.add(textMessage);

        frame.setLayout(new FlowLayout(1));
        frame.add(panel1);
        frame.add(panel2);
        frame.add(panel3);
        frame.add(panel4);
        frame.setSize(550, 400);
        frame.setVisible(true);
    }

    public void TextToSend(DataOutputStream output) {

        buttonSend.addActionListener(e -> {
            try {
                output.writeUTF(textSend.getText());
                textMessage.append("客户端：\n"+textSend.getText()+'\n');
                textSend.setText("");
            } catch (IOException ex) {
                throw new RuntimeException(ex);
            }

        });

    }

    public void TextFromReceive(String textReceive) {
        textMessage.append("服务端：\n"+textReceive+'\n');
    }
}

