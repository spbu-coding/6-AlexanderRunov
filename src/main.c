#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sortings.h"

bool asc;
typedef struct _StringItem {
    char *str;
    struct _StringItem *next;
} StringItem;

int main(int argc, char *argv[]) {
    if (argc < 6)
        return -1;
    array_size_t n = atoi(argv[1]);
    char file_input[255];
    strcpy(file_input, argv[2]);
    char file_output[255];
    strcpy(file_output, argv[3]);
    int alg;
    if (strcmp(argv[4], "bubble") == 0)
        alg = 1;
    else if (strcmp(argv[4], "insertion") == 0)
        alg = 2;
    else if (strcmp(argv[4], "merge") == 0)
        alg = 3;
    else if (strcmp(argv[4], "quick") == 0)
        alg = 4;
    else if (strcmp(argv[4], "radix") == 0)
        alg = 5;
    else
        return -1;

    if (strcmp(argv[5], "asc") == 0)
        asc = true;
    else if (strcmp(argv[5], "des") == 0)
        asc = false;
    else
        return -1;
    strings_array_t string_array;
    string_array = (strings_array_t) calloc(n, sizeof(char *));
    FILE *file = fopen(file_input, "rt");
    for (array_size_t i = 0; i < n; i++) {
        if (feof(file))
            return -1;
        string_array[i] = (char *) calloc(MAX_INPUT_STRING_SIZE, sizeof(char));

        fgets(string_array[i], MAX_INPUT_STRING_SIZE, file);
    }
    fclose(file);
    switch (alg) {
        case 1:
            bubble(string_array, n, strcmp);
            break;
        case 2:
            insertion(string_array, n, strcmp);
            break;
        case 3:
            merge(string_array, n, strcmp);
            break;
        case 4:
            quick(string_array, n, strcmp);
            break;
        case 5:
            radix(string_array, n, strcmp);
            break;
    }

    file = fopen(file_output, "wt");
    for (array_size_t i = 0; i < n; i++) {
        fprintf(file, "%s", string_array[i]);
        free(string_array[i]);
    }
    fclose(file);
    free(string_array);
    return 0;
}

void bubble(strings_array_t string_array, array_size_t n, comparator_func_t cmp) {
    for (array_size_t i = 0; i < n - 1; i++) {
        bool f = true;
        for (array_size_t j = 0; j < n - 1 - i; j++) {
            if ((asc && cmp(string_array[j], string_array[j + 1]) > 0) || (!asc &&
                                                                           cmp(string_array[j], string_array[j + 1]) <
                                                                           0)) {
                char *t = string_array[j];
                string_array[j] = string_array[j + 1];
                string_array[j + 1] = t;
                f = false;
            }
        }
        if (f)
            return;
    }
}

void insertion(strings_array_t string_array, array_size_t n, comparator_func_t cmp) {
    int j;
    char *key;
    for (array_size_t i = 1; i < n; i++) {
        key = string_array[i];
        j = i - 1;
        while (j >= 0 && ((asc && cmp(string_array[j], key) > 0) || (!asc && cmp(string_array[j], key) < 0))) {
            string_array[j + 1] = string_array[j];
            j = j - 1;
        }
        string_array[j + 1] = key;
    }
}

void merge1(strings_array_t a, array_size_t n, comparator_func_t cmp, int l, int r) {
    if (r == l)
        return;
    if (r - l == 1) {
        if (((asc && cmp(a[l], a[r]) > 0) || (!asc && cmp(a[l], a[r]) < 0))) {
            char *t = a[r];
            a[r] = a[l];
            a[l] = t;
        }
        return;
    }
    int m = (r + l) / 2;
    merge1(a, n, cmp, l, m);
    merge1(a, n, cmp, m + 1, r);
    char *buf[n];
    int xl = l;
    int xr = m + 1;
    int cur = 0;
    while (r - l + 1 != cur) {
        if (xl > m)
            buf[cur++] = a[xr++];
        else if (xr > r)
            buf[cur++] = a[xl++];
        else if (cmp(a[xl], a[xr]) > 0)
            buf[cur++] = a[xr++];
        else buf[cur++] = a[xl++];

    }
    for (int i = 0; i < cur; i++)
        a[i + l] = buf[i];
}


void merge(strings_array_t string_array, array_size_t n, comparator_func_t cmp) {
    merge1(string_array, n, cmp, 0, n - 1);
}

void quick1(strings_array_t a, array_size_t n, comparator_func_t cmp, int l, int r) {
    if (l < r) {
        int left = l, right = r;
        char *middle = a[(left + right) / 2];
        do {
            while (cmp(a[left], middle) < 0) left++;
            while (cmp(a[right], middle) > 0) right--;
            if (left <= right) {
                char *tmp = a[left];
                a[left] = a[right];
                a[right] = tmp;
                left++;
                right--;
            }
        } while (left <= right);
        quick1(a, n, cmp, l, right);
        quick1(a, n, cmp, left, r);
    }
}

void quick(strings_array_t string_array, array_size_t n, comparator_func_t cmp) {
    quick1(string_array, n, cmp, 0, n - 1);
}


StringItem *radix1(StringItem *pList, unsigned int iDigit) {
    const int iRange = 256;

    StringItem *front[iRange];
    memset(front, 0, sizeof(front));

    StringItem **ppNextItem[iRange];
    for (int i = 0; i < iRange; i++)
        ppNextItem[i] = &front[i];

    while (pList) {
        StringItem *temp = pList;
        pList = pList->next;

        temp->next = NULL;

        unsigned char c = (unsigned char) temp->str[iDigit];
        *ppNextItem[c] = temp;
        ppNextItem[c] = &temp->next;
    }

    StringItem *pResult = NULL;
    StringItem **ppNext = &pResult;

    *ppNext = front[0];
    while (*ppNext)
        ppNext = &((*ppNext)->next);

    for (int i = 1; i < iRange; i++) {
        if (!front[i])
            continue;
        if (front[i]->next == NULL)
            *ppNext = front[i];
        else
            *ppNext = (StringItem *) radix1(front[i], iDigit + 1);

        while (*ppNext)
            ppNext = &((*ppNext)->next);
    }
    return pResult;
}

void radix(strings_array_t string_array, array_size_t n, comparator_func_t cmp) {
    cmp(string_array[0],string_array[0]);
    StringItem *pHead;
    StringItem *pTail;
    pHead = malloc(sizeof(StringItem));
    pHead->str = string_array[0];
    pTail = pHead;
    for (array_size_t i = 1; i < n; i++) {
        StringItem *t = malloc(sizeof(StringItem));
        t->str = string_array[i];
        t->next = NULL;
        pTail->next = t;
        pTail = t;
    }
    pHead = (StringItem *) radix1(pHead, 0);
    for (array_size_t i = 0; i < n; i++) {
        pTail = pHead->next;
        string_array[i] = pHead->str;
        free(pHead);
        pHead = pTail;
    }
}
