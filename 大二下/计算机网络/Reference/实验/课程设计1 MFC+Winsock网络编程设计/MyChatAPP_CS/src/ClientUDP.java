import java.net.DatagramSocket;
import java.net.SocketException;

public class ClientUDP {
    public static void main(String[] args) throws SocketException {
        DatagramSocket datagramSocket=new DatagramSocket(6666);
        new Thread(new ReceiveUDP(datagramSocket,9999)).start();
        new Thread(new SendUDP(datagramSocket,9999)).start();
    }
}
