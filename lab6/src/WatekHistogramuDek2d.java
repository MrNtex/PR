public class WatekHistogramuDek2d implements Runnable {
    final int thread_id;
    final int start;
    final int end;
    final Obraz obraz;

    WatekHistogramuDek2d(int thread_id, int start, int end, Obraz obraz) {
        this.thread_id = thread_id;
        this.start = start;
        this.end = end;
        this.obraz = obraz;
    }

    @Override
    public void run() {
        int[] local_histogram = new int[94];

        for (int idx = start; idx < end; idx++)
        {
            int i = idx / obraz.getSize_m();
            int j = idx % obraz.getSize_m();

            char c = obraz.getCharFromTab(i, j);
            local_histogram[c-33]++;
        }

        obraz.save_local_histogram(thread_id, local_histogram);
    }

}
