import java.util.HashSet;
import java.util.Objects;

class Book{
    String title;
    String author;
    String isbn;
    Book(String title, String author, String isbn){
        this.title = title;
        this.author = author;
        this.isbn = isbn;
    }
    public String toString(){
        return "《"+title+"》，作者："+author+"，ISBN："+isbn;
    }
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Book book = (Book) o;
        return Objects.equals(title, book.title) &&
                Objects.equals(author, book.author) &&
                Objects.equals(isbn, book.isbn);
    }
    public int hashCode() {
        return Objects.hash(title, author, isbn);
    }
}
class BookManager {
    HashSet<Book> books=new HashSet<Book>();
    void addBook(Book book){
        books.add(book);
    }
    Book findBookByTitle(String title){
        for(Book book:books){
            if(book.title.equals(title)){
                return book;
            }
        }
        return null;
    }
    void listAllBooks(){
        for(Book book:books){
            System.out.println(book);
        }
    }
}
public class T2_2 {
    public static void main(String[] args){
        BookManager bookManager=new BookManager();
        Book book1=new Book("Java编程思想","Bruce Eckel","978-7-111-23866-3");
        Book book2=new Book("Java编程思想","Bruce Eckel","978-7-111-23866-3");
        bookManager.addBook(book1);
        bookManager.addBook(book1);
        bookManager.addBook(book2);
        bookManager.listAllBooks();
    }
}
