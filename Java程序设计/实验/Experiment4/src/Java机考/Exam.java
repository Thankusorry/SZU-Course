//package Java机考;
//
//import javax.swing.*;
//import javax.swing.border.EmptyBorder;
//import java.awt.*;
//import java.awt.event.ActionEvent;
//import java.awt.event.ActionListener;
//import java.util.Collections;
//import java.util.LinkedList;
//import java.util.Random;
//
//class OneChoice {
//    String question,option1,option2
//            ,option3,option4,answer;
//    OneChoice(String a,String b,String c,String d,String e,String g) {
//        question=a;option1=b;option2=c;
//        option3=d;option4=e;answer=g;
//    }
//}
//class MoreChoice{
//    String question,option1,option2
//            ,option3,option4,answer;
//    MoreChoice(String a,String b,String c,String d,String e,String g) {
//        question=a;option1=b;option2=c;
//        option3=d;option4=e;answer=g;
//    }
//}
//class TF{
//    String question;
//    Boolean answer;
//    TF(String a,Boolean b){
//        question=a;answer=b;
//    }
//}
//class Questions{
//    LinkedList<OneChoice> oneChoices=new LinkedList<OneChoice>();
//    LinkedList<MoreChoice> moreChoices=new LinkedList<MoreChoice>();
//    LinkedList<TF> tfs=new LinkedList<TF>();
//    Questions(){
//        // 添加单选题
//        oneChoices.add(new OneChoice("被称为JAVA之父的是?",
//                "A.Rod Johnson",
//                "B.James Gosling",
//                "C.Marc Fleury",
//                "D.Gavin King",
//                "B"));
//        oneChoices.add(new OneChoice("Java是高级编程语言的一种。由什么公司于1995年5月推出",
//                "A.SUNB",
//                "B.甲骨文",
//                "C.微软",
//                "D.索尼",
//                "A"));
//        oneChoices.add(new OneChoice("Java的前身名字叫?",
//                "A.Oracle",
//                "B.mysql",
//                "C.spring",
//                "D.OAK",
//                "D"));
//        oneChoices.add(new OneChoice("什么可将一个java文件转换成一个class文件?",
//                "A.调试程序",
//                "B.编译程序",
//                "C.转换器程序 ",
//                "D.JRE",
//                "B"));
//        oneChoices.add(new OneChoice(" jdk文件夹中的lib文件夹用于存放什么文件?",
//                "A.java文件",
//                "B.开发工具使用的归档包文件",
//                "C.开发工具使用的归档包文件",
//                "D.日志文件",
//                "B"));
//        oneChoices.add(new OneChoice("cmd下编译Java程序使用的命令是?",
//                "A.java",
//                "B.java",
//                "C.java -version",
//                "D.javac",
//                "A"));
//        oneChoices.add(new OneChoice(" 项目中src文件夹代表含义?",
//                "A.源文件目录",
//                "B.配置文件目录",
//                "C.编译文件目录",
//                "D.编译文件目录",
//                "A"));
//        oneChoices.add(new OneChoice("哪个关键字可以用于定义抽象类？",
//                "A.abstract",
//                "B.interface",
//                "C.static",
//                "D.final",
//                "A"));
//        oneChoices.add(new OneChoice("哪个关键字用于创建一个新的对象？",
//                "A.new",
//                "B.create",
//                "C.make",
//                "D.build",
//                "A"));
//        oneChoices.add(new OneChoice("哪个不是Java的基本数据类型？",
//                "A.int",
//                "B.bool",
//                "C.double",
//                "D.String",
//                "B"));
//        // 添加多选题
//        moreChoices.add(new MoreChoice("Java与C语言的区别有哪些",
//                "A.面向对象",
//                "B.多线程",
//                "C.半编译半解释",
//                "D.健壮的",
//                "AC"));
//        moreChoices.add(new MoreChoice("Java语言的特点有哪些",
//                "A.跨平台性",
//                "B.多继承",
//                "C.垃圾回收机制",
//                "D.动态绑定",
//                "ACD"));
//        moreChoices.add(new MoreChoice("以下哪些属于Java的访问控制修饰符",
//                "A.private",
//                "B.protected",
//                "C.static",
//                "D.public",
//                "ABD"));
//        moreChoices.add(new MoreChoice("以下关于Java接口的描述正确的是",
//                "A.接口可以有静态方法",
//                "B.接口支持多继承",
//                "C.接口可以有默认方法",
//                "D.接口中可以包含构造方法",
//                "ABC"));
//        moreChoices.add(new MoreChoice("以下哪些属于Java的集合框架类",
//                "A.ArrayList",
//                "B.HashMap",
//                "C.TreeNode",
//                "D.LinkedHashSet",
//                "ABD"));
//        moreChoices.add(new MoreChoice("以下关于Java异常处理的说法正确的是",
//                "A.异常分为受检异常和运行时异常",
//                "B.finally块中的代码一定会执行",
//                "C.throw关键字用于抛出异常",
//                "D.可以捕获Error类型的错误",
//                "ABC"));
//        moreChoices.add(new MoreChoice("以下哪些是Java的基本数据类型",
//                "A.int",
//                "B.String",
//                "C.float",
//                "D.char",
//                "ACD"));
//        moreChoices.add(new MoreChoice("以下哪些属于Java中的继承特性",
//                "A.单继承",
//                "B.支持继承和接口实现",
//                "C.父类的private成员可以被子类继承",
//                "D.子类可以重写父类的非final方法",
//                "ABD"));
//        moreChoices.add(new MoreChoice("以下哪些属于Java虚拟机的内存区域",
//                "A.堆",
//                "B.方法区",
//                "C.栈",
//                "D.数据区",
//                "ABC"));
//        moreChoices.add(new MoreChoice("以下关于泛型的说法正确的是",
//                "A.泛型可以用在类、接口和方法中",
//                "B.泛型的类型参数可以为基本数据类型",
//                "C.可以通过通配符来表示不确定的类型",
//                "D.泛型在运行时会被类型擦除",
//                "ACD"));
//        // 添加判断题
//        tfs.add(new TF("Java是一门支持跨平台的编程语言。", true));
//        tfs.add(new TF("Java中的所有类都必须显式继承Object类。", false));
//        tfs.add(new TF("Java支持多继承，因此一个类可以同时继承多个父类。", false));
//        tfs.add(new TF("Java中的String是不可变的。", true));
//        tfs.add(new TF("使用`System.gc()`可以强制Java垃圾回收。", false));
//        tfs.add(new TF("Java的数组长度是固定的，不能动态改变。", true));
//        tfs.add(new TF("`HashMap`是线程安全的。", false));
//        tfs.add(new TF("`final`关键字可以用于修饰类、方法和变量。", true));
//        tfs.add(new TF("Java支持运算符重载。", false));
//        tfs.add(new TF("`public static void main(String[] args)`是Java程序的入口点。", true));
//    }
//}
//public class Exam extends JFrame {
//    // 组件：
//    JLabel QuestionName; // 标签：题目
//    // 标签：显示单选题，多选题，判断题数量，成绩，时间
//    JLabel labelOneChoiceNum, labelmoreChoicesNum, labelTFNum, labelScore, labelTotalTime,labelTime;
//    JRadioButton ButtonA, ButtonB, ButtonC, ButtonD; // 四个单选按钮ABCD
//    ButtonGroup buttonGroup; // 作用:同一时间只能选择一个
//    JButton buttonSubmit;// 一个单选按钮：提交
//    JCheckBox checkBoxA, checkBoxB, checkBoxC, checkBoxD; // 四个多选框ABCD
//    JPanel panelQuestion=new JPanel(); // 题目面板：管理按钮和标签组件
//    // 状态变量：
//    Questions questions=new Questions();
//    Timer timer; // 计时器
//    Boolean isOneChoice,isMoreChoice,isTF,submited=false; // 当前题目类型，是否提交
//    int score=0,seconds=0,count=0;// 总成绩，每道题总时长，题目数量
//    int oneCorrectCount=0,moreCorrectCount=0,tfCorrectCount=0; // 正确题目计数器
//    int OneChoiceNum=0,MoreChoicesNum=0,TFChoiceNum=0; // 记录已经出过的题量
//    // 统一初始化
//    Exam(){
//        super("单机版Java简易机考程序"); // 父类构造方法设置窗口名字
//        // 设置窗口参数
//        setVisible(true);// 窗口可见
//        setSize(500, 300);
//        setLocationRelativeTo(null);//设置窗口的位置
//        //null 作为参数表示相对于屏幕的中心，而不是其他窗口
//        setLayout(new BorderLayout());
//        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);// 关闭窗口时，程序将退出
//        //题目部分
//        panelQuestion.setLayout(new GridLayout(5,1));// 题目和四个选项-5行,并列为1列
//        panelQuestion.setBorder(new EmptyBorder(0,10,0,10));//控制组件与其他组件之间的距离
//        QuestionName=new JLabel("",SwingConstants.CENTER); // 居中显示
//        ButtonA=new JRadioButton("A");
//        ButtonB=new JRadioButton("B");
//        ButtonC=new JRadioButton("C");
//        ButtonD=new JRadioButton("D");
//        buttonGroup=new ButtonGroup();
//        buttonGroup.add(ButtonA);
//        buttonGroup.add(ButtonB);
//        buttonGroup.add(ButtonC);
//        buttonGroup.add(ButtonD);
//        checkBoxA=new JCheckBox("A");
//        checkBoxB=new JCheckBox("B");
//        checkBoxC=new JCheckBox("C");
//        checkBoxD=new JCheckBox("D");
//        // 实时显示部分
//        JPanel panelShow =new JPanel();
//        panelShow.setBackground(Color.ORANGE);
//        labelOneChoiceNum =new JLabel("单选题:0/0/5",SwingConstants.CENTER);
//        labelmoreChoicesNum =new JLabel("多选题:0/0/5",SwingConstants.CENTER);
//        labelTFNum=new JLabel("判断题:0/0/5",SwingConstants.CENTER);
//        labelScore=new JLabel("成绩:0分",SwingConstants.CENTER);
//        labelTime=new JLabel("用时:0秒",SwingConstants.CENTER);
//        panelShow.add(labelOneChoiceNum);
//        panelShow.add(labelmoreChoicesNum);
//        panelShow.add(labelTFNum);
//        panelShow.add(labelScore);
//        panelShow.add(labelTime);
//        add(panelShow,BorderLayout.NORTH);// 在上方实时显示
//        add(panelQuestion,BorderLayout.CENTER); // 中间题目
//        // 提交按钮
//        buttonSubmit =new JButton("提交");
//        add(buttonSubmit,BorderLayout.SOUTH); // 下方提交
//        buttonSubmit.addActionListener(new ActionListener() { // 点击按钮调用方法
//            public void actionPerformed(ActionEvent e) {
//                submitAnswer();
//            }
//        });
//        timer = new Timer(1000, new ActionListener() {
//            public void actionPerformed(ActionEvent e) {
//                seconds++;
//                labelTime.setText("用时：" + seconds + "秒");
//                if (seconds >= 20) {
//                    submitAnswer();
//                }
//            }
//        });
//        Thread thread =new Thread(new giveQuestion());
//        thread.start();
//    }
//    void submitAnswer() {
//        timer.stop();
//        String correctAnswer="";
//        Boolean isRight=false;
//        if(isOneChoice){
//            String input="";
//            correctAnswer=questions.oneChoices.get(OneChoiceNum-1).answer;
//            // 已设置只能选一个
//            if(ButtonA.isSelected()){
//                input="A";
//            }
//            if(ButtonB.isSelected()){
//                input="B";
//            }
//            if(ButtonC.isSelected()){
//                input="C";
//            }
//            if(ButtonD.isSelected()){
//                input="D";
//            }
//            if(input.equals(correctAnswer)){
//                isRight=true;
//                score++;
//                oneCorrectCount++;
//            }
//        }
//        else if (isMoreChoice){
//            String input="";
//            correctAnswer=questions.moreChoices.get(MoreChoicesNum-1).answer;
//            if(checkBoxA.isSelected()){
//                input+="A";
//            }
//            if(checkBoxB.isSelected()){
//                input+="B";
//            }
//            if(checkBoxC.isSelected()){
//                input+="C";
//            }
//            if(checkBoxD.isSelected()){
//                input+="D";
//            }
//            if(input.equals(correctAnswer)) {
//                isRight = true;
//                score += 2;
//                moreCorrectCount++;
//            }
//        }
//        else if(isTF){
//            Boolean correctTFAnswer = questions.tfs.get(TFChoiceNum-1).answer;
//            correctAnswer=String.valueOf(correctTFAnswer);
//            if(ButtonA.isSelected() && correctTFAnswer || ButtonB.isSelected() && !correctTFAnswer) {
//                isRight = true;
//                score++;
//                tfCorrectCount++;
//            }
//        }
//        if(isRight){
//            JOptionPane.showMessageDialog(this, "回答正确,总共用时" + seconds + "秒"+",太强了!");
//        }
//        else{
//            JOptionPane.showMessageDialog(this, "回答错误,正确答案是 " + correctAnswer + "\n总共用时" + seconds + "秒,还要加油哦!");
//        }
//        updateShowPanel();
//        submited=true;
//    }
//    void updateShowPanel(){
//        labelScore.setText("成绩:" + score+"分");
//        labelOneChoiceNum.setText("单选题:"+oneCorrectCount+"/"+OneChoiceNum+"/5");
//        labelmoreChoicesNum.setText("多选题"+moreCorrectCount+"/"+MoreChoicesNum+"/5");
//        labelTFNum.setText("判断题"+tfCorrectCount+"/"+TFChoiceNum+"/5");
//
//        //清除所有按钮状态
//        buttonGroup.clearSelection();
//        checkBoxA.setSelected(false);
//        checkBoxB.setSelected(false);
//        checkBoxC.setSelected(false);
//        checkBoxD.setSelected(false);
//        ButtonA.setSelected(false);
//        ButtonB.setSelected(false);
//        ButtonC.setSelected(false);
//        ButtonD.setSelected(false);
//        seconds=0;
//        labelTime.setText("用时:"+seconds+"秒");
//        timer.restart(); // 重新开始计时
//    }
//    class giveQuestion implements Runnable {
//        public void run(){
//            // 打乱顺序,保证随机
//            Collections.shuffle(questions.oneChoices);
//            Collections.shuffle(questions.moreChoices);
//            Collections.shuffle(questions.tfs);
//            while(count<15){
//                submited=false;
//                if(count%3==0){
//                    isOneChoice=true;
//                    isTF=false;
//                    isMoreChoice=false;
//                    showOneChoice(questions.oneChoices.get(OneChoiceNum));
//                    OneChoiceNum++;
//                }
//                else if(count%3==1){
//                    isMoreChoice=true;
//                    isTF=false;
//                    isOneChoice=false;
//                    showMoreChoice(questions.moreChoices.get(MoreChoicesNum));
//                    MoreChoicesNum++;
//                }
//                else if (count%3==2){
//                    isTF=true;
//                    isMoreChoice=false;
//                    isOneChoice=false;
//                    showTFChoice(questions.tfs.get(TFChoiceNum));
//                    TFChoiceNum++;
//                }
//                timer.start();
//                while(!submited){
//                    try{
//                        Thread.sleep(1000);
//                    } catch (InterruptedException e) {
//                        throw new RuntimeException(e);
//                    }
//                }
//                count++;
//            }
//            endGUI();
//        }
//    }
//    // 显示题目信息 -> 修改各个组件的值
//    void showOneChoice(OneChoice oneChoice){
//        panelQuestion.removeAll();
//        QuestionName.setText(count+ 1 +"/15 （单选题）"+"   "+oneChoice.question);
//        ButtonA.setText(oneChoice.option1);
//        ButtonB.setText(oneChoice.option2);
//        ButtonC.setText(oneChoice.option3);
//        ButtonD.setText(oneChoice.option4);
//        panelQuestion.add(QuestionName);
//        panelQuestion.add(ButtonA);
//        panelQuestion.add(ButtonB);
//        panelQuestion.add(ButtonC);
//        panelQuestion.add(ButtonD);
//        panelQuestion.revalidate();// 重新调整布局，确保组件的显示是正确的
//        panelQuestion.repaint();//刷新显示，确保显示的是最新的内容
//    }
//    void showMoreChoice(MoreChoice moreChoice){
//        panelQuestion.removeAll();
//        QuestionName.setText(count+ 1 +"/15 （多选题）"+"   "+moreChoice.question);
//        checkBoxA.setText(moreChoice.option1);
//        checkBoxB.setText(moreChoice.option2);
//        checkBoxC.setText(moreChoice.option3);
//        checkBoxD.setText(moreChoice.option4);
//        panelQuestion.add(QuestionName);
//        panelQuestion.add(checkBoxA);
//        panelQuestion.add(checkBoxB);
//        panelQuestion.add(checkBoxC);
//        panelQuestion.add(checkBoxD);
//        panelQuestion.revalidate();
//        panelQuestion.repaint();
//    }
//    void showTFChoice(TF tf){
//        panelQuestion.removeAll();
//        QuestionName.setText(count+ 1 +"/15 （判断题）"+"   "+tf.question);
//        ButtonA.setText("正确");
//        ButtonB.setText("错误");
//        panelQuestion.add(QuestionName);
//        panelQuestion.add(ButtonA);
//        panelQuestion.add(ButtonB);
//        panelQuestion.revalidate();
//        panelQuestion.repaint();
//    }
//    // 结束
//    void endGUI(){
//        timer.stop();
//        JOptionPane.showMessageDialog(this, "Java机考结束,最终成绩是" + score + "分,再接再厉!");
//        System.exit(0);
//    }
//    public static void main(String[] args) {
//        Exam exam = new Exam();
//    }
//}
