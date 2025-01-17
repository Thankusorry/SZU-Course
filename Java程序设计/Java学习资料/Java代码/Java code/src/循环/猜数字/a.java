package A.猜数字;

import java.util.Random;
import java.util.Scanner;

public class a {
    public static void main(String args[]){
        Random rand = new Random();
        int num=rand.nextInt(100)+1;
        Scanner sc=new Scanner(System.in);
        while(true) {
            int A = sc.nextInt();
            if (A > num) {
                System.out.println("Larger");
            } else if (A < num) {
                System.out.println("Smaller");
            } else {
                System.out.println("Bingo");
                break;
            }
        }

    }
}
