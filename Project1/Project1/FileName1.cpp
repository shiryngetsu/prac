#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_FILENAME_LEN 256
#define MAX_BUFFER_SIZE 1024

typedef struct {
    int index;
    int value;
} SearchResult;

// 1. �������� �����
int linear_search(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

// 2. ����� ���� ���������
void search_all_occurrences(int arr[], int size, int target, FILE* output_file) {
    char buffer[MAX_BUFFER_SIZE] = { 0 };
    snprintf(buffer, sizeof(buffer), "������� %d ������ �� ��������: ", target);

    int found = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            char temp[20];
            snprintf(temp, sizeof(temp), "%d ", i);
            strncat(buffer, temp, sizeof(buffer) - strlen(buffer) - 1);
            found = 1;
        }
    }

    if (!found) {
        strncat(buffer, "�� ������.", sizeof(buffer) - strlen(buffer) - 1);
    }
    strncat(buffer, "\n", sizeof(buffer) - strlen(buffer) - 1);

    fprintf(output_file, "%s", buffer);
    printf("%s", buffer);
}

// 3. ����� � �����������
int pointer_search(int* arr, int size, int target) {
    int* end = arr + size;
    for (int* ptr = arr; ptr < end; ptr++) {
        if (*ptr == target) {
            return (int)(ptr - arr);
        }
    }
    return -1;
}

// 4. ����� � ��������� ���������
SearchResult search_with_struct(int arr[], int size, int target) {
    SearchResult res = { -1, -1 };
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            res.index = i;
            res.value = arr[i];
            break;
        }
    }
    return res;
}

// ������ ������� �� �����
int read_array_from_file(const char* filename, int** arr) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("������ ��� �������� �����");
        return 0;
    }

    int count = 0;
    int num;
    while (fscanf(file, "%d", &num) == 1) {
        count++;
    }

    *arr = (int*)malloc(count * sizeof(int));
    if (!*arr) {
        perror("������ ��������� ������");
        fclose(file);
        return 0;
    }

    rewind(file);
    for (int i = 0; i < count; i++) {
        if (fscanf(file, "%d", &(*arr)[i]) != 1) {
            free(*arr);
            *arr = NULL;
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return count;
}

int main() {
    setlocale(LC_ALL, "Rus");

    char filename[MAX_FILENAME_LEN];
    printf("������� ��� ����� � �������: ");
    if (scanf("%255s", filename) != 1) {
        printf("������ ����� ����� �����!\n");
        return 1;
    }

    int* arr = NULL;
    int size = read_array_from_file(filename, &arr);
    if (size == 0 || !arr) {
        return 1;
    }

    printf("������ ������� �������� (%d ���������)\n�������� ������:\n", size);
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    int target;
    printf("������� ����� ��� ������: ");
    if (scanf("%d", &target) != 1) {
        printf("������ ����� ����� ��� ������!\n");
        free(arr);
        return 1;
    }

    // ������� ������ �����
    while (getchar() != '\n');

    printf("\n�������� ����� ������:\n"
        "1. �������� ����� (������ ���������)\n"
        "2. ����� ���� ���������\n"
        "3. ����� � �����������\n"
        "4. ����� � ��������� ���������\n"
        "��� �����: ");

    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("������ ����� ������ ������!\n");
        free(arr);
        return 1;
    }

    FILE* output_file = fopen("search_results.txt", "w");
    if (!output_file) {
        perror("������ ��� �������� ����� �����������");
        free(arr);
        return 1;
    }

    fprintf(output_file, "�������� ������ (%d ���������):\n", size);
    for (int i = 0; i < size; i++) {
        fprintf(output_file, "%d ", arr[i]);
    }
    fprintf(output_file, "\n\n");
    switch (choice) {
    case 1: {
        int result = linear_search(arr, size, target);


        char message[100];
        snprintf(message, sizeof(message),
            "�������� �����: ������� %d %s�� ������� %d.\n",
            target, result == -1 ? "�� ������.\n" : "������ ", result);
        fprintf(output_file, "%s", message);
        printf("%s", message);
        break;
    }
    case 2:
        fprintf(output_file, "��������� ������ ���� ���������:\n");
        printf("��������� ������ ���� ���������:\n");
        search_all_occurrences(arr, size, target, output_file);
        break;
    case 3: {
        int result = pointer_search(arr, size, target);
        char message[100];
        snprintf(message, sizeof(message),
            "����� � �����������: ������� %d %s�� ������� %d.\n",
            target, result == -1 ? "�� ������.\n" : "������ ", result);
        fprintf(output_file, "%s", message);
        printf("%s", message);
        break;
    }
    case 4: {
        SearchResult result = search_with_struct(arr, size, target);
        char message[100];
        snprintf(message, sizeof(message),
            "����� � ����������: ������� %d %s�� ������� %d.\n",
            target, result.index == -1 ? "�� ������.\n" : "������ ", result.index);
        fprintf(output_file, "%s", message);
        printf("%s", message);
        break;
    }
    default:
        fprintf(output_file, "�������� ����� ������ ������.\n");
        printf("�������� ����� ������ ������.\n");
    }

    fclose(output_file);
    free(arr);

    printf("\n���������� ������ ����� ��������� � ���� 'search_results.txt'.\n");
    return 0;
}