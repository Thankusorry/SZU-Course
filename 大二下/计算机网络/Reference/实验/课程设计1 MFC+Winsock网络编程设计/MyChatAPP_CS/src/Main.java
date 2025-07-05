import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
    }
}

class Send implements Runnable {
    private Socket socket;

    Send(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {
        try {
            DataOutputStream output = new DataOutputStream(socket.getOutputStream());
            Scanner scanner = new Scanner(System.in);
            while (true) {
                String text = scanner.nextLine();
                output.writeUTF(text);
            }
        } catch (IOException error) {
            error.printStackTrace();
        }
    }
}

class Receive implements Runnable {
    private Socket socket;

    Receive(Socket socket) {

        this.socket = socket;
    }

    @Override
    public void run() {
        try {
            DataInputStream input = new DataInputStream(socket.getInputStream());
            while (true) {
                System.out.println(input.readUTF());
            }
        } catch (IOException error) {
            error.printStackTrace();
        }
    }
}