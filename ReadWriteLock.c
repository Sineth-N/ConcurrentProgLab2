//
// Created by dimuth on 4/9/17.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

struct list_node_s {
    int data;
    struct list_node_s *next;
};

struct list_node_s *head_pp = NULL;
bool unique_array[65536] = {0};
int *operation_array;
float *time_array;
int elements;
int count = 0;
float overhead;

long thread_count;
pthread_mutex_t mutex;


//checking for membership
int member(int value) {
    struct list_node_s *curr_p = head_pp;

    while (curr_p != NULL && curr_p->data < value) {
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        return 0;
    } else {
        return 1;
    }
}


//inserting values to the linkedlist
int insert(int value) {
    struct list_node_s *curr_p = head_pp;
    struct list_node_s *pred_p = NULL;
    struct list_node_s *temp_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;

        if (pred_p == NULL) {
            head_pp = temp_p;
        } else {
            pred_p->next = temp_p;
        }
        return 1;
    } else {
        return 0;
    }
}
//delete value if present
int delete(int value) {
    struct list_node_s *curr_p = head_pp;
    struct list_node_s *pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data < value) {
        if (pred_p == NULL) {
            head_pp = curr_p->next;
            free(curr_p);
        } else {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        unique_array[value] = false;
        return 1;
    } else {
        return 0;
    }
}

void populateLinkedList(int size) {
    srand(time(NULL));
    int *rand_array = malloc(size * sizeof(int));
    int i;
    for (i = 0; i < size; i++) {
        int value = rand() % 65536;
        if (!unique_array[value]) {
            insert(value);
            unique_array[value] = true;
        } else {
            i = i - 1;
        }
    }
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


//algorithm for shuffling integer array
void shuffleArray(int *array, int size) {
    srand(time(NULL));
    int i;
    for (i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&array[i], &array[j]);
    }
}

int getUniqueRandomNumber() {
    int random = rand() % 65536;

    while (unique_array[random]) {
        random = rand() % 65536;
    }
    return random;
}


//generating operations
int *operationProceduence(int m, float mem, float ins, float del) {
    float tot_mem = m * mem;
    float tot_ins = tot_mem + m * ins;
    float tot_del = tot_ins + m * del;

    operation_array = malloc(m * sizeof(int));
    int j;
    for (j = 0; j < m; j++) {
        if (j < tot_mem) {

            operation_array[j] = 1;
        } else if (j < tot_ins) {
            operation_array[j] = 2;
        } else if (j < tot_del) {
            operation_array[j] = 3;
        }
    }
    shuffleArray(operation_array, m);

    return operation_array;
}

int printList() {
    struct list_node_s *curr_p = head_pp;
    int count = 0;
    while (curr_p != NULL) {
        curr_p = curr_p->next;
        count++;
    }
    return count;
}

float calculateSD(float data[], int size) {
    float sum = 0.0, mean, standardDeviation = 0.0;
    int i;
    for (i = 0; i < size; ++i) {
        sum += data[i];
    }
    mean = sum / size;
    for (i = 0; i < size; ++i) {
        standardDeviation += pow(data[i] - mean, 2);
    }
    return sqrt(standardDeviation / size);
}

float calculateMean(float data[], int size) {
    float sum = 0.0, mean = 0.0;
    int i;
    for (i = 0; i < size; ++i) {
        sum += data[i];
    }
    return sum / size;
}


//method that execute parallel
void *thread_oparation(void *rank) {

    int thread_number = (long) rank;
    int instances = elements / thread_count;
    int val1, val2, val3;
    int start = thread_number, j;
    for (j = start; j < elements; j = j + thread_count) {
        count++;
        if (operation_array[j] == 1) {
            pthread_rwlock_rdlock(&rwlock);
            val1 = rand() % 65536;
            member(val1);
            pthread_rwlock_unlock(&rwlock);

        } else if (operation_array[j] == 2) {
            pthread_rwlock_wrlock(&rwlock);
            val2 = getUniqueRandomNumber();
            insert(val2);
            pthread_rwlock_unlock(&rwlock);
        } else if (operation_array[j] == 3) {
            pthread_rwlock_wrlock(&rwlock);
            val3 = rand() % 65536;
            delete(val3);
            pthread_rwlock_unlock(&rwlock);
        }

        //printf("%f\n",time_spent);
    }
    return NULL;
}

int main(int argc, char **argv) {
    int i, size = 100;
    time_array = malloc(size * sizeof(int));
    elements = atoi(argv[2]);

    thread_count = atoi(argv[6]);
    long thread;
    pthread_t *thread_handles;

    thread_handles = (pthread_t *) malloc(thread_count * sizeof(pthread_t));

    for (i = 0; i < size; i++) {
        overhead = 0;
        populateLinkedList(atoi(argv[1]));
        operationProceduence(atoi(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));



        clock_t begin = clock();
        for (thread = 0; thread < thread_count; thread++) {
            pthread_create(&thread_handles[thread], NULL, thread_oparation, (void *) thread);
        }

        for (thread = 0; thread < thread_count; thread++) {
            pthread_join(thread_handles[thread], NULL);
        }
        clock_t end = clock();

        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
        time_array[i] = time_spent - overhead;
        printf("%d\n", i);
        printf("count = %d \n", printList());
        head_pp = NULL;
        int l;
        for (l = 0; l < 65536; ++l) {
            unique_array[l] = 0;
        }

    }


    printf("STD = %f \t", calculateSD(time_array, size));
    printf("AVG = %f \n", calculateMean(time_array, size));


    return 0;
}
