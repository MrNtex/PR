public class WatekHistogramuDek3d implements Runnable {
    final int thread_id;
    final int start;
    final int end;
    final int ascii_start;
    final int ascii_end;
    final Obraz obraz;

    WatekHistogramuDek3d(int thread_id, int start, int end, int ascii_start, int ascii_end, Obraz obraz) {
        this.thread_id = thread_id;
        this.start = start;
        this.end = end;
        this.ascii_start = ascii_start;
        this.ascii_end = ascii_end;
        this.obraz = obraz;
    }

    @Override
    public void run() {
        int[] local_histogram = new int[94];

        for (int idx = start; idx < end; idx++)
        {
            int i = idx / obraz.getSize_m();
            int j = idx % obraz.getSize_m();

            for (int k = ascii_start; k < ascii_end; k++)
            {
                if (obraz.getCharFromTab(i, j) == obraz.getSymbol(k)) local_histogram[k]++;
            }
        }

        obraz.save_local_histogram(thread_id, local_histogram);
    }

}
