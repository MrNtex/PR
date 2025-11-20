package lab6;

import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

class Histogram_test {

    private static void executeTasks(ExecutorService executor) {
        executor.shutdown();
        try {
            if (!executor.awaitTermination(60, TimeUnit.SECONDS)) {
                System.err.println("Pula wątków nie zakończyła pracy w zadanym czasie.");
            }
        } catch (InterruptedException e) {
            System.err.println("Przerwano oczekiwanie na zakończenie wątków.");
            Thread.currentThread().interrupt();
        }
    }

    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);

        System.out.println("Set image size: n (#rows), m(#kolumns)");
        int n = scanner.nextInt();
        int m = scanner.nextInt();
        Obraz obraz_1 = new Obraz(n, m);

        obraz_1.calculate_histogram();
        obraz_1.print_histogram();

        // Wariant 1: Oryginalny - Dekompozycja zakresu znaków (jeden znak na wątek)
        int num_threads = 94;

        WatekHistogramu[] NewThr = new WatekHistogramu[num_threads];

        for (int i = 0; i < num_threads; i++) {
            (NewThr[i] = new WatekHistogramu(obraz_1, obraz_1.getSymbol(i), i)).start();
        }

        for (int i = 0; i < num_threads; i++) {
            try {
                NewThr[i].join();
            } catch (InterruptedException _) {}
        }

        obraz_1.compareHistograms();

        obraz_1.clean_histogram_parallel();

        // Wariant 2: Pula Wątków - Dekompozycja zakresu znaków
        System.out.println("\n--- Wariant 2: Pula Wątków (Zakres Znaków) ---");
        System.out.println("Set number of threads");
        num_threads = scanner.nextInt();

        ExecutorService executor2 = Executors.newFixedThreadPool(num_threads);

        int symbolsPerThread = 94 / num_threads;
        int remainder = 94 % num_threads;

        int start = 0;
        long startTime2 = System.nanoTime();

        for (int i = 0; i < num_threads; i++) {
            int extra = (i < remainder) ? 1 : 0;
            int end = start + symbolsPerThread + extra;

            executor2.execute(new WatekHistogramu2(obraz_1, start, end));

            start = end;
        }

        executeTasks(executor2);

        long endTime2 = System.nanoTime();
        System.out.printf("Wariant 2 - Czas wykonania: %.3f ms\n", (endTime2 - startTime2) / 1_000_000.0);

        obraz_1.compareHistograms();

        obraz_1.clean_histogram_parallel();

        // Wariant 4.0: Oryginalny - Dekompozycja 1D (Cykliczny)
        System.out.println("\n--- Wariant 4.0: Dekompozycja 1D (Cykliczny) ---");
        System.out.println("Set number of threads");
        num_threads = scanner.nextInt();

        Thread[] threads = new Thread[num_threads];
        obraz_1.create_local_histogram(num_threads);

        for (int i = 0; i < num_threads; i++) {
            threads[i] = new Thread(new WatekHistogramuDek1d(i, num_threads, obraz_1));
            threads[i].start();
        }

        for (int i = 0; i < num_threads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException _) {}
        }

        obraz_1.sum_local_histograms();
        obraz_1.compareHistograms();

        obraz_1.clean_histogram_parallel();

        // Wariant 5.0 (2D): Pula Wątków - Dekompozycja 2D (blokowa)
        System.out.println("\n--- Wariant 5.0: Pula Wątków (Dekompozycja 2D) ---");
        System.out.println("Set number of threads");
        num_threads = scanner.nextInt();

        ExecutorService executor5 = Executors.newFixedThreadPool(num_threads);

        obraz_1.create_local_histogram(num_threads);

        int totalSize = obraz_1.getSize_n() * obraz_1.getSize_m();
        int blockSize = totalSize / num_threads;

        long startTime5 = System.nanoTime();

        for (int i = 0; i < num_threads; i++) {
            int b_start = i * blockSize;
            int b_end = (i == num_threads - 1) ? totalSize : (i + 1) * blockSize;

            executor5.execute(new WatekHistogramuDek2d(i, b_start, b_end, obraz_1));
        }

        executeTasks(executor5);

        long endTime5 = System.nanoTime();
        System.out.printf("Wariant 5.0 - Czas wykonania: %.3f ms\n", (endTime5 - startTime5) / 1_000_000.0);

        obraz_1.sum_local_histograms();
        obraz_1.compareHistograms();

        obraz_1.clean_histogram_parallel();

        // Wariant 5.0 (3D): Oryginalny - Dekompozycja 3D
        System.out.println("\n--- Wariant 5.0: Dekompozycja 3D ---");
        System.out.println("Set number of threads");
        num_threads = scanner.nextInt();

        threads = new Thread[num_threads*2];
        obraz_1.create_local_histogram(num_threads*2);

        totalSize = obraz_1.getSize_n() * obraz_1.getSize_m();
        blockSize = totalSize / num_threads;

        for (int i = 0; i < num_threads*2; i+=2) {
            int b_start = i/2 * blockSize;
            int b_end = (i/2 == num_threads - 1) ? totalSize : (i/2 + 1) * blockSize;

            threads[i] = new Thread(new WatekHistogramuDek3d(i, b_start, b_end, 0, 94/2, obraz_1));
            threads[i].start();

            threads[i+1] = new Thread(new WatekHistogramuDek3d(i+1, b_start, b_end, 94/2, 94, obraz_1));
            threads[i+1].start();
        }

        for (int i = 0; i < num_threads*2; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException _) {}
        }

        obraz_1.sum_local_histograms();
        obraz_1.compareHistograms();

        scanner.close();
    }

}