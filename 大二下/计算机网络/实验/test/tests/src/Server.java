import javafx.application.Application;
import javafx.application.Platform;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.TextArea;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

import zhou.pane.FiveChessGameConstants;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Date;

public class FiveChessGameServer extends Application implements FiveChessGameConstants
{

    public static void main(String[] args)
    {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage)
    {
        // UI界面
        TextArea textArea = new TextArea();
        textArea.setEditable(false);

        Scene scene = new Scene(new Pane(textArea), 650, 250);
        // 文本域背景透明
//        scene.getStylesheets().add(getClass().getResource("BackgroundColor.css").toExternalForm());
        primaryStage.setScene(scene);
        primaryStage.setTitle("五子棋服务器");
        primaryStage.show();

        // 关闭线程
        primaryStage.setOnCloseRequest(new EventHandler<WindowEvent>()
        {
            @Override
            public void handle(WindowEvent event)
            {
                System.exit(0);
            }
        });

        new Thread(() ->
        {
            ServerSocket serverSocket = null;
            try
            {
                // 一个服务器端口
                serverSocket = new ServerSocket(9520);
                Platform.runLater(() ->
                {
                    textArea.appendText(new Date() + "：服务器端口为9520！\n");
                });
                // 等待用户连接
                while (true)
                {
                    Platform.runLater(() ->
                    {
                        textArea.appendText("\n" + new Date() + "：等待玩家连接。。。\n");
                    });
                    // 等待用户一连接
                    Socket user1 = serverSocket.accept();
                    Platform.runLater(() ->
                    {
                        textArea.appendText(new Date() + "：玩家一连接成功！\n");
                        textArea.appendText("玩家一的名称和IP地址为：" + user1.getInetAddress().getHostName() + " "
                                + user1.getInetAddress().getHostAddress() + "\n");
                    });
                    // IO流
                    DataOutputStream toUser1 = new DataOutputStream(user1.getOutputStream());
                    // 提示玩家一其为黑子
                    toUser1.writeInt(player1);

                    // 等待用户二连接
                    Socket user2 = serverSocket.accept();
                    Platform.runLater(() ->
                    {
                        textArea.appendText(new Date() + "：玩家二连接成功！\n");
                        textArea.appendText("玩家二的名称和IP地址为：" + user2.getInetAddress().getHostName() + " "
                                + user2.getInetAddress().getHostAddress() + "\n");
                    });
                    // IO流
                    DataOutputStream toUser2 = new DataOutputStream(user2.getOutputStream());
                    // 提示玩家二其为白子
                    toUser2.writeInt(player2);

                    // 开始一局游戏
                    new Thread(new HandleASession(user1, user2)).start();
                }

            } catch (IOException e)
            {
                e.printStackTrace();
            }

        }).start();
    }
}
————————————————

版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。

原文链接：https://blog.csdn.net/qq_43703976/article/details/105083107