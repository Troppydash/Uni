package workshop.twelve;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

public class CycleList<T> implements Collection<T> {
    protected final List<T> items = new ArrayList<>();
    protected int iterator = 0;

    public T next() {
        T item = items.get(iterator++);
        iterator = iterator % items.size();
        return item;
    }

    public boolean add(T value) {
        items.add(value);
        return true;
    }

    @Override
    public int size() {
        return items.size();
    }

    @Override
    public boolean isEmpty() {
        return size() == 0;
    }

    public boolean contains(Object value) {
        return items.contains(value);
    }

    @Override
    public Iterator<T> iterator() {
        CycleList<T> list = this;

        return new Iterator<T>() {

            @Override
            public boolean hasNext() {
                return true;
            }

            @Override
            public T next() {
                return list.next();
            }
        };
    }

    @Override
    public Object[] toArray() {
        return items.toArray();
    }

    @Override
    public <T1> T1[] toArray(T1[] a) {
        return items.toArray(a);
    }

    public boolean addAll(Collection<? extends T> collection) {
        return items.addAll(collection);
    }

    @Override
    public boolean removeAll(Collection<?> c) {
        return items.removeAll(c);
    }

    @Override
    public boolean retainAll(Collection<?> c) {
        return items.retainAll(c);
    }

    @Override
    public void clear() {
        items.clear();
    }

    public boolean remove(Object item) {
        return items.remove(item);
    }

    @Override
    public boolean containsAll(Collection<?> c) {
        return items.containsAll(c);
    }
}