package A.part4;
import java.util.*;
class HexFormatException extends Exception {
    String message;
    public HexFormatException(String hex) {
        message=hex+"字符串不是有效的十六进制数字";
    }
    public String getMessage(){
        return message;
    }
}
public class HexToDecConverter {
    public int hex2Dec(String hexString) throws HexFormatException {
        if (!hexString.matches("[0-9A-Fa-f]+")) {
            HexFormatException hexFormatException = new HexFormatException(hexString);
            throw (hexFormatException);
        }
        return Integer.parseInt(hexString, 16);
    }
    public static void main(String[] args) {
        HexToDecConverter hex = new HexToDecConverter();
        Scanner scanner = new Scanner(System.in);
        System.out.print("请输入一个十六进制: ");
        String hexString = scanner.nextLine();
        try {
            System.out.println("对应的十进制数字是: " + hex.hex2Dec(hexString));
        } catch (HexFormatException e) {
            System.out.println(e.getMessage());
        }
    }
}

