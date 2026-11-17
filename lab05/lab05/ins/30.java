class Principal {
  public static int F50(int n, int a, int b) {
    int sum = 0; 
    int temp = (a * b) - (a + b) / 2;        

    if ((n % 2 == 0) && (a + b > 0)) {
        int x = (n * n) + (a * 2);   
        {
            int y = x - (b / 2); 
            sum = y + helper(a, b, n);   
        }
    }

    if ((a * a + b * b) % 3 == 0) { 
        int y = (a + b) * (a - b);  
        {
            sum = sum + y; 
        }
    } else {
        int d = ((a + b) * (a + b)) / (n + 1); 
        {
            sum = sum - d; 
        }
    }

    while (n > 1) {  
        int step = ((n % 3) + 1); 
        {
            sum = sum + (n * temp) - step + (a + b); 
            n = n - ((n % 2) + 1);
        }
    }

    for (n = 0; n < 4; n = n + 1) { 
        int v = (n * n) - (a - b) * (n + 1); 
        {
            sum = sum + v;  
        }
    }

    if (sum > 100) {    
        {
            int r = ((sum / 3) - F50(n + 1, a - 1, b + 2)) * 2 + (a % 3); 
            sum = sum + r;
        }
    }

    int tail = ((a * b) % (n + 2)) - temp;  
    sum = sum + tail;

    return sum;
    }
}
