package workshop.nine;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.stream.Collectors;

public class Q3 {

    public static HashMap<Character, Integer> frequency(String text) {
        var map = new HashMap<Character, Integer>();
        for (char c : text.toCharArray()) {
            if (!map.containsKey(c)) {
                map.put(c, 1);
            } else {
                map.put(c, map.get(c)+1);
            }
        }

        return map;
    }


    public static String process(ArrayList<String> list) {
        return list.stream()
                .map(str -> str.trim().split(" "))
                .filter(str -> str.length == 1)
                .map(str -> str[0])
                .collect(Collectors.joining(","));
    }


    public static void main(String[] args) {
        var list = new ArrayList<String>();
        list.add("Hello");
        list.add("Hello World");
        list.add(" Yes ");
        list.add(" Yes No ");

        System.out.println(process(list));


        String text = "Hello World";
        var freq = frequency(text);
        System.out.println(freq);
    }
}
