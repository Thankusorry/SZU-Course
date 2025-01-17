/*
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class Student{
    String name;
    int age;
    float score;
    Student(String name, int age, float score){
        this.name = name;
        this.age = age;
        this.score = score;
    }
    public String toString(){
        return "Name:"+name+" Age:"+age+" Score:"+score;
    }
}
class StudentParser {
    public static List<Student> parseStudents(String csvData){
        List<Student> students = new ArrayList<Student>();
        String[] lines=csvData.split("\\n");

        for(String line:lines){
            String[]parts=line.split(",");

            String name=parts[0];
            int age=Integer.parseInt(parts[1]);
            float score=Float.parseFloat(parts[2]);
            students.add(new Student(name,age,score));
        }
        return students;
    }
}

public class Task3 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        StringBuffer csvData=new StringBuffer();
        while(true){
            String line=scanner.nextLine();
            if(line.isEmpty()) break;
            csvData.append(line+"\n");
        }
        List<Student> students = StudentParser.parseStudents(csvData.toString());
        for(Student student:students){
            System.out.println(student);
        }
    }
}
*/
