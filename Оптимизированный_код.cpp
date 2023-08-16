#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <intrin.h>
//#pragma intrinsic(__rdtsc)

// Подбор решения крипторифма
int permutation2(char* word1, char* word2, char* word3, int c, int j, unsigned long long k);

unsigned long long res1 = 0, res2 = 0, res3 = 0;// Числа соответствующие каждому слова (решение крипторифма)
int len1, len2, len3;// Длинна каждого из слов
char ar[10];// Массив символов которые используются

// Тестовые крипторифмы:
// send  9567
// more  1085
//money 10652

//attractions      94419547368  
//intentions        7640647368
//regeneration    102060194736

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

    long beginс = clock();
    long long begin = __rdtsc();

    // Длинна каждого из слов
    len3 = strlen(word3);
    len2 = strlen(word2);
    len1 = strlen(word1);
    
    /*Проверка случая когда результат сложения длиннее
    каждого из слов. И одно из слогаемых длиннее второго.
    
        abw
      +  vr  => c=1 a=9 r=0
       crtw
    */

    if (len3 > len2 && len3 > len1) {
        ar[1] = word3[0];
        if (len2 > len1) {
            ar[9] = word2[0];
            ar[0] = word3[1];
        }
        else if (len1 > len2) {
            ar[9] = word1[0];
            ar[0] = word3[1];
        }
    }

    /* Выравниваем слова друг относительно друг относительно друга.
    Словам с меньшей длинной вначале строки добавляем " "*/

    char buffer1[65];
    char buffer2[65];
    buffer2[0] = '\0';
    buffer1[0] = '\0';

    while (len3 != len2 || len2 != len1) {
        if (len2 < len1 || len2 < len3) {
            strcat(buffer2, " ");

            len2++;
        }
        if (len1 < len2 || len1 < len3) {
            strcat(buffer1, " ");

            len1++;
        }
    }

    strcat(buffer2, word2);
    strcat(buffer1, word1);

    // Подбор решения крипторифма
    int f = permutation2(buffer1, buffer2, word3, 0, len3 - 1, 1);
    if (f == 1) {
        // Решение найдено, выводим ответ
        printf("\n%lld", res1);
        printf("\n%lld", res2);
        printf("\n%lld", res3);
    }
    else printf("\nрешение не найдено");

    printf("\n%f\n", (double)(clock() - beginс) / CLOCKS_PER_SEC);
    printf_s("%I64d ticks\n", __rdtsc() - begin);
}

/* Подбор решения крипторифма. Движемся по трём словам порозрядно
 начиная с младшего разряда.
 Тоесть рассматриваем такие блоки из трёх символов:
         _
    a b |w|
 +    v |r|
  c r t |w|

Для каждого блока рассматриваем разные случаи. 
Подробнее алгоритм и случаи описаны в отчёте.

word1 - первое слогаемое крипторифма (первое слова)
word2 - второе слогаемое крипторифма (второе слова)
word3 - рузультат сложения крипторифма (третье слово)
с - переход через разряд (принимает значения 1 или 0)
j - номер разряда
k - коэффицент разрада (первы разряд 10, второй 100 и тд.)
 */
int permutation2(char* word1, char* word2, char* word3, int c, int j, unsigned long long k) {
    if (j < 0) {// Для всех разрядов подобраны значения, ответ найден
        return 1;
    }
    
    // Пременные с цыврами соответствующиие каждому символу из текущего блока
    int w1 = -1, w2 = -1, w3 = -1;

    // В этом разряде нет символа
    if (word1[j] == ' ')
        w1 = 0;
    if (word2[j] == ' ')
        w2 = 0;
    
    // Проверяем присвоено ли символу значение
    for (int i = 0; i < 10; i++) {
        if (word1[j] == ar[i])
            w1 = i;
        
        if (word2[j] == ar[i])
            w2 = i;
        
        if (word3[j] == ar[i])
            w3 = i;
        
        if (w1 != -1 && w2 != -1 && w3 != -1)
            break;
    }
    
    int f;
    if (w3 != -1) {
        if (w1 != -1) {
            if (w2 != -1) {
                // Всем символам присвоено значение, проверяем корректность подобранных значений
                if ((w1 + w2 + c) % 10 == w3) {
                    // Всё верно, переходим на следующий разряд
                    f = permutation2(word1, word2, word3, (w1 + w2 + c) / 10, --j, k * 10);

                    if (f == 1) {
                        res1 += w1 * k;
                        res2 += w2 * k;
                        res3 += w3 * k;
                    }
                    return f;
                }
                else
                    return 0;
            }
            else {
                // Неизвестин один символ
                // Расчитываем второй, проверяем занято ли число
                // В зависимости от результата проверки переходим на следующий разряд или возращаемся на предыдущий
                if (w3 - w1 - c >= 0)
                    w2 = w3 - w1 - c;
                else
                    w2 = w3 - w1 - c + 10;
                
                if (ar[w2] != NULL)
                    return 0;
                
                ar[w2] = word2[j];
                f = permutation2(word1, word2, word3, (w1 + w2 + c) / 10, --j, k * 10);

                if (f == 1) {
                    res1 += w1 * k;
                    res2 += w2 * k;
                    res3 += w3 * k;
                }
                else 
                    ar[w2] = NULL;
                return f;
            }
        }
        else if (w2 != -1) {
            // Неизвестин один символ
            // Расчитываем второй, проверяем занято ли число
            // В зависимости от результата проверки переходим на следующий разряд или возращаемся на предыдущий
            if (w3 - w2 - c >= 0)
                w1 = w3 - w2 - c;
            else
                w1 = w3 - w2 - c + 10;
            if (ar[w1] != NULL)
                return 0;
            ar[w1] = word1[j];
            f = permutation2(word1, word2, word3, (w1 + w2) / 10, --j, k * 10);
            if (f == 1) {
                res1 += w1 * k;
                res2 += w2 * k;
                res3 += w3 * k;
            }
            else 
                ar[w1] = NULL;
            return f;
        }
        else {
            if (word1[j] == word2[j]) {
                // Случий двух равных символов
                // a+a=b => b=2*a || b=2*a+1
                if (c == 0) {
                    // На предыдущем разряде не было перехода через разряд
                    // Случай b=2*a
                    w1 = w3 >> 1;

                    if (ar[w1] == NULL) {
                        ar[w1] = word1[j];

                        f = permutation2(word1, word2, word3, (w1<<1) / 10, j - 1, k * 10);

                        if (f == 1) {
                            res1 += w1 * k;
                            res2 += w1 * k;
                            res3 += w3 * k;
                            return 1;
                        }
                        else 
                            ar[w1] = NULL;
                            
                    }

                    w1 = (w3 + 10) >> 1;
                    if (ar[w1] == NULL) {
                        ar[w1] = word1[j];

                        f = permutation2(word1, word2, word3, (w1 <<1) / 10, j - 1, k * 10);

                        if (f == 1) {
                            res1 += w1 * k;
                            res2 += w1 * k;
                            res3 += w3 * k;
                            return 1;
                        }
                        else 
                            ar[w1] = NULL;
                    }
                    return 0;
                }
                else {
                    // На предыдущем разряде был переход через разряд
                    // Случай b=2*a+1
                    w1 = (w3-1) >> 1;

                    if (ar[w1] == NULL) {
                        ar[w1] = word1[j];

                        f = permutation2(word1, word2, word3, (w1<<1 +1) / 10, j - 1, k * 10);
                        if (f == 1) {
                            res1 += w1 * k;
                            res2 += w1 * k;
                            res3 += w3 * k;
                            return 1;
                        }
                        else 
                            ar[w1] = NULL;
                            
                    }

                    w1 = (w3 + 9) >>1;

                    if (ar[w1] == NULL) {
                        ar[w1] = word1[j];

                        f = permutation2(word1, word2, word3, (w1<<1 + 1) / 10, j - 1, k * 10);
                        if (f == 1) {
                            res1 += w1 * k;
                            res2 += w1 * k;
                            res3 += w3 * k;
                            return 1;
                        }
                        else
                            ar[w1] = NULL;
                    }
                    return 0;
                }
            }
            else {
                for (int i = 1; i < 10; i++) {
                    // Неизвестны два символа один из них подбераем, второй расчитываем
                    if (ar[i] == NULL) {
                        w2 = w3 - i - c;

                        if (w2 < 0)
                            w2 = w3 + 10 - i - c;
                        
                        if (i != w2 && ar[w2] == NULL) {
                            ar[i] = word1[j];
                            ar[w2] = word2[j];

                            f = permutation2(word1, word2, word3, (i + w2 + c) / 10, j - 1, k * 10);
                            if (f == 1) {
                                res1 += i * k;
                                res2 += w2 * k;
                                res3 += w3 % 10 * k;
                                return 1;
                            }
                            else {
                                ar[i] = NULL;
                                ar[w2] = NULL;
                            }
                        }
                    }
                }
                // При таких условиях решение не найдено
                // Возврат на предыдущий разряд
                return 0;
            }
        }
    }
    /*
    Аналогичные случиие расматриваем для других символов.
    Ветвь результат сложения не известен.
    */
    else {
        if (w1 != -1) {
            if (w2 != -1) {
                // Неизвестен только результат сложения
                // Расчитываем результат, проверяем занято ли число
                // В зависимости от результата проверки переходим на следующий разряд или возращаемся на предыдущий
                w3 = (w1 + w2 + c) % 10;

                if (ar[w3] != NULL)
                    return 0;
                
                ar[w3] = word3[j];

                f = permutation2(word1, word2, word3, (w1 + w2 + c) / 10, --j, k * 10);
                if (f == 1) {
                    res1 += w1 * k;
                    res2 += w2 * k;
                    res3 += w3 * k;
                }
                else 
                    ar[w3] = NULL;
                return f;
            }
            else {
                // Неизвестны два символа
                // Один подбераем, второй расчитываем
                int sum = w1 + c;

                for (int i = 0; i < 10; i++) {
                    w3 = (i + sum) % 10;

                    if (ar[i] == NULL && ar[w3] == NULL) {
                        ar[i] = word2[j];
                        ar[w3] = word3[j];

                        f = permutation2(word1, word2, word3, (i + sum) / 10, j-1, k * 10);
                        if (f == 1) {
                            res1 += w1 * k;
                            res2 += i * k;
                            res3 += w3 * k;
                            return 1;
                        }
                        else {
                            ar[i] = NULL;
                            ar[w3] = NULL;
                        }
                    }
                }
                // При текущих значениях решение не найдено
                // Вернуться на предыдущий разряд
                return 0;
            }
        }
        else if (w2 != -1) {
            // Неизвестны два символа
            // Один подбераем, второй расчитываем
            int sum = w2 + c;

            for (int i = 0; i < 10; i++) {
                w3 = (i + sum) % 10;

                if (ar[i] == NULL && ar[w3] == NULL) {
                    ar[i] = word1[j];
                    ar[w3] = word3[j];

                    f = permutation2(word1, word2, word3, (i + sum) / 10, j-1, k * 10);
                    if (f == 1) {
                        res1 += i * k;
                        res2 += w2 * k;
                        res3 += w3 % 10 * k;
                        return 1;
                    }
                    else {
                        ar[i] = NULL;
                        ar[w3] = NULL;
                    }
                }
            }
            // При текущих значениях решение не найдено
            // Вернуться на предыдущий разряд
            return 0;
        }
        /*
        Все символы в блоке не известны
        */
        else {
            /* В блоке три одинаковых символа
            a+a=a => a=0 или a=9*/
            if (word1[j] == word2[j] && word1[j] == word3[j]) {
                // На предыдущем разряде небыло прехода через разряд
                // Случай a=0
                if (c == 0) {

                    if (ar[0] != NULL)
                        return 0;
                    
                    ar[0] = word3[j];

                    f = permutation2(word1, word2, word3, 0, --j, k * 10);
                    if (f == 0)
                        ar[0] = NULL;
                    return f;
                }
                else {
                    // На предыдущем разряде был преход  через разряд
                    // Случай a=9
                    if (ar[9] != NULL)
                        return 0;
                    
                    ar[9] = word3[j];

                    f = permutation2(word1, word2, word3, 1, --j, k * 10);
                    if (f == 1) {
                        res1 += 9 * k;
                        res2 += 9 * k;
                        res3 += 9 * k;
                    }
                    else 
                        ar[9] = NULL;
                    return f;
                }
            }
            // Одно из слогаемых и результат сложения одинаковые 
            else if (word1[j] == word3[j]) {
                //a+b=a => b=0 или b=9 и был переход через разряд (3+9+1=13)
                if (c == 0) {
                    // Небыло перехода через разряд, b=0 
                    // а - любое, подбираем значение
                    if (ar[0] != NULL)
                        return 0;
                    
                    ar[0] = word2[j];

                    for (int i = 1; i < 10; i++) {
                        ar[i] = word1[j];

                        f = permutation2(word1, word2, word3, 0, j - 1, k * 10);
                        if (f == 1) {
                            res1 += i * k;
                            res3 += i * k;
                            return 1;
                        }
                        else
                            ar[i] = NULL;
                    }
                    ar[0] = NULL;

                    // При текущих значениях решение не найдено
                    // Вернуться на предыдущий разряд
                    return 0;
                }
                else {
                    // Был переход через разряд
                    // b=9, а>1
                    if (ar[9] != NULL)
                        return 0;
                    
                    ar[9] = word2[j];

                    for (int i = 1; i < 10; i++) {
                        ar[i] = word1[j];

                        f = permutation2(word1, word2, word3, 1, j - 1, k * 10);
                        if (f == 1) {
                            res1 += i * k;
                            res2 += 9 * k;
                            res3 += i * k;
                            return 1;
                        }
                        else
                            ar[i] = NULL;
                    }
                    ar[9] = NULL;

                    // При текущих значениях решение не найдено
                    // Вернуться на предыдущий разряд
                    return 0;
                }
            }
            // Одно из слогаемых и результат сложения одинаковые 
            // Оналогично, но для другого символа
            else if (word2[j] == word3[j]) {
                //b+a=a => b=0 или b=9 и был переход через разряд (3+9+1=13)
                if (c == 0) {
                    // Небыло перехода через разряд, b=0 
                    // а - любое, подбираем значение
                    if (ar[0] != NULL)
                        return 0;
                    
                    ar[0] = word1[j];

                    for (int i = 1; i < 10; i++) {
                        ar[i] = word2[j];

                        f = permutation2(word1, word2, word3, 0, j - 1, k * 10);
                        if (f == 1) {
                            res2 += i * k;
                            res3 += i * k;
                            return 1;
                        }
                        else
                            ar[i] = NULL;
                    }
                    ar[0] = NULL;

                    // При текущих значениях решение не найдено
                    // Вернуться на предыдущий разряд
                    return 0;
                }
                else {
                    // Был переход через разряд
                    // b=9, а>1
                    if (ar[9] != NULL)
                        return 0;
                    
                    ar[9] = word1[j];

                    for (int i = 1; i < 10; i++) {
                        ar[i] = word2[j];

                        f = permutation2(word1, word2, word3, 1, j - 1, k * 10);
                        if (f == 1) {
                            res1 += 9 * k;
                            res2 += i * k;
                            res3 += i * k;
                            return 1;
                        }
                        else
                            ar[i] = NULL;
                    }
                    ar[9] = NULL;

                    // При текущих значениях решение не найдено
                    // Вернуться на предыдущий разряд
                    return 0;
                }
            }
            // Слагаемые одинаковые
            else if (word2[j] == word1[j]) {
                //a+a=b => b=2*a || b=2*a+1
                if (c == 0) {
                    // Был переход через разояд
                    // b=2*a+1
                    // Подбираем один символ, второй расчитываем
                    for (int i = 2; i < 20; i += 2) {
                        w1 = i >> 1;
                        w3 = i % 10;

                        if (ar[w3] == NULL && ar[w1] == NULL) {
                            ar[w1] = word1[j];
                            ar[w3] = word3[j];

                            f = permutation2(word1, word2, word3, i / 10, j - 1, k * 10);
                            if (f == 1) {
                                res1 += w1 * k;
                                res2 += w1 * k;
                                res3 += i % 10 * k;
                                return 1;
                            }
                            else {
                                ar[w1] = NULL;
                                ar[w3] = NULL;
                            }   
                        }
                    }

                    // При текущих значениях решение не найдено
                    // Вернуться на предыдущий разряд
                    return 0;
                }
                else {
                    // Прехода через разряд не было
                    // b=2*a 
                    // Подбираем один символ, второй расчитываем
                    for (int i = 1; i < 20; i += 2) {
                        w1 = (i-1) >> 1;
                        w3 = i % 10;

                        if (ar[w3] == NULL && ar[w1] == NULL) {
                            ar[w1] = word1[j];
                            ar[w3] = word3[j];

                            f = permutation2(word1, word2, word3, (i - 1) / 10, j - 1, k * 10);
                            if (f == 1) {
                                res1 += w1 * k;
                                res2 += w1 * k;
                                res3 += w3 * k;
                                return 1;
                            }
                            else {
                                ar[w1] = NULL;
                                ar[w3] = NULL;
                            }  
                        }
                    }

                    // При текущих значениях решение не найдено
                    // Вернуться на предыдущий разряд
                    return 0;
                }
            }
            /*
            Все символы не известны и все символы различны
            Не один из предыдущих случаев
            Подбираем слогаемые и расчитываем результат
            */
            else {
                int sum;

                for (int i = 1; i < 10; i++) {
                    if (ar[i] == NULL) {
                        ar[i] = word1[j];
                        sum = i + c;

                        for (int r = 1; r < 10 && i != r; r++) {
                            w3 = (r + sum) % 10;

                            if (ar[r] == NULL && ar[w3] == NULL ) {
                                if (w3 != 0 || (word3[j] != word1[0] && word3[j] != word2[0])) {
                                    ar[r] = word2[j];
                                    ar[w3] = word3[j];

                                    f = permutation2(word1, word2, word3, (r + sum) / 10, j - 1, k * 10);
                                    if (f == 1) {
                                        res1 += i * k;
                                        res2 += r * k;
                                        res3 += w3 * k;
                                        return 1;
                                    }
                                    else {
                                        ar[r] = NULL;
                                        ar[w3] = NULL;
                                    }
                                        
                                }
                            }
                        }

                        ar[i] = NULL;
                    }
                }

                // При текущих значениях решение не найдено
                // Вернуться на предыдущий разряд
                return 0;
            }
        }
    }
}
