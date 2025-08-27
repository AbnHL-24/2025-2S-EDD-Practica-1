#pragma once
#include <stdexcept>

template<typename T>
class LinkedList {
    struct Node {
        T data; // Dato almacenado en el nodo
        Node *next; // Puntero al siguiente nodo

        // Constructor del nodo: inicializa el dato con el valor recibido y el puntero next en nullptr
        Node(const T &d) : data(d), next(nullptr) {
        }
    };

    Node *head = nullptr; // Puntero al primer nodo
    Node *tail = nullptr; // Puntero al último nodo
    int n = 0; // Contador de elementos

public:
    ~LinkedList() { clear(); } // Destructor que libera memoria

    LinkedList() = default; // Constructor por defecto

    LinkedList(const LinkedList &) = delete; // Prohíbe copia
    LinkedList &operator=(const LinkedList &) = delete; // Prohíbe asignación

    int size() const { return n; } // Retorna número de elementos
    bool empty() const { return n == 0; } // Verifica si está vacía

    // Inserta un nuevo elemento al final de la lista enlazada
    void push_back(const T &v) {
        Node *nn = new Node(v); // Crea nuevo nodo
        if (!head) { head = tail = nn; } // Primera inserción
        else {
            tail->next = nn; // Conecta al final
            tail = nn; // Actualiza tail
        }
        ++n; // Incrementa contador
    }

    // Inserta un nuevo elemento al inicio de la lista enlazada
    void push_front(const T &v) {
        Node *nn = new Node(v); // Crea nuevo nodo
        nn->next = head; // Conecta al inicio
        head = nn; // Actualiza head
        if (!tail) tail = nn; // Si era vacía, actualiza tail
        ++n; // Incrementa contador
    }

    // Obtiene una referencia al elemento en la posición especificada (versión no constante)
    T &getAt(int idx) {
        if (idx < 0 || idx >= n) throw std::out_of_range("idx"); // Valida índice
        Node *cur = head;
        for (int i = 0; i < idx; ++i) cur = cur->next; // Navega hasta posición
        return cur->data; // Retorna referencia
    }

    // Obtiene una referencia constante al elemento en la posición especificada
    const T &getAt(int idx) const {
        if (idx < 0 || idx >= n) throw std::out_of_range("idx"); // Valida índice
        Node *cur = head;
        for (int i = 0; i < idx; ++i) cur = cur->next; // Navega hasta posición
        return cur->data; // Retorna referencia constante
    }

    // Elimina el elemento en la posición especificada de la lista
    void removeAt(int idx) {
        if (idx < 0 || idx >= n) throw std::out_of_range("idx"); // Valida índice
        Node *cur = head;
        Node *prev = nullptr;
        for (int i = 0; i < idx; ++i) {
            // Busca nodo a eliminar
            prev = cur;
            cur = cur->next;
        }
        if (prev) prev->next = cur->next; // Desconecta nodo
        else head = cur->next; // Si es el primero
        if (cur == tail) tail = prev; // Si es el último
        delete cur; // Libera memoria
        --n; // Decrementa contador
    }

    // Elimina todos los elementos de la lista y libera la memoria
    void clear() {
        while (head) {
            // Mientras haya nodos
            Node *t = head;
            head = head->next; // Avanza head
            delete t; // Elimina nodo anterior
        }
        tail = nullptr; // Resetea tail
        n = 0; // Resetea contador
    }

    // Iterador simple para recorrer la lista enlazada
    struct It {
        Node *p; // Puntero al nodo actual
        bool has() const { return p != nullptr; } // Verifica si hay elemento
        T &val() const { return p->data; } // Obtiene valor actual
        void next() { if (p) p = p->next; } // Avanza al siguiente
    };

    // Retorna un iterador que apunta al primer elemento de la lista
    It iter() const { return It{head}; }
};
