#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

int* multiplyLongNumberByConstant(const int* number, int length, int constant, int base) {
    int* outnumber = (int*)malloc((length + 1) * sizeof(int));
    int carry = 0;
    for (int i = 0; i < length; i++) {
        int product = number[i] * constant + carry;
        outnumber[i] = product % base; // Зберігаємо лише цифру для поточного розряду
        carry = product / base;     // Переносимо залишок на наступний розряд
    }

    // Обробка випадку, коли після множення останнього розряду залишається перенос
    if (carry > 0) {
        // Припускаємо, що є достатньо місця в масиві для зберігання додаткового розряду
        // Якщо місця недостатньо, потрібно збільшити масив
        outnumber[length] = carry;
    }
    return outnumber;
}

int isGreaterOrEqual(const int* a, int lenA, const int* b, int lenB) {
    if (lenA > lenB) return 1; // a більше, бо має більше значущих розрядів
    if (lenA < lenB) return 0; // b більше, бо має більше значущих розрядів

    // Якщо довжини однакові, порівнюємо по розрядах, починаючи з найбільш значущого
    for (int i = lenA - 1; i >= 0; i--) {
        if (a[i] > b[i]) return 1; // Розряд a більший, ніж відповідний розряд b
        if (a[i] < b[i]) return 0; // Розряд b більший, ніж відповідний розряд a
    }

    // Якщо всі розряди однакові, числа рівні
    return 1;
}

void subtractLongNumbers(const int* a, int lenA, const int* b, int lenB, int* result, int base) {
    int borrow = 0, i = 0;

    // Віднімання спільної частини
    for (; i < lenB; i++) {
        int diff = a[i] - b[i] - borrow;
        if (diff < 0) {
            diff += 10; // Для десяткової системи
            borrow = 1;
        }
        else {
            borrow = 0;
        }
        result[i] = diff;
    }

    // Обробка залишку довшого числа
    for (; i < lenA; i++) {
        int diff = a[i] - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        }
        else {
            borrow = 0;
        }
        result[i] = diff;
    }
}

void addLongNumbers(int* a, int lenA, int* b, int lenB, int** result, int* resultLen, int c) {
    int maxLength = lenA > lenB ? lenA : lenB; // Визначаємо максимальну довжину
    int carry = 0; // Перенос

    // Виділяємо пам'ять для результату, додаткова клітинка для можливого переносу
    *result = (int*)calloc(maxLength + 1, sizeof(int));

    int i = 0;
    for (; i < maxLength; i++) {
        int sum = carry;
        if (i < lenA) {
            sum += a[lenA - 1 - i]; // Додаємо цифри першого числа з кінця
        }
        if (i < lenB) {
            sum += b[lenB - 1 - i]; // Додаємо цифри другого числа з кінця
        }
        (*result)[i] = sum % 10; // Зберігаємо одиниці в результат
        carry = sum / 10; // Зберігаємо перенос
    }
    if (carry > 0) {
        (*result)[i] = carry; // Якщо залишився перенос на кінець
        *resultLen = maxLength + 1;
    }
    else {
        *resultLen = maxLength;
    }
}

void divideLargeNumbers(int* x, int* y, int* q, int* r, int n, int t, int c) {
    for (int j = 0; j <= (n - t); j++) {
        q[j] = 0;
    }
    int* yByNum = multiplyLongNumberByConstant(y, t, pow(c, (n - t)), c);
    while (isGreaterOrEqual(x, (sizeof(x) / sizeof(x[0])), yByNum, (sizeof(yByNum) / sizeof(yByNum[0])))) {
        q[n - t] = q[n - t] + 1;
        int sizeX = (sizeof(x) / sizeof(x[0]));
        int sizeY = (sizeof(yByNum) / sizeof(yByNum[0]));
        int maxLength = sizeX > sizeY ? sizeX : sizeY;
        int* result = (int*)malloc(maxLength * sizeof(int));
        subtractLongNumbers(x, sizeX, yByNum, sizeY, result, c);
        free(x);
        x = result;
    }
    for (int i = n; i >= (t + 1); i--) {
        if (x[i] == y[t]) {
            q[i - t - 1] = c - 1;
        }
        else {
            q[i - t - 1] = (x[i] * c + x[i - 1]) / y[t];
        }
        while ((q[i - t - 1] * (y[t] * c + y[t - 1])) > (x[i] * pow(c, 2) + x[i - 1] * c + x[i - 2])) {
            q[i - t - 1] -= 1;
        }
        int* yByQ = multiplyLongNumberByConstant(y, t, q[i - t - 1], c);
        int* yByQByC = multiplyLongNumberByConstant(yByQ, t, pow(c, (i - t - 1)), c);
        int sizeX = (sizeof(x) / sizeof(x[0]));
        int sizeY = (sizeof(yByQByC) / sizeof(yByQByC[0]));
        int maxLength = sizeX > sizeY ? sizeX : sizeY;
        int* result = (int*)malloc(maxLength * sizeof(int));
        subtractLongNumbers(x, sizeX, yByQByC, sizeY, result, c);
        free(x);
        x = result;
        sizeX = (sizeof(x) / sizeof(x[0]));
        if (x[sizeX - 1] < 0) {
            int* yByC = multiplyLongNumberByConstant(y, t, pow(c, (i - t - 1)), c);
            sizeY = (sizeof(yByC) / sizeof(yByC[0]));
            int* result = NULL;
            int resultLen = 0;
            addLongNumbers(x, sizeX, yByC, sizeY, &result, &resultLen, c);
            free(x);
            x = result;
            q[i - t - 1] -= 1;
        }
    }
    r = x;
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
    int n, t, c;

    printf("Enter the base of the number c: ");
    scanf("%d", &c);
    printf("Enter the number of digits n+1 (dividend): ");
    scanf("%d", &n);
    printf("Enter the number of digits t+1 <= n+1 (divisor): ");
    scanf("%d", &t);
    n -= 1; // Коригування, оскільки користувач вводить кількість цифр, а не індекс n
    t -= 1; // Коригування, оскільки користувач вводить кількість цифр, а не індекс n
    if (n < t) {
        printf("n can't be less than t\n");
    }

    // Виділення пам'яті для масивів
    int* x = (int*)malloc((n + 1) * sizeof(int));
    int* y = (int*)malloc((t + 1) * sizeof(int));
    int* q = (int*)malloc((n - t + 1) * sizeof(int));
    int* r = (int*)malloc((t + 1) * sizeof(int));


    if (x == NULL || y == NULL || q == NULL || r == NULL) {
        printf("Memory allocation error\n");
        // Очищення пам'яті, якщо була виділена частково
        free(x); free(y); free(q); free(r);
        return 1; // Повернення коду помилки
    }

    printf("Enter number x:\n");
    inputArray(x, n, c);
    printf("Enter number y:\n");
    inputArray(y, t, c);
    // перевірка умови x >= y
    if (y[t] == 0) {
        printf("First digit of y can't be 0\n");
        // Очищення пам'яті, якщо була виділена частково
        free(x); free(y); free(q); free(r);
        return -1; // Повернення коду помилки
    }
    divideLargeNumbers(x, y, q, r, n, t, c);

    printf("Result q: ");
    int sizeQ = (sizeof(q) / sizeof(q[0]));
    for (int i = sizeQ - 1; i >= 0; i--) { // Вивід результату з найвищого порядку
        printf("%d", q[i]);
    }
    printf("\n");

    printf("Result r: ");
    int sizeR = (sizeof(r) / sizeof(r[0]));
    for (int i = sizeR - 1; i >= 0; i--) { // Вивід результату з найвищого порядку
        printf("%d", r[i]);
    }
    printf("\n");

    // Звільнення виділеної пам'яті
    free(x);
    free(y);
    free(q);
    free(r);

    system("pause");
    return 0;
}


