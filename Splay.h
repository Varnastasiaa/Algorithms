#pragma once

// Splay-дерево: оголошення вузла і всі функції.
//
// Як це працює (коротко):
// Splay-дерево це звичайне двійкове дерево пошуку (менші ключі ліворуч,
// більші праворуч), але після кожної операції потрібний вузол
// піднімається до кореня за допомогою поворотів (функція splay).
// Завдяки цьому вузли, до яких звертаються часто, лежать близько до кореня.
// Вставлення, пошук і видалення зроблені через splay.

#include <iostream>
#include "Utils.h"
using namespace std;

struct SplayNode {
    int key;           // ключ вузла
    SplayNode* left;   // лівий син (ключі менші)
    SplayNode* right;  // правий син (ключі більші)
};

// Створює новий вузол без синів.
SplayNode* createSplayNode(int key) {
    SplayNode* node = new SplayNode;
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

// Правий поворот: лівий син піднімається вгору, а вузол top опускається вправо.
// До повороту: у top лівий син son і праве піддерево C,
// а в son ліве піддерево A і праве піддерево B.
// Після повороту: son стає верхнім вузлом, його правим сином стає top,
// піддерево B переходить до top як лівий нащадок, A і C лишаються на місці.
SplayNode* rotateRightSplay(SplayNode* top) {
    SplayNode* son = top->left;
    top->left = son->right;
    son->right = top;
    return son; // новий корінь цього піддерева
}

// Лівий поворот: правий син піднімається вгору, а вузол top опускається вліво.
// Це дзеркальна копія правого повороту.
SplayNode* rotateLeftSplay(SplayNode* top) {
    SplayNode* son = top->right;
    top->right = son->left;
    son->left = top;
    return son;
}

// Піднімає вузол з ключем key до кореня і повертає новий корінь.
// Якщо такого ключа немає, до кореня піднімається останній вузол
// на шляху пошуку (найближчий до key).
SplayNode* splay(SplayNode* root, int key) {
    if (root == nullptr || root->key == key) {
        return root;
    }

    if (key < root->key) {
        if (root->left == nullptr) {
            return root;
        }

        if (key < root->left->key) {
            // Лівий-лівий випадок (zig-zig):
            // спочатку піднімаємо вузол у лівому-лівому піддереві,
            // потім повертаємо корінь вправо.
            root->left->left = splay(root->left->left, key);
            root = rotateRightSplay(root);
        } else if (key > root->left->key) {
            // Лівий-правий випадок (zig-zag):
            // піднімаємо вузол у лівому-правому піддереві,
            // потім повертаємо лівого сина вліво.
            root->left->right = splay(root->left->right, key);
            if (root->left->right != nullptr) {
                root->left = rotateLeftSplay(root->left);
            }
        }

        // Останній поворот: піднімаємо знайдений вузол на місце кореня.
        if (root->left == nullptr) {
            return root;
        }
        return rotateRightSplay(root);
    } else {
        // Ключ має бути в правому піддереві (все дзеркально).
        if (root->right == nullptr) {
            return root;
        }

        if (key > root->right->key) {
            // Правий-правий випадок (zag-zag).
            root->right->right = splay(root->right->right, key);
            root = rotateLeftSplay(root);
        } else if (key < root->right->key) {
            // Правий-лівий випадок (zag-zig).
            root->right->left = splay(root->right->left, key);
            if (root->right->left != nullptr) {
                root->right = rotateRightSplay(root->right);
            }
        }

        if (root->right == nullptr) {
            return root;
        }
        return rotateLeftSplay(root);
    }
}

// Вставлення ключа.
// Ідея: піднімаємо до кореня найближчий вузол, а потім новий вузол
// ставимо на місце кореня і розрізаємо старе дерево навколо нього.
void insertSplay(SplayNode*& root, int key) {
    if (root == nullptr) {
        root = createSplayNode(key);
        return;
    }

    // Піднімаємо найближчий до key вузол до кореня.
    root = splay(root, key);

    if (root->key == key) {
        return;
    }

    SplayNode* node = createSplayNode(key);
    if (key < root->key) {
        // Новий ключ менший за корінь: старий корінь стає правим сином,
        // а лівий син старого кореня стає лівим сином нового вузла.
        node->right = root;
        node->left = root->left;
        root->left = nullptr;
    } else {
        node->left = root;
        node->right = root->right;
        root->right = nullptr;
    }
    root = node;
}

// Пошук ключа.
// Піднімаємо вузол до кореня (дерево змінюється) і дивимось,
// чи корінь має потрібний ключ.
bool searchSplay(SplayNode*& root, int key) {
    root = splay(root, key);
    return root != nullptr && root->key == key;
}

// Видалення ключа.
void deleteSplay(SplayNode*& root, int key) {
    // Якщо ключа немає, нічого не робимо. Якщо є, він стає коренем.
    if (!searchSplay(root, key)) {
        return;
    }

    SplayNode* oldRoot = root; // вузол, який видаляємо

    if (root->left == nullptr) {
        // Лівого піддерева немає: новий корінь це правий син.
        root = root->right;
    } else {
        // Ключ більший за всі ключі лівого піддерева,
        // тому splay підніме до кореня найбільший з них.
        // У нього немає правого сина, тож чіпляємо туди праве піддерево.
        root = splay(root->left, key);
        root->right = oldRoot->right;
    }

    delete oldRoot;
}

// Підраховує кількість вузлів у дереві.
int countSplay(SplayNode* root) {
    if (root == nullptr) {
        return 0;
    }
    return 1 + countSplay(root->left) + countSplay(root->right);
}

// Записує ключі в масив у порядку зростання.
// Порядок: спочатку ліве піддерево, потім вузол, потім праве піддерево.
void collectSplay(SplayNode* root, int* keys, int& count) {
    if (root == nullptr) {
        return;
    }
    collectSplay(root->left, keys, count);
    keys[count] = root->key;
    count++;
    collectSplay(root->right, keys, count);
}

// Завдання А: знаходить найбільший ключ.
// Найбільший ключ лежить у вузлі, куди приведе рух весь час праворуч.
// Повертає false, якщо дерево порожнє.
bool findMaxSplay(SplayNode* root, int& result) {
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
int removeRangeSplay(SplayNode*& root, int a, int b, int* removed) {
    // Спочатку збираємо всі ключі в масив: під час видалення дерево змінюється,
    // тому обходити його і видаляти одночасно небезпечно.
    int total = countSplay(root);
    int* allKeys = new int[total + 1];
    int count = 0;
    collectSplay(root, allKeys, count);

    // Видаляємо по одному ті ключі, що потрапили в проміжок.
    int removedCount = 0;
    for (int i = 0; i < count; i++) {
        if (allKeys[i] >= a && allKeys[i] <= b) {
            deleteSplay(root, allKeys[i]);
            removed[removedCount] = allKeys[i];
            removedCount++;
        }
    }

    delete[] allKeys;
    return removedCount;
}

// Завдання В: перевіряє, чи в двох деревах однакові набори ключів.
// Збираємо ключі обох дерев у впорядковані масиви і порівнюємо їх.
bool sameKeysSplay(SplayNode* first, SplayNode* second) {
    int firstTotal = countSplay(first);
    int secondTotal = countSplay(second);
    int* firstKeys = new int[firstTotal + 1];
    int* secondKeys = new int[secondTotal + 1];
    int firstCount = 0;
    int secondCount = 0;
    collectSplay(first, firstKeys, firstCount);
    collectSplay(second, secondKeys, secondCount);

    bool result = sameKeys(firstKeys, firstCount, secondKeys, secondCount);

    delete[] firstKeys;
    delete[] secondKeys;
    return result;
}

// Виведення дерева таблицею вузлів за рівнями (обхід у ширину).
// Для обходу потрібна черга. Стандартну чергу використовувати не можна,
// тому черга це звичайний масив: head - звідки читаємо, tail - куди додаємо.
void printSplay(SplayNode* root) {
    if (root == nullptr) {
        outputFile << "порожнє дерево\n";
        outputFile << "висота: 0\n";
        return;
    }

    int total = countSplay(root);
    SplayNode** queue = new SplayNode*[total]; // черга вузлів
    int* parents = new int[total];             // номер батька кожного вузла
    int* levels = new int[total];              // рівень кожного вузла
    int* sides = new int[total];               // 0 - корінь, 1 - лівий, 2 - правий

    int head = 0;
    int tail = 0;

    // Кладемо в чергу корінь: рівень 1, батька і боку немає.
    queue[tail] = root;
    parents[tail] = 0;
    levels[tail] = 1;
    sides[tail] = 0;
    tail++;

    outputFile << "вузол\tрівень\tбатько\tбік\tключ\n";

    // Поки в черзі є вузли, беремо наступний.
    while (head < tail) {
        SplayNode* node = queue[head];

        // Номер вузла = його позиція в черзі + 1 (нумерація з одиниці).
        outputFile << head + 1 << "\t" << levels[head] << "\t";
        printNumberOrDash(parents[head]);
        outputFile << "\t";
        printSide(sides[head]);
        outputFile << "\t" << node->key << "\n";

        // Додаємо в чергу синів: спочатку лівого, потім правого.
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

    // Останній вузол обходу за рівнями лежить на найнижчому рівні,
    // тому його рівень і є висотою дерева.
    outputFile << "висота: " << levels[total - 1] << "\n";

    delete[] queue;
    delete[] parents;
    delete[] levels;
    delete[] sides;
}

// Звільнення пам'яті: спочатку видаляємо синів, потім сам вузол.
void freeSplay(SplayNode* root) {
    if (root == nullptr) {
        return;
    }
    freeSplay(root->left);
    freeSplay(root->right);
    delete root;
}