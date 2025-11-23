import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class Calka_Executor {
    public static void main(String[] args) throws ExecutionException, InterruptedException {

        double a = 0;
        double b = Math.PI;

        int NWATKOW = 4;
        int NZADAN = 20;

        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter dx: ");
        double dx = scanner.nextDouble();

        ExecutorService executor = Executors.newFixedThreadPool(NWATKOW);

        double step = (b-a) / NZADAN;
        List<Future<Double>> results = new ArrayList<Future<Double>>();
        for (int i = 0; i < NZADAN; i++) {
            double start = a + i * step;
            double end = start + step;

            Calka_callable task = new Calka_callable(start, end, dx);
            results.add(executor.submit(task));
        }

        executor.shutdown();

        double final_result = 0;
        for (Future<Double> result : results) {
            final_result += result.get();
        }

        System.out.println("Final result: " + final_result);
    }
}
