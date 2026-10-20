#include <iostream>
#include "Utils.h"
#include "Splay.h"
#include "RedBlack.h"
#include "Tree23.h"
using namespace std;

// Виводить усі чотири дерева з їхньою висотою:
// splay-дерева T1 і T2, червоно-чорні дерева T1 і T2.
// Її викликаємо після кожного завдання.
void printAllTrees(SplayNode* splay1, SplayNode* splay2,
                   RedBlackNode* rb1, RedBlackNode* rb2) {
    outputFile << "\nSplay-дерево T1:\n";
    printSplay(splay1);
    outputFile << "\nSplay-дерево T2:\n";
    printSplay(splay2);
    outputFile << "\nЛіве червоно-чорне дерево T1:\n";
    printRB(rb1);
    outputFile << "\nЛіве червоно-чорне дерево T2:\n";
    printRB(rb2);
    outputFile << "\n";
}

int main() {
    // T1 - дерево з першого набору ключів, T2 - дерево з другого набору.
    // Кожен набір ми зберігаємо у двох видах дерев: splay і червоно-чорне.
    SplayNode* splay1 = nullptr;
    SplayNode* splay2 = nullptr;
    RedBlackNode* rb1 = nullptr;
    RedBlackNode* rb2 = nullptr;

    ifstream inputFile("input.txt");
    outputFile.open("output.txt");
    if (!inputFile.is_open() || !outputFile.is_open()) {
        cout << "Не вдалося відкрити input.txt або output.txt\n";
        return 1;
    }

    int count1;
    int count2;
    int a;
    int b;
    int key;

    // Читаємо перший набір і одразу вставляємо кожен ключ в обидва дерева.
    count1 = readValue(inputFile);
    skipToEquals(inputFile); 
    for (int i = 0; i < count1; i++) {
        inputFile >> key;
        insertSplay(splay1, key);
        insertRB(rb1, key);
    }

    // Те саме для другого набору.
    count2 = readValue(inputFile);
    skipToEquals(inputFile); 
    for (int i = 0; i < count2; i++) {
        inputFile >> key;
        insertSplay(splay2, key);
        insertRB(rb2, key);
    }

    a = readValue(inputFile);
    b = readValue(inputFile);

    outputFile << "=== Початкові дерева ===";
    printAllTrees(splay1, splay2, rb1, rb2);

    // ---------------- Завдання А ----------------
    // Знайти найбільший ключ дерева T (для обох видів дерев).
    outputFile << "=== Завдання А: найбільший ключ дерева T ===\n";
    int maximum;
    outputFile << "Splay-дерево: ";
    if (findMaxSplay(splay1, maximum)) {
        outputFile << maximum << "\n";
    } else {
        outputFile << "немає\n"; // дерево порожнє
    }
    outputFile << "Ліве червоно-чорне дерево: ";
    if (findMaxRB(rb1, maximum)) {
        outputFile << maximum << "\n";
    } else {
        outputFile << "немає\n";
    }
    outputFile<<"\n ";
    // printAllTrees(splay1, splay2, rb1, rb2);


    outputFile << "=== Завдання Б: видалити з дерева T ключі з проміжку [" << a << ", " << b << "] ===\n";

    // Масив removed збереже ключі, які були видалені (щоб їх вивести).
    int* removed = new int[countSplay(splay1) + 1];
    int removedCount = removeRangeSplay(splay1, a, b, removed);
    outputFile << "Splay-дерево, видалені ключі: ";
    printKeys(removed, removedCount);
    delete[] removed;

    removed = new int[countRB(rb1) + 1];
    removedCount = removeRangeRB(rb1, a, b, removed);
    outputFile << "Ліве червоно-чорне дерево, видалені ключі: ";
    printKeys(removed, removedCount);
    delete[] removed;
    outputFile<<"\n ";
    //printAllTrees(splay1, splay2, rb1, rb2);


    outputFile << "=== Завдання В: чи містять T1 і T2 однакові набори ключів ===\n";
    outputFile << "Splay-дерева: ";
    if (sameKeysSplay(splay1, splay2)) {
        outputFile << "так\n";
    } else {
        outputFile << "ні\n";
    }
    outputFile << "Ліві червоно-чорні дерева: ";
    if (sameKeysRB(rb1, rb2)) {
        outputFile << "так\n";
    } else {
        outputFile << "ні\n";
    }
    //printAllTrees(splay1, splay2, rb1, rb2);
    outputFile<<"\n ";


    outputFile << "=== Завдання Г: перетворити ліве червоно-чорне дерево T на 2-3 дерево ===\n";
    outputFile << "\nДерево T (ліве червоно-чорне) до перетворення:\n";
    printRB(rb1);

    Node23* tree23 = convertTo23(rb1);
    outputFile << "\nДерево T (2-3) після перетворення:\n";
    print23(tree23);
    outputFile << "\n";
    //printAllTrees(splay1, splay2, rb1, rb2);

    // Звільняємо пам'ять, яку займають вузли всіх дерев.
    free23(tree23);
    freeSplay(splay1);
    freeSplay(splay2);
    freeRB(rb1);
    freeRB(rb2);

    return 0;
}