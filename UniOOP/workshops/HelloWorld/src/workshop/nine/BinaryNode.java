package workshop.nine;

import java.util.*;
import java.util.concurrent.ThreadLocalRandom;
import java.util.stream.Collectors;

public class BinaryNode<T extends Comparable<T>> implements Iterable<T> {
    private T data;
    private BinaryNode<T> left;
    private BinaryNode<T> right;
    private BinaryNode<T> parent;

    public BinaryNode(T data) {
        this.data = data;
        this.left = this.right = this.parent = null;
    }

    public void insert(T data) {
        if (data.compareTo(this.data) <= 0) {
            if (this.left == null) {
                this.left = new BinaryNode<>(data);
                this.left.parent = this;
            } else {
                this.left.insert(data);
            }
        } else {
            if (this.right == null) {
                this.right = new BinaryNode<>(data);
                this.right.parent = this;
            } else {
                this.right.insert(data);
            }
        }
    }


    @Override
    public Iterator<T> iterator() {
        Stack<BinaryNode<T>> stack = new Stack<>();
        stack.add(this);
        Map<BinaryNode<T>, Integer> initial = new HashMap<>();
        initial.put(this, 0);

        return new Iterator<T>() {
            private Map<BinaryNode<T>, Integer> visited = initial;
            private Stack<BinaryNode<T>> prev = stack;

            @Override
            public boolean hasNext() {
                while (!prev.empty()) {
                    var current = prev.peek();
                    if (visited.get(current) == 0) {
                        if (current.left != null) {
                            prev.push(current.left);
                            visited.put(current.left, 0);
                        }
                        visited.put(current, 1);
                    } else if (visited.get(current) == 1) {
                        return true;
                    } else if (visited.get(current) == 2) {
                        if (current.right != null) {
                            prev.push(current.right);
                            visited.put(current.right, 0);
                        }
                        visited.put(current, 3);
                    } else {
                        prev.pop();
                    }
                }

                return false;
            }

            @Override
            public T next() {
                while (true) {
                    var current = prev.peek();
                    if (visited.get(current) == 0) {
                        if (current.left != null) {
                            prev.push(current.left);
                            visited.put(current.left, 0);
                        }
                        visited.put(current, 1);
                    } else if (visited.get(current) == 1) {
                        visited.put(current, 2);
                        return current.data;
                    } else if (visited.get(current) == 2) {
                        if (current.right != null) {
                            prev.push(current.right);
                            visited.put(current.right, 0);
                        }
                        visited.put(current, 3);
                    } else {
                        prev.pop();
                    }
                }
            }
        };
    }

    public static void main(String[] args) {
        List<Integer> numbers = new ArrayList<>();
        for (int i = 0; i < 1000; ++i) {
            numbers.add(ThreadLocalRandom.current().nextInt(1000000));
        }

        var tree = new BinaryNode<Integer>(numbers.get(0));
        for (int i : numbers.stream().skip(1).toList()) {
            tree.insert(i);
        }

        for (var num : tree) {
            System.out.println(num);
        }
    }
}
