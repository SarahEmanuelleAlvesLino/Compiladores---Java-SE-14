class Ponto_t{
    int x;
    int y;
    
    public Ponto_t(int x, int y){
        this.x = x;
        this.y = y;
    }
}

class Triangulo_t{
    Ponto_t a,b, c;
    int cor;

    public Triangulo_t(Ponto_t a, Ponto_t b, Ponto_t c, int cor){
        this.a = a;
        this.b = b;
        this.c = c;
        this.cor = cor;
    }
}

class Ponto_2d_t{
    int x,y;

    public Ponto_2d_t(int x, int y){
        this.x = x;
        this.y = y;
    }
}

class Ponto_3d_t{
    int x,y,z;


    public Ponto_3d_t(int x, int y, int z){
        this.x = x;
        this.y = y;
        this.z = z;
    }
}

class Segmento_2d_t {
    Ponto_2d_t ini;
    Ponto_2d_t fim;

    public Segmento_2d_t(Ponto_2d_t ini, Ponto_2d_t fim) {
        this.ini = ini;
        this.fim = fim;
    }
}

class Segmento_3d_t {
    Ponto_3d_t ini;
    Ponto_3d_t fim;

    public Segmento_3d_t(Ponto_3d_t ini, Ponto_3d_t fim) {
        this.ini = ini;
        this.fim = fim;
    }
}

class Amostra{
    public static double func(Ponto_t[] v, int n, Triangulo_t t){
        if(n <= 0){
            return 1.0;
        }else if(n == 1){
           return 1.01 + v[0].x/ 1.e2 + v[0].y / 0.1e-2 - t.a.x * t.a.x + t.b.y * t.c.x;
        }
        double res = .25e-13;
        for(int i = n - 1; i >= 0 && v[i].x > 0; --i){
            double temp = (v[i].y * v[i].x) % 123;
            if(temp < 0.0){
                res -= res * 2.e-2 + func(v, n-1, t)* temp - t.a.y * t.cor;

            }else{
                res += res * .3e3 + func(v, n - 2, t) * temp + t.c.x * t.cor;
                System.out.println("Estranho, ne?");
            }
        }
        return res;

    }

    public static int F2(Triangulo_t t){
        int a = 0;
        double[] soma = new double[10];
        
        if((t.a.x >= 10 || t.b.y > 20 || t.a.y < 30 || t.b.x <= 50) && !(t.c.x != 90 || t.c.y == 0)){
            return 10 % 3;
        }else{
            a = 1;
        }

        while(a < 10){
            int total = 0;
            total += t.c.x * t.c.y;
            total += t.b.x * t.a.y;
            total += t.a.x * t.b.y;
            soma[a] = total % 100;
            a = a + 1;
        }
        return a;
    }

    public static int fatorialA(int n) {
        if (n <= 1)
            return 1;
        return n * fatorialA(n - 1);
    }

    public static int fatorialB(int n) {
        int a = n - 1;
        if (n <= 1)
            return 1;
        return n * fatorialB(a);
    }

    public static int fatorialC(int n, int P) {
        if (n <= 1)
            return 1;
        return fatorialC(n - 1, P * n);
    }
    public static int fatorialD(int n, int P) {
        int a = n - 1;
        int b = P * n;
        if (n <= 1)
            return 1;
        return fatorialD(a, b);
    }

    public static int fatorialE(int n, int P) {
        if (n <= 1) {
            return 1;
        } else {
            int a = n - 1;
            int b = P * n;
            return fatorialD(a, b);
        }
    }

    public static int C1(int a, int b) {
        if (a < b)
            return 1;
        return 2;
    }

    public static int C2(int a, int b) {
        if (a <= b)
            return 1;
        return 2;
    }

    public static int C3(int a, int b) {
        if (a > b)
            return 1;
        return 2;
    }

    public static int C4(int a, int b) {
        if (a >= b)
            return 1;
        return 2;
    }

    public static int C5(int a, int b) {
        if (a == b)
            return 1;
        return 2;
    }

    public static int C5_(int a, int b) {
        if (a != b)
            return 1;
        return 2;
    }

    public static int D1(int a, int b) {
        if (a != b && a < b)
            return 1;
        return 2;
    }

    public static int D2(int a, int b) {
        if (!(a != b && a < b))
            return 1;
        return 2;
    }

    public static int D3(int a, int b) {
        if ((!(a != b && a < b)) || (a + 2 == b))
            return 1;
        return 2;
    }

    public static int E1(int a, int b) {
        boolean X = ((!(a != b && a < b)) || (a + 2 == b));
        if (a != b || X || a + 5 == b) {
            a++;
        }
        return 2;
    }

    public static int E2(int a, int b) {
        boolean X = ((!(a != b && a < b)) || (a + 2 == b));
        while (a != b || X || a + 5 == b) {
            a++;
            while (a < b) {
                b--;
            }
        }
        return 2;
    }


    public static boolean E2_bool(int a, int b) {
        boolean X = ((!(a != b && a < b)) || (a + 2 == b));
        while (a != b || X || a + 5 == b) {
            a++;
            while (fatorialA(a) < b) {
                b--;
            }
        }
        return X || a > 0;
    }

    public static double F1(Segmento_2d_t seg, Ponto_2d_t p) {
        double ax = seg.ini.x;
        double ay = seg.ini.y;

        double bx = seg.fim.x;
        double by = seg.fim.y;

        double px = p.x;
        double py = p.y;

        double abx = bx - ax;
        double aby = by - ay;

        double apx = px - ax;

        double apy = py - ay;
        double ab2 = abx * abx + aby * aby;
        double t = (apx * abx + apy * aby) / ab2;

        if (t < 0.0)
            t = 0.0;
        if (t > 1.0)
            t = 1.0;

        double qx = ax + t * abx;
        double qy = ay + t * aby;
        double dx = px - qx;
        double dy = py - qy;

        return Math.sqrt(dx * dx + dy * dy);
    }

    public static long dist2(Ponto_2d_t a, Ponto_2d_t b) {
        long dx = (long) a.x - b.x;
        long dy = (long) a.y - b.y;
        return dx * dx + dy * dy;
    }

    // Sobrecarga de método: F2 com parâmetros diferentes
    public static long F2(Segmento_2d_t seg, Ponto_2d_t p) {
        long ax = seg.ini.x;

        long ay = seg.ini.y;
        long bx = seg.fim.x;
        long by = seg.fim.y;

        long px = p.x;
        long py = p.y;
        long abx = bx - ax;
        
        long aby = by - ay;
        long apx = px - ax;
        long apy = py - ay;
        long bpx = px - bx;
        long bpy = py - by;
        long dot1 = apx * abx + apy * aby;
        long dot2 = bpx * abx + bpy * aby;

        if (dot1 <= 0)
            return dist2(p, seg.ini);
        if (dot2 >= 0)
            return dist2(p, seg.fim);

        long cross = abx * apy - aby * apx;
        return (cross * cross) / (abx * abx + aby * aby);
    }

    public static double area_poligono(Ponto_2d_t[] p, int n) {
        long soma = 0;
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            soma += (long) p[i].x * p[j].y - (long) p[j].x * p[i].y;
        }
        if (soma < 0)
            soma = -soma;
        return soma / 2.0;
    }

    public static double area_pol() {
        Ponto_2d_t[] poli = new Ponto_2d_t[1000];
        return area_poligono(poli, 150);
    }

    
}

