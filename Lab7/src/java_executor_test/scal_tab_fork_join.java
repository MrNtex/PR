import java.util.concurrent.ForkJoinPool;
import java.util.Arrays;

public class ScalTabForkJoin extends java.util.concurrent.RecursiveTask<int[]> {
    public static void main(String[] args) {
        int[] arr = {5, 2, 9, 1, 7, 3, 6};

        ForkJoinPool pool = new ForkJoinPool();
        ScalTabForkJoin task = new ScalTabForkJoin(arr);
        int[] sorted = pool.invoke(task);

        System.out.println("Posortowana tablica:");
        System.out.println(Arrays.toString(sorted));
    }
}