package workshop.five.graphics;

public class Rectangle extends Shape {
    public static final String NAME = "Rectangle";

    private double width;
    private double height;


    public Rectangle(double width, double height, Color color) {
        super(color, NAME);

        this.width = width;
        this.height = height;
    }

    public Rectangle(double width, double height) {
        super(NAME);

        this.width = width;
        this.height = height;
    }

    @Override
    public double getArea() {
        return this.width * this.height;
    }
}
