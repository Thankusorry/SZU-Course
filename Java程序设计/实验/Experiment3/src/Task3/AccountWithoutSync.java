package Task3;
import java.util.concurrent.*;
public class AccountWithoutSync {
    private static Account account=new Account();
    public static void main(String[] args) {
        ExecutorService executor = Executors.newCachedThreadPool();
        // 创造和启动100个线程
        for(int i=0;i<100;i++){
            executor.execute(new AddAPennyTask());
        }
        executor.shutdown();
        // 等待所有的任务完成
        while(!executor.isTerminated()){
        }
        System.out.println("What is balance? "+account.getBalance());
    }
    // 线程——用于存款
    private static class AddAPennyTask implements  Runnable{
        public  void run(){
            account.deposit(1);
        }
    }
    // 内部类 account
    private static class Account{
        private int balance =0;
        public int getBalance(){
            return balance;
        }
        public synchronized void deposit(int amount){
            int newBalance = balance + amount;
            try{
                Thread.sleep(5);
            }
            catch (InterruptedException ex){
            }
            balance = newBalance;
        }
    }
}