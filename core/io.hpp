#pragma once
#include <string>

// Espacio de nombres que contiene funciones utilitarias para entrada/salida de datos
namespace IO {
    // Función que lee un número entero desde la entrada estándar con validación de rango
    // Parámetros: prompt = mensaje mostrado al usuario, minVal = valor mínimo permitido, maxVal = valor máximo permitido
    // Retorna: número entero válido dentro del rango especificado
    // Nota: repite la solicitud hasta obtener un valor válido
    int readInt(const std::string &prompt, int minVal, int maxVal);

    // Función que lee una línea completa de texto desde la entrada estándar
    // Parámetro: prompt = mensaje mostrado al usuario antes de leer la entrada
    // Retorna: cadena de texto ingresada por el usuario (puede incluir espacios)
    std::string readLine(const std::string &prompt);
}
