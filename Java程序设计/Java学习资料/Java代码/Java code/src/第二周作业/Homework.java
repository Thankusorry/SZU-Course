package 第二周作业;

class Student {
    // 成员变量
    private String name;
    private int age;
    private float score;

    // 设置 name 方法
    public void setName(String name) {
        this.name = name;
    }

    // 获取 name 方法
    public String getName() {
        return name;
    }

    // 设置 age 方法
    public void setAge(int age) {
        this.age = age;
    }

    // 获取 age 方法
    public int getAge() {
        return age;
    }

    // 设置 score 方法
    public void setScore(float score) {
        this.score = score;
    }

    // 获取 score 方法
    public float getScore() {
        return score;
    }

    // 类内部的 main 方法
    public static void main(String[] args) {
        // 在 Student 类内创建对象并调用方法
        Student student = new Student();
        student.setName("Alice");
        student.setAge(20);
        student.setScore(89.5f);

        System.out.println("Name: " + student.getName());
        System.out.println("Age: " + student.getAge());
        System.out.println("Score: " + student.getScore());
    }
}

// 在类外的 main 方法
public class Homework{
    public static void main(String[] args) {
        // 在类外创建对象并调用方法
        Student student = new Student();
        student.setName("Bob");
        student.setAge(22);
        student.setScore(92.3f);

        System.out.println("Name: " + student.getName());
        System.out.println("Age: " + student.getAge());
        System.out.println("Score: " + student.getScore());
    }
}
