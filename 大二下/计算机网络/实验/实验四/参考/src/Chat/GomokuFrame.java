package Chat;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.*;
import java.net.Socket;

public class GomokuFrame extends JFrame {
    private static final int BOARD_SIZE = 15;
    private static final int CELL_SIZE = 40;
    private static final int OFFSET = 30;
    private int[][] board = new int[BOARD_SIZE][BOARD_SIZE];
    private int currentPlayer;
    private final String roomId;
    private Socket gameSocket;
    private PrintWriter out;

    public GomokuFrame(String roomId, boolean isFirst) {
        this.roomId = roomId;
        this.currentPlayer = isFirst ? 1 : 2;

        setTitle("五子棋对战 - 房间:" + roomId);
        setSize(CELL_SIZE * (BOARD_SIZE - 1) + OFFSET * 2, CELL_SIZE * (BOARD_SIZE - 1) + OFFSET * 2 + 50);
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        setLocationRelativeTo(null);

        GameCanvas gameCanvas = new GameCanvas();
        gameCanvas.addMouseListener(new GameMouseListener());
        add(gameCanvas);

        connectToGameServer();
    }

    private void connectToGameServer() {
        new Thread(() -> {
            try {
                gameSocket = new Socket("localhost", 5999);
                out = new PrintWriter(gameSocket.getOutputStream(), true);
                BufferedReader in = new BufferedReader(new InputStreamReader(gameSocket.getInputStream()));

                // 发送加入房间请求
                out.println("JOIN:" + roomId);

                // 监听服务器消息
                String message;
                while ((message = in.readLine()) != null) {
                    handleServerMessage(message);
                }
            } catch (IOException e) {
                JOptionPane.showMessageDialog(this, "连接游戏服务器失败");
                dispose();
            }
        }).start();
    }

    private void handleServerMessage(String message) {
        if (message.startsWith("MOVE:")) {
            String[] parts = message.split(":");
            int x = Integer.parseInt(parts[1]);
            int y = Integer.parseInt(parts[2]);
            SwingUtilities.invokeLater(() -> {
                board[x][y] = currentPlayer == 1 ? 2 : 1;
                repaint();
                checkWinner(x, y);
                currentPlayer = currentPlayer == 1 ? 2 : 1;
            });
        }
    }

    class GameCanvas extends JPanel {
        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            drawChessboard(g);
            drawStones(g);
        }

        private void drawChessboard(Graphics g) {
            // 设置棋盘背景色
            g.setColor(new Color(222, 184, 135));
            g.fillRect(0, 0, getWidth(), getHeight());

            // 绘制棋盘线
            g.setColor(Color.BLACK);
            for (int i = 0; i < BOARD_SIZE; i++) {
                int pos = OFFSET + i * CELL_SIZE;
                // 水平线
                g.drawLine(OFFSET, pos, OFFSET + (BOARD_SIZE - 1) * CELL_SIZE, pos);
                // 垂直线
                g.drawLine(pos, OFFSET, pos, OFFSET + (BOARD_SIZE - 1) * CELL_SIZE);
            }

            // 绘制星位
            drawStarPoint(g, 3, 3);
            drawStarPoint(g, 3, 11);
            drawStarPoint(g, 11, 3);
            drawStarPoint(g, 11, 11);
            drawStarPoint(g, 7, 7);
        }

        private void drawStarPoint(Graphics g, int x, int y) {
            int posX = OFFSET + x * CELL_SIZE - 3;
            int posY = OFFSET + y * CELL_SIZE - 3;
            g.fillOval(posX, posY, 6, 6);
        }

        private void drawStones(Graphics g) {
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (board[i][j] != 0) {
                        int x = OFFSET + i * CELL_SIZE;
                        int y = OFFSET + j * CELL_SIZE;
                        g.setColor(board[i][j] == 1 ? Color.BLACK : Color.WHITE);
                        g.fillOval(x - 15, y - 15, 30, 30);
                        g.setColor(Color.BLACK);
                        g.drawOval(x - 15, y - 15, 30, 30);
                    }
                }
            }
        }
    }

    class GameMouseListener extends MouseAdapter {
        @Override
        public void mouseClicked(MouseEvent e) {
            if (currentPlayer != 1) return;

            int x = (e.getX() - OFFSET + CELL_SIZE / 2) / CELL_SIZE;
            int y = (e.getY() - OFFSET + CELL_SIZE / 2) / CELL_SIZE;

            if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[x][y] == 0) {
                board[x][y] = 1;
                repaint();
                out.println("MOVE:" + x + ":" + y);
                checkWinner(x, y);
                currentPlayer = 2;
            }
        }
    }

    private void checkWinner(int x, int y) {
        if (checkLine(x, y, 1, 0) ||  // 水平
                checkLine(x, y, 0, 1) ||  // 垂直
                checkLine(x, y, 1, 1) ||  // 主对角线
                checkLine(x, y, 1, -1)) { // 副对角线

            JOptionPane.showMessageDialog(this, (board[x][y] == 1 ? "黑方" : "白方") + "获胜！");
            dispose();
        }
    }

    private boolean checkLine(int x, int y, int dx, int dy) {
        int player = board[x][y];
        int count = 1;

        // 正向检查
        int i = x + dx, j = y + dy;
        while (i >= 0 && i < BOARD_SIZE &&
                j >= 0 && j < BOARD_SIZE &&
                board[i][j] == player) {
            count++;
            i += dx;
            j += dy;
        }

        // 反向检查
        i = x - dx;
        j = y - dy;
        while (i >= 0 && i < BOARD_SIZE &&
                j >= 0 && j < BOARD_SIZE &&
                board[i][j] == player) {
            count++;
            i -= dx;
            j -= dy;
        }

        return count >= 5;
    }

    public static void main(String[] args) {
        // 测试代码
        SwingUtilities.invokeLater(() -> {
            GomokuFrame frame = new GomokuFrame("TEST_ROOM", true);
            frame.setVisible(true);
        });
    }
}
