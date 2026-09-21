#include "iostream"
#include "fstream"
using namespace std;

struct Node {
    long long value;
    Node* next;
};

bool isEmpty(Node* head);
void addToHead(Node*& head, long long value);
void addToTail(Node*& head, long long value);
void printList(Node* head, ostream& out = cout);
long long getLength(Node* head);
void freeList(Node*& head);

long long taskA(Node*& head, long long E);
long long taskB(Node* head, ostream& out = cout);
long long taskC(Node* head, long long& midValue);

// У функціях, які змінюють сам список або можуть переставити його початок пишемо &