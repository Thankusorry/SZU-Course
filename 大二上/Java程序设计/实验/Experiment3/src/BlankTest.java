import java.util.*;
class Student
{
    String name;
    int score;
    Student(String name, int score)
    {
        this.name = name;
        this.score = score;
    }
}
public class BlankTest
{
    public static void main(String args[])
    {
        Student stu1 = new Student("S1",78);
        Student stu2 = new Student("S2",98);
        HashSet<Student> set = new HashSet<Student>();
        HashSet<Student> subset = new HashSet<Student>();
        set.add(stu1);
        set.add(stu1);
        subset.add(stu1);
        System.out.println("set contains subset:" + set.containsAll(subset));
        // true
        Object s[] = set.toArray();
        for(int i=0; i<s.length;i++)
        {
            System.out.printf("%s:%d\n",((Student)s[i]).name, ((Student)s[i]).score);
        }
        // S2:98
        // S1:98
        // 输出类似队列
        ////////////////////
        HashSet<Student> tempSet = (HashSet<Student>)set.clone();// 记得强转    
        tempSet.removeAll(subset);
        Iterator<Student> iter = tempSet.iterator();
        while(iter.hasNext())
        {
            Student temp = iter.next();
            System.out.printf("%s:%d\n",temp.name,temp.score);
        }

    }
}