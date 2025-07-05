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
import java.nio.file.StandardOpenOption;
import java.util.*;
import java.util.concurrent.*;
import java.util.regex.Pattern;
import java.lang.*;
public class SimpleWebCrawler {
    // 配置参数
    private static final String BASE_DIR = "D:\\Course\\计算机网络\\实验\\实验四\\CrawlerResult";
    private static final int MAX_DEPTH = 2;
    private static final int TIMEOUT_MS = 5000;
    private static final int MAX_RETRY = 3;
    private static final Set<String> VISITED_URLS = ConcurrentHashMap.newKeySet();
    private static final String TARGET_DOMAIN = "szu.edu.cn"; // 限制爬取域名
    private static final int MAX_PAGES = 50;                  // 最大爬取页面数
    private static final Pattern IGNORE_EXT = Pattern.compile(".*\\.(pdf|docx|zip)$"); // 忽略文件类型

    // 线程池配置
    private static final ExecutorService executor = Executors.newFixedThreadPool(4);

    public static void main(String[] args) {
        // 种子URL（深圳大学官网）
        try {
            crawlAsync("https://www.szu.edu.cn", 0);

            // 优雅关闭线程池
            executor.shutdown();
            if (!executor.awaitTermination(60, TimeUnit.SECONDS)) {
                executor.shutdownNow();
                System.out.println("强制关闭剩余任务");
            }
        } catch (InterruptedException e) {  // 添加捕获
            System.err.println("主线程被中断: " + e.getMessage());
            Thread.currentThread().interrupt(); // 重新设置中断状态
        }
        System.out.println("爬虫已停止");
    }

    // 异步爬取入口
    private static void crawlAsync(String url, int depth) {
        if (depth > MAX_DEPTH ||
                VISITED_URLS.size() >= MAX_PAGES ||
                !isSameDomain(url) ||
                IGNORE_EXT.matcher(url).matches()) {
            return;
        }

        executor.submit(() -> {
            try {
                System.out.printf("[深度 %d] 开始处理: %s%n", depth, url);

                // 下载资源并获取文档
                Document doc = downloadResource(url);
                if (doc == null) return;

                // 解析新链接
                parseAndScheduleLinks(doc, depth + 1);

            } catch (Exception e) {
                System.err.printf("处理 %s 时发生错误: %s%n", url, e.getMessage());
            }
        });
    }
    private static boolean isSameDomain(String url) {
        try {
            URL parsedUrl = new URL(url);
            return parsedUrl.getHost().endsWith(TARGET_DOMAIN);
        } catch (Exception e) {
            return false; // 忽略无效URL
        }
    }
    // 下载资源并返回解析后的文档
    private static Document downloadResource(String url) {
        try {
            // 判断资源类型
            String fileType = getFileType(url);
            Path savePath = buildSavePath(url, fileType);

            // 下载文件
            long fileSize = downloadWithRetry(url, savePath, fileType);
            logDownload(url, savePath, fileSize);

            // 如果是HTML则返回解析文档
            if (fileType.equals("html")) {
                return Jsoup.parse(savePath.toFile(), "UTF-8", url);
            }
        } catch (IOException e) {
            System.err.println("下载失败: " + url + " - " + e.getMessage());
        }
        return null;
    }

    // 解析文档中的链接并调度爬取
    private static void parseAndScheduleLinks(Document doc, int nextDepth) {
        Elements links = doc.select("a[href], img[src], script[src], link[href]");
        for (Element el : links) {
            String absUrl = el.absUrl("href").isEmpty() ? el.absUrl("src") : el.absUrl("href");
            if (isValidUrl(absUrl)) {
                crawlAsync(absUrl, nextDepth);
            }
        }
    }

    // 带重试机制的下载方法
    private static long downloadWithRetry(String url, Path savePath, String fileType) throws IOException {
        for (int i = 0; i < MAX_RETRY; i++) {
            try {
                URL source = new URL(url);
                try (InputStream in = new BufferedInputStream(source.openStream())) {
                    Files.createDirectories(savePath.getParent());
                    return Files.copy(in, savePath, StandardCopyOption.REPLACE_EXISTING);
                }
            } catch (IOException e) {
                if (i == MAX_RETRY - 1) throw e;
                System.out.printf("第 %d 次重试下载: %s%n", i + 1, url);
            }
        }
        return 0;
    }

    // 构建保存路径
    private static Path buildSavePath(String url, String fileType) {
        String filename = url.substring(url.lastIndexOf('/') + 1);
        String host = url.replaceFirst("^(http[s]?://)", "").replaceAll("/.*", "");
        return Paths.get(BASE_DIR, fileType + "s", host, filename);
    }

    // 判断URL有效性
    private static boolean isValidUrl(String url) {
        return url.startsWith("http") && !url.contains("javascript:") && !VISITED_URLS.contains(url);
    }

    // 获取文件类型
    private static String getFileType(String url) {
        if (url.matches("(?i).*\\.(png|jpg|jpeg|gif|webp|bmp)$")) return "image";
        if (url.matches("(?i).*\\.(css)$")) return "css";
        if (url.matches("(?i).*\\.(js)$")) return "js";
        return "html"; // 默认视为HTML
    }

    // 记录下载日志
    private static void logDownload(String url, Path path, long size) {
        String log = String.format("[%s] %s -> %s (%d bytes)%n",
                new Date(), url, path.toString(), size);
        try {
            Files.write(Paths.get(BASE_DIR, "logs", "crawl.log"),
                    log.getBytes(), StandardOpenOption.CREATE, StandardOpenOption.APPEND);
        } catch (IOException e) {
            System.err.println("日志写入失败: " + e.getMessage());
        }
    }
}