package preworkshops.ten;

import java.util.Scanner;
import java.util.TreeMap;

public class Program{
    public static void main(String[] args){
        System.out.println("Please enter integers:");

        TreeMap<Integer, Integer> frequency = new TreeMap<>();

        Scanner scanner = new Scanner(System.in);

        int highest = 0;
        while (scanner.hasNext()) {
            if (!scanner.hasNextInt()) {
                break;
            }

            int num = scanner.nextInt();
            frequency.put(num, frequency.getOrDefault(num, 0)+1);
            highest = Math.max(highest, frequency.get(num));
        }

        for (var entry : frequency.entrySet()) {
            if (entry.getValue() == highest) {
                System.out.format("Most Frequent Number: %d, Frequency: %d\n", entry.getKey(), entry.getValue());
            }
        }
    }
}