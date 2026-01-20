package workshop.three;


import java.util.ArrayList;
import java.util.List;
import java.util.Random;

class Point {
    private final int x;
    private final int y;

    public Point(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public Point(Point other) {
        this.x = other.x;
        this.y = other.y;
    }

    public double distanceTo(Point other) {
        return Math.sqrt((other.x - this.x) * (other.x - this.x) + (other.y - this.y) * (other.y - this.y));
    }


}


class Household {
    private final String name;
    private final ArrayList<Person> people;

    public Household(String name) {
        this.name = name;
        this.people = new ArrayList<>(5);
    }

    public String getName() {
        return name;
    }

    public void addPerson(Person person) {
        if (this.people.size() >= 5) {
            return;
        }

        this.people.add(person);
    }

    public boolean contains(Person person) {
        return this.people.contains(person);
    }

    public int numCloseOutsideHousehold(double distance) {
        return (int) this.people.stream().filter(person -> person.numCloseOutsideHousehold(distance) > 0).count();
    }
}

public class Person {
    private final String name;
    private final Point point;
    private final Household household;
    private static Person[] people = new Person[100];
    private static int peopleCount = 0;

    public Person(String name, Point point, Household household) {
        this.name = name;
        this.point = point;
        this.household = household;
        if (peopleCount < 100) {
            people[peopleCount++] = this;
        }
    }

    public String getName() {
        return this.name;
    }

    private Person[] peopleCloserThan(double distance) {
        int numCloser = 0;
        // Count how many people are close
        for (int i = 0; i < peopleCount; ++i) {
            if (point.distanceTo(people[i].point) < distance) {
                ++numCloser;
            }
        }
        // Create an appropriately-sized array, and then fill it
        Person[] result = new Person[numCloser];
        int count = 0;
        for (int i = 0; i < peopleCount; ++i) {
            if (point.distanceTo(people[i].point) < distance) {
                result[count++] = people[i];
            }
        }
        return result;
    }

    public int numCloseOutsideHousehold(double distance) {
        Person[] people = peopleCloserThan(distance);
        int count = 0;
        for (Person person : people) {
            // If they are not from this person's household, increment counter
            if (!this.household.contains(person)) {
                ++count;
            }
        }
        return count;
    }

    public static String randomName(Random rng) {
        String out = "";
        for (int i = 0; i < 10; ++i) {
            out = out + ('a' + rng.nextInt(26));
        }
        return out;
    }


    public static Point randomPoint(Random rng) {
        return new Point(rng.nextInt(20), rng.nextInt(20));
    }


    public static void main(String[] args) {
        Household household1 = new Household("household1");
        Household household2 = new Household("household2");
        Random random = new Random();
        for (int i = 0; i < 5; ++i) {
            Person person1 = new Person(randomName(random), randomPoint(random), household1);
            Person person2 = new Person(randomName(random), randomPoint(random), household2);

            household1.addPerson(person1);
            household2.addPerson(person2);
        }

        System.out.println(household1.numCloseOutsideHousehold(10));
    }
}