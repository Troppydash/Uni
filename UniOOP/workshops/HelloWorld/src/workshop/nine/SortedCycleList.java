package workshop.nine;

import java.util.*;

public class SortedCycleList<T extends Comparable<T>> extends CycleList<T> {
    @Override
    public Iterator<T> iterator() {
        List<T> list = new ArrayList<>(this.list);
        Collections.sort(list);
        return new Iterator<T>() {
            private int index = 0;

            @Override
            public boolean hasNext() {
                return !list.isEmpty();
            }

            @Override
            public T next() {
                T value = list.get(index);
                index = (index + 1) % list.size();
                return value;
            }
        };
    }
}
