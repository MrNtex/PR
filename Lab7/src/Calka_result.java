public class Calka_result
{
    private double result;

    public synchronized void dodaj_calke_czastkowa(double value){
        result += value;
    }

    public double getTotal(){
        return result;
    }
}
