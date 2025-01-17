2、GUI应用（共1题，30分）

(1) 编写Java应用程序，实现“Java机考”的功能（“单机版Java简易机考程序”），包含单选题、多选题和判断题三种题型。

在主线程中创建一个**Frame类型的窗口**，在该窗口中再**创建一个线程giveQuestion**。

线程giveQuestion**每隔20秒钟输出一个选择题**（含A,B,C,D共4个选项，要求支持单选题和多选题，

**单选题用radio button，多选题用check box）或一个判断题（用radio button）**，

选择题和判断题**混合着给出**；用户输入答案并按提交按钮提交结果（**达到20秒自动提交结果**）；程序判断用户输入的答案是否正确（**如果错选或漏选均得零分**）

##### 实时显示

1. **当前题目的正确答案**
2. **已经给出的题目的数量（分别给出单选题数量、多选题数量和判断题数量）**
3. 用户**答对的数量**（分别给出单选题数量、多选题数量和判断题数量）
4. 用户的**成绩**和用户答题所**花的总的时间**。

如此**循环15次**，包括随机选择的5个单选题（每题1分）、随机选择的5个多选题（每题2分）和随机选择的5个判断题（每题1分），结束测试时给出最终成绩。

题库应包含10个单选题、10个多选题和10个判断题。要求使用图形用户界面。















#### 第九章 图形用户界面设计

 java.awt 包中的类创建的组件习惯上称为重组件。创建一个按钮组件时，都有 一个相应的本地组
件在为它工作，即显示主件和处理主件事件，该本地主件称 为它的同位体。 javax.swing 包为我们提供了更加丰富的、功能
强大的组件，称为 Swing 组件， 其中大部分组件是轻组件，没有同位体，而是把与显示组件有关的许多工作和 处理组件事
件的工作交给相应的 UI 代表来完成。 这些 UI 代表是用 Java 语言编写的类，这些类被增加到 Java 的运行环境中， 因此
组件的外观不依赖平台，不仅在不同平台上的外观是相同的，而且与重组件相比有更高的性能。 JComponent 类的子类都是
轻组件 JFrame, JApplet, JDialog 都是重组件，即有同位体的组件。这样，窗口（JFrame）、 小应用程序（Java Applet）、对话
框（JDialog）可以和操作系统交互信息。轻 组件必须在这些容器中绘制自己，习惯上称这些容器为 Swing 的底层容器。 对
于 JFrame 窗口，默认布局是 BorderLayout 布局 FlowLayout、BorderLayout、CardLayout、GridLayout 布局 null 布局 
p.setLayout(null); setBounds(int a, int b, int width, int height) JButton next = new JButton("确定(5s)"); //确认按钮 Font style = 
new Font("宋体", Font.BOLD, 24); //字体格式：宋体 24 号 JLabel A, B, C, D, question, tips, rate, fin; //框架中的文字，用 JLabel 
JTextField inputans; //输入框 setTitle(s); setLayout(null); translate1.setFont(style); setBounds(500, 250, 500, 300); //给整个窗口设
置位置大小 setVisible(true); validate(); setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE); public void 
actionPerformed(ActionEvent e) {e.getSource() == translate1 } 
初始化线程、事件调度线程、工作线程（自己创建）, 
MouseAdaptor：类，MouseListener：接口。前者为后者的一个实现，提供了空实现。 
Scanner: 有nextByte(), nextShort(), nextInt(), nextLong(), nextFloat(), nextDouble(), nextLine()、hasNext()、
hasNextDouble() 
BufferedReader有readline() 
new DataInputStream(InputStream is)有readByte(), readShort(), readInt()…… 
new DataOutputStream(OutputStream os)有writeByte(), writeShort(), writeInt()…… 
读入用Scanner in = new Scanner(System.in)，然后while(hasnextxxx) {nextxxx()} 
Trick：可以catch (InputMismatchException)里调用scanner.next()来跳过不符合的部分 


Synchronized 

线程池： 
ExecutorService executor = Executors.newCachedThreadPool(); 
executor.execute(new XXXRunnable); 
executor.shutdown() // run to finish 

 

 

 


JFrame JApplet JDialog默认是BorderLayout。JPanel默认是FlowLayout 
JButton JTextField JTextArea JPanel JCheckBox JRadioButton JLabel JComboBox---JComponent 
LayoutManager: FlowLayout, BorderLayout, GridLayout, CardLayout, BoxLayout, Null 
添加子组件add()，删除组件remove(comp)，删除全部removeAll() 
添加和删除后，要validate()一下 
JFrame setBounds(x,y,w,h) setSize(w,h) setVisible(true) 
窗口默认不可见，要setVisible 
JScrollPane(comp)封装滚动窗格 
JSplitPane(int how_to_split, comp_b, comp_c)拆分窗格，how_to_split为HORIZONTAL_SPLIT、VERTICAL_SPLIT 
JSplitPane.setDividerLocation(double pos) 

JDialog有modaless和modal（“有模”），modal的会堵塞线程。 
Dialog = new JDialog(jframe, str, has_model) 
If(dialog.getMessage() == MyDialog.YES) {……}  

JinternalFrame-内部窗口MDI 

布局： 
FlowLayout、BorderLayout、CardLayout、Gridlayout(m,n)、BoxLayout 
comp.setLayout(layout); 

layout.setHgap layout.setVgap 

JFrame布局： 
win.add(bNorth, BorderLayout.NORTH); 
win.add(bSouth, BorderLayout.SOUTH); 
win.add(bEast, BorderLayout.EAST); 
win.add(bWest, BorderLayout.WEST);  
win.add(bCenter, BorderLayout.CENTER); 

win.validate(); 
setBounds(100, 100, 500, 300); 
setVisible(true); 
setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE); 

JTextField： 
setText、getText、setEditable 

JPasswordField： 
setEchoChar(char c)设置回显 
char[] getPassword() 获得密码 


actionEvent： 
事件源、监视/监听器和处理事件的接口 
class PoliceStation implements ActionListener 
{ 
    public void actionPerformed(ActionEvent e)



{ 
    //comp = (JTextField) e.getSource() 
        String str = e.getActionCommand();     
        System.out.println(str); 
        System.out.println(str.length()); 
    } 
} 


JTextArea： 
setLineWrap(true) 
setWrapStyleWord(true) 
append(s) insert(s, int x) getCaretPosition()获取光标位置 
copy() cut()复制或剪切到剪切板 
paste()粘贴 

 

WindowListener接口，WindowAdaptor类 
最小化最大化之类的事件 
addWindowListener 

URL： 
try  
{ 
      url=new URL("http://yahoo.com.cn"); 
} 
catch(MalformedURLException e) 
{  
      System.out.println("Bad URL:"+url); 
} 

InputStream URL.openStream() 
可以用JEditorPane(String url)来显示网页 
JEditorPane对象调用addHyperlinkListener(HyperlinkListener listener)获得监视器。 
监视器需要实现HyperlinkListener接口，该接口中的方法是void hyperlinkUpdate(HyperlinkEvent e) 

UDP发送： 
byte data[ ]="近来好吗".getByte(); 
InetAddress address = InetAddtress.getName("www.sina.com.cn"); 
DatagramPacket  data_pack = new  DatagramPacket(data, data.length, address, 5678); 
DatagramSocket mail_out = new DatagramSocket(); 
mail_out.send(data_pack);

返回值返回值可以不同可以不同
返回值可以不同