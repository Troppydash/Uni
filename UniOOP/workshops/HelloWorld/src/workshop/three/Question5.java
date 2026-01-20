package workshop.three;

import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;

public class Question5 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String line = scanner.nextLine();
        List<String> words = List.of(line.split(" "));
        System.out.println(
                words.stream().map(word -> String.valueOf(word.charAt(0)).toUpperCase()).collect(Collectors.joining())
        );
    }
}
