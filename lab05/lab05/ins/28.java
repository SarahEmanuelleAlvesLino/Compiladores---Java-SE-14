class Principal {
  public static int F28(int n, int a, int b) {
    int sum = 0;                

    if (n <= 0) {     
        return 0;
    }

    if (a > b) {             
        sum = a - b;
    } else {
        sum = b - a;
    }

    while (n > 0) {               
        sum = sum + n;
        n = n - 1;
    }

    for (n = 0; n < 3; n = n + 1) { 
        sum = sum + n;
    }

    if (sum > 10) {              
        return F50(sum / 3, a, b); 
    }

    return sum;
    }
}
