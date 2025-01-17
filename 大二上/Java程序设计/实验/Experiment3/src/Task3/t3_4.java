package Task3;
class Account {
    private double balance =0;
    Account (double balance){this.balance=balance;}
    public  void withdraw(double amount){
        double newBalance = balance - amount;
        if(newBalance > 0){
            balance = newBalance;
            System.out.println(Thread.currentThread().getName() + " 取款成功，剩余余额：" + balance);
        }
        else {
            System.out.println(Thread.currentThread().getName() + " 取款失败，余额不足！");
        }
        try{
            Thread.sleep(100);
        }
        catch (InterruptedException ex){
        }
    }
}
class Withdraw implements Runnable{
    private Account account;
    private double amount;
    public Withdraw(Account account, double amount){
        this.account = account;
        this.amount = amount;
    }
    public synchronized  void run(){
        account.withdraw(amount);
    }
}
public class t3_4 {
    public static void main(String[] args) {
        Account account =new Account(1000);
        Runnable task1 = new Withdraw(account, 500);
        Runnable task2 = new Withdraw(account, 700);
        Runnable task3 = new Withdraw(account, 300);
        Thread thread1 = new Thread(task1, "线程1");
        Thread thread2 = new Thread(task2, "线程2");
        Thread thread3 = new Thread(task3, "线程3");
        thread1.start();
        thread2.start();
        thread3.start();

    }
}