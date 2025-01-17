package 第二周作业;

import java.util.Scanner;
public class 城市普通出租车 {
    public static void main(String[] args) {
        for(int i=0;i<5;i++){
            int money=0;
            int count=0;
            Scanner sc = new Scanner(System.in);
            int mile=sc.nextInt();
            int WaitTime=sc.nextInt();
            // 标准收费
            if(mile<=3) money=10;
            else if (mile<=10) money=10+(mile-3)*2;
            else{
                money=10+7*2+4*(mile-10);
            }
            // 加上超时费用
            money+=WaitTime/5*3;
            System.out.println("车费是"+money);
        }
    }
}
