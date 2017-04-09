#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <time.h>
#include "timer.h"

#define ERR_NO_NUM -1
#define ERR_NO_MEM -2


typedef struct node {
    int value;
    struct node *next;
} node_t;

node_t *createNewLinkedList(int value) {
    node_t *head = NULL;
    head = malloc(sizeof(node_t));
    head->value = value;
    head->next = NULL;
    return head;
}


void insert(node_t *head, int value) {
    node_t *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    node_t *newNode = malloc(sizeof(node_t));
    current->next = newNode;
    newNode->value = value;
    newNode->next = NULL;
}

void printLinkedList(node_t *head) {
    node_t *current = head;
    while (current->next != NULL) {
        printf("%d ->", current->value);
        current = current->next;
    }
    printf("%d \n", current->value);

}

bool member(int value, node_t *head) {
    node_t *current = head;
    while (current->value != value && current->next != NULL) {
        current = current->next;
    }
    if (current->value == value) {
        return true;
    }
    return false;
}

bool delete(int value, node_t *head) {
    if (head->value == value) {
        if (head->next == NULL) {
            head->value = NULL;
            return 1;
        } else {
            head->value = head->next->value;
            head->next = head->next->next;
            return 1;
        }
    } else {
        node_t *current = head->next;
        node_t *previous = head;

        while (current->value != value && current->next != NULL) {
            current = current->next;
            previous = previous->next;
        }
        if (current->value == value) {
            previous->next = current->next;
            return 1;
        } else {
            return 0;
        }

    }

}


node_t *createRandomLinkedList(int n) {
    int i;
    time_t t;
    int randomNumbers[n];
    /* Initialises random number generator */
    srand((unsigned) time(&t));
    /* Print 5 random numbers from 0 to 49 */
    for (i = 0; i < n; i++) {
        randomNumbers[i] = rand() % 65535;
    }
    node_t *head = createNewLinkedList(randomNumbers[0]);
    for (int index = 1; index < n; index++) {
        insert(head, randomNumbers[index]);
    }
    return head;
}


int main() {
    double start, finish, elapsed;
    time_t t;

    srand((unsigned) time(&t));
    node_t *head = createRandomLinkedList(1000);
    GET_TIME(start);
    for (int i = 0; i < 9900; ++i) {
        member(rand() % 65535, head);
    }
    GET_TIME(finish);
    elapsed = finish - start;
    printf("The elapsed time is %e seconds\n", elapsed);
    return 0;
}

