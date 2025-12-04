package java_executor_test;

import java.util.concurrent.ForkJoinPool;
import java.util.Arrays;

public class ScalTabForkJoin {
    public static int[] sort(int[] arr) {
        ForkJoinPool pool = new ForkJoinPool();
        DivideTask task = new DivideTask(arr);
        int[] sorted = pool.invoke(task);
        return sorted;
    }
}
