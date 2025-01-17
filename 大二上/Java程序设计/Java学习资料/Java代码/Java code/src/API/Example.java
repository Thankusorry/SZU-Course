package A.API;

interface Computable{
    int[] add(int[] a,int[] b);
    int[] minus(int[] a,int[] b);
    int[] elementwiseProduct(int[] a,int[] b);
}
class VectorCal implements Computable{
    public int[] add(int[] a,int[] b){
        int[] c = new int[5];
        for(int i=0;i<5;i++){
            c[i]=a[i]+b[i];
        }
        return c;
    }
    public int[] minus(int[] a,int[] b){
        int[] c = new int[5];
        for(int i=0;i<5;i++){
            c[i]=a[i]-b[i];
        }
        return c;
    }
    public int[] elementwiseProduct(int[] a,int[] b){
        int[] c = new int[5];
        for(int i=0;i<5;i++){
            c[i]=a[i]*b[i];
        }
        return c;
    }
    public void display(int[] result){
        System.out.print("(");
        for(int i=0;i<5;i++) {
            System.out.print(result[i]);
            if (i < 4) System.out.print(",");
        }
        System.out.println(")");
    }
}
public class Example {
    public static void main(String[] args) {
        int[] a={5,2,1,8,10};
        int[] b={3,-1,0,-4,1};
        VectorCal v=new VectorCal();
        System.out.println("a add b :");
        v.display(v.add(a,b));
        System.out.println("a minus b :");
        v.display(v.minus(a,b));
        System.out.println("a elementwise product b :");
        v.display(v.elementwiseProduct(a,b));
    }
}
