package workshop.eight;

import java.util.ArrayList;

class Subject {
    private String name;

    private ArrayList<Student> students;

    public Subject(String name) {
        this.name = name;
        this.students = new ArrayList<>();
    }

    public void enrol(Student student) {
        this.students.add(student);
    }
}

class Student extends Person {
    private int number;
    private ArrayList<Subject> subjects;

    public Student(String name, int age, int number) {
        super(name, age);

        this.number = number;
        this.subjects = new ArrayList<>();
    }

    public void enrol(Subject subject) {
        this.subjects.add(subject);
    }
}


public class Person implements Comparable<Person> {

    private String name;
    private int age;

    public Person(String name, int age) {
        this.name = name;
        this.age = age;
    }

    @Override
    public int compareTo(Person other) {
        return age - other.age;
    }
}
