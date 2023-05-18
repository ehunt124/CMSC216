#include <stdio.h>
#include <math.h>

#define MAX_SIZE 50

void swap(int* x, int* y) {
    int tmp = *x;
    *x = *y;
    *y= tmp;
}

void modified_bubble_sort(int sort_arr[], int size) {
    int i, j;
    double c1, c2;

    for(i = 0; i < size; i += 4) {
        for(j = 0; j < size - i; j += 4) {
            c1 = ((double)sort_arr[j + 1] * ((double)sort_arr[j + 2] / 100.0));
            c2 = ((double)sort_arr[j + 5] * ((double)sort_arr[j + 6] / 100));
            if(c1 < c2) { /* Sorting based on value */
                swap(&sort_arr[j], &sort_arr[j + 4]);
                swap(&sort_arr[j + 1], &sort_arr[j + 5]);
                swap(&sort_arr[j + 2], &sort_arr[j + 6]);
                swap(&sort_arr[j + 3], &sort_arr[j + 7]);
            } else if (c1 == c2 && sort_arr[j] > sort_arr[j + 4]) { 
                /* Sorting based on assign number */
                swap(&sort_arr[j], &sort_arr[j + 4]);
                swap(&sort_arr[j + 1], &sort_arr[j + 5]);
                swap(&sort_arr[j + 2], &sort_arr[j + 6]);
                swap(&sort_arr[j + 3], &sort_arr[j + 7]);
            }
        }
    }
}

double numeric_score(int assign_arr[], int size, int penalty, int num_drop) {
    int i, weight_lost = 0;
    double total = 0;

    /* ((size - 1) * 4) for size to simplify bubble sort method */
    modified_bubble_sort(assign_arr, (size - 1) * 4);

    if(num_drop > 0) { /* If there is a drop calculate the weight lost */
        for(i = (size - num_drop) * 4; i < size * 4; i += 4) {
            weight_lost += assign_arr[i + 2];
        }
        weight_lost /= (size - num_drop); /* Compute the average weight lost */
        for(i = 0; i < (size - num_drop) * 4; i += 4) {
            /* Add average weight to each weight in the array */
            total += ((assign_arr[i + 2] + (double)weight_lost) / 100.0) * 
                (assign_arr[i + 1] - (assign_arr[i + 3] * penalty));
        }
    } else {/* Same computation as above just without the weight lost in drop */
        for(i = 0; i < size * 4; i += 4) {
            total += (assign_arr[i + 2] / 100.0) * 
                (assign_arr[i + 1] - (assign_arr[i + 3] * penalty));
        }
    }
    return total;
}

double square(double x) {
    return x * x;
}

double mean_calc(int assign_arr[], int size, int penalty) {
    int i;
    double total = 0;

    for(i = 1; i < size * 4; i += 4) {
        total += assign_arr[i] - (assign_arr[i + 2] * penalty);
    }
    return (total / size);
}

double standev_calc(int assign_arr[], int size, int penalty) {
    int i;
    double total = 0, mean = mean_calc(assign_arr, size, penalty);
    
    for(i = 1; i < size * 4; i += 4) {
        total += square(mean - (assign_arr[i] - (assign_arr[i + 2] * penalty)));
    }
    return sqrt(total / size);
}

int main() {
    int num_assignments, penalty, num_drop, bool_stat, i, j, k, weight_total;
    char char_stat;
    int assign_arr[4 * MAX_SIZE] = {0}, sort_arr[4 * MAX_SIZE] = {0};

    /* Line 1 scan */
    scanf("%d %d %c ", &penalty, &num_drop, &char_stat);
    if(char_stat == 'y' || char_stat == 'Y') {
        bool_stat = 1;
    } else {
        bool_stat = 0;
    }

    /* Line 2 scan */
    scanf("%d ", &num_assignments);

    /* Assignment scan */
    for(i = 0; i < num_assignments * 4; i++) {
        scanf("%d%*c", &(assign_arr[i]));
    }

    /* Checking weight total */
    for(i = 2; i < num_assignments * 4; i += 4) {
        weight_total += assign_arr[i];
    }
    if(weight_total != 100) {
        printf("ERROR: Invalid values provided");
        return 1; /* Skips output */
    }

    /* Output */
    printf("Numeric Score: %5.4f\nPoints Penalty Per Day Late: %d\nNumber of "
        "Assignments Dropped: %d\nValues Provided:\nAssignment, Score, Weight, "
        "Days Late\n", numeric_score(assign_arr, num_assignments, penalty, 
        num_drop), penalty, num_drop);

    /* Assignment sort */
    i = 0;
    while(i < num_assignments) {
        for(j = 0; j < num_assignments * 4; j += 4) {
            if(assign_arr[j] == (i + 1)) {
                for(k = 0; k < 4; k++) {
                    sort_arr[(4 * i) + k] = assign_arr[j + k];
                }
                i++;
            }
        }
    }
    /* Sorted Print */
    for(i = 0; i < num_assignments * 4; i += 4) {
        printf("%d, %d, %d, %d\n", sort_arr[i], sort_arr[i + 1], sort_arr[i + 2]
            , sort_arr[i + 3]);
    }

    /* Statistic Calc */
    if(bool_stat) {
        printf("Mean: %5.4f, Standard Deviation: %5.4f\n", 
            mean_calc(assign_arr, num_assignments, penalty), 
            standev_calc(assign_arr, num_assignments, penalty));
    }
    return 0;
}