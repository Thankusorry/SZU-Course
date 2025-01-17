//import java.util.concurrent.locks.Lock;
//import java.util.concurrent.locks.ReentrantLock;
//
//public static class Account{
//    private static Lock lock =new ReentrantLock();
//    private int balance=0;
//
//    public int getBalance(){
//        return balance;
//    }
//    public void deposit(int amount){
//        lock.lock(); // Acquire the lock
//        try {
//            int newBalance=balance+amount;
//
//            // This delay is deliberately added to magnify the
//            // data-corruption problem and make it easy to see.
//            Thread.sleep(5);
//            balance=newBalance;
//        }
//        catch(InterruptedException e){}
//        finally {
//            lock.unlock(); // Release the lock
//        }
//    }
//}