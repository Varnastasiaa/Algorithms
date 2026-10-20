#pragma once

// 2-3 дерево (потрібне для завдання Г): оголошення вузла і всі функції.
//
// У 2-3 дереві кожен вузол має один або два ключі:
// - вузол з одним ключем має двох нащадків (лівий і правий);
// - вузол з двома ключами має трьох нащадків (лівий, середній і правий).
// Усі листки лежать на одному рівні.
// Для цього дерева нам потрібні лише перетворення з червоно-чорного
// дерева, виведення і звільнення пам'яті.

#include <iostream>
#include "RedBlack.h"
#include "Utils.h"
using namespace std;

struct Node23 {
    int keyCount;        // скільки ключів у вузлі: 1 або 2
    int keys[2];         // ключі у порядку зростання
    Node23* children[3]; // нащадки зліва направо (використовується keyCount + 1)
};

// Завдання Г: перетворює ліве червоно-чорне дерево на 2-3 дерево.
// Ідея: чорний вузол разом зі своїм червоним лівим сином утворює
// один вузол 2-3 дерева з двома ключами.
Node23* convertTo23(RedBlackNode* node) {
    if (node == nullptr) {
        return nullptr;
    }

    // Створюємо вузол 2-3 дерева і спочатку ставимо всім нащадкам nullptr.
    Node23* result = new Node23;
    result->children[0] = nullptr;
    result->children[1] = nullptr;
    result->children[2] = nullptr;

    if (isRed(node->left)) {
        // Вузол з двома ключами: менший ключ це червоний лівий син,
        // більший ключ це поточний чорний вузол.
        result->keyCount = 2;
        result->keys[0] = node->left->key;
        result->keys[1] = node->key;
        // Три нащадки: ліве піддерево червоного сина,
        // праве піддерево червоного сина і праве піддерево поточного вузла.
        result->children[0] = convertTo23(node->left->left);
        result->children[1] = convertTo23(node->left->right);
        result->children[2] = convertTo23(node->right);
    } else {
        // Вузол з одним ключем.
        result->keyCount = 1;
        result->keys[0] = node->key;
        // Два нащадки: ліве і праве піддерево.
        result->children[0] = convertTo23(node->left);
        result->children[1] = convertTo23(node->right);
    }
    return result;
}

// Підраховує кількість вузлів (не ключів).
int count23(Node23* root) {
    if (root == nullptr) {
        return 0;
    }
    int total = 1; // сам вузол
    // У вузла keyCount + 1 нащадків.
    for (int i = 0; i <= root->keyCount; i++) {
        total += count23(root->children[i]);
    }
    return total;
}

// Виведення дерева таблицею вузлів за рівнями (обхід у ширину).
// Стовпець «нащадок» це номер вузла серед нащадків батька зліва направо (1, 2 або 3).
void print23(Node23* root) {
    if (root == nullptr) {
        outputFile << "порожнє дерево\n";
        outputFile << "висота: 0\n";
        return;
    }

    int total = count23(root);
    Node23** queue = new Node23*[total]; // черга вузлів
    int* parents = new int[total];       // номер батька
    int* levels = new int[total];        // рівень
    int* childNumbers = new int[total];  // який це нащадок батька (0 - корінь)

    int head = 0;
    int tail = 0;

    // Кладемо в чергу корінь.
    queue[tail] = root;
    parents[tail] = 0;
    levels[tail] = 1;
    childNumbers[tail] = 0;
    tail++;

    outputFile << "вузол\tрівень\tбатько\tнащадок\tключі\n";

    while (head < tail) {
        Node23* node = queue[head];

        outputFile << head + 1 << "\t" << levels[head] << "\t";
        printNumberOrDash(parents[head]);
        outputFile << "\t";
        printNumberOrDash(childNumbers[head]);
        outputFile << "\t";
        // Виводимо один або два ключі.
        for (int i = 0; i < node->keyCount; i++) {
            outputFile << node->keys[i] << " ";
        }
        outputFile << "\n";

        // Додаємо в чергу всіх існуючих нащадків зліва направо.
        for (int i = 0; i <= node->keyCount; i++) {
            if (node->children[i] != nullptr) {
                queue[tail] = node->children[i];
                parents[tail] = head + 1;
                levels[tail] = levels[head] + 1;
                childNumbers[tail] = i + 1; // нумерація нащадків з одиниці
                tail++;
            }
        }
        head++;
    }

    // Рівень останнього вузла в обході це висота дерева.
    outputFile << "висота: " << levels[total - 1] << "\n";

    delete[] queue;
    delete[] parents;
    delete[] levels;
    delete[] childNumbers;
}

// Звільнення пам'яті: спочатку всі нащадки, потім сам вузол.
void free23(Node23* root) {
    if (root == nullptr) {
        return;
    }
    for (int i = 0; i <= root->keyCount; i++) {
        free23(root->children[i]);
    }
    delete root;
}