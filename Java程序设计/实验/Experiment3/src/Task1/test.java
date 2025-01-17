//class A implements Runnable {
//    int number = 0;
//    volatile boolean stop = false; // 保证线程间可见性
//
//    boolean getStop() { return stop; }
//
//    public void run() {
//        while (true) {
//            number++;
//            System.out.println(Thread.currentThread().getName() + " : " + number);
//            if (number == 3) {
//                try {
//                    System.out.println(Thread.currentThread().getName() + " : hang up");
//                    stop = true;
//                    hangUP();
//                    System.out.println(Thread.currentThread().getName() + " : resumed");
//                } catch (Exception e) {
//                    e.printStackTrace();
//                }
//            }
//            try {
//                Thread.sleep(1000);
//            } catch (Exception e) {
//                e.printStackTrace();
//            }
//        }
//    }
//
//    public synchronized void hangUP() throws InterruptedException {
//        wait();
//    }
//
//    public synchronized void restart() {
//        notifyAll();
//    }
//}
//
//public class test {
//    public static void main(String[] args) {
//        A target = new A();
//        Thread thread = new Thread(target);
//        thread.setName("Zhang San");
//        thread.start();
//
//        while (!target.getStop()) {
//            try {
//                Thread.sleep(100); // 减少忙等
//            } catch (InterruptedException e) {
//                e.printStackTrace();
//            }
//        }
//        System.out.println("Main Thread");
//        target.restart();
//    }
//}
