package A.API;

import java.util.regex.*;
import java.util.Scanner;
public class test
{
    public static void main (String args[])
    {
        Scanner reader = new Scanner(System.in);
        String str = reader.nextLine();
        //空格字符、数字和符号(!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)组成的正则表达式
        String regex = "[\\s\\d\\p{Punct}]+";
        String words[] = str.split(regex);
        for(int i=0; i<words.length; i++)
        {
            int m = i+1;
            System.out.println("Word" + m + ":" + words[i]);
        }
    }
}



