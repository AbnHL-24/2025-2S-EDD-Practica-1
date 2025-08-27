#pragma once
#include <stdexcept>

template<typename T>
class Queue {
    struct Node { T data; Node* next; Node(const T& d): data(d), next(nullptr) {} };
    Node* head=nullptr; Node* tail=nullptr; int n=0;
public:
    ~Queue(){ clear(); }
    void enqueue(const T& v){ Node* nn=new Node(v); if(!tail){ head=tail=nn; } else { tail->next=nn; tail=nn; } ++n; }
    T dequeue(){ if(!head) throw std::underflow_error("Queue underflow"); Node* t=head; T v=t->data; head=head->next; if(!head) tail=nullptr; delete t; --n; return v; }
    const T& front() const { if(!head) throw std::underflow_error("Queue empty"); return head->data; }
    bool isEmpty() const { return head==nullptr; }
    int size() const { return n; }
    void clear(){ while(head){ Node* t=head; head=head->next; delete t; } tail=nullptr; n=0; }
};