package Task1;
import java.util.*;
public class t1 {
    public static void main(String[] args) {
        String input;
        Scanner sc=new Scanner (System.in);
        input=sc.nextLine();
        StringBuilder str =new StringBuilder(input);
        for(int i=0;i<str.length();i++){
            if(str.charAt(i)=='.' || str.charAt(i)==',') str.delete(i,i+1);
        }
        input=str.toString();
        StringTokenizer st=new StringTokenizer(input," ");
        String [] words=new String[st.countTokens()];
        int [] count=new int[st.countTokens()];
        for(int i=0;st.hasMoreTokens();i++){
            words[i]=st.nextToken();
            count[i]=1;
        }
        int [] count2=new int[st.countTokens()];
        int cnt=0;
        String [] newwords=new String[words.length];
        for(int i=0;i<words.length;i++){
            if(count[i]==-1){continue;}
            count2[cnt]=1;
            newwords[cnt]=words[i];
            for(int j=i+1;j<words.length;j++){
                if(words[j].equals(words[i])){
                    count2[cnt]++;
                    count[j]=-1;
                }
            }
            cnt++;
        }
        for(int i=0;i<cnt;i++){
            System.out.print(newwords[i]+" : "+count2[i]+"; ");
        }
    }
}
