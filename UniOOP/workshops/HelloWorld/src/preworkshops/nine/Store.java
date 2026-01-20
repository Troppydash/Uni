package preworkshops.nine;

import java.util.ArrayList;
import java.util.List;

class Entertainment {

}

public class Store<T extends Entertainment> {
    private List<T> stock = new ArrayList<>();
    private String name;

    public Store(String name) {
        this.name = name;
    }


    public void addEntertainment(T entry) {
        stock.add(entry);
    }

    public T getLast() {
        int size = stock.size();
        if (size > 0) {
            return stock.get(size - 1);
        }
        return null;
    }
}