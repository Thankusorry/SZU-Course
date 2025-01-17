import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;

public class Server {
    private static Map<String, PrintWriter> clientWriters = new HashMap<>();  // 保存用户名与输出流的映射
    private static ExecutorService pool = Executors.newFixedThreadPool(10);  // 使用线程池

    public static void main(String[] args) throws IOException {
        System.out.println("Server started");
        ServerSocket serverSocket = new ServerSocket(12345);

        try {
            while (true) {
                Socket clientSocket = serverSocket.accept();
                pool.submit(new ClientHandler(clientSocket));  // 将每个客户端的处理提交到线程池
            }
        } finally {
            serverSocket.close();
        }
    }

    private static class ClientHandler implements Runnable {
        private Socket socket;
        private PrintWriter out;
        private BufferedReader in;
        private String username;
        ClientHandler(Socket socket) {
            this.socket = socket;
        }
        public void run() {
            try {
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                out = new PrintWriter(socket.getOutputStream(), true);
                // 向客户端发送用户名提示
                out.println("Enter your username: ");
                username = in.readLine();  // 接收用户名
                // 将用户名和对应的 PrintWriter 添加到 clientWriters
                synchronized (clientWriters) {
                    clientWriters.put(username, out);
                }
                // 向所有客户端广播欢迎消息
                broadcast(username + " has joined the chat.", username);
                String message;
                while ((message = in.readLine()) != null) {
                    if (message.equalsIgnoreCase("exit")) {
                        break;
                    }
                    // 广播消息给其他客户端
                    broadcast(message, username);
                }
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                try {
                    socket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }

                // 客户端退出时，移除用户名与输出流的映射
                synchronized (clientWriters) {
                    clientWriters.remove(username);
                }

                // 向其他客户端广播退出消息
                broadcast(username + " has left the chat.", username);
            }
        }

        // 向所有客户端广播消息，但排除发送者自己
        void broadcast(String message, String senderUsername) {
            synchronized (clientWriters) {
                for (Map.Entry<String, PrintWriter> entry : clientWriters.entrySet()) {
                    PrintWriter writer = entry.getValue();
                    if (!entry.getKey().equals(senderUsername)) {
                        writer.println(senderUsername + ":" + message);  // 广播消息到其他客户端
                    }
                }
            }
        }
    }
}
