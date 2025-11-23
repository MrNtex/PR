public class WatekHistogramu2 implements Runnable {
    private final Obraz obraz;
    private final int start_znak;
    private final int end_znak;

    public WatekHistogramu2(Object obraz, int start_znak, int end_znak) {
        this.obraz = (Obraz) obraz;
        this.start_znak = start_znak;
        this.end_znak = end_znak;
    }

    public void run() {
        obraz.calculate_for_char_range(start_znak, end_znak);
        for (int i = start_znak; i < end_znak; i++) {
            obraz.print_parallel_bar(obraz.getSymbol(i), obraz.getParallelValue(i), i);
        }
    }
}
