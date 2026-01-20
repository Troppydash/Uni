package preworkshops.three;

public class Bus {
    private int route;
    private String colour;
    private String name;

    public Bus(int route, String colour, String name){
        this.route = route;
        this.colour = colour;
        this.name = name;
    }

    public int getRoute() {
        return route;
    }

    public void setRoute(int route) {
        this.route = route;
    }

    public String getColour() {
        return colour;
    }

    public void setColour(String colour) {
        this.colour = colour;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}