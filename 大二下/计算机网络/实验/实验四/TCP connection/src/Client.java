import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Scanner;
import java.nio.file.Files;

public class Client {
    public static void main(String[] args) {
        String host = "localhost";
        int port = 12345;

        try (Socket socket = new Socket(host, port);
             DataInputStream dataIn = new DataInputStream(socket.getInputStream());
             DataOutputStream dataOut = new DataOutputStream(socket.getOutputStream());
             Scanner scanner = new Scanner(System.in)) {

            System.out.println("已连接到服务器");
            System.out.println("可输入命令:Time/Exit/dir/GetFile <文件名>");
            while (true) {

                System.out.print("请输入命令:");
                String command = scanner.nextLine();

                dataOut.writeUTF(command);
                dataOut.flush();
                System.out.println("发送命令：" + command);
                if (command.equalsIgnoreCase("dir")) {
                    String fileList = dataIn.readUTF();
                    System.out.println("服务器文件列表：\n" + fileList);
                }
                else if (command.startsWith("GetFile")) {
                    //接收服务器响应
                    String response = dataIn.readUTF();

                    if (response.startsWith("FileName:")) {
                        // 提取文件名
                        String fileName = response.split(":")[1];
                        System.out.println("收到文件名：" + fileName);
                        // 发送确认
                        System.out.print("是这个文件吗：Yes or NO?  ");
                        String Confirm = scanner.nextLine();
                        if(Confirm.equals("Yes")){
                            dataOut.writeUTF("Yes");
                            dataOut.flush();
                        }
                        else{
                            dataOut.writeUTF("No");
                        }
                        // 接收文件内容
                        long fileSize = dataIn.readLong();
                        byte[] fileContent = new byte[(int) fileSize];
                        dataIn.readFully(fileContent);

                        // 改名保存（例如添加前缀）

                        // 定义保存目录
                        String saveDir = "Received_Files";
                        // 创建目录（如果不存在）
                        Files.createDirectories(Paths.get(saveDir));

                        String newFileName = "client_" + fileName;
                        System.out.println("是否需要重命名? 默认名字为 "+newFileName);
                        String rename = scanner.nextLine();
                        if(rename.equals("Yes")){
                            System.out.println("请输入文件的名字：");
                            newFileName = scanner.nextLine();
                        }
                        Path savePath = Paths.get(saveDir,newFileName);
                        Files.write(savePath, fileContent);
                        System.out.println("文件保存为：" + savePath);

                    } else if (response.equals("ERROR")) {
                        System.out.println("错误：" + response.split(":")[1]);
                    }
                }
                else if(command.startsWith("test1")){
                    dataOut.writeInt(0x12345678);
                    dataOut.flush();
                }
                else if(command.startsWith("test2")){
                    dataOut.writeUTF("Time");
                    dataOut.writeUTF("dir");
                    dataOut.flush();
                }
                else {
                    String response = dataIn.readUTF();
                    System.out.println("服务器响应：" + response);
                    if (response.equals("Bye")) {
                        break;
                    }
                }
            }
        } catch (IOException e) {
            System.out.println("服务器连接已关闭");
        }
        System.out.println("客户端已关闭");
    }
}