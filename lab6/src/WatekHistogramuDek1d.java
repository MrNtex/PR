public class WatekHistogramuDek1d implements Runnable {
    final int thread_id;
    final int step;
    final Obraz obraz;

    WatekHistogramuDek1d(int thread_id, int step, Obraz obraz) {
        this.thread_id = thread_id;
        this.step = step;
        this.obraz = obraz;
    }

    @Override
    public void run() {
        int[] local_histogram = new int[94];

        for (int i = thread_id; i < obraz.getSize_n(); i += step)
        {
            for (int j = 0; j < obraz.getSize_m(); j++){
                char c = obraz.getCharFromTab(i, j);
                local_histogram[c-33]++;
            }
        }

        obraz.save_local_histogram(thread_id, local_histogram);
    }

}
