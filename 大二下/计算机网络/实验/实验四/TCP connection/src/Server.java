import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.Date;
import java.util.Map;
import java.util.*;
public class Server {
    public static StudentDB studentDB;

    public static void main(String[] args) {

        // 初始化学生数据库（二选一）
        try {
            // 方式1：从文件加载
            studentDB = new StudentDB("student_data.csv");

            // 方式2：生成随机数据（当文件不存在时）
            if (studentDB.size() == 0) {
                System.out.println("检测到空数据库，生成随机数据...");
                studentDB = new StudentDB(100);
                studentDB.exportToFile("student_data.csv");
            }

            System.out.println("成功加载 " + studentDB.size() + " 条学生记录");
        } catch (IOException e) {
            System.err.println("数据库初始化失败: " + e.getMessage());
            return;
        }

        //服务器根据端口号创建Serversocket
        int port = 12345;
        try (ServerSocket serverSocket = new ServerSocket(port)) {
            System.out.println("服务器启动，监听端口：" + port);
            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("客户端连接：" + clientSocket.getRemoteSocketAddress());
                new Thread(new ClientHandler(clientSocket)).start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

class ClientHandler implements Runnable {
    private final Socket clientSocket;

    public ClientHandler(Socket socket) {
        this.clientSocket = socket;
    }

    @Override
    public void run() {
        try (DataInputStream dataIn = new DataInputStream(clientSocket.getInputStream());
             DataOutputStream dataOut = new DataOutputStream(clientSocket.getOutputStream())) {

            while (true) {
                //  定义服务器文件存储路径（固定目录）
                String serverFileDir = "server_files/";
                String command = dataIn.readUTF();
                System.out.println("收到命令：" + command);
                if (command.equalsIgnoreCase("Time")) {
                    String currentTime = new Date().toString();
                    dataOut.writeUTF(currentTime);
                    dataOut.flush();
                    System.out.println("发送时间：" + currentTime);
                }
                else if (command.equalsIgnoreCase("Exit")) {
                    dataOut.writeUTF("Bye");
                    dataOut.flush();
                    System.out.println("发送Bye，关闭连接");
                    break;
                }
                else if (command.equalsIgnoreCase("dir")){
                    File dir=new File(serverFileDir);
                    if(!dir.exists() || !dir.isDirectory()){
                        dataOut.writeUTF("ERROR:服务器目录不存在");
                        continue;
                    }
                    // 获取文件列表
                    try{
                        String[] files = dir.list();
                        if(files==null || files.length==0){
                            dataOut.writeUTF("服务器目录为空");
                        }
                        else {
                            String fileList =String.join("\n",files);
                            dataOut.writeUTF(fileList);
                        }
                        dataOut.flush();
                        System.out.println("已发送文件列表");
                    }
                    catch(Exception e){
                        dataOut.writeUTF("ERROR:无法读取目录");
                    }

                }
                else if (command.startsWith("GetFile")) {
                    String[] parts = command.split(" ", 2);
                    if (parts.length < 2) {
                        System.out.println(parts[0]+" "+parts[1]);
                        dataOut.writeUTF("ERROR: 无效命令");
                        continue;
                    }
                    String filePath = parts[1];
                    System.out.println("尝试读取文件：" +serverFileDir+filePath);
                    File file = new File(serverFileDir+filePath);
                    if (!file.exists() || !file.isFile()) {
                        dataOut.writeUTF("ERROR: 文件不存在");
                        continue;
                    }

                    // 2. 先发送文件名
                    String fileName = file.getName();
                    dataOut.writeUTF("FileName:" + fileName); // 发送文件名标识
                    dataOut.flush();

                    // 3. 等待客户端确认
                    String ack = dataIn.readUTF();
                    if (!"Yes".equals(ack)) {
                        System.out.println("客户端未确认，终止传输");
                        continue;
                    }
                    System.out.println("文件已确认,传输中....");
                    // 4. 发送文件内容
                    byte[] fileContent = Files.readAllBytes(file.toPath());
                    dataOut.writeLong(fileContent.length); // 发送文件大小
                    dataOut.write(fileContent); // 发送内容
                    dataOut.flush();
                    System.out.println("文件内容已发送");
                    byte[] fileNameBytes = fileName.getBytes(StandardCharsets.UTF_8);
                    System.out.println("文件发送完毕：" + fileName);

                }
                else if (command.startsWith("Query")){
                    String[] parts = command.split(" ", 2);
                    if (parts.length < 2) {
                        dataOut.writeUTF("ERROR: 无效命令格式");
                        dataOut.flush();
                        continue;
                    }

                    String[] info = Server.studentDB.getStudentInfo(parts[1]);

                    if (info == null) {
                        dataOut.writeUTF("ERROR: 学号不存在");
                    } else {
                        dataOut.writeUTF(String.format("查询成功 :姓名:%s,性别:%s", info[0], info[1]));
                    }
                    dataOut.flush();
                }
                else if (command.startsWith("test1")) {
                    int received = dataIn.readInt();
                    if (received == 0x12345678) {
                        System.out.println("收到"+received+" 字节序正确");
                    } else {
                        System.out.println("❌ 字节序错误，收到: 0x" + Integer.toHexString(received));
                    }
                }
                else {
                    dataOut.writeUTF("未知命令");
                    dataOut.flush();
                }
            }
        } catch (IOException e) {
            System.out.println("客户端连接异常关闭");
        } finally {
            try {
                clientSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}