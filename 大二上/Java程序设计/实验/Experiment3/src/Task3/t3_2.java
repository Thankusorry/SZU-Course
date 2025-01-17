package Task3;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

class PrintChar implements Runnable{
    private char charToPrint;
    private int times;

    public PrintChar (char c,int t){
        charToPrint = c;
        times=t;
    }
    @Override
    public void run() {
        for(int i=0;i<times;i++){
            System.out.print(charToPrint);
        }
    }
}
class PrintNum implements Runnable{
    private int lastNum;
    public PrintNum(int n){
        lastNum=n;
    }
    @Override
    public void run(){
        for(int i=1;i<=lastNum;i++){
            System.out.print(" "+i);
        }
    }
}
public class t3_2{
    public static void main(String[] args) {
        ExecutorService executor = Executors.newFixedThreadPool(3);
        executor.execute(new PrintChar ('a',100));
        executor.execute(new PrintChar ('b',100));
        executor.execute(new PrintNum(100));
        executor.shutdown();
    }
}
