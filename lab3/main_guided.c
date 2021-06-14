#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

#define ABS(val) (val >= 0 ? val : -val)
#define EXPERIMENTS 50
#define SCHEDULER_TYPE guided
#define CHUNK_SIZE 1
//#define CHUNK_SIZE 5
//#define CHUNK_SIZE 25
//#define CHUNK_SIZE 125
//#define CHUNK_SIZE 625
//#define CHUNK_SIZE 3125

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

    const size_t length = atol(argv[1]);
    const unsigned threadCount = atol(argv[2]);

    if (!length || !threadCount) {
        printf("Bad args\n");
        return 1;
    }

    double full_result = 0;
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    size_t lengthFirstArray = length;
    size_t lengthSecondArray = length/2;
    double *firstArray = malloc(sizeof(double) * lengthFirstArray);
    double *secondArray = malloc(sizeof(double) * lengthSecondArray);
    double *secondArrayCopy = malloc(sizeof(double) * lengthSecondArray);

    for(size_t i = 0; i < EXPERIMENTS; i++){

        srand(i);
        unsigned seed = i;

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

        ///Map
        /// This code maps some functions over the array
        size_t j;

        #pragma omp parallel for default(none) private(j) shared(firstArray, lengthFirstArray) schedule(SCHEDULER_TYPE, CHUNK_SIZE) num_threads(threadCount)
        for(j = 0; j < lengthFirstArray; j++){
            firstArray[j] = pow(firstArray[j] / M_PI, 3);
        }

        #pragma omp parallel for default(none) private(j) shared(secondArray, secondArrayCopy, lengthSecondArray) schedule(SCHEDULER_TYPE, CHUNK_SIZE) num_threads(threadCount)
        for(j = 0; j < lengthSecondArray; j++){
            secondArrayCopy[j] = secondArray[j];
        }

        #pragma omp parallel for default(none) private(j) shared(secondArray, secondArrayCopy, lengthSecondArray) schedule(SCHEDULER_TYPE, CHUNK_SIZE) num_threads(threadCount)
        for(j = 0; j < lengthSecondArray; j++) {
            double previous = 0;
            if (j > 0){
                previous = secondArrayCopy[j-1];
            }
            secondArray[j] = ABS(cos((secondArray[j] + previous)));
        }

        ///Merge
        /// This code merges two arrays
        #pragma omp parallel for default(none) private(j) shared(firstArray, secondArray, lengthSecondArray) schedule(SCHEDULER_TYPE, CHUNK_SIZE) num_threads(threadCount)
        for (j = 0; j < lengthSecondArray; j++) {
            secondArray[j] = firstArray[j] / secondArray[j];
        }

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
        double minNonZero = __DBL_MAX__;
        for (j = 0; j < lengthSecondArray; j++) {
            double value = secondArray[j];
            if (minNonZero > value && value != 0) {
                minNonZero = value;
            }
        }

        double result = 0;
        #pragma omp parallel for default(none) private(j) shared(secondArray, minNonZero, lengthSecondArray) reduction(+:result) schedule(SCHEDULER_TYPE, CHUNK_SIZE) num_threads(threadCount)
        for (j = 0; j < lengthSecondArray; j++) {
            if ((int)floor(secondArray[j] / minNonZero) % 2) {
                result += sin(secondArray[j]);
            }
        }

        full_result+=result;
    }
    free(firstArray);
    free(secondArray);

    gettimeofday(&endTime, NULL);
    long const delta_ms = 1000 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000;

//    printf("FullResult: %f\n", full_result);
//    printf("\nN=%zu. Milliseconds passed: %ld\n", length, delta_ms);
    printf("%zu,%ld,%f\n", length, delta_ms, full_result);
}