package Task2;
import java.util.Scanner;
public class t2 {
    public static void main(String[] args) {
        for(int t=0;t<10;t++){
            Scanner sc = new Scanner(System.in);
            String str = sc.nextLine();
            StringBuffer upper=new StringBuffer("");
            StringBuffer lower=new StringBuffer("");
            StringBuffer digit=new StringBuffer("");
            for(int i=str.length()-1;i>=0;i--){
                if(Character.isUpperCase(str.charAt(i))){
                    upper.append(str.charAt(i));
                }
                else if(Character.isLowerCase(str.charAt(i))){
                    lower.append(str.charAt(i));
                }
                else if (Character.isDigit(str.charAt(i))){
                    digit.append(str.charAt(i));
                }
            }
            System.out.println(upper+" "+lower+" "+digit);
        }
    }
}
