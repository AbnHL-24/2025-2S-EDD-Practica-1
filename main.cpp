#include <iostream>                    // Biblioteca estándar para entrada/salida por consola
#include "game/engine.hpp"             // Incluye la clase Engine que controla toda la lógica del juego

// Función principal del programa - punto de entrada de la aplicación
int main(int argc, char *argv[]) {
    Engine eng; // Crea una instancia del motor de juego
    eng.setupInteractively(); // Configura el juego de forma interactiva (tablero, jugadores)
    eng.gameLoop(); // Ejecuta el bucle principal del juego hasta que termine
    std::cout << "Gracias por jugar.\n"; // Mensaje de despedida al finalizar la partida
    return 0; // Retorna código de éxito del programa
}
