#include "engine.hpp"
#include "../core/io.hpp"
#include <iostream>

Engine::~Engine() {
    for (auto it = players.iter(); it.has(); it.next()) {
        delete it.val();
    }
}

Player *Engine::getPlayer(int id) const {
    return players.getAt(id);
}

void Engine::setupInteractively() {
    std::cout << "=== Totito Chino / Cajas ===\n";
    int Pw = IO::readInt("Puntos en ancho (>=3): ", 3, 100);
    int Ph = IO::readInt("Puntos en alto (>=3): ", 3, 100);
    board.initializeFromPoints(Pw, Ph);
    board.distributePowerUps(15); // 15% de celdas con power-up visibles

    int nPlayers = IO::readInt("Numero de jugadores (2..9): ", 2, 9);
    // Leer nombres e iniciales únicas simples
    bool used[256] = {false};
    for (int i = 0; i < nPlayers; ++i) {
        Player *pl = new Player();
        pl->id = i;
        pl->name = IO::readLine("Nombre del jugador " + std::to_string(i) + ": ");
        std::string ini = IO::readLine("Inicial (1 caracter) para " + pl->name + " (enter = auto): ");
        char ch = 0;
        if (!ini.empty()) ch = ini[0];
        if (ch == 0 || used[(unsigned char) ch]) {
            // asigna A,B,C,...
            ch = (char) ('A' + i);
            while (used[(unsigned char) ch]) ch++;
        }
        used[(unsigned char) ch] = true;
        pl->initial = ch;
        players.push_back(pl);
    }

    tm.initCircular(nPlayers);
}

void Engine::renderBoard() {
    board.render(clairvoyant);
    std::cout << "Jugadores: ";
    for (auto it = players.iter(); it.has(); it.next()) {
        auto *p = it.val();
        std::cout << p->initial << "=" << p->name << " (" << p->scorePoints << " pts)  ";
    }
    std::cout << "\nTurno de: " << getPlayer(tm.getCurrent())->name << " [" << getPlayer(tm.getCurrent())->initial <<
            "]\n";
    if (clairvoyant) std::cout << "(Vista clarividente activa)\n";
}

void Engine::printScores() {
    std::cout << "Puntajes:\n";
    for (auto it = players.iter(); it.has(); it.next()) {
        auto *p = it.val();
        std::cout << "- " << p->name << " [" << p->initial << "] : " << p->scorePoints << " puntos, " << p->squaresOwned
                << " cuadrados\n";
    }
}

bool Engine::isGameFinished() const {
    // termina cuando todas las celdas están cerradas (sin NT/EX consideradas)
    int total = board.cellsRows() * board.cellsCols();
    int closed = 0;
    for (int r = 0; r < board.cellsRows(); ++r) {
        for (int c = 0; c < board.cellsCols(); ++c) {
            if (board.getSquare(r, c)->closed) ++closed;
        }
    }
    return closed == total;
}

void Engine::gameLoop() {
    while (true) {
        std::cout << "\n=== Menu ===\n";
        std::cout << "1) Ver tablero\n";
        std::cout << "2) Colocar linea\n";
        std::cout << "3) Usar PowerUp\n";
        std::cout << "4) Alternar vista clarividente\n";
        std::cout << "5) Ver puntajes\n";
        std::cout << "0) Salir\n";
        int op = IO::readInt("Opcion: ", 0, 5);

        if (op == 0) break;
        if (op == 1) renderBoard();
        else if (op == 2) actionPlaceLine();
        else if (op == 3) actionUsePower();
        else if (op == 4) {
            clairvoyant = !clairvoyant;
            renderBoard();
        } else if (op == 5) printScores();

        if (isGameFinished()) {
            std::cout << "\nJuego terminado. Calculando ganador...\n";
            printScores();
            std::cout << "Desempates no implementados aun (pendiente).\n";
            break;
        }
    }
}

void Engine::pushPower(Player *p, PowerType t) {
    if (t == PowerType::None) return;
    p->powerStack.push(PowerUp{t});
}

void Engine::actionUsePower() {
    Player *p = getPlayer(tm.getCurrent());
    if (p->powerStack.isEmpty()) {
        std::cout << "No tienes power-ups en tu pila.\n";
        return;
    }
    PowerUp pu = p->powerStack.top(); // mostramos el top
    std::cout << "Usar tope de la pila: " << PowerCode(pu.type) << " ? (1=si, 0=no): ";
    int yes = IO::readInt("", 0, 1);
    if (!yes) return;

    // Consumir y aplicar buffers o efectos inmediatos
    p->powerStack.pop();
    p->powerUpsUsed++;

    switch (pu.type) {
        case PowerType::DL: p->activeDL = true;
            p->dlRemaining = 1;
            std::cout << "Activado Doble Linea para este turno.\n";
            break;
        case PowerType::PS: std::cout << "Pase usado. Vas al final.\n";
            tm.passTurn();
            break;
        case PowerType::LS: p->hasLS = true;
            std::cout << "Llave Secreta preparada para el proximo cierre bloqueado.\n";
            break;
        case PowerType::ES: p->hasES = true;
            std::cout << "Escurridizo activo para el proximo cierre/trampa.\n";
            break;
        case PowerType::NT: std::cout << "Nuevas Tierras (pendiente de implementar expandir tablero).\n";
            break;
        case PowerType::EX: std::cout << "Explosivos (pendiente de implementar eliminar punto).\n";
            break;
        default:
            std::cout << "Este poder se aplica al colocar linea (TS/BL/UF/AC): al colocar la linea, se marcara.\n";
            // Para TS/BL/UF/AC: devolver a la pila si no se aplica ahora
            p->powerUpsUsed--; // revert count
            p->powerStack.push(pu);
            break;
    }
}

void Engine::actionPlaceLine() {
    Player *p = getPlayer(tm.getCurrent());
    std::cout << "Colocar linea:\n";
    std::cout << "Orientacion (H=horizontal, V=vertical): ";
    std::string s;
    std::getline(std::cin >> std::ws, s);
    bool isH = !s.empty() && (s[0] == 'H' || s[0] == 'h');

    int r = IO::readInt("r (fila de puntos): ", 0, isH ? (board.pointsRows() - 1) : (board.pointsRows() - 2));
    int c = IO::readInt("c (columna de puntos): ", 0, isH ? (board.pointsCols() - 2) : (board.pointsCols() - 1));

    Edge *e = isH ? board.getEdgeH(r, c) : board.getEdgeV(r, c);
    if (!e) {
        std::cout << "Edge invalida.\n";
        return;
    }
    if (e->placed) {
        std::cout << "Esa linea ya existe.\n";
        return;
    }

    // Opcional: si el tope de la pila es TS/BL/UF/AC, preguntar si marcar esta linea con ese efecto
    if (!p->powerStack.isEmpty()) {
        PowerUp puTop = p->powerStack.top();
        if (puTop.type == PowerType::TS || puTop.type == PowerType::BL || puTop.type == PowerType::UF || puTop.type ==
            PowerType::AC) {
            std::cout << "Usar poder de linea " << PowerCode(puTop.type) << " en esta linea? (1=si/0=no): ";
            int yes = IO::readInt("", 0, 1);
            if (yes) {
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

    // Colocar linea
    e->placed = true;
    e->placedBy = p->id;
    e->roundPlaced = tm.getRoundIndex();

    // Evaluar cierres/efectos
    resolveSquaresForEdge(e, p);

    // Manejo de DL: permitir segunda colocación (simplificado: solo 1 adicional)
    if (p->activeDL && p->dlRemaining > 0) {
        std::cout << "Doble Linea: puedes colocar una segunda linea ahora.\n";
        p->dlRemaining = 0;
        p->activeDL = false;
        // No avanzamos turno: el jugador colocará otra línea volviendo a invocar actionPlaceLine
        return;
    } else {
        // Pasar turno por defecto cuando no hay DL pendiente
        tm.advance();
    }
}

void Engine::onLinePlaced(Edge * /*e*/, Player * /*p*/) {
    // hook si quieres loggear
}

void Engine::resolveSquaresForEdge(Edge *e, Player *p) {
    LinkedList<Square *> adj;
    board.collectSquaresAdjacentToEdge(e, adj);

    bool anyClosed = false;
    for (auto it = adj.iter(); it.has(); it.next()) {
        Square *s = it.val();
        if (!s->isClosable()) continue;

        // Simplificado: no aplicamos BL/LS/TS/ES/UF/AC aquí en detalle todavía.
        // Cierre normal:
        s->closed = true;
        s->ownerId = p->id; // visual
        s->pointsAwardedTo = p->id;
        p->scorePoints += 1;
        p->squaresOwned += 1;
        anyClosed = true;

        // PowerUp de la casilla
        if (s->hasPower) {
            pushPower(p, s->power.type);
            s->hasPower = false;
        }
    }

    if (anyClosed) {
        std::cout << "Has cerrado uno o mas cuadrados. Obtienes turno extra.\n";
        // turno extra: no avanzamos TM aquí; la lógica de avance se maneja en actionPlaceLine
    }
}
