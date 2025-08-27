#pragma once
#include <stdexcept>

template<typename T>
class Stack {
    struct Node {
        T data; // Dato almacenado en el nodo
        Node *next; // Puntero al siguiente nodo

        // Constructor del nodo: inicializa el dato y el puntero al siguiente nodo
        Node(const T &d, Node *n = nullptr) : data(d), next(n) {
        }
    };

    Node *head = nullptr; // Puntero al nodo superior de la pila
    int n = 0; // Contador de elementos en la pila

public:
    ~Stack() { clear(); } // Destructor que libera toda la memoria

    // Inserta un nuevo elemento en la parte superior de la pila
    void push(const T &v) {
        head = new Node(v, head); // Crea nuevo nodo apuntando al anterior tope
        ++n; // Incrementa contador
    }

    // Extrae y retorna el elemento superior de la pila
    T pop() {
        if (!head) throw std::underflow_error("Stack underflow"); // Valida que no esté vacía
        Node *t = head; // Guarda referencia al nodo a eliminar
        T v = t->data; // Copia el dato antes de eliminar
        head = t->next; // Actualiza head al siguiente
        delete t; // Libera memoria del nodo
        --n; // Decrementa contador
        return v; // Retorna el valor extraído
    }

    // Retorna una referencia constante al elemento superior sin extraerlo
    const T &top() const {
        if (!head) throw std::underflow_error("Stack empty"); // Valida que no esté vacía
        return head->data; // Retorna referencia al dato del tope
    }

    bool isEmpty() const { return head == nullptr; } // Verifica si la pila está vacía
    int size() const { return n; } // Retorna el número de elementos

    // Elimina todos los elementos de la pila y libera la memoria
    void clear() {
        while (head) {
            // Mientras haya nodos
            Node *t = head;
            head = head->next; // Avanza al siguiente
            delete t; // Elimina nodo anterior
        }
        n = 0; // Resetea contador
    }
};
