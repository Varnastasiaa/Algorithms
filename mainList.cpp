#include "algolist.h"

int main() {
    ifstream inFile("input.txt");
    if (!inFile.is_open()) {
        cerr << "Помилка: не вдалося відкрити файл input.txt!\n";
        return 1;
    }

    Node* head = nullptr;
    long long val;

    while (inFile >> val) {
        addToTail(head, val);
    }
    inFile.close();

    long long elementE;
    cout << "Введіть число E для вставки: ";
    cin >> elementE;

    ofstream outFile("output.txt");
    if (!outFile.is_open()) {
        cerr << "Помилка: не вдалося відкрити файл output.txt!\n";
        freeList(head);
        return 1;
    }

    outFile << "=== ПОЧАТКОВИЙ СТАН СПИСКУ ===\n";
    outFile << "Вміст: ";
    printList(head, outFile);
    outFile << "Довжина: " << getLength(head) << "\n\n";

    long long hopsA = 0;
    long long hopsB = 0;
    long long hopsC = 0;

    hopsA = taskA(head, elementE);
    outFile << "--- Після завдання А (вставка " << elementE << " після останнього додатного) ---\n";
    outFile << "Вміст: ";
    printList(head, outFile);
    outFile << "Довжина: " << getLength(head) << "\n\n";
    

    outFile << "--- Завдання Б: Від'ємні елементи у зворотному порядку ---\n";
    outFile << "Результат: ";
    hopsB = taskB(head, outFile);
    outFile << "\nВміст: ";
    printList(head, outFile);
    outFile << "Довжина: " << getLength(head) << "\n\n";


    long long midVal = 0;
    hopsC = taskC(head, midVal);
    outFile << "--- Завдання В: Пошук середнього елемента ---\n";
    outFile << "Середній елемент: " << midVal << "\n";
    outFile << "Вміст: ";
    printList(head, outFile);
    outFile << "Довжина: " << getLength(head) << "\n\n";


    outFile << "===========================================\n";
    outFile << "ТАБЛИЦЯ ПЕРЕХОДІВ ЗА ПОКАЖЧИКОМ\n";
    outFile << "===========================================\n";
    outFile << "Завдання\t\tКількість переходів\n";
    outFile << "Завдання А\t\t" << hopsA << "\n";
    outFile << "Завдання Б\t\t" << hopsB << "\n";
    outFile << "Завдання В\t\t" << hopsC << "\n";
    outFile << "===========================================\n";

    outFile.close();
    freeList(head);

    cout << "Результати збережено в output.txt!\n";
    return 0;
}
