#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void subtractLargeNumbers(int* x, int* y, int* s, int n, int c) {
    int b = 0;
    for (int i = 0; i <= n; i++) {
        s[i] = (x[i] - y[i] + b) % c;
        if (x[i] - y[i] + b >= 0) {
            b = 0;
        }
        else {
            s[i] += c;
            b = -1;
        }
    }
}

void inputArray(int* array, int n, int c) {
    printf("Enter %d digits of number with base %d:\n", n + 1, c);
    for (int i = n; i >= 0; i--) {
        scanf("%d", &array[i]);
        // Перевірка на коректність введення
        if (array[i] < 0 || array[i] >= c) {
            printf("Incorrect input: %d is not a digit with base %d!\n", array[i], c);
            i++; // Простий спосіб змусити користувача повторити введення
        }
    }
}

int main() {
    int n, c;

    printf("Enter the base of the number c: ");
    scanf("%d", &c);
    printf("Enter the number of digits n+1: ");
    scanf("%d", &n);
    n -= 1; // Коригування, оскільки користувач вводить кількість цифр, а не індекс n

    // Виділення пам'яті для масивів
    int* x = (int*)malloc((n + 1) * sizeof(int));
    int* y = (int*)malloc((n + 1) * sizeof(int));
    int* s = (int*)malloc((n + 1) * sizeof(int));


    if (x == NULL || y == NULL || s == NULL) {
        printf("Memory allocation error\n");
        // Очищення пам'яті, якщо була виділена частково
        free(x); free(y); free(s);
        return 1; // Повернення коду помилки
    }

    printf("Enter number x:\n");
    inputArray(x, n, c);
    printf("Enter number y:\n");
    inputArray(y, n, c);
    // перевірка умови x >= y
    for (int i = n; i >= 0; i--) {
        if (x[i] < y[i]) {
            printf("x should be greater than y\n");
            free(x); free(y); free(s);
            return -1;
        }
        if (x[i] > y[i]) {
            break;
        }
    }
    subtractLargeNumbers(x, y, s, n, c);

    printf("Result s: ");
    for (int i = n; i >= 0; i--) { // Вивід результату з найвищого порядку
        printf("%d", s[i]);
    }
    printf("\n");

    // Звільнення виділеної пам'яті
    free(x);
    free(y);
    free(s);

    system("pause");
    return 0;
}




