class Print implements Runnable {
    String name1,name2;
    Boolean Num=true;
    Print(String s1,String s2){
        name1=s1;
        name2=s2;
    }
    int a=1;
    char A='A';
    public  void run(){

            if(Thread.currentThread().getName().equals(name1)){
                while(true) {
                    while(!Num){
                        try {
                            Thread.sleep(100);
                        }
                        catch(InterruptedException e){
                        }
                    }
                    System.out.print(a);
                    Num = false;
                    if(a==13) break;
                    a++;


                }

            }
            else if (Thread.currentThread().getName().equals(name2)){

                while (true){
                    while(Num) {
                        try {
                            Thread.sleep(100);
                        } catch (InterruptedException e) {
                        }
                    }
                    System.out.print(A++);
                    System.out.print(A++);
                    if(A>'Z')break;
                    Num=true;

                }

            }

    }
}
public class test{
    public static void main(String []args){
        String name1="Number",name2="Char";
        Print p=new Print(name1,name2);
        Thread t1=new Thread(p);
        Thread t2=new Thread(p);
        t1.setName(name1);
        t2.setName(name2);
        t1.start();
        t2.start();
    }
}