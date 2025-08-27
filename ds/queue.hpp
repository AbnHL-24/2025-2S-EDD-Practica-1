#pragma once
#include <stdexcept>

template<typename T>
class Queue {
    struct Node {
        T data; // Dato almacenado en el nodo
        Node *next; // Puntero al siguiente nodo

        // Constructor del nodo: inicializa el dato con el valor recibido y el puntero next en nullptr
        Node(const T &d) : data(d), next(nullptr) {
        }
    };

    Node *head = nullptr; // Puntero al primer nodo (frente de la cola)
    Node *tail = nullptr; // Puntero al último nodo (final de la cola)
    int n = 0; // Contador de elementos en la cola

public:
    ~Queue() { clear(); } // Destructor que libera toda la memoria

    // Inserta un nuevo elemento al final de la cola (operación FIFO)
    void enqueue(const T &v) {
        Node *nn = new Node(v); // Crea nuevo nodo
        if (!tail) { head = tail = nn; } else {
            // Si está vacía, head y tail apuntan al mismo nodo
            tail->next = nn; // Conecta al final
            tail = nn; // Actualiza tail
        }
        ++n; // Incrementa contador
    }

    // Extrae y retorna el elemento del frente de la cola (operación FIFO)
    T dequeue() {
        if (!head) throw std::underflow_error("Queue underflow"); // Valida que no esté vacía
        Node *t = head; // Guarda referencia al nodo a eliminar
        T v = t->data; // Copia el dato antes de eliminar
        head = head->next; // Actualiza head al siguiente
        if (!head) tail = nullptr; // Si se vació, actualiza tail
        delete t; // Libera memoria del nodo
        --n; // Decrementa contador
        return v; // Retorna el valor extraído
    }

    // Retorna una referencia constante al elemento del frente sin extraerlo
    const T &front() const {
        if (!head) throw std::underflow_error("Queue empty"); // Valida que no esté vacía
        return head->data; // Retorna referencia al dato del frente
    }

    bool isEmpty() const { return head == nullptr; } // Verifica si la cola está vacía
    int size() const { return n; } // Retorna el número de elementos

    // Elimina todos los elementos de la cola y libera la memoria
    void clear() {
        while (head) {
            // Mientras haya nodos
            Node *t = head;
            head = head->next; // Avanza al siguiente
            delete t; // Elimina nodo anterior
        }
        tail = nullptr; // Resetea tail
        n = 0; // Resetea contador
    }
};
