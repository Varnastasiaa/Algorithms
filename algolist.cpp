#include "algolist.h"

bool isEmpty(Node* head) {
    return head == nullptr;
}

void addToHead(Node*& head, long long value) {
    Node* node = new Node{value, nullptr};
    if (isEmpty(head)) {
        node->next = node; 
        head = node;
        return;
    }
    Node* last = head;
    while (last->next != head) {
        last = last->next;
    }
    node->next = head;
    last->next = node;
    head = node;
}

void addToTail(Node*& head, long long value) {
    Node* node = new Node{value, nullptr};
    if (isEmpty(head)) {
        node->next = node;
        head = node;
        return;
    }
    Node* last = head;
    while (last->next != head) {
        last = last->next;
    }
    last->next = node;
    node->next = head; 
}

void printList(Node* head, ostream& out) {
    if (isEmpty(head)) {
        out << "[список порожній]\n";
        return;
    }
    Node* pointer = head;
    do {
        out << pointer->value;
        pointer = pointer->next;
        if (pointer != head) out << " -> ";
    } while (pointer != head);
    out << " -> (початок: " << head->value << ")\n";
}

long long getLength(Node* head) {
    if (isEmpty(head)) return 0;

    long long count = 0;

    Node* pointer = head;
    do {
        count++;
        pointer = pointer->next;
    } while (pointer != head);
    return count;
}

void freeList(Node*& head) {
    if (isEmpty(head)) return;

    // Розмикаємо кільце, щоб коректно видалити як звичайний список
    Node* last = head;
    while (last->next != head) {
        last = last->next;
    }
    last->next = nullptr;

    Node* pointer = head;
    while (pointer != nullptr) {
        Node* next = pointer->next;
        delete pointer;
        pointer = next;
    }
    head = nullptr;
}

long long taskA(Node*& head, long long E) {
    if (isEmpty(head)) return 0;

    long long hops = 0;
    Node* pointer = head;
    Node* lastPositive = nullptr;

    do {
        if (pointer->value > 0) {
            lastPositive = pointer;
        }
        pointer = pointer->next;
        hops++;
    } while (pointer != head);

    if (lastPositive != nullptr) {
        Node* node = new Node{E, lastPositive->next};
        lastPositive->next = node;
    } else {
        // Якщо додатних елементів не знайдено, вставляємо в кінець
        addToTail(head, E);
    }
    return hops;
}

static void printNegativesReverse(Node* current, Node* head, long long& hops, ostream& out) {
    if (current->next != head) {
        hops++;
        printNegativesReverse(current->next, head, hops, out);
    }
    if (current->value < 0) {
        out << current->value << " ";
    }
}

long long taskB(Node* head, ostream& out) {
    if (isEmpty(head)) {
        out << "Немає елементів.";
        return 0;
    }
    long long hops = 0;
    printNegativesReverse(head, head, hops, out);
    return hops;
}

long long taskC(Node* head, long long& midValue) {
    if (isEmpty(head)) return 0;

    long long hops = 0;
    Node* slow = head;
    Node* fast = head;

    // fast робить 2 кроки, slow — 1 крок
    while (fast->next != head && fast->next->next != head) {
        fast = fast->next->next;
        hops += 2;
        slow = slow->next;
        hops += 1;
    }

    midValue = slow->value;
    return hops;
}
