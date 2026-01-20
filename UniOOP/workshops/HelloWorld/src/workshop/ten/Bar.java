package workshop.ten;


import java.util.Map;


interface DiscountStrategy {
    double getPrice(double original);

    DiscountStrategy Regular = price -> price;
    DiscountStrategy Relative = price -> 0.01 * price;
    DiscountStrategy Member = price -> 0.9 * price;
}

abstract class AbstractBar {
    private final Map<String, DiscountStrategy> people;

    public AbstractBar() {
        this.people = Map.of(
                "me", DiscountStrategy.Relative,
                "alice", DiscountStrategy.Member,
                "bob", DiscountStrategy.Regular
        );
    }


    public double serve(String name, String drink) {
        return this.people.get(name).getPrice(this.getDrinks().get(drink));
    }

    protected abstract Map<String, Double> getDrinks();
}

class CasualBar extends AbstractBar {
    @Override
    protected Map<String, Double> getDrinks() {
        return Map.of(
                "cocktail", 15.00,
                "wine", 10.00,
                "beer", 5.00
        );
    }
}

class ExclusiveBar extends AbstractBar {
    @Override
    protected Map<String, Double> getDrinks() {
        return Map.of(
                "cocktail", 55.00,
                "wine", 40.00,
                "beer", 35.00
        );
    }
}


public class Bar {
    private final Map<String, Double> drinks;
    private final Map<String, DiscountStrategy> people;

    public Bar() {
        this.drinks = Map.of(
                "cocktail", 15.00,
                "wine", 10.00,
                "beer", 5.00
        );
        this.people = Map.of(
                "me", DiscountStrategy.Relative,
                "alice", DiscountStrategy.Member,
                "bob", DiscountStrategy.Regular
        );
    }


    public double serve(String name, String drink) {
        return this.people.get(name).getPrice(this.drinks.get(drink));
    }

    public static void main(String[] args) {
        AbstractBar bar = new ExclusiveBar();
        System.out.format("Me: $%.2f\n", bar.serve("me", "wine"));
        System.out.format("Alice: $%.2f\n", bar.serve("alice", "beer"));
        System.out.format("Bob: $%.2f\n", bar.serve("bob", "cocktail"));

        bar = new CasualBar();
        System.out.format("Me: $%.2f\n", bar.serve("me", "wine"));
        System.out.format("Alice: $%.2f\n", bar.serve("alice", "beer"));
        System.out.format("Bob: $%.2f\n", bar.serve("bob", "cocktail"));

    }
}

