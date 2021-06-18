#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

#if defined(_OPENMP)
#include "omp.h"
#else
void omp_set_nested(int val)
{
    val = 1;
}
#endif

#define ABS(val) (val >= 0 ? val : -val)
#define EXPERIMENTS 50
#define SCHEDULER_TYPE guided
//#define CHUNK_SIZE 1
//#define CHUNK_SIZE 5
//#define CHUNK_SIZE 25
//#define CHUNK_SIZE 125
//#define CHUNK_SIZE 625
#define CHUNK_SIZE 3125

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

#if defined(_OPENMP)
void do_timer(int *status)
{
	int val;

	#pragma omp critical
	{
		val = *status;
	}

	while(val < 100) {
		#pragma omp critical
		{
			val = *status;
		}
		printf("Status = %d%%\n", val);
		usleep(1000*1000);
	}
}
#endif

void gnomeSort(double *secondArray, int lengthSecondArray) {
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
}

void mergeArrays(double *dst, double *arr1, double *arr2, int len1, int len2) {
    int i, j, k;
    i = j = k = 0;
    for (i = 0; i < len1 && j < len2;) {
        if (arr1[i] < arr2[j]) {
            dst[k] = arr1[i];
            k++;
            i++;
        } else {
            dst[k] = arr2[j];
            k++;
            j++;
        }
    }
    while (i < len1) {
        dst[k] = arr1[i];
        k++;
        i++;
    }
    while (j < len2) {
        dst[k] = arr2[j];
        k++;
        j++;
    }
}

void do_main(int argc, char *argv[], int *status){

    const size_t length = atol(argv[1]);
    const unsigned threadCount = atol(argv[2])?:1;
    double T1, T2;

    double full_result = 0;
    struct timeval startTime, endTime;

    #if defined(_OPENMP)
    T1 = omp_get_wtime();
    #else
    gettimeofday(&startTime, NULL);
    #endif

    size_t lengthFirstArray = length;
    size_t lengthSecondArray = length/2;
    double *firstArray = malloc(sizeof(double) * lengthFirstArray);
    double *secondArray = malloc(sizeof(double) * lengthSecondArray);
    double *secondArrayCopy = malloc(sizeof(double) * lengthSecondArray);
    double *mergedArray = malloc(sizeof(double) * lengthSecondArray);

    for(size_t i = 0; i < EXPERIMENTS; i++){
        #if defined(_OPENMP)
        #pragma omp critical
		{
			*status = 100 * (i + 1) / EXPERIMENTS;
		}
        #endif

        srand(i);
        size_t j;

        ///Generate
        /// This code fills two arrays using generation task
        /// @param seed the seed for rand_r() function
        unsigned int seed = i;
        #pragma omp parallel for default(none) private(j) shared(firstArray, lengthFirstArray, seed) schedule(SCHEDULER_TYPE, CHUNK_SIZE) num_threads(threadCount)
        for (j = 0; j < lengthFirstArray; j++) {
            unsigned int seed_i = j + seed;
            firstArray[j] = rand_r(&seed_i) % A_task + 1;
        }
        #pragma omp parallel for default(none) private(j) shared(secondArray, lengthSecondArray, seed) schedule(SCHEDULER_TYPE, CHUNK_SIZE) num_threads(threadCount)
        for (j = 0; j < lengthSecondArray; j++) {
            unsigned int seed_i = j + seed;
            secondArray[j] = A_task + rand_r(&seed_i) % (9 * A_task + 1);
        }

        ///Map
        /// This code maps some functions over the array
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
        #if defined(_OPENMP)
        #pragma omp parallel sections shared(secondArray, lengthSecondArray) num_threads(2)
		{
				#pragma omp section
				gnomeSort(secondArray, lengthSecondArray/2);
				#pragma omp section
				gnomeSort(secondArray + (lengthSecondArray / 2), (lengthSecondArray + 1) / 2);
		}
		mergeArrays(mergedArray, secondArray, secondArray + (lengthSecondArray / 2), lengthSecondArray / 2, (lengthSecondArray + 1) / 2);
        #else
        gnomeSort(secondArray, lengthSecondArray);
        #endif

        ///Reduce
        /// This code reduces

        double minNonZero = __DBL_MAX__;
        for (j = 0; j < lengthSecondArray; j++) {
            #if defined(_OPENMP)
            double value = mergedArray[j];
            #else
            double value = secondArray[j];
            #endif
            if (minNonZero > value && value != 0) {
                minNonZero = value;
            }
        }

        #if defined(_OPENMP)
        double result = 0;
        #pragma omp parallel for default(none) private(j) shared(mergedArray, minNonZero, lengthSecondArray) reduction(+:result) schedule(SCHEDULER_TYPE, CHUNK_SIZE) num_threads(threadCount)
        for (j = 0; j < lengthSecondArray; j++) {
            if ((int)floor(mergedArray[j] / minNonZero) % 2) {
                result += sin(mergedArray[j]);
            }
        }
        #else
        double result = 0;
        for (j = 0; j < lengthSecondArray; j++) {
            if ((int)floor(secondArray[j] / minNonZero) % 2) {
                result += sin(secondArray[j]);
            }
        }
        #endif

        full_result+=result;

    }

    #if defined(_OPENMP)
    T2 = omp_get_wtime();
    long const delta_ms = (T2 - T1) * 1000;
    #else
    gettimeofday(&endTime, NULL);
    long const delta_ms = 1000 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000;
    #endif

    printf("%zu,%ld,%f\n", length, delta_ms, full_result);

    free(firstArray);
    free(secondArray);
    free(secondArrayCopy);
    free(mergedArray);
}

int main(int argc, char* argv[]) {
    const size_t length = atol(argv[1]);
//    const unsigned threadCount = atol(argv[2])?:1;
    int status = 0;

    omp_set_nested(1);

    if (!length) {
        printf("Bad args\n");
    }

    #if defined(_OPENMP)
    #pragma omp parallel sections shared(status) num_threads(2)
	{
		#pragma omp section
        {
		do_timer(&status);
		}
		#pragma omp section
        {
		do_main(argc, argv, &status);
		}
	}
    #else
    do_main(argc, argv, &status);
    #endif

    return 0;
}

