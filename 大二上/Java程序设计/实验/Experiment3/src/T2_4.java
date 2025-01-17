import java.util.*;

public class T2_4 {
    public static void CountTime(String Name, Collection<Integer> collection, List<Integer> test){
        long start=System.nanoTime();
        for(Integer num :test){
            collection.contains(num);
        }
        long end=System.nanoTime();
        System.out.printf("%s took %d ms for contains() 10000 times\n",Name,(end-start)/1000000);
    }
    public static void main(String[] args) {

        List <Integer> random=new ArrayList<>();
        for(int i=1;i<=50000;i++){
            random.add(i);
        }
        Collections.shuffle(random);
        ArrayList<Integer> arrayList = new ArrayList<>(random);
        LinkedList<Integer> linkedList = new LinkedList<>(random);
        HashSet<Integer> hashSet = new HashSet<>(random);
        LinkedHashSet<Integer> linkedHashSet = new LinkedHashSet<>(random);
        TreeSet<Integer> treeSet = new TreeSet<>(random);

        Random rand=new Random();
        List <Integer> list=new ArrayList<>();
        for(int i=0;i<10000;i++){
            list.add(rand.nextInt(50000)+1);
        }
        CountTime("ArrayList",arrayList,list);
        CountTime("LinkedList",linkedList,list);
        CountTime("HashSet",hashSet,list);
        CountTime("LinkedHashSet",linkedHashSet,list);
        CountTime("TreeSet",treeSet,list);


    }
}
