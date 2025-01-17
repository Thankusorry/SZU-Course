package 第二周作业;

public class 闰年 {
    public static void main(String[] args) {
        for(int i=2000;i<3000;i++){
            if((i%4==0 && i%100!=0)||(i%400==0)){
                System.out.printf("%d ",i);
            }
        }
    }
}
