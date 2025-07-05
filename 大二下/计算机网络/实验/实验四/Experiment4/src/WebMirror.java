//import org.jsoup.Jsoup;
//import org.jsoup.nodes.Document;
//import org.jsoup.nodes.Element;
//import org.jsoup.select.Elements;
//import org.openqa.selenium.JavascriptExecutor;
//import org.openqa.selenium.WebDriver;
//import org.openqa.selenium.chrome.ChromeDriver;
//import org.openqa.selenium.support.ui.WebDriverWait;
//
//import java.io.*;
//import java.net.*;
//import java.nio.charset.StandardCharsets;
//import java.nio.file.*;
//import java.time.Duration;
//import java.util.*;
//import java.util.concurrent.*;
//import java.util.regex.*;
//import java.util.stream.*;
//import java.util.regex.MatchResult;
//
//public class WebMirror {
//    private static final Set<String> ALLOWED_DOMAINS = new HashSet<>();
//    private static final String USER_AGENT = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36";
//
//    public static void main(String[] args) {
//        try {
//            Path saveDir = Paths.get("mirrored_site");
//            String targetUrl = "https://www.szu.edu.cn";
//
//            // 初始化配置
//            System.setProperty("webdriver.chrome.driver", "/path/to/chromedriver");
//            Files.createDirectories(saveDir);
//
//            // 主流程
//            long indexSize = downloadMainPage(targetUrl, saveDir);
//            System.out.println("主页下载完成，大小: " + indexSize + " 字节");
//
//            downloadResources(saveDir);
//            rewriteLocalLinks(saveDir.resolve("index.html"), saveDir);
//            verifyIntegrity(saveDir);
//
//            System.out.println("网站镜像完成！使用以下命令查看：");
//            System.out.println("cd " + saveDir.toAbsolutePath() + " && python3 -m http.server 8080");
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
//    }
//
//    // 下载主页面（含动态渲染）
//    private static long downloadMainPage(String url, Path saveDir) throws IOException {
//        String dynamicHtml = renderWithBrowser(url);
//        Path indexPath = saveDir.resolve("index.html");
//        Files.write(indexPath, dynamicHtml.getBytes(StandardCharsets.UTF_8));
//        return dynamicHtml.length();
//    }
//
//    // 无头浏览器渲染
//    private static String renderWithBrowser(String url) {
//        WebDriver driver = new ChromeDriver();
//        try {
//            driver.get(url);
//            new WebDriverWait(driver, Duration.ofSeconds(15))
//                    .until(d -> ((JavascriptExecutor)d)
//                            .executeScript("return document.readyState").equals("complete"));
//
//            // 捕获所有动态加载的资源
//            ((JavascriptExecutor)driver).executeScript(
//                    "Array.from(document.querySelectorAll('img[data-src]')).forEach(img => img.src = img.dataset.src);");
//
//            return driver.getPageSource();
//        } finally {
//            driver.quit();
//        }
//    }
//
//    // 下载所有资源
//    private static void downloadResources(Path saveDir) throws Exception {
//        Document doc = Jsoup.parse(saveDir.resolve("index.html").toFile(), "UTF-8");
//        Set<String> resourceUrls = Collections.synchronizedSet(new HashSet<>());
//
//        // 资源提取
//        String[][] resourceRules = {
//                {"img[src,data-src,data-original]", "abs:(src|data-src|data-original)"},
//                {"script[src]", "abs:src"},
//                {"link[href]", "abs:href"},
//                {"a[href]", "abs:href"},
//                {"video[src]", "abs:src"},
//                {"source[src,srcset]", "abs:src,abs:srcset"},
//                {"embed[src]", "abs:src"},
//                {"object[data]", "abs:data"},
//                {"[style]", "css:url"}
//        };
//
//        ExecutorService extractor = Executors.newVirtualThreadPerTaskExecutor();
//        for (String[] rule : resourceRules) {
//            extractor.submit(() -> extractResources(doc, rule[0], rule[1], resourceUrls));
//        }
//        extractor.shutdown();
//        extractor.awaitTermination(1, TimeUnit.MINUTES);
//
//        // 并行下载
//        System.out.println("发现资源数量: " + resourceUrls.size());
//        ExecutorService downloader = Executors.newFixedThreadPool(16);
//        List<Future<Boolean>> futures = resourceUrls.stream()
//                .filter(url -> isAllowedType(url) && isSameDomain(url))
//                .map(url -> downloader.submit(() -> downloadFile(url, saveDir)))
//                .toList();
//
//        int successCount = 0;
//        for (Future<Boolean> future : futures) {
//            if (future.get()) successCount++;
//        }
//        downloader.shutdown();
//        System.out.println("成功下载: " + successCount + " 个资源");
//    }
//
//    // 资源提取逻辑
//    private static void extractResources(Document doc, String selector, String attrSpec, Set<String> urls) {
//        doc.select(selector).forEach(el -> {
//            // 处理属性提取
//            Arrays.stream(attrSpec.split(",")).forEach(attr -> {
//                if (attr.startsWith("abs:")) {
//                    String absAttr = attr.substring(4);
//                    String absUrl = el.absUrl(absAttr);
//                    if (!absUrl.isEmpty()) urls.add(absUrl);
//                } else if (attr.startsWith("css:")) {
//                    Matcher m = Pattern.compile("url(['\"]?)(.*?)\\\\1")
//                            .matcher(el.attr("style"));
//                    while (m.find()) urls.add(el.absUrl(m.group(1)));
//                }
//            });
//        });
//    }
//
//    // 文件下载（带重试机制）
//    private static boolean downloadFile(String fileUrl, Path baseDir) {
//        for (int retry = 0; retry < 3; retry++) {
//            try {
//                URL url = new URL(fileUrl);
//                HttpURLConnection conn = (HttpURLConnection) url.openConnection();
//                conn.setRequestProperty("User-Agent", USER_AGENT);
//
//                Path savePath = buildLocalPath(url, baseDir);
//                Files.createDirectories(savePath.getParent());
//
//                try (InputStream in = conn.getInputStream()) {
//                    Files.copy(in, savePath, StandardCopyOption.REPLACE_EXISTING);
//                    System.out.printf("下载成功: %s → %s\n", url, savePath);
//                    return true;
//                }
//            } catch (Exception e) {
//                System.err.printf("下载失败（尝试 %d/3）: %s - %s\n", retry+1, fileUrl, e.getMessage());
//                try { Thread.sleep(2000); } catch (InterruptedException ie) {}
//            }
//        }
//        return false;
//    }
//
//    // 路径生成系统
//    private static Path buildLocalPath(URL url, Path baseDir) throws URISyntaxException {
//        String path = url.getPath().replaceFirst("^/", "");
//        String query = url.getQuery() != null ? "_" + url.getQuery().hashCode() : "";
//
//        // 分类存储
//        String typeDir = switch (getFileExtension(path)) {
//            case "mp4", "webm" -> "videos/";
//            case "mp3", "wav" -> "audios/";
//            case "png", "jpg", "gif" -> "images/";
//            default -> "files/";
//        };
//
//        return baseDir.resolve(typeDir + path + query);
//    }
//
//    // 路径重写系统
//    private static void rewriteLocalLinks(Path htmlPath, Path baseDir) throws IOException {
//        Document doc = Jsoup.parse(htmlPath.toFile(), "UTF-8");
//        String baseUri = doc.baseUri();
//
//        doc.select("[href],[src],[srcset],[data],[style]").forEach(el -> {
//            rewriteAttribute(el, "href", baseDir, baseUri);
//            rewriteAttribute(el, "src", baseDir, baseUri);
//            rewriteAttribute(el, "srcset", baseDir, baseUri);
//            rewriteAttribute(el, "data", baseDir, baseUri);
//            rewriteStyleUrls(el, baseDir, baseUri);
//        });
//
//        try (BufferedWriter writer = Files.newBufferedWriter(htmlPath)) {
//            writer.write(doc.outerHtml());
//        }
//    }
//
//    private static void rewriteAttribute(Element el, String attr, Path baseDir, String baseUri) {
//        String value = el.attr(attr);
//        if (!value.isEmpty()) {
//            el.attr(attr, convertToLocalRef(value, baseDir, baseUri));
//        }
//    }
//    private static final Pattern URL_PATTERN = Pattern.compile(
//            "url\\s*$ \\s*([\"']?)(.*?)\\1\\s*  $ ",
//            Pattern.CASE_INSENSITIVE
//    );
//    public static void rewriteStyleUrls(Element el, Path baseDir, String baseUri) {
//        String style = el.attr("style");
//        if (style.isEmpty()) return;
//
//        // 使用明确的MatchResult类型处理
//        Matcher matcher = URL_PATTERN.matcher(style);
//        StringBuffer result = new StringBuffer();
//
//        while (matcher.find()) {
//            String quote = matcher.group(1);  // 引号类型（可能为空）
//            String originalUrl = matcher.group(2).trim();
//            String converted = convertToLocalRef(originalUrl, baseDir, baseUri);
//
//            // 重构后的替换逻辑
//            matcher.appendReplacement(result, "url(" + quote + converted + quote + ")");
//        }
//        matcher.appendTail(result);
//
//        el.attr("style", result.toString());
//    }
//
//    private static String convertToLocalRef(String url, Path baseDir, String baseUri) {
//        try {
//            URL absUrl = new URL(new URL(baseUri), url);
//            if (!isSameDomain(absUrl.toString())) return url; // 保留外链
//
//            Path localPath = buildLocalPath(absUrl, baseDir);
//            return baseDir.relativize(localPath).toString();
//        } catch (Exception e) {
//            return url;
//        }
//    }
//
//    // 完整性校验
//    private static void verifyIntegrity(Path saveDir) throws IOException {
//        Document doc = Jsoup.parse(saveDir.resolve("index.html").toFile(), "UTF-8");
//        Set<Path> expected = Collections.synchronizedSet(new HashSet<>());
//
//        doc.select("[href],[src],[srcset],[data]").parallelStream().forEach(el -> {
//            Stream.of(el.attr("href"), el.attr("src"), el.attr("data"))
//                    .filter(v -> !v.isEmpty())
//                    .map(v -> saveDir.resolve(v))
//                    .forEach(expected::add);
//        });
//
//        long missing = expected.parallelStream()
//                .filter(path -> !Files.exists(path))
//                .peek(path -> System.err.println("缺失文件: " + path))
//                .count();
//
//        if (missing > 0) throw new IOException("完整性校验失败，缺失 " + missing + " 个文件");
//    }
//
//    // 辅助方法
//    private static String getFileExtension(String filename) {
//        int dot = filename.lastIndexOf('.');
//        return (dot == -1) ? "" : filename.substring(dot + 1).toLowerCase();
//    }
//
//    private static boolean isAllowedType(String url) {
//        String path = url.split("[?#]")[0];
//        return path.matches("(?i).*\\.(html?|css|js|png|jpe?g|gif|webp|svg|ico|mp4|webm|mp3|wav|pdf|docx?|xlsx?|zip|rar)");
//    }
//
//    private static boolean isSameDomain(String url) {
//        try {
//            String host = new URL(url).getHost();
//            return ALLOWED_DOMAINS.stream().anyMatch(d -> host.endsWith(d));
//        } catch (MalformedURLException e) {
//            return false;
//        }
//    }
//}