import java.net.DatagramSocket;
import java.net.SocketException;

public class ServerUDP {
    public static void main(String[] args) throws SocketException {
        DatagramSocket datagramSocket=new DatagramSocket(9999);
        new Thread(new SendUDP(datagramSocket,6666)).start();
        new Thread(new ReceiveUDP(datagramSocket,6666)).start();
    }
}
