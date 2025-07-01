#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct {
    int index;
    int value;
} SearchResult;

// 1. Линейный поиск (возвращает первое вхождение)
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
    char buffer[1024];
    sprintf(buffer, "Элемент %d найден на позициях: ", target);
    int found = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            char temp[20];
            sprintf(temp, "%d ", i);
            strcat(buffer, temp);
            found = 1;
        }
    }
    if (!found) {
        strcat(buffer, "не найден.");
    }
    strcat(buffer, "\n");

    // Вывод в файл и консоль
    fprintf(output_file, "%s", buffer);
    printf("%s", buffer);
}

// 3. Поиск с использованием указателей
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

// Функция для чтения массива из файла
int read_array_from_file(const char* filename, int** arr) {
    FILE* file;
    if (fopen_s(&file, filename, "r") != 0) {
        printf("Ошибка при открытии файла!\n");
        return 0;
    }

    int count = 0;
    int num;
    while (fscanf_s(file, "%d", &num) == 1) {
        count++;
    }

    *arr = (int*)malloc(count * sizeof(int));
    if (*arr == NULL) {
        printf("Ошибка выделения памяти!\n");
        fclose(file);
        return 0;
    }

    rewind(file);
    for (int i = 0; i < count; i++) {
        fscanf_s(file, "%d", &(*arr)[i]);
    }

    fclose(file);
    return count;
}

int main() {
    setlocale(LC_ALL, "Rus");
    char filename[256];
    printf("Введите имя файла с числами: ");
    if (scanf_s("%255s", filename, (unsigned)_countof(filename)) != 1) {
        printf("Ошибка ввода имени файла!\n");
        return 1;
    }

    int* arr = NULL;
    int size = read_array_from_file(filename, &arr);
    if (size == 0) {
        return 1;
    }

    printf("Массив успешно загружен (%d элементов)\n", size);

    // Вывод массива в консоль
    printf("Исходный массив:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    int target;
    printf("Введите число для поиска: ");
    if (scanf_s("%d", &target) != 1) {
        printf("Ошибка ввода числа для поиска!\n");
        free(arr);
        return 1;
    }

    printf("\nВыберите метод поиска:\n");
    printf("1. Линейный поиск (первое вхождение)\n");
    printf("2. Поиск всех вхождений\n");
    printf("3. Поиск с указателями\n");
    printf("4. Поиск с возвратом структуры\n");
    printf("Ваш выбор: ");
    int choice;
    if (scanf_s("%d", &choice) != 1) {
        printf("Ошибка ввода выбора метода!\n");
        free(arr);
        return 1;
    }

    FILE* output_file;
    if (fopen_s(&output_file, "search_results.txt", "w") != 0) {
        printf("Ошибка при создании файла результатов!\n");
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
        if (result != -1) {
            char message[100];
            sprintf(message, "Линейный поиск: элемент %d найден на позиции %d.\n", target, result);
            fprintf(output_file, "%s", message);
            printf("%s", message);
        }
        else {
            char message[100];
            sprintf(message, "Линейный поиск: элемент %d не найден.\n", target);
            fprintf(output_file, "%s", message);
            printf("%s", message);
        }
        break;
    }
    case 2:
        fprintf(output_file, "Результат поиска всех вхождений:\n");
        printf("Результат поиска всех вхождений:\n");
        search_all_occurrences(arr, size, target, output_file);
        break;
    case 3: {
        int result = pointer_search(arr, size, target);
        if (result != -1) {
            char message[100];
            sprintf(message, "Поиск с указателями: элемент %d найден на позиции %d.\n", target, result);
            fprintf(output_file, "%s", message);
            printf("%s", message);
        }
        else {
            char message[100];
            sprintf(message, "Поиск с указателями: элемент %d не найден.\n", target);
            fprintf(output_file, "%s", message);
            printf("%s", message);
        }
        break;
    }
    case 4: {
        SearchResult result = search_with_struct(arr, size, target);
        if (result.index != -1) {
            char message[100];
            sprintf(message, "Поиск с структурой: элемент %d найден на позиции %d.\n", result.value, result.index);
            fprintf(output_file, "%s", message);
            printf("%s", message);
        }
        else {
            char message[100];
            sprintf(message, "Поиск с структурой: элемент %d не найден.\n", target);
            fprintf(output_file, "%s", message);
            printf("%s", message);
        }
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