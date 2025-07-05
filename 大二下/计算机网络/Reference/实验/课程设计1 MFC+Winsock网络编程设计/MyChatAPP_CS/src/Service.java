import java.net.*;

public class Service {
    public static void main(String[] args) throws Exception {
        ServerGUI serverGUI=new ServerGUI();
        ServerSocket serverSocket=new ServerSocket(6666);
        Socket socket = serverSocket.accept();
        new Thread(new ServerSendGUI(socket,serverGUI)).start();
        new Thread(new ServerReceiveGUI(socket,serverGUI)).start();
    }
}