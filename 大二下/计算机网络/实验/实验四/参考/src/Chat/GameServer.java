package Chat;

import java.io.*;
import java.net.*;
import java.util.concurrent.*;

public class GameServer {
    private static final int PORT = 5999;
    private static final String ip="192.168.1.100";
    private static final ConcurrentHashMap<String, GameRoom> rooms = new ConcurrentHashMap<>();

    public static void main(String[] args) throws IOException {
        try {
            ServerSocket serverSocket = new ServerSocket(PORT);
            System.out.println("五子棋服务器已启动，监听地址：" + serverSocket.getInetAddress().getHostAddress() + ":" + serverSocket.getLocalPort());

            ExecutorService pool = Executors.newCachedThreadPool();

            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("新客户端接入：" +
                        clientSocket.getInetAddress().getHostAddress() + ":" + clientSocket.getPort());
                pool.execute(new ClientHandler2(clientSocket));
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    static class ClientHandler2 implements Runnable {
        private final Socket socket;
        private PrintWriter out;
        private String roomId;
        private String playerType;

        public ClientHandler2(Socket socket) {
            this.socket = socket;
        }

        @Override
        public void run() {
            try (BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {
                out = new PrintWriter(socket.getOutputStream(), true);

                String message;
                while ((message = in.readLine()) != null) {
                    if (message.startsWith("JOIN:")) {
                        handleJoin(message.substring(5));
                    } else if (message.startsWith("MOVE:")) {
                        handleMove(message.substring(5));
                    }
                }
            } catch (IOException e) {
                handleDisconnect();
            }
        }

        private void handleJoin(String roomId) {
            this.roomId = roomId;
            GameRoom room = rooms.getOrDefault(roomId, new GameRoom());

            synchronized (room) {
                if (room.player1 == null) {
                    room.player1 = out;
                    playerType = "1";
                    rooms.put(roomId, room);
                    out.println("PLAYER:1");
                } else if (room.player2 == null) {
                    room.player2 = out;
                    playerType = "2";
                    rooms.put(roomId, room);
                    out.println("PLAYER:2");
                    // 通知双方游戏开始
                    room.player1.println("START:1");
                    room.player2.println("START:2");
                } else {
                    out.println("ERROR:房间已满");
                }
            }
        }

        private void handleMove(String move) {
            GameRoom room = rooms.get(roomId);
            if (room == null) return;

            String[] parts = move.split(":");
            if (parts.length != 2) return;

            // 转发移动指令给对方玩家
            PrintWriter target = (playerType.equals("1")) ? room.player2 : room.player1;
            if (target != null) {
                target.println("MOVE:" + parts[0] + ":" + parts[1]);
            }
        }

        private void handleDisconnect() {
            if (roomId != null) {
                GameRoom room = rooms.get(roomId);
                if (room != null) {
                    synchronized (room) {
                        PrintWriter other = (playerType.equals("1")) ? room.player2 : room.player1;
                        if (other != null) {
                            other.println("OPPONENT_LEFT");
                        }
                        rooms.remove(roomId);
                    }
                }
            }
            try {
                socket.close();
            } catch (IOException e) {
                // 忽略关闭异常
            }
        }
    }

    static class GameRoom {
        PrintWriter player1;
        PrintWriter player2;
    }
}