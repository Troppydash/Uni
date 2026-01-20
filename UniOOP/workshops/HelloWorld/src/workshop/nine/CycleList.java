package workshop.nine;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class CycleList<T> implements Iterable<T> {
    protected ArrayList<T> list;


    public CycleList() {
        this.list = new ArrayList<>();
    }

    public void add(T value) {
        this.list.add(value);
    }

    public boolean contains(T value) {
        return this.list.contains(value);
    }

    public void remove(T value) {
        this.list.remove(value);
    }

    public void addAll(Collection<T> collection) {
        this.list.addAll(collection);
    }

    @Override
    public Iterator<T> iterator() {
        ArrayList<T> list = this.list;
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

    public static void main(String[] args) {
        CycleList<Integer> list = new SortedCycleList<>();
        list.add(1);
        list.add(4);
        list.add(4);
        list.add(8);
        list.add(5);
        list.add(2);
        list.add(7);

        var it = list.iterator();
        for (int i = 0; i < 20; ++i) {
            int value = it.next();
            System.out.println(value);
        }
    }
}

