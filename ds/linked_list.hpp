#pragma once
#include <stdexcept>

template<typename T>
class LinkedList {
    struct Node {
        T data;
        Node *next;

        Node(const T &d) : data(d), next(nullptr) {
        }
    };

    Node *head = nullptr;
    Node *tail = nullptr;
    int n = 0;

public:
    ~LinkedList() { clear(); }
    int size() const { return n; }
    bool empty() const { return n == 0; }

    void push_back(const T &v) {
        Node *nn = new Node(v);
        if (!head) { head = tail = nn; } else {
            tail->next = nn;
            tail = nn;
        }
        ++n;
    }

    T &getAt(int idx) {
        if (idx < 0 || idx >= n) throw std::out_of_range("idx");
        Node *cur = head;
        for (int i = 0; i < idx; i++) cur = cur->next;
        return cur->data;
    }

    void removeAt(int idx) {
        if (idx < 0 || idx >= n) throw std::out_of_range("idx");
        Node *cur = head;
        Node *prev = nullptr;
        for (int i = 0; i < idx; i++) {
            prev = cur;
            cur = cur->next;
        }
        if (prev) prev->next = cur->next;
        else head = cur->next;
        if (cur == tail) tail = prev;
        delete cur;
        --n;
    }

    void clear() {
        while (head) {
            Node *t = head;
            head = head->next;
            delete t;
        }
        tail = nullptr;
        n = 0;
    }

    struct It {
        Node *p;
        bool has() const { return p != nullptr; }
        T &val() const { return p->data; }
        void next() { if (p) p = p->next; }
    };

    It iter() { return It{head}; }
};
