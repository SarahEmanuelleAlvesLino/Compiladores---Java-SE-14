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

    
}

