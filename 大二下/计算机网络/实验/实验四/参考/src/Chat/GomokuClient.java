//// 五子棋客户端 GomokuClient.java
//import javax.swing.*;
//import java.awt.*;
//import java.awt.event.*;
//import java.net.*;
//import java.io.*;
//
//public class GomokuClient extends JFrame {
//    private static final String SERVER_IP = "127.0.0.1";
//    private static final int SERVER_PORT = 5555;
//
//    private PrintWriter out;
//    private int[][] board = new int[15][15];
//    private int myColor;
//    private boolean myTurn = false;
//
//    public GomokuClient() {
//        setTitle("五子棋客户端");
//        setSize(600, 600);
//        setDefaultCloseOperation(EXIT_ON_CLOSE);
//
//        add(new BoardPanel(), BorderLayout.CENTER);
//        connectToGameServer();
//    }
//
//    // 连接游戏服务器的方法
//    private void connectToGameServer() {
//        try {
//            Socket socket = new Socket(SERVER_IP, SERVER_PORT);
//            out = new PrintWriter(socket.getOutputStream(), true);
//
//            new Thread(() -> {
//                try (BufferedReader in = new BufferedReader(
//                        new InputStreamReader(socket.getInputStream()))) {
//                    String response;
//                    while ((response = in.readLine()) != null) {
//                        processServerMessage(response);
//                    }
//                } catch (IOException e) {
//                    e.printStackTrace();
//                }
//            }).start();
//
//        } catch (IOException e) {
//            JOptionPane.showMessageDialog(this, "连接服务器失败");
//            System.exit(0);
//        }
//    }
//
//    private void processServerMessage(String msg) {
//        String[] parts = msg.split(" ");
//        switch (parts[0]) {
//            case "START":
//                myColor = parts[1].equals("BLACK") ? 1 : 2;
//                myTurn = (myColor == 1);
//                break;
//            case "MOVE":
//                String[] coords = parts[1].split(",");
//                int x = Integer.parseInt(coords[0]);
//                int y = Integer.parseInt(coords[1]);
//                board[x][y] = (myColor == 1) ? 2 : 1;
//                myTurn = true;
//                repaint();
//                break;
//            case "WIN":
//                JOptionPane.showMessageDialog(this, "游戏结束！获胜方：" + parts[1]);
//                break;
//        }
//    }
//
//    class BoardPanel extends JPanel implements MouseListener {
//        public BoardPanel() {
//            addMouseListener(this);
//        }
//
//        protected void paintComponent(Graphics g) {
//            super.paintComponent(g);
//            // 绘制棋盘和棋子（需要实现完整绘制逻辑）
//        }
//
//        public void mouseClicked(MouseEvent e) {
//            if (!myTurn) return;
//
//            int x = e.getX() * 15 / getWidth();
//            int y = e.getY() * 15 / getHeight();
//
//            if (board[x][y] == 0) {
//                out.println(x + "," + y);
//                board[x][y] = myColor;
//                myTurn = false;
//                repaint();
//            }
//        }
//        // 其他MouseListener方法省略...
//    }
//
//    public static void main(String[] args) {
//        SwingUtilities.invokeLater(() -> new GomokuClient().setVisible(true));
//    }
//}