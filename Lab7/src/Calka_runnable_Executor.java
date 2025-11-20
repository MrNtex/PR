import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

public class Calka_runnable_Executor {
    public static void main(String[] args) throws ExecutionException, InterruptedException {

        double a = 0;
        double b = Math.PI;

        int NWATKOW = Runtime.getRuntime().availableProcessors();
        int NZADAN = 20;
        double dx = 0.00001;

        ExecutorService executor = Executors.newFixedThreadPool(NWATKOW);

        double step = (b-a) / NZADAN;
        List<Future<Double>> tasks = new ArrayList<Future<Double>>();

        Calka_result calka_result = new Calka_result();

        for (int i = 0; i < NZADAN; i++) {
            double start = a + i * step;
            double end = start + step;

            Calka_runnable task = new Calka_runnable(start, end, dx,calka_result);
            executor.execute(task);
        }

        executor.shutdown();
        executor.awaitTermination(60, TimeUnit.SECONDS);

        System.out.println("Final result: " + calka_result.getTotal());
    }
}
