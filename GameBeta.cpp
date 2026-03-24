
// Esta función recorre todos los jugadores y todas sus cartas.
// La función obtenerCartaMinimaGlobal recorre todas las cartas de todos los jugadores para encontrar la de menor valor. Empieza con un valor inicial de 101 (mayor que cualquier carta), compara cada carta y va actualizando la variable minima cuando encuentra una más pequeña. Al final, devuelve ese valor mínimo, es decir, la carta más baja en todo el juego.


int Juego::obtenerCartaMinimaGlobal() const {
    int minima = 101; // mayor que cualquier carta posible (1-100)

    for (int i = 0; i < numJugadores; i++) {
        for (int j = 0; j < jugadores[i].getCantidadCartas(); j++) {
            int valor = jugadores[i].getCarta(j).getValor();

            if (valor < minima) {
                minima = valor;
            }
        }
    }

    return minima;
}

// Elimiar cartas menores cuando se falla
// La función eliminarCartasMenores(int valorJugada) recorre todas las cartas de todos los jugadores y elimina aquellas que tengan un valor menor al de la jugada dada. Para cada jugador usa un while en lugar de for porque al eliminar una carta, las posiciones cambian: si elimina, no avanza el índice j para revisar la nueva carta que quedó en esa posición; si no elimina, simplemente incrementa j. Así se asegura de borrar correctamente todas las cartas menores sin saltarse ninguna.

void Juego::eliminarCartasMenores(int valorJugada) {
    for (int i = 0; i < numJugadores; i++) {

        int j = 0;
        while (j < jugadores[i].getCantidadCartas()) {

            int valor = jugadores[i].getCarta(j).getValor();

            if (valor < valorJugada) {
                jugadores[i].eliminarCarta(j);
            } else {
                j++;
            }
        }
    }
}

// La función jugarCarta() gestiona todo el proceso cuando un jugador intenta jugar una carta. Primero pide el id del jugador y valida que exista y que tenga cartas; si no, termina la función. Luego solicita la posición de la carta y también valida que sea correcta. Con esos datos, obtiene la carta seleccionada y su valor.

// Después calcula la carta mínima global del juego usando obtenerCartaMinimaGlobal() y compara: si el valor jugado es igual a esa mínima, la jugada es correcta; si no, es incorrecta, se pierde una vida y se eliminan todas las cartas menores al valor jugado (porque fueron “saltadas”).

// Independientemente del resultado, elimina la carta que se jugó y actualiza ultimaCartaJugada. Luego verifica si se quedaron sin vidas (fin del juego) o si cumplieron la condición para ganar el nivel, en cuyo caso avanza al siguiente.

void Juego::jugarCarta() {
    int idJugador, posCarta;

    cout << "Jugador que va a jugar: ";
    cin >> idJugador;

    // VALIDACIÓN 1: jugador válido
    if (idJugador < 0 || idJugador >= numJugadores) {
        cout << "Jugador invalido.\n";
        return;
    }

    // VALIDACIÓN 2: tiene cartas
    if (jugadores[idJugador].getCantidadCartas() == 0) {
        cout << "Ese jugador no tiene cartas.\n";
        return;
    }

    cout << "Posicion de la carta: ";
    cin >> posCarta;

    // VALIDACIÓN 3: posición válida
    if (posCarta < 0 || posCarta >= jugadores[idJugador].getCantidadCartas()) {
        cout << "Posicion invalida.\n";
        return;
    }

    // Obtener carta jugada
    Carta carta = jugadores[idJugador].getCarta(posCarta);
    int valorJugada = carta.getValor();

    cout << "Carta jugada: " << valorJugada << endl;

    // Obtener la mínima global
    int minima = obtenerCartaMinimaGlobal();

    cout << "Carta minima real: " << minima << endl;

    // VALIDACIÓN REAL DEL JUEGO
    if (valorJugada == minima) {
        cout << "Jugada correcta!\n";
    } else {
        cout << "Jugada incorrecta! Pierden una vida.\n";

        vidas--;

        // eliminar cartas menores (las que se saltaron)
        eliminarCartasMenores(valorJugada);
    }

    // eliminar la carta jugada
    jugadores[idJugador].eliminarCarta(posCarta);

    // actualizar última carta jugada
    ultimaCartaJugada = valorJugada;

    // verificar derrota
    if (vidas <= 0) {
        cout << "Game Over.\n";
        partidaActiva = false;
        return;
    }

    // verificar si ganaron el nivel
    if (verificarVictoriaNivel()) {
        cout << "Nivel completado!\n";
        nivelActual++;
    }
}