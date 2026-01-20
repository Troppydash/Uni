package workshop.nine;

public class Rectangle<T extends Number> {
    public final Pair<T, T> topLeft;
    public final Pair<T, T> bottomRight;

    public Rectangle(Pair<T, T> topLeft, Pair<T, T> bottomRight) {
        this.topLeft = topLeft;
        this.bottomRight = bottomRight;
    }
}
