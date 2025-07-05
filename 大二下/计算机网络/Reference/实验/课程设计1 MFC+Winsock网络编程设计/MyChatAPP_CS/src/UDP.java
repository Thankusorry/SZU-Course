import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Scanner;

class SendUDP implements Runnable {
    private DatagramSocket datagramSocket;
    private int port;

    SendUDP(DatagramSocket datagramSocket, int port) {
        this.datagramSocket = datagramSocket;
        this.port = port;
    }

    @Override
    public void run() {
        DatagramPacket sendPacket;
        Scanner scanner = new Scanner(System.in);
        try {
            while(true){
                String message = scanner.nextLine();
                byte[] data = message.getBytes();
                sendPacket = new DatagramPacket(data, 0, data.length, InetAddress.getLocalHost(), port);
                datagramSocket.send(sendPacket);
            }

        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}

class ReceiveUDP implements Runnable {
    private DatagramSocket datagramSocket;
    private int port;

    ReceiveUDP(DatagramSocket datagramSocket, int port) {

        this.datagramSocket = datagramSocket;
        this.port = port;
    }

    @Override
    public void run() {
        byte[] data = new byte[1024];
        DatagramPacket datagramPacket;
        try {
            datagramPacket = new DatagramPacket(data, data.length, InetAddress.getLocalHost(), port);
            while (true) {
                datagramSocket.receive(datagramPacket);
                String message = new String(datagramPacket.getData(), 0, datagramPacket.getLength());
                System.out.println(message);
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}