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

public class PdfCrawler {
    private static final String WEBSITE = "https://www.uooc.net.cn/home/course/191442589";
    private static final String SAVE_PATH = "D:/CrawlData/uooc-pdf";

    public static void main(String[] args) {
        try {
            Path saveDir = Paths.get(SAVE_PATH);
            Files.createDirectories(saveDir);

            // 添加 User-Agent 绕过反爬
            Document doc = Jsoup.connect(WEBSITE)
                    .userAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36")
                    .get();

            // 提取所有 PDF 链接
            Set<String> pdfUrls = new HashSet<>();
            Elements links = doc.select("a[href$=.pdf]"); // 精确匹配以 .pdf 结尾的链接
            for (Element link : links) {
                String absUrl = link.absUrl("href");
                pdfUrls.add(absUrl);
                System.out.println("发现PDF链接: " + absUrl);
            }

            // 直接下载 PDF 文件（避免用 Jsoup 处理二进制内容）
            int successCount = 0;
            for (String pdfUrl : pdfUrls) {
                if (downloadPdf(pdfUrl, saveDir)) successCount++;
            }
            System.out.println("成功下载: " + successCount + " 个PDF文件");

        } catch (IOException e) {
            System.err.println("错误: " + e.getMessage());
        }
    }

    private static boolean downloadPdf(String fileUrl, Path baseDir) {
        try {
            URL url = new URL(fileUrl);
            String fileName = fileUrl.substring(fileUrl.lastIndexOf('/') + 1); // 直接从 URL 提取文件名
            Path targetPath = baseDir.resolve(fileName);

            // 使用 try-with-resources 确保流关闭
            try (InputStream in = url.openStream()) {
                Files.copy(in, targetPath, StandardCopyOption.REPLACE_EXISTING);
                System.out.println("下载成功: " + fileName);
                return true;
            }
        } catch (IOException e) {
            System.err.println("下载失败: " + fileUrl + " - " + e.getMessage());
            return false;
        }
    }
}