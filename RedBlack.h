#pragma once

// Ліве червоно-чорне дерево: оголошення вузла і всі функції.
//
// Правила лівого червоно-чорного дерева:
// 1. Кожен вузол червоний або чорний. Корінь завжди чорний.
// 2. Червоним може бути лише лівий син (правих червоних синів не буває).
// 3. Два червоних вузли поспіль (батько і син) не можуть стояти підряд.
// 4. На будь-якому шляху від кореня до порожнього місця
//    однакова кількість чорних вузлів.
//
// Зв'язок з 2-3 деревом: чорний вузол разом зі своїм червоним лівим сином
// це один вузол 2-3 дерева з двома ключами. Чорний вузол без червоного сина
// це вузол 2-3 дерева з одним ключем. Це використовується в завданні Г.

#include <iostream>
#include "Utils.h"
using namespace std;

struct RedBlackNode {
    int key;              // ключ вузла
    bool red;             // true - червоний, false - чорний
    RedBlackNode* left;   // лівий син (ключі менші)
    RedBlackNode* right;  // правий син (ключі більші)
};

// Перевіряє, чи вузол червоний. Порожнє місце (nullptr) вважається чорним.
bool isRed(RedBlackNode* node) {
    return node != nullptr && node->red;
}

// Створює новий вузол. Новий вузол завжди червоний.
RedBlackNode* createRedBlackNode(int key) {
    RedBlackNode* node = new RedBlackNode;
    node->key = key;
    node->red = true;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

// Лівий поворот: червоне праве посилання стає лівим.
// Правий син піднімається вгору і забирає колір верхнього вузла,
// а верхній вузол опускається вліво і стає червоним.
RedBlackNode* rotateLeftRB(RedBlackNode* top) {
    RedBlackNode* son = top->right;
    top->right = son->left;
    son->left = top;
    son->red = top->red;
    top->red = true;
    return son; // новий корінь цього піддерева
}

// Правий поворот: дзеркальна копія лівого повороту.
RedBlackNode* rotateRightRB(RedBlackNode* top) {
    RedBlackNode* son = top->left;
    top->left = son->right;
    son->right = top;
    son->red = top->red;
    top->red = true;
    return son;
}

// Змінює кольори вузла і обох його синів на протилежні.
// Під час вставлення це «розщеплення» вузла з трьома ключами:
// два червоних сини стають чорними, а батько стає червоним.
// Під час видалення працює навпаки: батько віддає червоний колір синам.
void flipColors(RedBlackNode* node) {
    node->red = !node->red;
    node->left->red = !node->left->red;
    node->right->red = !node->right->red;
}

// Відновлює правила лівого червоно-чорного дерева в одному вузлі.
// Викликається після вставлення або видалення на шляху назад до кореня.
RedBlackNode* balance(RedBlackNode* node) {
    // Червоне праве посилання має бути лівим: повертаємо вліво.
    if (isRed(node->right) && !isRed(node->left)) {
        node = rotateLeftRB(node);
    }
    // Два червоних вузли поспіль ліворуч: повертаємо вправо.
    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRightRB(node);
    }
    // Обидва сини червоні: міняємо кольори (розщеплюємо вузол).
    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
    }
    return node;
}

// Вставляє ключ у піддерево з коренем node і повертає новий корінь піддерева.
RedBlackNode* insertNode(RedBlackNode* node, int key) {
    // Дійшли до порожнього місця: сюди ставимо новий червоний вузол.
    if (node == nullptr) {
        return createRedBlackNode(key);
    }

    // Як у звичайному дереві пошуку: менші ключі ліворуч, більші праворуч.
    if (key < node->key) {
        node->left = insertNode(node->left, key);
    } else if (key > node->key) {
        node->right = insertNode(node->right, key);
    }
    // Якщо key == node->key, такий ключ уже є і нічого не вставляємо.

    // Повертаючись назад, виправляємо порушені правила.
    return balance(node);
}

// Вставлення ключа в усе дерево.
void insertRB(RedBlackNode*& root, int key) {
    root = insertNode(root, key);
    root->red = false; // корінь завжди чорний
}

// Пошук ключа (дерево не змінюється).
bool searchRB(RedBlackNode* root, int key) {
    while (root != nullptr) {
        if (key < root->key) {
            root = root->left;
        } else if (key > root->key) {
            root = root->right;
        } else {
            return true;
        }
    }
    return false;
}

// Повертає вузол з найменшим ключем (йдемо весь час ліворуч).
RedBlackNode* minNode(RedBlackNode* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Під час видалення ми спускаємось униз і хочемо, щоб поточний вузол
// або його син був червоним: тоді видалення не порушить кількість чорних вузлів.
// moveRedLeft «позичає» червоний колір, щоб рухатись ліворуч.
RedBlackNode* moveRedLeft(RedBlackNode* node) {
    flipColors(node);
    // Якщо у правого сина є червоний лівий син, позичаємо в нього.
    if (isRed(node->right->left)) {
        node->right = rotateRightRB(node->right);
        node = rotateLeftRB(node);
        flipColors(node);
    }
    return node;
}

// Те саме, але для руху праворуч.
RedBlackNode* moveRedRight(RedBlackNode* node) {
    flipColors(node);
    // Якщо у лівого сина є червоний лівий син, позичаємо в нього.
    if (isRed(node->left->left)) {
        node = rotateRightRB(node);
        flipColors(node);
    }
    return node;
}

// Видаляє найменший вузол піддерева і повертає новий корінь піддерева.
RedBlackNode* deleteMinNode(RedBlackNode* node) {
    // Лівого сина немає: цей вузол найменший, видаляємо його.
    if (node->left == nullptr) {
        delete node;
        return nullptr;
    }

    // Спускаємось ліворуч, потрібен червоний вузол на шляху.
    if (!isRed(node->left) && !isRed(node->left->left)) {
        node = moveRedLeft(node);
    }

    node->left = deleteMinNode(node->left);
    return balance(node);
}

// Видаляє ключ з піддерева (ключ точно є в дереві) і повертає новий корінь.
RedBlackNode* deleteNode(RedBlackNode* node, int key) {
    if (key < node->key) {
        // Ключ ліворуч: спускаємось ліворуч, забезпечивши червоний вузол.
        if (!isRed(node->left) && !isRed(node->left->left)) {
            node = moveRedLeft(node);
        }
        node->left = deleteNode(node->left, key);
    } else {
        // Ключ у цьому вузлі або праворуч.
        // Якщо лівий син червоний, повертаємо вправо, щоб червоний був з правого боку.
        if (isRed(node->left)) {
            node = rotateRightRB(node);
        }
        // Знайшли ключ, і справа нічого немає: просто видаляємо вузол.
        if (key == node->key && node->right == nullptr) {
            delete node;
            return nullptr;
        }
        // Спускаємось праворуч, потрібен червоний вузол на шляху.
        if (!isRed(node->right) && !isRed(node->right->left)) {
            node = moveRedRight(node);
        }
        if (key == node->key) {
            // Ключ у цьому вузлі, а справа є піддерево. Замінюємо ключ
            // найменшим ключем правого піддерева і видаляємо той найменший вузол.
            RedBlackNode* smallest = minNode(node->right);
            node->key = smallest->key;
            node->right = deleteMinNode(node->right);
        } else {
            node->right = deleteNode(node->right, key);
        }
    }
    // Повертаючись назад, виправляємо порушені правила.
    return balance(node);
}

// Видалення ключа з усього дерева.
void deleteRB(RedBlackNode*& root, int key) {
    // Якщо ключа немає, нічого не робимо.
    if (!searchRB(root, key)) {
        return;
    }

    // Якщо обидва сини кореня чорні, робимо корінь червоним,
    // щоб було за рахунок чого видаляти.
    if (!isRed(root->left) && !isRed(root->right)) {
        root->red = true;
    }

    root = deleteNode(root, key);

    // Корінь знову має бути чорним.
    if (root != nullptr) {
        root->red = false;
    }
}

// Підраховує кількість вузлів.
int countRB(RedBlackNode* root) {
    if (root == nullptr) {
        return 0;
    }
    return 1 + countRB(root->left) + countRB(root->right);
}

// Записує ключі в масив у порядку зростання (ліве піддерево, вузол, праве піддерево).
void collectRB(RedBlackNode* root, int* keys, int& count) {
    if (root == nullptr) {
        return;
    }
    collectRB(root->left, keys, count);
    keys[count] = root->key;
    count++;
    collectRB(root->right, keys, count);
}

// Завдання А: знаходить найбільший ключ (йдемо весь час праворуч).
// Повертає false, якщо дерево порожнє.
bool findMaxRB(RedBlackNode* root, int& result) {
    if (root == nullptr) {
        return false;
    }
    while (root->right != nullptr) {
        root = root->right;
    }
    result = root->key;
    return true;
}

// Завдання Б: видаляє ключі з проміжку [a, b] і записує їх у масив removed.
// Повертає кількість видалених ключів.
int removeRangeRB(RedBlackNode*& root, int a, int b, int* removed) {
    // Збираємо ключі в масив, бо під час видалення дерево змінюється.
    int total = countRB(root);
    int* allKeys = new int[total + 1];
    int count = 0;
    collectRB(root, allKeys, count);

    // Видаляємо по одному ті ключі, що потрапили в проміжок.
    int removedCount = 0;
    for (int i = 0; i < count; i++) {
        if (allKeys[i] >= a && allKeys[i] <= b) {
            deleteRB(root, allKeys[i]);
            removed[removedCount] = allKeys[i];
            removedCount++;
        }
    }

    delete[] allKeys;
    return removedCount;
}

// Завдання В: перевіряє, чи в двох деревах однакові набори ключів.
bool sameKeysRB(RedBlackNode* first, RedBlackNode* second) {
    int firstTotal = countRB(first);
    int secondTotal = countRB(second);
    int* firstKeys = new int[firstTotal + 1];
    int* secondKeys = new int[secondTotal + 1];
    int firstCount = 0;
    int secondCount = 0;
    collectRB(first, firstKeys, firstCount);
    collectRB(second, secondKeys, secondCount);

    bool result = sameKeys(firstKeys, firstCount, secondKeys, secondCount);

    delete[] firstKeys;
    delete[] secondKeys;
    return result;
}

// Виведення дерева таблицею вузлів за рівнями (обхід у ширину).
// Працює так само, як printSplay, але ще виводить колір вузла.
void printRB(RedBlackNode* root) {
    if (root == nullptr) {
        outputFile << "порожнє дерево\n";
        outputFile << "висота: 0\n";
        return;
    }

    int total = countRB(root);
    RedBlackNode** queue = new RedBlackNode*[total]; // черга вузлів
    int* parents = new int[total];                   // номер батька
    int* levels = new int[total];                    // рівень
    int* sides = new int[total];                     // 0 - корінь, 1 - лівий, 2 - правий

    int head = 0;
    int tail = 0;

    // Кладемо в чергу корінь.
    queue[tail] = root;
    parents[tail] = 0;
    levels[tail] = 1;
    sides[tail] = 0;
    tail++;

    outputFile << "вузол\tрівень\tбатько\tбік\tключ\tколір\n";

    while (head < tail) {
        RedBlackNode* node = queue[head];

        outputFile << head + 1 << "\t" << levels[head] << "\t";
        printNumberOrDash(parents[head]);
        outputFile << "\t";
        printSide(sides[head]);
        outputFile << "\t" << node->key << "\t";
        if (node->red) {
            outputFile << "червоний\n";
        } else {
            outputFile << "чорний\n";
        }

        // Додаємо в чергу синів.
        if (node->left != nullptr) {
            queue[tail] = node->left;
            parents[tail] = head + 1;
            levels[tail] = levels[head] + 1;
            sides[tail] = 1;
            tail++;
        }
        if (node->right != nullptr) {
            queue[tail] = node->right;
            parents[tail] = head + 1;
            levels[tail] = levels[head] + 1;
            sides[tail] = 2;
            tail++;
        }
        head++;
    }

    // Рівень останнього вузла в обході це висота дерева.
    outputFile << "висота: " << levels[total - 1] << "\n";

    delete[] queue;
    delete[] parents;
    delete[] levels;
    delete[] sides;
}

// Звільнення пам'яті: спочатку сини, потім сам вузол.
void freeRB(RedBlackNode* root) {
    if (root == nullptr) {
        return;
    }
    freeRB(root->left);
    freeRB(root->right);
    delete root;
}