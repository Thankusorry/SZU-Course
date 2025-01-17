import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
public class t1 {
    public static void main(String[] args) {
        File logFile = new File("C:/logs.txt.txt");
        // 1.判断日志文件是否存在
        if(!logFile.exists()){
            System.out.println("日志文件缺失，程序退出");
            return;
        }
        //2.统计日志级别数量
        List<String > errorlogs = new ArrayList<String >();
        List<String> warnlogs=new ArrayList<>();
        List<String> infoLogs=new ArrayList<>();
        HashMap<String,Integer> Count =new HashMap<>();
        // 3.读取日志文件
        try{
            BufferedReader reader=new BufferedReader(new FileReader(logFile));
            String line;
            while((line=reader.readLine())!=null){
                // 每行空格分成三部分
                String[] parts =line.split(" ",4);
                String type=parts[2];
                // 统计数量
                Count.put(type,Count.getOrDefault(type,0) + 1);
                switch(type) {
                    case "[ERROR]":
                        errorlogs.add(line);
                        break;
                    case "[WARN]":
                        warnlogs.add(line);
                        break;
                    case "[INFO]":
                        infoLogs.add(line);
                        break;
                }
            }
        }
        catch(IOException e){
            System.out.println("读取日志错误");
            return;
        }
        //输出数量统计结果
        System.out.println("日志数量统计结果：");
        Count.forEach((type, count) -> System.out.println(type + ": " + count));
        // 将分类后的日志写入文件
        String error="error_logs.txt";
        String warn="warn_logs.txt";
        String info="info_logs.txt";
        logsToFile(error,errorlogs);
        logsToFile(warn,warnlogs);
        logsToFile(info,infoLogs);
        // 读取分类后文件
        System.out.println("分类后的日志:");
        printFile(error);
        printFile(warn);
        printFile(info);
    }
    public static void logsToFile(String filename,List<String> logs){
        try(BufferedWriter writer =new BufferedWriter(new FileWriter(filename))){
            for(String log:logs){
                writer.write(log);
                writer.newLine();
            }
        }
        catch(IOException e){
            System.out.println("写入"+filename+"错误");
            return;
        }
    }
    public static void printFile(String filename){
        System.out.println(filename+":");
        try(BufferedReader reader =new BufferedReader(new FileReader(filename))){
            String line;
            while((line =reader.readLine())!=null){
                System.out.println(line);
            }
        }
        catch(IOException e){
            System.out.println("读取"+filename+"出错");
        }
    }
}