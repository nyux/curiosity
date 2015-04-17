#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>

#define MAX_SIZE 4096
#define CUTOFF 10

/* timer algorithm found http://stackoverflow.com/a/361492 */
void timer(void fn(int[], size_t), int *arr, size_t size)
{
    struct timeval start, end, result;
    gettimeofday(&start, NULL);
    fn(arr, size);
    gettimeofday(&end, NULL);
    timersub(&end, &start, &result);
    printf("%ld.%06ld seconds\n", (long int) result.tv_sec,
            (long int) result.tv_usec);
}

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

void insertion_sort(int arr[], size_t size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = i; i > 0 && arr[j - 1] > arr[j]; j--)
        {
            swap(&arr[j - 1], &arr[j]);
        }
    }
}

void selection_sort(int arr[], size_t size)
{
    for (int i = 0; i < size; i++)
    {
        int min = i;

        for (int j = i; j < size; j++)
        {
            if (arr[j] < arr[min]) min = j;
        }

        swap(&arr[i], &arr[min]);
    }
}   

void bubble_sort(int arr[], size_t size)
{
    bool swapped = true;

    while (swapped)
    {
        swapped = false;
        
        for (int i = 1; i < size; i++)
        {
            if (arr[i] < arr[i - 1])
            {
                swapped = true;
                swap(&arr[i], &arr[i - 1]);
            }
        }
    }
}

void qs(int arr[], int low, int high, int cutoff)
{
    if (high - low <= cutoff) return;

    int mid = low + (high - low)/2;
    int pivot_pos = low;

    swap(&arr[mid], &arr[high - 1]);

    for (int i = low; i < high-1; i++)
    {
        if (arr[i] <= arr[high - 1])
        {
            swap(&arr[pivot_pos], &arr[i]);
            pivot_pos++;
        }
    }

    swap(&arr[high - 1], &arr[pivot_pos]);
    qs(arr, low, pivot_pos, cutoff);
    qs(arr, pivot_pos, high, cutoff);
}


void quicksort(int arr[], size_t size)
{
    /* only stop once you reach 1 element subarrays */
    qs(arr, 0, size, 1); 
}

void optimized_quicksort(int arr[], size_t size)
{
    /* stop once you reach CUTOFF element subarrays
     * and then insertion sort the rest, because (if i understand correctly)
     *  - every element will be at most CUTOFF spaces from its proper place,
     *    and insertion sort will go by quickly
     *  - insertion sort for small arrays has less overhead than quicksort 
     * i didn't believe this'd actually be faster until i tried it. it's cool */
    qs(arr, 0, size, CUTOFF);
    insertion_sort(arr, size);
}

int* get_random_array(size_t size)
{
    int *arr = malloc(sizeof(int) * size);

    if (!arr) exit(-1);

    for (int i = 0; i < size; i++) arr[i] = rand();

    return arr;
}

void sort_arrays(void sort_fn(int[], size_t), char *name)
{
    int *arr;

    for (int i = 4; i < MAX_SIZE; i *= 2)
    {
        arr = get_random_array(i);
        printf("a %d element array sorted using %s took ", i, name);
        timer(sort_fn, arr, i);
        free(arr);
    }
}

void iterate_sort_fns()
{
    sort_arrays(insertion_sort, "insertion sort");
    printf("=====\n=====\n");
    sort_arrays(selection_sort, "selection sort");
    printf("=====\n=====\n");
    sort_arrays(bubble_sort, "bubble sort");
    printf("=====\n=====\n");
    sort_arrays(quicksort, "quicksort");
    printf("=====\n=====\n");
    sort_arrays(optimized_quicksort, "optimized quicksort");
}

int main(void)
{
    /* this is probably generating randomness wrong, but i don't need it
     * to be very high-quality for this. */
    srand(time(NULL));
    iterate_sort_fns();
}
