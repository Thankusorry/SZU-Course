//import java.text.SimpleDateFormat;
//import java.util.*;
//class Employee implements Comparable{
//    String name;
//    int age;
//    Date birthday;
//    Employee(String s1, int a, Date d) {
//        name=s1;age=a;birthday=d;
//    }
//    public int compareTo(Object o){
//        Employee a=(Employee)o;
//        if(a.name.equals(this.name)){
//            if(this.birthday.getTime()-a.birthday.getTime()<0)
//                return 1;
//            else
//                return -1;
//        }
//        return this.name.compareTo(a.name);
//    }
//}
//public class EmployeeTest {
//    public static void main(String[] args) {
//        Calendar a=Calendar.getInstance();
//        a.set(1970,1,1);
//        long time=a.getTimeInMillis();
//        String name1="liudehua",name2="zhangxueyou",
//                name3="liudehua",name4="liming",name5="liangchaowei";
//        int a1=57,a2=45,a3=44,a4=58,a5=50;
//        Calendar b=Calendar.getInstance();
//        Calendar c=Calendar.getInstance();
//        Calendar d=Calendar.getInstance();
//        Calendar e=Calendar.getInstance();
//        Calendar f=Calendar.getInstance();
//        b.set(1965,5,4);
//        c.set(1977,5,4);
//        d.set(1978,5,9);
//        e.set(1964,8,12);
//        f.set(1972,12,4);
//        Date b1=new Date(b.getTimeInMillis()-time);
//        Date c1=new Date(c.getTimeInMillis()-time);
//        Date d1=new Date(d.getTimeInMillis()-time);
//        Date e1=new Date(d.getTimeInMillis()-time);
//        Date f1=new Date(f.getTimeInMillis()-time);
//        TreeSet<Employee> t=new TreeSet<Employee>();
//        Employee A=new Employee(name1,a1,b1);
//        Employee B=new Employee(name2,a2,c1);
//        Employee C=new Employee(name3,a3,d1);
//        Employee D=new Employee(name4,a4,e1);
//        Employee E=new Employee (name5,a5,f1);
//        t.add(A);
//        t.add(B);
//        t.add(C);
//        t.add(D);
//        t.add(E);
//        Iterator<Employee>iter=t.iterator();
//        while(iter.hasNext()){
//            Employee temp=iter.next();
//            SimpleDateFormat sdf=new SimpleDateFormat("yyyy.MM.dd");
//            System.out.println("Name:"+temp.name+" Age:"+temp.age+" Birthday:"+sdf.format(temp.birthday));
//        }
//    }
