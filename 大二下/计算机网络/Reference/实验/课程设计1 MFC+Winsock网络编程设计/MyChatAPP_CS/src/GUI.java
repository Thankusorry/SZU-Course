import javax.swing.*;
import java.awt.*;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.net.NetworkInterface;
import java.util.Enumeration;
import java.net.SocketException;

public class GUI {
    public static void main(String[] args) {
        JFrame frame = new JFrame("客户端");
        JPanel panel1=new JPanel();
        JLabel labelServer=new JLabel("客户端名称：");
        JTextField textServer=new JTextField(15);
        JLabel labelServerPort=new JLabel("客户端端口：");
        JTextField textServerPort=new JTextField(6);
        JButton buttonListen=new JButton("连接");
        JButton buttonStop=new JButton("断开");
        panel1.add(labelServer);
        panel1.add(textServer);
        panel1.add(labelServerPort);
        panel1.add(textServerPort);
        panel1.add(buttonListen);
        panel1.add(buttonStop);

        JPanel panel2=new JPanel();
        JLabel labelSend=new JLabel("消息：");
        JTextField textSend=new JTextField(26);
        JButton buttonSend=new JButton("发送");
        JButton buttonIP=new JButton("本机网络配置信息");
        buttonIP.addActionListener(e->{
            JFrame IPFrame=new JFrame("本机网络配置信息");
            JTextArea IPArea=new JTextArea();

//            try {
//                InetAddress IP=InetAddress.getLocalHost();
//                InetAddress[] IPAll=InetAddress.getAllByName(IP.getHostName());
//                for(InetAddress inetAddress:IPAll){
//                    IPArea.append(inetAddress.getAddress()+": "+inetAddress.getHostAddress()+'\n');
//                }
//            } catch (UnknownHostException ex) {
//                throw new RuntimeException(ex);
//            }

            try {
                Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetworkInterfaces();
                while (interfaces.hasMoreElements()) {
                    NetworkInterface ni = interfaces.nextElement();
                    Enumeration<InetAddress> addresses = ni.getInetAddresses();
                    while (addresses.hasMoreElements()) {
                        InetAddress addr = addresses.nextElement();
                        if (!addr.isLinkLocalAddress() && !addr.isLoopbackAddress() && addr.getHostAddress().indexOf(":") == -1) {
                            IPArea.append("Interface: " + ni.getName()+'\n');
                            IPArea.append("IP Address: " + addr.getHostAddress()+'\n');
                            byte[] mac = ni.getHardwareAddress();
                            if (mac != null) {
                                StringBuilder sb = new StringBuilder();
                                for (byte b : mac) {
                                    sb.append(String.format("%02X:", b));
                                }
                                if (sb.length() > 0) {
                                    sb.deleteCharAt(sb.length() - 1);
                                }
                                IPArea.append("MAC Address: " + sb+'\n');
                            }
                        }
                    }
                }
            } catch (SocketException ee) {
                ee.printStackTrace();
            }

            IPFrame.add(IPArea);
            IPFrame.setLayout(new FlowLayout(1));
            IPFrame.setVisible(true);
            IPFrame.setSize(275,200);
        });
        panel2.add(labelSend);
        panel2.add(textSend);
        panel2.add(buttonSend);
        panel2.add(buttonIP);

        JPanel panel3=new JPanel();
        JLabel labelMessage=new JLabel("消息记录");
        panel3.add(labelMessage);

        JPanel panel4=new JPanel();
        JTextArea testMessage=new JTextArea(14,50);
        panel4.add(testMessage);

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
        frame.setSize(550,400);
        frame.setVisible(true);
    }
}
