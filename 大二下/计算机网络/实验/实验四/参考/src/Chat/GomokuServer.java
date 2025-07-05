//package Chat;// 五子棋服务器端 GomokuServer.java
//import java.io.*;
//import java.net.*;
//import java.util.*;
//
//public class GomokuServer {
//    private static final int PORT = 5555;
//    public static List<ClientHandler> waitingPlayers = new ArrayList<>();
//
//    public static void main(String[] args) {
//        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
//            System.out.println("五子棋服务器已启动，监听端口：" + PORT);
//
//            while (true) {
//                Socket clientSocket = serverSocket.accept();
//                new ClientHandler(clientSocket).start();
//            }
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
//    }
//
//    static class ClientHandler extends Thread {
//        public Socket socket;
//        public PrintWriter out;
//        public BufferedReader in;
//        public GameRoom gameRoom;
//
//        public ClientHandler(Socket socket) {
//            this.socket = socket;
//        }
//
//        public void run() {
//            try {
//                out = new PrintWriter(socket.getOutputStream(), true);
//                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
//
//                // 等待匹配玩家
//                synchronized (waitingPlayers) {
//                    if (waitingPlayers.isEmpty()) {
//                        waitingPlayers.add(this);
//                        out.println("WAIT");
//                    } else {
//                        ClientHandler opponent = waitingPlayers.remove(0);
//                        gameRoom = new GameRoom(this, opponent);
//                        gameRoom.startGame();
//                    }
//                }
//
//                // 处理游戏指令
//                String inputLine;
//                while ((inputLine = in.readLine()) != null) {
//                    if (gameRoom != null) {
//                        gameRoom.processMove(this, inputLine);
//                    }
//                }
//            } catch (IOException e) {
//                e.printStackTrace();
//            }
//        }
//
//        public void sendMessage(String msg) {
//            out.println(msg);
//        }
//    }
//
//    static class GameRoom {
//        private ClientHandler[] players = new ClientHandler[2];
//        private int[][] board = new int[15][15];
//        private int currentPlayer = 0;
//
//        public GameRoom(ClientHandler p1, ClientHandler p2) {
//            players[0] = p1;
//            players[1] = p2;
//        }
//
//        public void startGame() {
//            players[0].sendMessage("START BLACK");
//            players[1].sendMessage("START WHITE");
//        }
//
//        public void processMove(ClientHandler player, String move) {
//            int playerIndex = (player == players[0]) ? 0 : 1;
//            if (playerIndex != currentPlayer) return;
//
//            String[] parts = move.split(",");
//            int x = Integer.parseInt(parts[0]);
//            int y = Integer.parseInt(parts[1]);
//
//            if (isValidMove(x, y)) {
//                board[x][y] = currentPlayer + 1;
//                broadcast("MOVE " + x + "," + y);
//
//                if (checkWin(x, y)) {
//                    broadcast("WIN " + currentPlayer);
//                } else {
//                    currentPlayer = 1 - currentPlayer;
//                }
//            }
//        }
//
//        private boolean isValidMove(int x, int y) {
//            return x >= 0 && x < 15 && y >= 0 && y < 15 && board[x][y] == 0;
//        }
//
//        private boolean checkWin(int x, int y) {
//            // 实现五子连珠判断逻辑（这里需要完整实现）
//            return false;
//        }
//
//        private void broadcast(String msg) {
//            for (ClientHandler p : players) {
//                p.sendMessage(msg);
//            }
//        }
//    }
//}