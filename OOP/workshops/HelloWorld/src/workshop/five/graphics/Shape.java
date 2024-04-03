package workshop.five.graphics;

public abstract class Shape {
    public static final Color DEFAULT_COLOR = Color.BLACK;
    private Color color;
    private String name;

    public Shape(String name) {
        this.name = name;
        this.color = DEFAULT_COLOR;
    }

    public Shape(Color color, String name) {
        this.color = color;
        this.name = name;
    }

    public abstract double getArea();

    public void render() {
        System.out.format("Drawing a %s with color:%s and area:%.2fpx2\n", this.name, this.color, this.getArea());
    }
}
