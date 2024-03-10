package workshop2;


import java.util.*;

class Circle {
    public double radius;
    public double x, y;

    public Circle() {
        this(1, 0, 0);
    }

    public Circle(double radius) {
        this(radius, 0, 0);
    }

    public Circle(double radius, double x, double y) {
        this.radius = radius;
        this.x = x;
        this.y = y;
    }


    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Circle circle = (Circle) o;
        return Double.compare(radius, circle.radius) == 0 && Double.compare(x, circle.x) == 0 && Double.compare(y, circle.y) == 0;
    }

    @Override
    public String toString() {
        return "Circle{" +
                "radius=" + radius +
                ", x=" + x +
                ", y=" + y +
                '}';
    }
}


class Rectangle {
    public double left, top;
    public double width, height;

    public Rectangle() {
        this(0, 0, 100, 100);
    }

    public Rectangle(double left, double top) {
        this(left, top, 100, 100);
    }

    public Rectangle(double left, double top, double width, double height) {
        this.left = left;
        this.top = top;
        this.width = width;
        this.height = height;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Rectangle rectangle = (Rectangle) o;
        return Double.compare(left, rectangle.left) == 0 && Double.compare(top, rectangle.top) == 0 && Double.compare(width, rectangle.width) == 0 && Double.compare(height, rectangle.height) == 0;
    }

    @Override
    public String toString() {
        return "Rectangle{" +
                "left=" + left +
                ", top=" + top +
                ", width=" + width +
                ", height=" + height +
                '}';
    }
}

class Book {
    private static int booksBorrowed = 0;

    private final String author;
    private final String title;
    private boolean borrowed;
    private String borrower;

    public Book(String author, String title) {
        this.author = author;
        this.title = title;

        this.borrower = "library";
        this.borrowed = false;
    }


    public void borrow(String borrowedBy) {
        if (this.borrowed) {
            throw new RuntimeException("book already borrowed");
        }

        this.borrowed = true;
        this.borrower = borrowedBy;

        Book.booksBorrowed += 1;
    }

    public void returnBook() {
        this.borrowed = false;
        this.borrower = "library";

        Book.booksBorrowed -= 1;
    }

    public static int getBooksBorrowed() {
        return Book.booksBorrowed;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Book book = (Book) o;
        return borrowed == book.borrowed && Objects.equals(author, book.author) && Objects.equals(title, book.title);
    }

    @Override
    public String toString() {
        return "Book{" +
                "author='" + author + '\'' +
                ", title='" + title + '\'' +
                ", borrowed=" + borrowed +
                '}';
    }

    public String getAuthor() {
        return author;
    }

    public String getTitle() {
        return title;
    }

    public boolean isBorrowed() {
        return borrowed;
    }

    public String getBorrower() {
        return borrower;
    }
}

class Library {
    private ArrayList<Book> books;
    private HashMap<String, Integer> cache;

    public Library() {
        this.books = new ArrayList<>(10);
        this.cache = new HashMap<>();
    }

    public Library(ArrayList<Book> books) {
        this.books = books;
        this.cache = new HashMap<>();

        for (int i = 0; i < books.size(); ++i) {
            this.cache.put(books.get(i).getTitle(), i);
        }
    }

    public void addBook(Book newBook) {
        if (books.size() == 10) {
            return;
        }

        this.cache.put(newBook.getTitle(), this.books.size());
        this.books.add(newBook);
    }

    public Optional<Book> lookup(String title) {
        if (this.cache.containsKey(title)) {
            int index = this.cache.get(title);
            return Optional.of(this.books.get(index));
        }

        return Optional.empty();
    }

    public Optional<Book> lookup(String title, String author) {
        Optional<Book> book = this.lookup(title);
        if (book.isEmpty()) {
            return Optional.empty();
        }

        if (Objects.equals(book.get().getAuthor(), author)) {
            return book;
        }
        return Optional.empty();
    }

    public String getCatalogue() {
        StringBuilder builder = new StringBuilder();

        for (Book book : this.books) {
            builder
                    .append(book.getAuthor())
                    .append(": ")
                    .append(book.getTitle())
                    .append("\n");

        }

        return builder.toString();
    }

    public int getNumberOfBooks() {
        return this.books.size();
    }
}


public class Workshop2 {
    public static void main(String[] args) {
        Library library = new Library();
        library.addBook(new Book("Charles Dickens", "Great Expectations"));
        library.addBook(new Book("Sun Tzu", "The Art of War"));

        library.lookup("The Art of War").get().borrow("me");
        System.out.println(library.lookup("The Art of War").get());
        System.out.println(library.lookup("The Art of War").get().getBorrower());

        library.lookup("The Art of War").get().returnBook();
        System.out.println(library.lookup("The Art of War").get());

        System.out.println(library.getCatalogue());
    }
}
