import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Czytelnia {
    private int liczba_czyt = 0;
    private int liczba_pisz = 0;

    private final ReentrantLock lock = new ReentrantLock(true);

    private final Condition czytelnicy = lock.newCondition();
    private final Condition pisarze = lock.newCondition();

    public void chce_pisac() throws InterruptedException {
        lock.lock();
        try {
            while (liczba_czyt + liczba_pisz > 0 || lock.hasWaiters(pisarze)) {
                pisarze.await();
            }
            liczba_pisz++;
        } finally {
            lock.unlock();
        }
    }

    public void koniec_pisania() {
        lock.lock();
        try {
            liczba_pisz--;

            if (lock.hasWaiters(pisarze)) {
                pisarze.signal();
            } else {
                czytelnicy.signal();
            }
        } finally {
            lock.unlock();
        }
    }


    public void chce_czytac() throws InterruptedException {
        lock.lock();
        try {
            while (liczba_pisz > 0 || lock.hasWaiters(pisarze)) {
                czytelnicy.await();
            }

            liczba_czyt++;

            czytelnicy.signal();

        } finally {
            lock.unlock();
        }
    }

    public void koniec_czytania() {
        lock.lock();
        try {
            liczba_czyt--;

            if (liczba_czyt == 0) {
                pisarze.signal();
            }
        } finally {
            lock.unlock();
        }
    }

    public void info(String kto) {
        System.out.printf("[%s] L_Czyt: %d, L_Pisz: %d\n", kto, liczba_czyt, liczba_pisz);
    }
}