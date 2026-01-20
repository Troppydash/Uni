package workshop.five.graphics;

public class Color {
    public final int red;
    public final int green;
    public final int blue;

    public Color(int red, int green, int blue) {
        this.red = red;
        this.green = green;
        this.blue = blue;
    }

    @Override
    public String toString() {
        return String.format("(%d, %d, %d)", this.red, this.green, this.blue);
    }

    public static final Color BLACK = new Color(0, 0, 0);
    public static final Color WHITE = new Color(255, 255, 255);

}
