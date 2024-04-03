package workshop.five.graphics;

public class Driver {
    private static final int MAX_SHAPES = 4;

    public static void main(String[] args) {
        Shape[] shapes = new Shape[MAX_SHAPES];
// Black rectangle (red=0, green=0, blue=0) with width 20.52px and height 50px
        shapes[0] = new Rectangle(20.52, 50);
// Crimson-coloured triangle (r=220, g=20, b=60) with base 392.2px and height 0.01px
        shapes[1] = new Triangle(392.2, 0.01, new Color(220, 20, 60));
// White (r=255, g=255, b=255) rectangle with width 50px and height 50.3px
        shapes[2] = new Rectangle(50, 50.3, Color.WHITE);
// Black triangle (red=0, green=0, blue=0) with base 10px and height 20.12px
        shapes[3] = new Triangle(10, 20.12);
        for (Shape shape : shapes) {
            shape.render();
        }
        double average = 0;
        for (int i = 0; i < MAX_SHAPES; i++) {
            average = (average * i + shapes[i].getArea()) / (i + 1);
        }
        System.out.format("Average area of rendered shapes: %.2fpx2\n", average);
    }
}