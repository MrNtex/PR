public class Calka_runnable implements Runnable {

    private double dx;
    private double xp;
    private double xk;
    private int N;
    private Calka_result calka_result;

    public Calka_runnable(double xp, double xk, double dx, Calka_result calka_result) {
        this.xp = xp;
        this.xk = xk;
        this.dx = dx;
        this.N = (int) Math.ceil((xk-xp)/dx);
        this.calka_result = calka_result;
    }

    @Override
    public void run() {
        calka_result.dodaj_calke_czastkowa(compute_integral());
    }

    private double getFunction(double x) {
        return Math.sin(x);
    }

    public double compute_integral() {
        double calka = 0;
        int i;
        for(i=0; i<N; i++){
            double x1 = xp+i*dx;
            double x2 = x1+dx;
            calka += ((getFunction(x1) + getFunction(x2))/2.)*dx;
        }
        System.out.println("Calka czastkowa: " + calka);
        return calka;
    }
}
