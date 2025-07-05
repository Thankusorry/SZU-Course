import java.io.*;
import java.util.*;
import java.util.concurrent.ThreadLocalRandom;

public class StudentDB {
    private static final String[] SURNAMES = {"王", "李", "张", "刘", "陈", "杨", "赵", "黄", "周", "吴"};
    private static final String[] MALE_NAMES = {"伟", "强", "磊", "超", "浩", "勇", "杰", "涛", "鹏", "鑫"};
    private static final String[] FEMALE_NAMES = {"芳", "娜", "敏", "静", "丽", "娟", "艳", "婷", "雪", "慧"};

    private final Map<String, String[]> studentMap = new HashMap<>();

    // 从文件加载数据的构造方法
    public StudentDB(String dataFilePath) throws IOException {
        loadDataFromFile(dataFilePath);
    }

    // 生成随机数据的构造方法
    public StudentDB(int dataSize) {
        generateRandomData(dataSize);
    }

    private void loadDataFromFile(String filePath) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(",");
                if (parts.length == 3) {
                    studentMap.put(parts[0].trim(), new String[]{parts[1].trim(), parts[2].trim()});
                }
            }
        }
    }

    private void generateRandomData(int size) {
        int baseYear = 2023150000;
        ThreadLocalRandom random = ThreadLocalRandom.current();

        for (int i = 1; i <= size; i++) {
            // 生成学号
            String studentId = String.valueOf(baseYear + i);

            // 生成性别
            String gender = random.nextBoolean() ? "男" : "女";

            // 生成姓名
            String surname = SURNAMES[random.nextInt(SURNAMES.length)];
            String givenName = gender.equals("男") ?
                    MALE_NAMES[random.nextInt(MALE_NAMES.length)] :
                    FEMALE_NAMES[random.nextInt(FEMALE_NAMES.length)];

            studentMap.put(studentId, new String[]{surname + givenName, gender});
        }
    }

    public void exportToFile(String filePath) throws IOException {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filePath))) {
            for (Map.Entry<String, String[]> entry : studentMap.entrySet()) {
                writer.write(String.format("%s,%s,%s\n",
                        entry.getKey(),
                        entry.getValue()[0],
                        entry.getValue()[1]));
            }
        }
    }

    public String[] getStudentInfo(String studentId) {
        return studentMap.get(studentId);
    }

    public int size() {
        return studentMap.size();
    }
}