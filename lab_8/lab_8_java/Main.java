import java.util.Random;

public class Main {
    public static void main(String[] args) {
        Czytelnia czytelnia = new Czytelnia();
        int ilosc_czytelnikow = 5;
        int ilosc_pisarzy = 2;

        for (int i = 0; i < ilosc_pisarzy; i++) {
            new Thread(new Pisarz(czytelnia, i)).start();
        }

        for (int i = 0; i < ilosc_czytelnikow; i++) {
            new Thread(new Czytelnik(czytelnia, i)).start();
        }
    }
}
