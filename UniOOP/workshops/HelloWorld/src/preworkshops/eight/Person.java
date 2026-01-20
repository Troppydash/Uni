package preworkshops.eight;

import java.util.ArrayList;

class Address {
    private String street;
    private String city;
    private String sate;
    private String country;
}


public class Person {
    private String name;
    private int age;
    private double phoneNumber;
    private String emailAddress;

    private ArrayList<Address> addresses;


    public boolean isSenior() {
        return false;
    }
}
