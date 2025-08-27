#include "io.hpp"
#include <iostream>
#include <limits>

// Función que lee un número entero validado desde la entrada estándar
int IO::readInt(const std::string &prompt, int minVal, int maxVal) {
    int v;
    while (true) {
        // Bucle hasta obtener entrada válida
        std::cout << prompt; // Muestra el mensaje al usuario
        if (std::cin >> v) {
            // Intenta leer un entero
            if (v >= minVal && v <= maxVal) return v; // Si está en rango, retorna el valor
            std::cout << "Valor fuera de rango [" << minVal << "," << maxVal << "]\n"; // Mensaje de error de rango
        } else {
            // Si la entrada no es un entero válido
            std::cin.clear(); // Limpia el estado de error del stream
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Descarta la línea actual completa
    }
}

// Función que lee una línea completa de texto desde la entrada estándar
std::string IO::readLine(const std::string &prompt) {
    std::cout << prompt; // Muestra el mensaje al usuario
    std::string s; // Variable para almacenar la línea leída
    std::getline(std::cin >> std::ws, s); // Lee línea completa, saltando espacios en blanco iniciales
    return s; // Retorna la cadena leída
}
