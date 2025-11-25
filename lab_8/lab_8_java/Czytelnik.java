import java.util.Random;

class Czytelnik implements Runnable {
    private final Czytelnia czytelnia;
    private final int id;
    private final Random rand = new Random();

    public Czytelnik(Czytelnia czytelnia, int id) {
        this.czytelnia = czytelnia;
        this.id = id;
    }

    @Override
    public void run() {
        try {
            while (true) {
                Thread.sleep(rand.nextInt(1000));

                System.out.println("Czytelnik " + id + " chce czytać.");
                czytelnia.chce_czytac();

                System.out.println("Czytelnik " + id + " --> CZYTA.");
                czytelnia.info("C " + id);
                Thread.sleep(rand.nextInt(500));

                System.out.println("Czytelnik " + id + " kończy.");
                czytelnia.koniec_czytania();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
