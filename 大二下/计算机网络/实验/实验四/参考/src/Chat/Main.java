package Chat;

import java.io.IOException;

public class Main {
    public static void main(String[] args) throws Exception {
        // 在新线程中启动服务器
        new Thread(new Runnable() {
            public void run() {
                Server server = new Server();
                try {
                    server.start();
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }
        }).start();
        Thread.sleep(1000);
        // 在新线程中启动每个客户端
        new Thread(new Runnable() {
            public void run() {
                Client client_1 = new Client("张三");
                try {
                    client_1.run();
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }
        }).start();
        Thread.sleep(1000);

        new Thread(new Runnable() {
            public void run() {
                Clientt client_2 = new Clientt("李四");
                try {
                    client_2.run();
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }
        }).start();
        Thread.sleep(1000);


//        new Thread(new Runnable() {
//            public void run() {
//                Client3 client_3 = new Client3("王五");
//                try {
//                    client_3.run();
//                } catch (Exception e) {
//                    throw new RuntimeException(e);
//                }
//            }
//        }).start();
//        Thread.sleep(1000);
    }
}
