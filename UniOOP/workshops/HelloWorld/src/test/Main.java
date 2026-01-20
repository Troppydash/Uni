package test;

import java.math.BigInteger;
import java.util.Arrays;

class Utils {
    public BigInteger fib(BigInteger n) {
        if (n.compareTo(BigInteger.ONE) <= 0) {
            return n;
        }


        return fib(n.subtract(BigInteger.ONE)).add(fib(n.subtract(BigInteger.TWO)));
    }

    public int gcd(int a, int b) {
        if (b == 0) {
            return a;
        }

        return gcd(b, a % b);
    }
}

public class Main {
    public static void main(String[] args) {
        // Double num1 = new Double(10);
        // Double num2 = 1;
        // Double num3 = 10.0;
        // Double num4 = new Double("10");
        // System.out.print(num1 == num2);
        // System.out.print(num2 == num3);
        // System.out.print(num3 == num4);
    }
}