package java_executor_test;

import java.util.Arrays;
import java.util.concurrent.ForkJoinPool;

public class MergeSortTest {
    public static void main(String[] args) {
        int[] data = {5, 2, 8, 1, 9, 4, 7, 3, 6, 0};
        System.out.println("Oryginalna tablica: " + Arrays.toString(data));

        ForkJoinPool pool = new ForkJoinPool();
        DivideTask mainTask = new DivideTask(data);

        int[] sortedData = pool.invoke(mainTask);

        System.out.println("Posortowana tablica: " + Arrays.toString(sortedData));

        pool.close();
    }
}
