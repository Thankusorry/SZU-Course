//package Chat;
//
//import java.io.InputStream;
//import java.util.Properties;
//
//public class AppConfig {
//    private static final Properties props = new Properties();
//
//    static {
//        try (InputStream input = AppConfig.class
//                .getResourceAsStream("/config.properties")) {
//            props.load(input);
//        } catch (Exception e) {
//            setDefaultValues();
//        }
//    }
//
//    private static void setDefaultValues() {
//        props.setProperty("game.server.ip", "localhost");
//        props.setProperty("game.server.port", "59999");
//    }
//
//    public static String getGameServerIP() {
//        return props.getProperty("game.server.ip");
//    }
//
//    public static int getGameServerPort() {
//        return Integer.parseInt(props.getProperty("game.server.port"));
//    }
//}