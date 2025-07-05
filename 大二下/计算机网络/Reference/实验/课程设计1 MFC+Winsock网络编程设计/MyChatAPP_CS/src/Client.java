import java.net.*;

public class Client {
    public static void main(String[] args) throws Exception {
        ClientGUI clientGUI=new ClientGUI();
        Socket socket = new Socket(InetAddress.getLocalHost(), 6666);
        new Thread(new ClientReceiveGUI(socket,clientGUI)).start();
        new Thread(new ClientSendGUI(socket,clientGUI)).start();
    }
}

