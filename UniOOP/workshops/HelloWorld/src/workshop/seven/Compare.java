package workshop.seven;


class AuctionBid implements Comparable<AuctionBid> {
    public final String name;
    public final String item;
    public final int amount;

    public AuctionBid(String name, String item, int amount) {
        this.name = name;
        this.item = item;
        this.amount = amount;
    }

    @Override
    public int compareTo(AuctionBid o) {
        if (name.compareTo(o.name) != 0) {
            return name.compareTo(o.name);
        }

        if (item.compareTo(o.item) != 0) {
            return item.compareTo(o.item);
        }

        return amount - o.amount;
    }
}

class ZooAnimal implements Comparable<ZooAnimal> {
    public final String name;
    public final String species;

    public ZooAnimal(String name, String species) {
        this.name = name;
        this.species = species;
    }

    @Override
    public int compareTo(ZooAnimal o) {
        if (name.compareTo(o.name) != 0) {
            return name.compareTo(o.name);
        }

        return species.compareTo(o.species);
    }
}

class Javamon implements Comparable<Javamon> {
    public final int javadex;
    public final String name;

    public Javamon(int javadex, String name) {
        this.javadex = javadex;
        this.name = name;
    }

    @Override
    public int compareTo(Javamon o) {
        if (javadex != o.javadex) {
            return javadex - o.javadex;
        }

        return name.compareTo(o.name);
    }
}


public class Compare {
}
