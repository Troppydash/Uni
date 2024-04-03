package workshop.five;

import java.util.List;

public class Node<T> {
    public final T value;
    private Node<T> next;
    private Node<T> prev;

    public Node(T value) {
        this.value = value;
        this.next = null;
        this.prev = null;
    }

    public Node<T> getNext() {
        return next;
    }

    public void setNext(Node<T> next) {
        this.next = next;
    }

    public Node<T> getPrev() {
        return prev;
    }

    public void setPrev(Node<T> prev) {
        this.prev = prev;
    }
}

class LinkedList<T> {
    private Node<T> root;
    private int size;

    public LinkedList() {
        this.root = null;
        this.size = 0;
    }

    public Node<T> root() {
        return this.root;
    }

    public int length() {
        return this.size;
    }

    protected Node<T> get(int index) {
        if (index == 0) {
            return this.root;
        }

        Node<T> current = this.root;
        for (int i = 0; i < index; ++i) {
            current = current.getNext();
        }

        return current;
    }

    public void append(T value) {
        this.insert(this.size, value);
    }

    public void insert(int index, T value) {
        if (index < 0 || index > this.size) {
            throw new RuntimeException("cannot insert at an invalid index");
        }

        Node<T> node = new Node<>(value);

        // if empty, we insert at start for index=0
        if (this.size == 0) {
            this.root = node;
            this.size += 1;
            return;
        }

        // else when we insert at head
        if (index == 0) {
            node.setNext(this.root);
            this.root.setPrev(node);
            this.root = node;
            this.size += 1;
            return;
        }

        // get one before
        Node<T> current = this.get(index-1);

        if (current.getNext() != null) {
            current.getNext().setPrev(node);
            node.setNext(current.getNext());
        }

        current.setNext(node);
        node.setPrev(current);
        this.size += 1;
    }

    public void remove(int n) {
        if (n < 0 || n >= this.size) {
            throw new RuntimeException("cannot remove at an invalid index");
        }

        if (this.size == 1) {
            this.root = null;
            this.size -= 1;
            return;
        }

        if (n == 0) {
            this.root.getNext().setPrev(null);
            this.root = this.root.getNext();
            this.size -= 1;
            return;
        }

        Node<T> target = this.get(n);
        if (target.getNext() != null) {
            target.getNext().setPrev(target.getPrev());
        }
        target.getPrev().setNext(target.getNext());
        this.size -= 1;
    }

    @Override
    public String toString() {
        StringBuilder output = new StringBuilder();
        for (int i = 0; i < this.size; ++i) {
            output.append(this.get(i).value.toString()).append(",");
        }

        return output.toString();
    }
}

class Stack<T> extends LinkedList<T> {
    public void push(T value) {
        this.append(value);
    }

    public T pop() {
        if (this.length() == 0) {
            return null;
        }

        T value = this.get(this.length()-1).value;
        this.remove(this.length()-1);
        return value;
    }
}

class Queue<T> extends LinkedList<T> {
    public void enqueue(T value) {
        this.append(value);
    }

    public T take() {
        if (this.length() == 0) {
            return null;
        }

        T value = this.get(0).value;
        this.remove(0);
        return value;
    }
}

class NodeTest {
    public static void main(String[] args) {
        LinkedList<Integer> list = new LinkedList<>();
        list.append(1);
        list.append(2);
        list.insert(0, 5);
        System.out.println(list);
        System.out.println(list.length());
        list.remove(2);
        System.out.println(list);
        System.out.println(list.length());
    }
}