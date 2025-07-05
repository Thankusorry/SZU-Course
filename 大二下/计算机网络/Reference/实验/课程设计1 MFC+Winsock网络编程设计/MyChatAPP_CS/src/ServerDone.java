import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.Scanner;

public class ServerDone {
    public static void main(String[] args) {
    }
}

class ServerSendGUI implements Runnable {
    private Socket socket;
    private ServerGUI serverGUI;

    ServerSendGUI(Socket socket, ServerGUI serverGUI) {
        this.socket = socket;
        this.serverGUI = serverGUI;
    }

    @Override
    public void run() {
        try {
            DataOutputStream output = new DataOutputStream(socket.getOutputStream());
            serverGUI.TextToSend(output);
        } catch (IOException error) {
            error.printStackTrace();
        }
    }
}

class ServerReceiveGUI implements Runnable {
    private Socket socket;
    private ServerGUI serverGUI;

    ServerReceiveGUI(Socket socket, ServerGUI serverGUI) {

        this.socket = socket;
        this.serverGUI = serverGUI;
    }

    @Override
    public void run() {
        try {
            DataInputStream input = new DataInputStream(socket.getInputStream());
            while (true) {
                serverGUI.TextFromReceive(input.readUTF());
            }
        } catch (IOException error) {
            error.printStackTrace();
        }
    }
}

class ServerGUI {
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


    ServerGUI() {
        frame = new JFrame("服务端");
        panel1 = new JPanel();
        labelServer = new JLabel("服务器名称：");
        textServer = new JTextField(14);
        labelServerPort = new JLabel("服务器端口：");
        textServerPort = new JTextField(6);
        buttonListen = new JButton("监听");
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

        JPanel panel5=new JPanel();
        JButton buttonSendFile= new JButton("发送文件");
        JLabel labelPath=new JLabel("文件路径：");
        JTextField textFile=new JTextField(26);
        JButton buttonReceiveFile= new JButton("接受文件");
        panel5.add(buttonSendFile);
        panel5.add(labelPath);
        panel5.add(textFile);
        panel5.add(buttonReceiveFile);

        frame.setLayout(new FlowLayout(1));
        frame.add(panel1);
        frame.add(panel5);
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
                textMessage.append("服务端：\n"+textSend.getText()+'\n');
                textSend.setText("");
            } catch (IOException ex) {
                throw new RuntimeException(ex);
            }

        });

    }

    public void TextFromReceive(String textReceive) {
        textMessage.append("客户端：\n"+textReceive+'\n');
    }
}
