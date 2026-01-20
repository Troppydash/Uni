package workshop.seven;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

interface FileWriteable {
    default void writeToFile(BufferedWriter writer) throws IOException {
        writer.write(this.toString());
    }
}

class Point implements FileWriteable {
    public final int x;
    public final int y;


    public Point(int x, int y) {
        this.x = x;
        this.y = y;
    }

    @Override
    public String toString() {
        return "Point{" +
                "x=" + x +
                ", y=" + y +
                '}';
    }
}

class Student implements FileWriteable {
    public final String name;
    public final int id;

    public Student(String name, int id) {
        this.name = name;
        this.id = id;
    }

    @Override
    public String toString() {
        return "Student{" +
                "name='" + name + '\'' +
                ", id=" + id +
                '}';
    }
}

class Car implements FileWriteable {
    public final String model;
    // using the least sig 24 bit of the integer
    public final int color;

    public Car(String model, int color) {
        this.model = model;
        this.color = color;
    }

    @Override
    public String toString() {
        return "Car{" +
                "model='" + model + '\'' +
                ", color=" + color +
                '}';
    }
}

// the classes should not inherit from a base class because they are not
// related in sharing attributes/methods, so they should not be the same
// base class type

public class Database {
    private ArrayList<FileWriteable> objects;

    public Database() {
        this.objects = new ArrayList<>();
    }

    public void addObject(FileWriteable object) {
        this.objects.add(object);
    }

    public void removeObject(FileWriteable object) {
        this.objects.remove(object);
    }

    public void writeAll(String filename) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename))) {
            for (var object : this.objects) {
                object.writeToFile(writer);
                writer.write('\n');
            }
        } catch (IOException exception) {
            System.err.println("Exception in database writing");
            exception.printStackTrace(System.err);
        }
    }

    public static void main(String[] args) {
        Database db = new Database();
        db.addObject(new Point(2, 5));
        db.addObject(new Student("me", 100000));
        db.addObject(new Car("audi", 0xff0000));
        db.writeAll("test.txt");

    }
}
