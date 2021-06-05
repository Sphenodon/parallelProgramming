#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include "ipp.h"

#define ABS(val) (val >= 0 ? val : -val)
#define EXPERIMENTS 50
//Task
//A = count(Егор)*count(Попов)*count(Александрович) = 4*5*13 = 260
int const A_task = 260;

//Map = 1 + ((260 mod 47) mod 7) = 1 + (25 mod 7) = 5
// Для М1 - деление на Пи с последующим возведением в третью степень
// Для М2 - Модуль косинуса

//Merge = 1 + ((260 mod 47) mod 8) = 1 + (25 mod 8) = 2
// Деление(т.е. М2[i] = M1[i]/M2[i])

//Sort = 1 + ((260 mod 47) mod 6) = 1 + (25 mod 6) = 2
// Гномья сортировка (Gnome sort)

//Reduce = 1 + ((260 mod 47) mod 7) = 1 + (25 mod 7) = 5

int main(int argc, char * argv[]) {

    size_t const length = atoll(argv[1]);
    int threadCount = atoll(argv[2]) ?: 1;
    ippInit();
    ippSetNumThreads(threadCount);

//    printf("ippInit status: %u\n", ippInit());
//    printf("ippSetNumThreads status: %u\n", ippSetNumThreads(threadCount));

    if (!length) {
        printf("Bad args\n");
        return 1;
    }

    double full_result = 0;

    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    size_t lengthFirstArray = length;
    size_t lengthSecondArray = length/2;
    double *firstArray = ippMalloc(lengthFirstArray * sizeof(double));
    double *secondArray = ippMalloc(lengthSecondArray * sizeof(double));
    double *secondArrayCopy = ippMalloc((lengthSecondArray + 1) * sizeof(double));

    for(size_t i = 0; i < EXPERIMENTS; i++){
        srand(i);
        unsigned seed = i;

        secondArrayCopy[0] = 0;

        ///Generate
        /// This code fills two arrays using generation task
        /// @param seed the seed for rand_r() function
        for (size_t j = 0; j < lengthFirstArray; j++) {
            firstArray[j] = rand_r(&seed) % A_task + 1;
        }
        seed = i;
        for (size_t j = 0; j < lengthSecondArray; j++) {
            secondArray[j] = A_task + rand_r(&seed) % (9 * A_task + 1);
        }
        ippsCopy_64f(secondArray, &secondArrayCopy[1], lengthSecondArray);

        ///Map
        /// This code maps some functions over the array
        //firstArray
        ippsDivC_64f(firstArray, M_PI, firstArray, lengthFirstArray);/* поэлементно делим на пи */
        ippsPowx_64f_A53(firstArray, 3, firstArray, lengthFirstArray); /* поэлементно возводим в третью степень */
        //secondArray
        ippsAdd_64f(secondArray, secondArrayCopy, secondArray, lengthSecondArray); /* поэлементно складываем */
        ippsCos_64f_A53(secondArray, secondArray, lengthSecondArray); /* поэлементно пишем косинус */
        ippsAbs_64f_A53(secondArray, secondArray, lengthSecondArray); /* поэлементно берем модуль */

        ///Merge
        /// This code merges two arrays
        ippsDiv_64f(secondArray, firstArray, secondArray, lengthSecondArray);

        ///Sort
        /// This code sorts given array using selection sorting algorithm
        size_t index = 0;
        while (index < lengthSecondArray) {
            if (index == 0) index++;
            if (secondArray[index] >= secondArray[index - 1]) index++;
            else {
                double buff = secondArray[index];
                secondArray[index] = secondArray[index - 1];
                secondArray[index - 1] = buff;
                index--;
            }
        }

        ///Reduce
        /// This code reduces
        /// @param result a value that should be returned as result of this function
        double minNonZero = __DBL_MAX__;
        ippsMin_64f(secondArray, lengthSecondArray, &minNonZero);

        double result = 0;
        for (size_t k = 0; k < lengthSecondArray; k++) {
            if ((int)floor(secondArray[k] / minNonZero) % 2) {
                result += sin(secondArray[k]);
            }
        }

        full_result+=result;
    }
    ippFree(firstArray);
    ippFree(secondArray);
    ippFree(secondArrayCopy);

    gettimeofday(&endTime, NULL);
    long const workTime = 1000 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000;

//    printf("FullResult: %f\n", full_result);
//    printf("\nN=%zu. Milliseconds passed: %ld\n", length, workTime);
    printf("%zu,%ld,%f\n", length, workTime, full_result);
}