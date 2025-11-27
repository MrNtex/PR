import java.util.Random;

class Pisarz implements Runnable {
    private final Czytelnia czytelnia;
    private final int id;
    private final Random rand = new Random();

    public Pisarz(Czytelnia czytelnia, int id) {
        this.czytelnia = czytelnia;
        this.id = id;
    }

    @Override
    public void run() {
        try {
            while (true) {
                Thread.sleep(rand.nextInt(2000) + 1000);

                System.out.println("Pisarz " + id + " chce pisać.");
                czytelnia.chce_pisac();

                System.out.println("Pisarz " + id + " --> PISZE !!!");
                czytelnia.info("P " + id);
                Thread.sleep(rand.nextInt(1000)); // Pisanie

                System.out.println("Pisarz " + id + " kończy.");
                czytelnia.koniec_pisania();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}