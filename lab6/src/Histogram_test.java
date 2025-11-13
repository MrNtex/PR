import java.util.Scanner;


class Histogram_test {

    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);

        System.out.println("Set image size: n (#rows), m(#kolumns)");
        int n = scanner.nextInt();
        int m = scanner.nextInt();
        Obraz obraz_1 = new Obraz(n, m);

        obraz_1.calculate_histogram();
        obraz_1.print_histogram();

        // Wariant - 1
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

        // Wariant - 2
        System.out.println("Set number of threads");
        num_threads = scanner.nextInt();

        Thread[] threads = new Thread[num_threads];

        int symbolsPerThread = 94 / num_threads;
        int remainder = 94 % num_threads;

        int start = 0;
        for (int i = 0; i < num_threads; i++) {
            int extra = (i < remainder) ? 1 : 0;
            int end = start + symbolsPerThread + extra;

            threads[i] = new Thread(new WatekHistogramu2(obraz_1, start, end));
            threads[i].start();

            start = end;
        }

        for (int i = 0; i < num_threads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException _) {}
        }

        obraz_1.compareHistograms();

        // 4.0 - Dekompozycja 1D wariant cykliczny
        obraz_1.clean_histogram_parallel();

        System.out.println("Set number of threads");
        num_threads = scanner.nextInt();

        threads = new Thread[num_threads];
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
    }

}

