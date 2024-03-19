import java.util.Scanner;
import java.util.stream.IntStream;

public class Program {

    public static double[] doublePowers(int n) {
        return IntStream.range(0, n).mapToDouble(val -> Math.pow(2, val)).toArray();
    }

    public static void main(String[] args) {
        Problems.problem_1_1(5);
        Problems.problem_1_2(9);
    }
}
