import java.net.InetAddress;
import java.net.UnknownHostException;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;
import java.io.*;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.HashSet;
import java.util.Set;


public class BasedOnTest {
    // 需要爬的域名
    private static String website="https://www.csdn.net/";
    // 本地保存路径
    private static final String SAVE_PATH = "D:/CrawlData/csdn";

    public static void main(String[] args) {
        //爬取主页资源
        try {
            // 创建保存目录
            Path saveDir = Paths.get(SAVE_PATH);
            Files.createDirectories(saveDir);

            // 下载主页
            long mainPageSize = downloadMainPage(website,saveDir);
            System.out.println("\n主页文件大小: " + mainPageSize + " 字节");

            // 解析并下载资源
            downloadResources(saveDir);

        } catch (IOException e) {
            System.err.println("发生错误: " + e.getMessage());
        }

    }

    // 下载主页
    private static long downloadMainPage(String url, Path saveDir) throws IOException {
        URL website = new URL(url);
        Path indexPath = saveDir.resolve("index.html");

        try (InputStream in = website.openStream()) {
            return Files.copy(in, indexPath, StandardCopyOption.REPLACE_EXISTING);
        }
    }

    // 解析并下载所有资源
    private static void downloadResources(Path saveDir) throws IOException {
        File htmlFile = saveDir.resolve("index.html").toFile();
        Document doc = Jsoup.parse(htmlFile, "UTF-8", "https://www.szu.edu.cn");

        Set<String> resourceUrls = new HashSet<>();

        // 提取不同资源类型
        extractResources(doc, "img[src]", "src", resourceUrls);     // 图片
        extractResources(doc, "script[src]", "src", resourceUrls); // JavaScript
        extractResources(doc, "link[href]", "href", resourceUrls); // CSS
        extractResources(doc, "a[href]", "href", resourceUrls);     // 超链接文件
        extractResources(doc, "video[src]", "src", resourceUrls);    // 视频元素
        extractResources(doc, "audio[src]", "src", resourceUrls);    // 音频元素
        extractResources(doc, "source[src]", "src", resourceUrls);   // 多媒体源
        extractResources(doc, "object[data]", "data", resourceUrls); // 嵌入对象
        extractResources(doc, "embed[src]", "src", resourceUrls);    // 插件内容
        System.out.println("\n发现需要下载的资源数量: " + resourceUrls.size());

        // 下载所有资源
        int successCount = 0;
        for (String url : resourceUrls) {
            if (downloadFile(url, saveDir)) successCount++;
        }
        System.out.println("\n成功下载资源数量: " + successCount);
    }

    // 资源解析方法
    private static void extractResources(Document doc, String selector, String attr, Set<String> urls) {
        Elements elements = doc.select(selector);
        for (Element el : elements) {
            String absUrl = el.absUrl(attr);
            if (!absUrl.isEmpty() && isAllowedType(absUrl)) {
                urls.add(absUrl);
            }
        }
    }

    // 文件下载方法
    private static boolean downloadFile(String fileUrl, Path baseDir) {
        try {
            URL url = new URL(fileUrl);
            Path targetPath = buildLocalPath(url.getPath(), baseDir);

            // 创建父目录
            Files.createDirectories(targetPath.getParent());

            // 下载文件
            try (InputStream in = url.openStream()) {
                long size = Files.copy(in, targetPath, StandardCopyOption.REPLACE_EXISTING);
                System.out.println("下载成功: " + fileUrl + " (大小: " + size + "B)");
                return true;
            }
        } catch (IOException e) {
            System.err.println("下载失败: " + fileUrl + " - " + e.getMessage());
            return false;
        }
    }

    // 构建本地保存路径
    private static Path buildLocalPath(String urlPath, Path baseDir) {
        String cleanPath = urlPath.replaceFirst("^/", ""); // 移除开头的斜杠
        return baseDir.resolve(cleanPath);
    }

    // 过滤允许的文件类型
    private static boolean isAllowedType(String url) {
        return url.matches("(?i).*\\.(html|htm|css|js|png|jpg|jpeg|gif|ico|pdf|docx?|xlsx?|"
                + "mp4|webm|ogg|mov|avi|"    // 新增视频格式
                + "mp3|wav|flac|aac|"        // 新增音频格式
                + "webp|svg|bmp|tiff)");     // 新增图片格式
    }

}