#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>


void toDecimal(int n, int c, int* a) {
    int i = 0;
    int x = n;
    int q = x / c;
    a[0] = x - q * c;
    while (q > 0) {
        ++i;
        x = q;
        q = x / c;
        a[i] = x - q * c;
    }
    printf("Result: ");
    for (; i >= 0; --i) {
        printf("%d", a[i]);
    }
}

int main() {
    int n;
    int c;
    int a[200];
    printf("Enter number n: ");
    scanf("%d", &n);
    printf("Enter base c: ");
    scanf("%d", &c);
    if (n < 0 || c < 2) {
        printf("Incorrect input, n should be greater than 0, c is greater than 2");
    }
    
    toDecimal(n, c, a);

    printf("\n");
    system("pause");
    return 0;
}
