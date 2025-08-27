#pragma once
#include <stdexcept>

template<typename T>
class Stack {
    struct Node { T data; Node* next; Node(const T& d, Node* n=nullptr): data(d), next(n) {} };
    Node* head = nullptr; int n = 0;
public:
    ~Stack() { clear(); }
    void push(const T& v) { head = new Node(v, head); ++n; }
    T pop() { if (!head) throw std::underflow_error("Stack underflow"); Node* t=head; T v=t->data; head=t->next; delete t; --n; return v; }
    const T& top() const { if(!head) throw std::underflow_error("Stack empty"); return head->data; }
    bool isEmpty() const { return head==nullptr; }
    int size() const { return n; }
    void clear(){ while(head){ Node* t=head; head=head->next; delete t; } n=0; }
};