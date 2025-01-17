package Task3;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

class Print implements Runnable {
    public boolean isNumber =true;
    String [] month={"January","February","March","April",
            "May","June","July","August",
            "September", "October","November","December"};
    String [] num={"One","Two","Three","Four","Five",
            "Six","Seven","Eight","Nine",
            "Ten","Eleven","Twelve"};
    String name1,name2;
    Print(String name1,String name2){
        this.name1=name1;
        this.name2=name2;
    }
    public synchronized void run() {
        Lock lock=new ReentrantLock();
        for(int i=0;i<12;i++){
            if (Thread.currentThread().getName().equals(name1)) {
                while(!isNumber){
                    try {
                        wait();
                    } catch (InterruptedException e) {
                    }
                }
                System.out.print(i+1);
                isNumber=false;
                notifyAll();
            }
            else if (Thread.currentThread().getName().equals(name2)) {
                while(isNumber){
                    try {
                        wait();
                    } catch (InterruptedException e) {
                    }
                }
                System.out.print(num[i] + month[i]);
                isNumber=true;
                notifyAll();
            }
        }
    }
}
public class t3_3 {
    public static void main(String[] args) {
        String name1="Number",name2="Month";
        Print p1 = new Print(name1,name2);
        Thread t1 = new Thread(p1);
        Thread t2 = new Thread(p1);
        t1.setName(name1);
        t2.setName(name2);
        t1.start();
        t2.start();
    }
}
