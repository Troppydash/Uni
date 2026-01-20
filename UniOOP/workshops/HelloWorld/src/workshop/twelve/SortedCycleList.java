package workshop.twelve;

import java.util.Arrays;
import java.util.Collections;

public class SortedCycleList<T extends Comparable<T>> extends CycleList<T> {
    @Override
    public T next() {
        Collections.sort(items);
        return super.next();
    }

    public static void main(String[] args) {
        var list = new SortedCycleList<Integer>();
        list.add(1);
        list.add(5);
        list.add(4);
        list.add(2);

        for (int i = 0; i < 10; ++i) {
            System.out.println(list.next());
        }
    }
}

