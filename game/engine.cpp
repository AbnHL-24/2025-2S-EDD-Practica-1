#include "engine.hpp"
#include "../core/io.hpp"
#include <iostream>

// Destructor del motor: libera memoria de todos los jugadores creados dinámicamente
Engine::~Engine() {
    for (auto it = players.iter(); it.has(); it.next()) {
        delete it.val(); // Elimina cada jugador de la lista
    }
}

// Obtiene un puntero al jugador con el ID especificado
Player *Engine::getPlayer(int id) const {
    return players.getAt(id); // Acceso directo por índice en la lista enlazada
}

// Configuración interactiva inicial del juego mediante entrada del usuario
void Engine::setupInteractively() {
    std::cout << "=== Totito Chino / Cajas ===\n";

    // Configuración del tablero basada en puntos de la cuadrícula
    int Pw = IO::readInt("Puntos en ancho (>=3): ", 3, 100);
    int Ph = IO::readInt("Puntos en alto (>=3): ", 3, 100);
    board.initializeFromPoints(Pw, Ph);
    board.distributePowerUps(15); // Distribuye power-ups en 15% de las celdas

    // Configuración de jugadores con validación de iniciales únicas
    int nPlayers = IO::readInt("Numero de jugadores (2..9): ", 2, 9);
    bool used[256] = {false}; // Rastrea iniciales ya utilizadas

    for (int i = 0; i < nPlayers; ++i) {
        Player *pl = new Player();
        pl->id = i;
        pl->name = IO::readLine("Nombre del jugador " + std::to_string(i) + ": ");
        std::string ini = IO::readLine("Inicial (1 caracter) para " + pl->name + " (enter = auto): ");

        char ch = 0;
        if (!ini.empty()) ch = ini[0]; // Usa inicial proporcionada si existe

        // Asigna inicial automática si no se proporcionó o ya está en uso
        if (ch == 0 || used[(unsigned char) ch]) {
            ch = (char) ('A' + i); // Asigna A, B, C, etc.
            while (used[(unsigned char) ch]) ch++; // Busca siguiente disponible
        }

        used[(unsigned char) ch] = true; // Marca inicial como usada
        pl->initial = ch;
        players.push_back(pl);
    }

    tm.initCircular(nPlayers); // Inicializa sistema de turnos circulares
}

// Renderiza el tablero completo con información de estado y jugadores
void Engine::renderBoard() {
    board.render(clairvoyant); // Renderiza tablero (con/sin modo clarividente)

    // Muestra información de todos los jugadores
    std::cout << "Jugadores: ";
    for (auto it = players.iter(); it.has(); it.next()) {
        auto *p = it.val();
        std::cout << p->initial << "=" << p->name << " (" << p->scorePoints << " pts)  ";
    }

    // Muestra jugador con turno actual
    std::cout << "\nTurno de: " << getPlayer(tm.getCurrent())->name << " [" << getPlayer(tm.getCurrent())->initial <<
            "]\n";
    if (clairvoyant) std::cout << "(Vista clarividente activa)\n";
}

// Muestra los puntajes detallados de todos los jugadores
void Engine::printScores() {
    std::cout << "Puntajes:\n";
    for (auto it = players.iter(); it.has(); it.next()) {
        auto *p = it.val();
        std::cout << "- " << p->name << " [" << p->initial << "] : " << p->scorePoints << " puntos, "
                << p->squaresOwned << " cuadrados\n";
    }
}

// Verifica si el juego ha terminado (todos los cuadrados están cerrados)
bool Engine::isGameFinished() const {
    int total = board.cellsRows() * board.cellsCols(); // Total de cuadrados en el tablero
    int closed = 0;

    // Cuenta cuadrados cerrados
    for (int r = 0; r < board.cellsRows(); ++r) {
        for (int c = 0; c < board.cellsCols(); ++c) {
            if (board.getSquare(r, c)->closed) ++closed;
        }
    }
    return closed == total; // Juego termina cuando todos están cerrados
}

// Bucle principal del juego que maneja el menú y coordina las acciones
void Engine::gameLoop() {
    while (true) {
        // Muestra menú principal de opciones
        std::cout << "\n=== Menu ===\n";
        std::cout << "1) Ver tablero\n";
        std::cout << "2) Colocar linea\n";
        std::cout << "3) Usar PowerUp\n";
        std::cout << "4) Alternar vista clarividente\n";
        std::cout << "5) Ver puntajes\n";
        std::cout << "0) Salir\n";
        int op = IO::readInt("Opcion: ", 0, 5);

        if (op == 0) break; // Salir del juego
        if (op == 1) renderBoard();
        else if (op == 2) actionPlaceLine();
        else if (op == 3) actionUsePower();
        else if (op == 4) {
            clairvoyant = !clairvoyant; // Alternar modo clarividente
            renderBoard();
        } else if (op == 5) printScores();

        // Verifica condición de victoria después de cada acción
        if (isGameFinished()) {
            std::cout << "\nJuego terminado. Calculando ganador...\n";
            printScores();
            std::cout << "Desempates no implementados aun (pendiente).\n";
            break;
        }
    }
}

// Añade un power-up a la pila de un jugador
void Engine::pushPower(Player *p, PowerType t) {
    if (t == PowerType::None) return; // No añade power-ups vacíos
    p->powerStack.push(PowerUp{t});
}

// Acción para usar un power-up del tope de la pila del jugador actual
void Engine::actionUsePower() {
    Player *p = getPlayer(tm.getCurrent());
    if (p->powerStack.isEmpty()) {
        std::cout << "No tienes power-ups en tu pila.\n";
        return;
    }

    // Muestra y confirma uso del power-up del tope
    PowerUp pu = p->powerStack.top();
    std::cout << "Usar tope de la pila: " << PowerCode(pu.type) << " ? (1=si, 0=no): ";
    int yes = IO::readInt("", 0, 1);
    if (!yes) return;

    // Consume el power-up y aplica sus efectos
    p->powerStack.pop();
    p->powerUpsUsed++;

    switch (pu.type) {
        case PowerType::DL: // Double Laser - permite colocar línea adicional
            p->activeDL = true;
            p->dlRemaining = 1;
            std::cout << "Activado Doble Linea para este turno.\n";
            break;
        case PowerType::PS: // Power Shot - pasa turno inmediatamente
            std::cout << "Pase usado. Vas al final.\n";
            tm.passTurn();
            break;
        case PowerType::LS: // Laser Shield - protección para próximo cierre
            p->hasLS = true;
            std::cout << "Llave Secreta preparada para el proximo cierre bloqueado.\n";
            break;
        case PowerType::ES: // Energy Shield - protección contra trampas
            p->hasES = true;
            std::cout << "Escurridizo activo para el proximo cierre/trampa.\n";
            break;
        case PowerType::NT: // Nuclear Torpedo - expansión de tablero (pendiente)
            std::cout << "Nuevas Tierras (pendiente de implementar expandir tablero).\n";
            break;
        case PowerType::EX: // Extra Life - eliminación de punto (pendiente)
            std::cout << "Explosivos (pendiente de implementar eliminar punto).\n";
            break;
        default: // Power-ups de línea (TS/BL/UF/AC) - se aplican al colocar líneas
            std::cout << "Este poder se aplica al colocar linea (TS/BL/UF/AC): al colocar la linea, se marcara.\n";
            // Devuelve a la pila si no se puede aplicar inmediatamente
            p->powerUpsUsed--; // Revierte contador
            p->powerStack.push(pu);
            break;
    }
}

// Acción para colocar una línea en el tablero con validación y efectos
void Engine::actionPlaceLine() {
    Player *p = getPlayer(tm.getCurrent());
    std::cout << "Colocar linea:\n";

    // Solicita orientación de la línea
    std::cout << "Orientacion (H=horizontal, V=vertical): ";
    std::string s;
    std::getline(std::cin >> std::ws, s);
    bool isH = !s.empty() && (s[0] == 'H' || s[0] == 'h');

    // Solicita coordenadas con validación de rangos según orientación
    int r = IO::readInt("r (fila de puntos): ", 0, isH ? (board.pointsRows() - 1) : (board.pointsRows() - 2));
    int c = IO::readInt("c (columna de puntos): ", 0, isH ? (board.pointsCols() - 2) : (board.pointsCols() - 1));

    // Obtiene y valida el borde especificado
    Edge *e = isH ? board.getEdgeH(r, c) : board.getEdgeV(r, c);
    if (!e) {
        std::cout << "Edge invalida.\n";
        return;
    }
    if (e->placed) {
        std::cout << "Esa linea ya existe.\n";
        return;
    }

    // Verifica si aplicar power-up de línea del tope de la pila
    if (!p->powerStack.isEmpty()) {
        PowerUp puTop = p->powerStack.top();
        if (puTop.type == PowerType::TS || puTop.type == PowerType::BL ||
            puTop.type == PowerType::UF || puTop.type == PowerType::AC) {
            std::cout << "Usar poder de linea " << PowerCode(puTop.type) << " en esta linea? (1=si/0=no): ";
            int yes = IO::readInt("", 0, 1);
            if (yes) {
                // Añade efecto de línea al borde
                LineEffect le;
                le.type = puTop.type;
                le.ownerId = p->id;
                le.active = true;
                le.roundIndexPlaced = tm.getRoundIndex();
                e->effects.push_back(le);
                p->powerStack.pop();
                p->powerUpsUsed++;
            }
        }
    }

    // Coloca la línea en el tablero
    e->placed = true;
    e->placedBy = p->id;
    e->roundPlaced = tm.getRoundIndex();

    // Resuelve cierres de cuadrados y efectos resultantes
    resolveSquaresForEdge(e, p);

    // Manejo especial para Double Laser (permite línea adicional)
    if (p->activeDL && p->dlRemaining > 0) {
        std::cout << "Doble Linea: puedes colocar una segunda linea ahora.\n";
        p->dlRemaining = 0;
        p->activeDL = false;
        // No avanza turno: permite otra colocación inmediata
        return;
    } else {
        // Avanza al siguiente turno normalmente
        tm.advance();
    }
}

// Hook llamado cuando se coloca una línea (para logging o efectos adicionales)
void Engine::onLinePlaced(Edge * /*e*/, Player * /*p*/) {
    // Función de extensión para logging o efectos especiales futuros
}

// Resuelve el cierre de cuadrados cuando se coloca una línea y otorga beneficios
void Engine::resolveSquaresForEdge(Edge *e, Player *p) {
    LinkedList<Square *> adj;
    board.collectSquaresAdjacentToEdge(e, adj); // Obtiene cuadrados adyacentes al borde

    bool anyClosed = false;
    for (auto it = adj.iter(); it.has(); it.next()) {
        Square *s = it.val();
        if (!s->isClosable()) continue; // Salta cuadrados que no pueden cerrarse

        // Cierre básico del cuadrado (sin efectos especiales por ahora)
        s->closed = true;
        s->ownerId = p->id; // Propietario visual
        s->pointsAwardedTo = p->id; // Receptor de puntos
        p->scorePoints += 1; // Otorga punto por cierre
        p->squaresOwned += 1; // Incrementa contador de cuadrados
        anyClosed = true;

        // Transfiere power-up del cuadrado al jugador si existe
        if (s->hasPower) {
            pushPower(p, s->power.type);
            s->hasPower = false; // Remueve power-up del cuadrado
        }
    }

    // Otorga turno extra si se cerró al menos un cuadrado
    if (anyClosed) {
        std::cout << "Has cerrado uno o mas cuadrados. Obtienes turno extra.\n";
        // Turno extra: no avanza TM aquí; se maneja en actionPlaceLine
    }
}
