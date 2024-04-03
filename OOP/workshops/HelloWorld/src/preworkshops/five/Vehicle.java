package preworkshops.five;

public abstract class Vehicle {

    private double weight;
    private String model;

    public Vehicle(double weight, String model) {
        this.weight = weight;
        this.model = model;
    }
}

class Aircraft extends Vehicle {
    private double height;
    public Aircraft(double weight, String model, double height) {
        super(weight, model);

        this.height = height;
    }
}