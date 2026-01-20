package workshop.five;

import java.util.ArrayList;
import java.util.Random;

public abstract class Shape {
    public final double x;
    public final double y;

    public Shape(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public abstract double getArea();

    public abstract double getPerimeter();

    public String toString() {
        return "Plain Shape";
    }
}


class Rectangle extends Shape {

    public final double width;
    public final double height;

    public Rectangle(double x, double y, double width, double height) {
        super(x, y);

        this.width = width;
        this.height = height;
    }

    @Override
    public double getArea() {
        return this.width * this.height;
    }

    @Override
    public double getPerimeter() {
        return 2 * (this.width + this.height);
    }

    @Override
    public String toString() {
        return "Rectangle";
    }
}


class Circle extends Shape {
    public final double radius;

    public Circle(double x, double y, double radius) {
        super(x, y);

        this.radius = radius;
    }

    @Override
    public double getArea() {
        return Math.PI * this.radius * this.radius;
    }

    @Override
    public double getPerimeter() {
        return 2 * this.radius * Math.PI;
    }

    @Override
    public String toString() {
        return "Circle";
    }
}

class ShapeTest {
    public static void main(String[] args) {
        Random rng = new Random();
        ArrayList<Shape> shapes = new ArrayList<>();
        for (int i = 0; i < 10; ++i) {
            if (rng.nextInt(2) == 0) {
                shapes.add(new Rectangle(rng.nextDouble(), rng.nextDouble(), rng.nextDouble() * 5, rng.nextDouble() * 5));
            } else {
                shapes.add(new Circle(rng.nextDouble(), rng.nextDouble(), rng.nextDouble() * 5));
            }
        }

        for (Shape shape : shapes) {
            System.out.println(shape);
        }
    }
}


