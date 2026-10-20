#pragma once

// Допоміжні функції для виведення і порівняння.

#include <iostream>
#include <fstream>
using namespace std;

ofstream outputFile;

// Пропускає все в файлі до знака «=» (сам «=» теж пропускається).
void skipToEquals(ifstream& inputFile) {
    char symbol;
    while (inputFile.get(symbol) && symbol != '=') {
    }
}

// Зчитує одне число з файлу у форматі «назва=число», наприклад: a=3
// Усе, що стоїть між значеннями (пробіли, «;», назви), ігнорується.
int readValue(ifstream& inputFile) {
    skipToEquals(inputFile);
    int value = 0;
    inputFile >> value;
    return value;
}

// Виводить номер або «-», якщо номера немає.
// Для кореня батька немає, тому в таблиці ми зберігаємо 0 і друкуємо «-».
void printNumberOrDash(int value) {
    if (value == 0) {
        outputFile << "-";
    } else {
        outputFile << value;
    }
}

// Виводить бік вузла відносно батька.
// 0 - боку немає (корінь), 1 - лівий син, 2 - правий син.
void printSide(int side) {
    if (side == 0) {
        outputFile << "-";
    } else if (side == 1) {
        outputFile << "лівий";
    } else {
        outputFile << "правий";
    }
}

// Виводить перелік ключів із масиву або «немає», якщо масив порожній.
void printKeys(int* keys, int count) {
    if (count == 0) {
        outputFile << "немає";
    }
    for (int i = 0; i < count; i++) {
        outputFile << keys[i] << " ";
    }
    outputFile << "\n";
}

// Порівнює два масиви ключів, які вже впорядковані за зростанням.
// Набори однакові, якщо в них однакова кількість ключів
// і на кожній позиції стоїть однакове число.
bool sameKeys(int* first, int firstCount, int* second, int secondCount) {
    if (firstCount != secondCount) {
        return false;
    }
    for (int i = 0; i < firstCount; i++) {
        if (first[i] != second[i]) {
            return false;
        }
    }
    return true;
}