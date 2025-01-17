package 数字加密;

import java.util.*;
public class a {
    public static void main(String args[]){
        int num;
        Scanner sc=new Scanner(System.in);
        num=sc.nextInt();
        int temp=num;
        int cnt=0;
        while(temp>0){
            temp=temp/10;
            cnt++;
        }
        int []arr =new int[cnt];
        temp=num;
        while(temp>0){
            arr[cnt-1]=temp%10;
            temp=temp/10;
            cnt--;
        }
        for(int i=0;i<arr.length;i++){
            arr[i]=(arr[i]+5)%10;
        }
        reverse(arr);
        for(int i=0;i<arr.length;i++){
            System.out.print(arr[i]+" ");
        }
    }
    public static void reverse (int []arr){
        for(int i=0,j=arr.length-1;i<j;i++,j--) {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

}
