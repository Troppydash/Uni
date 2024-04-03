package workshop.five.graphics;

public class Triangle extends Shape {
    public static final String NAME = "Triangle";

    private double base;
    private double height;


    public Triangle(double base, double height, Color color) {
        super(color, NAME);

        this.base = base;
        this.height = height;
    }

    public Triangle(double base, double height) {
        super(NAME);

        this.base = base;
        this.height = height;
    }

    @Override
    public double getArea() {
        return this.base * this.height / 2;
    }
}
