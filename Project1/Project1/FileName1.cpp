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

// 1. Линейный поиск
int linear_search(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

// 2. Поиск всех вхождений
void search_all_occurrences(int arr[], int size, int target, FILE* output_file) {
    char buffer[MAX_BUFFER_SIZE] = { 0 };
    snprintf(buffer, sizeof(buffer), "Элемент %d найден на позициях: ", target);

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
        strncat(buffer, "не найден.", sizeof(buffer) - strlen(buffer) - 1);
    }
    strncat(buffer, "\n", sizeof(buffer) - strlen(buffer) - 1);

    fprintf(output_file, "%s", buffer);
    printf("%s", buffer);
}

// 3. Поиск с указателями
int pointer_search(int* arr, int size, int target) {
    int* end = arr + size;
    for (int* ptr = arr; ptr < end; ptr++) {
        if (*ptr == target) {
            return (int)(ptr - arr);
        }
    }
    return -1;
}

// 4. Поиск с возвратом структуры
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

// Чтение массива из файла
int read_array_from_file(const char* filename, int** arr) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка при открытии файла");
        return 0;
    }

    int count = 0;
    int num;
    while (fscanf(file, "%d", &num) == 1) {
        count++;
    }

    *arr = (int*)malloc(count * sizeof(int));
    if (!*arr) {
        perror("Ошибка выделения памяти");
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
    printf("Введите имя файла с числами: ");
    if (scanf("%255s", filename) != 1) {
        printf("Ошибка ввода имени файла!\n");
        return 1;
    }

    int* arr = NULL;
    int size = read_array_from_file(filename, &arr);
    if (size == 0 || !arr) {
        return 1;
    }

    printf("Массив успешно загружен (%d элементов)\nИсходный массив:\n", size);
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    int target;
    printf("Введите число для поиска: ");
    if (scanf("%d", &target) != 1) {
        printf("Ошибка ввода числа для поиска!\n");
        free(arr);
        return 1;
    }

    // Очистка буфера ввода
    while (getchar() != '\n');

    printf("\nВыберите метод поиска:\n"
        "1. Линейный поиск (первое вхождение)\n"
        "2. Поиск всех вхождений\n"
        "3. Поиск с указателями\n"
        "4. Поиск с возвратом структуры\n"
        "Ваш выбор: ");

    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Ошибка ввода выбора метода!\n");
        free(arr);
        return 1;
    }

    FILE* output_file = fopen("search_results.txt", "w");
    if (!output_file) {
        perror("Ошибка при создании файла результатов");
        free(arr);
        return 1;
    }

    fprintf(output_file, "Исходный массив (%d элементов):\n", size);
    for (int i = 0; i < size; i++) {
        fprintf(output_file, "%d ", arr[i]);
    }
    fprintf(output_file, "\n\n");
    switch (choice) {
    case 1: {
        int result = linear_search(arr, size, target);


        char message[100];
        snprintf(message, sizeof(message),
            "Линейный поиск: элемент %d %sна позиции %d.\n",
            target, result == -1 ? "не найден.\n" : "найден ", result);
        fprintf(output_file, "%s", message);
        printf("%s", message);
        break;
    }
    case 2:
        fprintf(output_file, "Результат поиска всех вхождений:\n");
        printf("Результат поиска всех вхождений:\n");
        search_all_occurrences(arr, size, target, output_file);
        break;
    case 3: {
        int result = pointer_search(arr, size, target);
        char message[100];
        snprintf(message, sizeof(message),
            "Поиск с указателями: элемент %d %sна позиции %d.\n",
            target, result == -1 ? "не найден.\n" : "найден ", result);
        fprintf(output_file, "%s", message);
        printf("%s", message);
        break;
    }
    case 4: {
        SearchResult result = search_with_struct(arr, size, target);
        char message[100];
        snprintf(message, sizeof(message),
            "Поиск с структурой: элемент %d %sна позиции %d.\n",
            target, result.index == -1 ? "не найден.\n" : "найден ", result.index);
        fprintf(output_file, "%s", message);
        printf("%s", message);
        break;
    }
    default:
        fprintf(output_file, "Неверный выбор метода поиска.\n");
        printf("Неверный выбор метода поиска.\n");
    }

    fclose(output_file);
    free(arr);

    printf("\nРезультаты поиска также сохранены в файл 'search_results.txt'.\n");
    return 0;
}