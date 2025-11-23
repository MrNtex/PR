package lab6;

public class WatekHistogramu extends Thread {
    private final Obraz obraz;
    private final char znak;
    private final int idx;

    public WatekHistogramu(Obraz obraz, char znak, int idx) {
        this.obraz = obraz;
        this.znak = znak;
        this.idx = idx;
    }

    @Override
    public void run() {
        obraz.calculate_for_char(znak, idx);
        int liczba = obraz.getParallelValue(idx);
        obraz.print_parallel_bar(znak, liczba, idx+1);
    }
}
