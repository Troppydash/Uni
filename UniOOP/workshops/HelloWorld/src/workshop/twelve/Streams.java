package workshop.twelve;

import java.util.Collection;
import java.util.Optional;
import java.util.function.Predicate;

public class Streams {

    public static <T> T findFirst(Predicate<T> pred, Collection<T> collection) {
        for (var item : collection) {
            if (pred.test(item)) {
                return item;
            }
        }

        return null;
    }

    public static <T> T findFirstStream(Predicate<T> pred, Collection<T> collection) {
        return collection.stream()
                .filter(pred)
                .findFirst()
                .orElse(null);
    }

    // ifPresent calls the callback if the value is present
    // Map converts the value into another optional if it is present (Monad)
    // Optional forces testing for None values, null doesn't

    public static void test() {
        Optional<String> str = Optional.of("");
    }

    // Optional is a stream with size zero or one
    // A stream is a list of optionals

    // the common structure shared between them is an underlying list
}
