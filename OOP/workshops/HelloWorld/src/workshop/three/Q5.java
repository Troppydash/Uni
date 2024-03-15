package workshop.three;

import java.util.*;
import java.util.stream.*;

public class Q5 {
    public static void main(String[] args) {
        System.out.println(Stream.of((new Scanner(System.in)).nextLine().split(" ")).map(w -> String.valueOf(w.charAt(0)).toUpperCase()).collect(Collectors.joining()));
    }
}
