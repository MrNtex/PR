import java.util.concurrent.ForkJoinPool;
import java.util.Arrays;

public class ScalTabForkJoin extends java.util.concurrent.RecursiveTask<int[]> {
    public static int[] main(int[] arr) {
        ForkJoinPool pool = new ForkJoinPool();
        ScalTabForkJoin task = new ScalTabForkJoin(arr);
        int[] sorted = pool.invoke(task);
        return sorted;
    }
}