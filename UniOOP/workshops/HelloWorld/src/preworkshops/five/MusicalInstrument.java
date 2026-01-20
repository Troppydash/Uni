package preworkshops.five;

import java.util.ArrayList;
import java.util.List;

public abstract class MusicalInstrument {
    public abstract void play();
}

class Bass extends MusicalInstrument {

    @Override
    public void play() {
        System.out.println("Thumm");
    }
}

class Trombone extends MusicalInstrument {
    @Override
    public void play() {
        System.out.println("Wah wah wah");
    }
}

class Band {
    private ArrayList<MusicalInstrument> instruments;

    public Band(ArrayList<MusicalInstrument> instruments) {
        this.instruments = instruments;
    }

    public void perform() {
        for (MusicalInstrument instrument : this.instruments) {
            instrument.play();
        }
    }
}

class BandTester {
    public static void main(String[] args) {
        Band band = new Band(new ArrayList<>(List.of(new Bass(), new Trombone())));
        band.perform();
    }
}
