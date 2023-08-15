#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <intrin.h>

// Добовление символов из слов в массив уникальных символов
void char_list_add_elem(char* word);
// Проверка подобранных чисел
int chek(char* word1, char* word2, char* word3);
// Рекурсивный поиск решения (полный перебор)
int permutation(char* word1, char* word2, char* word3, int k);

// Структура символа
typedef struct node
{
    char c;// Символ
    int v;// Число соответствующее символу
}char_list;

char_list arr[10];// Массив уникальных символов (так как чисел 10, их не более 10)
int use[10] = { 0,0,0,0,0,0,0,0,0,0 };// Массив использованных чисел
int len;// Длинна максивального по длинне слова

int main()
{
    char word1[65];
    char word2[65];
    char word3[65];

    // Тестовый крипторифм
    /*
    char word1[65] = "attractions";
    char word2[65] = "intentions";
    char word3[65] = "regeneration";
    */

    // Считывание крипторифма
    scanf("%s", word1);
    scanf("%s", word2);
    scanf("%s", word3);

    clock_t begin = clock();
    long long begin2 = __rdtsc();

    // Длинна максимального слова
    len = strlen(word3);

    // Добавление символ из слов в массив символов
    char_list_add_elem(word1);
    char_list_add_elem(word2);
    char_list_add_elem(word3);

    // Рекурсивный поиск решения (полный перебор)
    if (permutation(word1, word2, word3, 0) != 1)
        printf("\nрешение не найдено");

    printf("\n%f\n", (double)(clock() - begin) / CLOCKS_PER_SEC);
    printf_s("%lld ticks\n", __rdtsc() - begin2);
}

//attractions      94419547368  
//intentions        7640647368
//regeneration    102060194736

// Добовление символов из слов в массив уникальных символов
void char_list_add_elem(char* word)
{
    int flag;
    // Перебор символов из слова
    for (int i = 0; i < strlen(word); i++) {
        flag = 0;
        int j = 0;

        // Проверка был ли символ добален ранее
        for (j = 0; j < 10 && arr[j].c != NULL; j++) {
            if (arr[j].c == word[i]) {
                flag = 1;

                break;
            }
        }

        // Символ раньше не встречался, добавляем его
        if (flag == 0) {
            arr[j].c = word[i];
            arr[j].v = -1;
        }
    }
}

// Проверка подобранных чисел
int chek(char* word1, char* word2, char* word3) {
    long long w1 = 0, w2 = 0, w3 = 0; 
    int f;

    // Поразрядный перебор крипторифма
    for (int i = 0; i < len; i++) {
        f = 0;

        // Поиск символов из текущего разряда, в массиве символов
        for (int j = 0; j < 10 && arr[j].c != NULL; j++) {
            if (arr[j].c == word1[i]) {
                w1 = w1 * 10 + (arr[j].v);

                f++;
            }

            if (arr[j].c == word2[i]) {
                w2 = w2 * 10 + arr[j].v;

                f++;
            }

            if (arr[j].c == word3[i]) {
                w3 = w3 * 10 + arr[j].v;

                f++;
            }

            // Все символы из текушего разряда найдены
            if (f == 3)
                break;
        }
    }

    // Проверка подобранных чисел
    if (w1 + w2 == w3) {
        printf("\n%lld", w1);
        printf("\n%lld", w2);
        printf("\n%lld", w3);

        return 1;
    }
    else return 0;
}

// Рекурсивный поиск решения (полный перебор)
int permutation(char* word1, char* word2, char* word3, int k) {
    // Устанавливаем значение для k-того элемента массива символов
    // Пребор всех символов из массива
    for (int i = 0; i < 10; i++) {
        // Число i не занято
        // Если i == 0 то k-тоый элемента массива символов не является началом слова
        if (use[i] != -1 && (arr[k].c != word1[0] || i != 0) && 
            (arr[k].c != word2[0] || i != 0) && (arr[k].c != word3[0] || i != 0)) {
            // Устанавливаем значение для сивола
            arr[k].v = i;
            // Помечаем что число занято
            use[i] = -1;

            if (arr[k + 1].c != NULL && k != 9) {// Продолжаем подбор
                if (permutation(word1, word2, word3, k + 1) == 1)
                    return 1;
            }
            else if (chek(word1, word2, word3) == 1) {// Массив заполнен проверяем
                return 1;
            }

            // Число не подошло, помечаем как свободный, продолжаем подбор
            use[i] = 0;
        }
    }
    arr[k].v = -1;
    return 0;
}
