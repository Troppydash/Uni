public class Problems {

    public static void problem_1_1(int size) {
        for (int i = 1; i <= size; ++i) {
            System.out.println("#".repeat(i));
        }
    }

    public static void problem_1_2(int size) {
        if (size % 2 != 1) {
            throw new RuntimeException("non-odd size");
        }

        for (int i = 1; i < size; i += 2) {
            System.out.println(" ".repeat((size-i)/2) + "#".repeat(i));
        }

        for (int i = size; i >= 1; i -= 2) {
            System.out.println(" ".repeat((size-i)/2) + "#".repeat(i));
        }
    }


}
