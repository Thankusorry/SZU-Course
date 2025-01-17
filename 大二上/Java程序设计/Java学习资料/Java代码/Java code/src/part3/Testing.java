package A.part3;

interface Human{
    void sayHello();
}

class Chinese implements Human{
    public void sayHello(){
        System.out.println("你好");
    }
}
class French implements Human{
    public void sayHello(){
        System.out.println("Bonjour");
    }
}
class Japanese implements Human{
    public void sayHello(){
        System.out.println("こんにちは");
    }
}
public class Testing {
    public static void main(String[] args) {
        Human[] humans = new Human[3];
        humans[0]=new Chinese();
        humans[1]=new French();
        humans[2]=new Japanese();
        for(int i=0;i<3;i++) {
            humans[i].sayHello();
        }
    }
}
