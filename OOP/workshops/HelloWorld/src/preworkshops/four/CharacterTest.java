package preworkshops.four;

import java.util.Scanner;
import java.util.stream.Stream;

public class CharacterTest {
    public static void main(String[] args) {
        long count = Stream.of(new Scanner(System.in).nextLine().split("")).filter(key -> Character.isAlphabetic(key.charAt(0))).count();
        System.out.println(count);
    }
}
