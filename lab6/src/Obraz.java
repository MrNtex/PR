import  java.util.Random;


class Obraz {

    private int size_n;
    private int size_m;
    private char[][] tab;
    private char[] tab_symb;
    private int[] histogram;
    private int[] hist_parallel;
    private int[][] local_histograms;

    public Obraz(int n, int m) {

        this.size_n = n;
        this.size_m = m;
        tab = new char[n][m];
        tab_symb = new char[94];

        final Random random = new Random();

        // for general case where symbols could be not just integers
        for(int k=0;k<94;k++) {
            tab_symb[k] = (char)(k+33); // substitute symbols
        }

        for(int i=0;i<n;i++) {
            for(int j=0;j<m;j++) {
                tab[i][j] = tab_symb[random.nextInt(94)];  // ascii 33-127
                //tab[i][j] = (char)(random.nextInt(94)+33);  // ascii 33-127
                System.out.print(tab[i][j]+" ");
            }
            System.out.print("\n");
        }
        System.out.print("\n\n");

        histogram = new int[94];
        hist_parallel = new int[94];
        clear_histogram();
    }

    public void clear_histogram(){

        for(int i=0;i<94;i++) histogram[i]=0;

    }

    public void calculate_histogram(){

        for(int i=0;i<size_n;i++) {
            for(int j=0;j<size_m;j++) {

                // optymalna wersja obliczania histogramu, wykorzystujÄca fakt, Ĺźe symbole w tablicy
                // moĹźna przeksztaĹciÄ w indeksy tablicy histogramu
                // histogram[(int)tab[i][j]-33]++;

                // wersja bardziej ogĂłlna dla tablicy symboli nie utoĹźsamianych z indeksami
                // tylko dla tej wersji sensowne jest zrĂłwnoleglenie w dziedzinie zbioru znakĂłw ASCII
                for(int k=0;k<94;k++) {
                    if(tab[i][j] == tab_symb[k]) histogram[k]++;
                    //if(tab[i][j] == (char)(k+33)) histogram[k]++;
                }

            }
        }

    }

    public void clean_histogram_parallel(){
        for(int i=0;i<94;i++) hist_parallel[i]=0;
    }

    public void calculate_for_char(char znak, int idx) {
        int count = 0;
        for (int i = 0; i < size_n; i++) {
            for (int j = 0; j < size_m; j++) {
                if (tab[i][j] == znak) count++;
            }
        }
        hist_parallel[idx] = count;
    }

    public void calculate_for_char_range(int start_znak, int end_znak) {
        for (int i = 0; i < size_n; i++) {
            for (int j = 0; j < size_m; j++) {
                for (int k = start_znak; k < end_znak; k++)
                {
                    if (tab[i][j] == tab_symb[k]) hist_parallel[k]++;
                }
            }
        }
    }

    public synchronized void print_parallel_bar(char znak, int liczba, int id) {
        System.out.print("Wątek " + id + ": " + znak + " ");
        for (int i = 0; i < liczba; i++) System.out.print("=");
        System.out.println();
    }

    public void create_local_histogram(int n) {
        local_histograms = new int[n][94];
    }

    public void save_local_histogram(int id, int[] histogram) {
        local_histograms[id] = histogram;
    }

    public void sum_local_histograms()
    {
        for (int i=0;i<94;i++) {
            for (int[] localHistogram : local_histograms) {
                hist_parallel[i] += localHistogram[i];
            }
        }
    }


// uniwersalny wzorzec dla rĂłĹźnych wariantĂłw zrĂłwnoleglenia - moĹźna go modyfikowaÄ dla
// rĂłĹźnych wersji dekompozycji albo stosowaÄ tak jak jest zapisane poniĹźej zmieniajÄc tylko
// parametry wywoĹania w wÄtkach
//
//calculate_histogram_wzorzec(start_wiersz, end_wiersz, skok_wiersz,
//                            start_kol, end_kol, skok_kol,
//                            start_znak, end_znak, skok_znak){
//
//  for(int i=start_wiersz;i<end_wiersz;i+=skok_wiersz) {
//     for(int j=start_kol;j<end_kol;j+=skok_kol) {
//        for(int k=start_znak;k<end_znak;k+=skok_znak) {
//           if(tab[i][j] == tab_symb[k]) histogram[k]++;
//


    public void print_histogram(){

        for(int i=0;i<94;i++) {
            System.out.print(tab_symb[i]+" "+histogram[i]+"\n");
            //System.out.print((char)(i+33)+" "+histogram[i]+"\n");
        }

    }
    public void compareHistograms() {
        boolean ok = true;
        for (int i = 0; i < 94; i++) {
            if (histogram[i] != hist_parallel[i]) {
                System.out.println("Roznica dla znaku '" + tab_symb[i] +
                        "': seq=" + histogram[i] +
                        " par=" + hist_parallel[i]);
                ok = false;
            }
        }
        if (ok) System.out.println("Histogramy sa identyczne!");
    }

    public char getSymbol(int i) { return tab_symb[i]; }
    public int getParallelValue(int i) { return hist_parallel[i]; }
    public int getSize_n() { return size_n; }
    public int getSize_m() { return size_m; }
    public char getCharFromTab(int i, int j) { return tab[i][j]; }
    public int[][] getLocalHistograms() { return local_histograms; }
}
