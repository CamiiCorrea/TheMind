#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

/*==================== CLASE CARTA ====================*/
class Carta {
private:
    int valor;

public:
    Carta(int v = 0) : valor(v) {}

    int getValor() const { return valor; }
    void setValor(int v) { valor = v; }
};

/*==================== CLASE JUGADOR ====================*/
class Jugador {
private:
    Carta mano[20];
    int cantidadCartas;
    int id;

public:
    Jugador(int idJugador = 0);

    void agregarCarta(Carta c);
    void eliminarCarta(int posicion);
    void limpiarMano();
    void mostrarMano() const;

    int getCantidadCartas() const;
    Carta getCarta(int pos) const;
    int getId() const;
};

/*==================== CLASE MAZO ====================*/
class Mazo {
private:
    Carta cartas[100];
    int tope;

public:
    Mazo();

    void inicializar();
    void barajar();
    Carta repartir();
    bool vacio() const;

    Carta getCarta(int i) const;
    void setCarta(int i, Carta c);
    int getTope() const;
    void setTope(int t);
};

/*==================== CLASE JUEGO ====================*/
class Juego {
private:
    Jugador jugadores[4];
    int numJugadores;
    int nivelActual;
    int vidas;
    Mazo mazo;
    int ultimaCartaJugada;
    bool partidaActiva;

public:
    Juego(int nJugadores = 2);

    void iniciarPartida();
    void repartirCartas();
    void mostrarEstado() const;

    bool jugarCarta(int idJugador, int posicionCarta);
    void jugarCarta();

    bool verificarVictoriaNivel() const;
    bool verificarDerrota() const;

    int buscarMenorCarta() const;
    void eliminarCartasMenores(int valorJugada);

    struct EstadoJuego {
        int numJugadores;
        int nivelActual;
        int vidas;
        int ultimaCartaJugada;
        bool partidaActiva;

        int cantidadCartas[4];
        int manos[4][20];

        int mazo[100];
        int topeMazo;
    };

    EstadoJuego obtenerEstado() const;
    void restaurarEstado(const EstadoJuego& estado);
    void guardarPartida() const;
    void cargarPartida();
};

/*======================================================
=                    MÉTODOS JUGADOR                   =
======================================================*/
Jugador::Jugador(int idJugador) {
    id = idJugador;
    cantidadCartas = 0;
}

void Jugador::agregarCarta(Carta c) {
    if (cantidadCartas < 20) {
        mano[cantidadCartas] = c;
        cantidadCartas++;
    } else {
        cout << "No se pueden agregar más cartas a la mano.\n";
    }
}

void Jugador::eliminarCarta(int posicion) {
    if (posicion < 0 || posicion >= cantidadCartas) {
        cout << "Posición de carta inválida.\n";
        return;
    }

    for (int i = posicion; i < cantidadCartas - 1; i++) {
        mano[i] = mano[i + 1];
    }

    cantidadCartas--;
}

void Jugador::limpiarMano() {
    cantidadCartas = 0;
}

void Jugador::mostrarMano() const {
    cout << "Jugador " << id << " tiene: ";
    for (int i = 0; i < cantidadCartas; i++) {
        cout << mano[i].getValor() << " ";
    }
    cout << endl;
}

int Jugador::getCantidadCartas() const {
    return cantidadCartas;
}

Carta Jugador::getCarta(int pos) const {
    if (pos < 0 || pos >= cantidadCartas) {
        return Carta(-1);
    }
    return mano[pos];
}

int Jugador::getId() const {
    return id;
}

/*======================================================
=                      MÉTODOS MAZO                    =
======================================================*/
Mazo::Mazo() {
    inicializar();
}

void Mazo::inicializar() {
    for (int i = 0; i < 100; i++) {
        cartas[i] = Carta(i + 1);
    }
    tope = 100;
}

void Mazo::barajar() {
    for (int i = 0; i < tope; i++) {
        int j = rand() % tope;
        Carta temp = cartas[i];
        cartas[i] = cartas[j];
        cartas[j] = temp;
    }
}

Carta Mazo::repartir() {
    if (tope > 0) {
        return cartas[--tope];
    } else {
        cout << "El mazo está vacío.\n";
        return Carta(-1);
    }
}

bool Mazo::vacio() const {
    return tope == 0;
}

Carta Mazo::getCarta(int i) const {
    return cartas[i];
}

void Mazo::setCarta(int i, Carta c) {
    cartas[i] = c;
}

int Mazo::getTope() const {
    return tope;
}

void Mazo::setTope(int t) {
    tope = t;
}

/*======================================================
=                     MÉTODOS JUEGO                    =
======================================================*/
Juego::Juego(int nJugadores) {
    if (nJugadores < 2 || nJugadores > 4) {
        cout << "Número de jugadores inválido. Se establecerá a 2 por defecto.\n";
        numJugadores = 2;
    } else {
        numJugadores = nJugadores;
    }

    nivelActual = 1;
    vidas = 3;
    ultimaCartaJugada = 0;
    partidaActiva = false;

    for (int i = 0; i < 4; i++) {
        jugadores[i] = Jugador(i);
    }
}

void Juego::iniciarPartida() {
    cout << "Ingrese número de jugadores (2-4): ";
    cin >> numJugadores;

    if (numJugadores < 2 || numJugadores > 4) {
        cout << "Número inválido. Se usará 2 por defecto.\n";
        numJugadores = 2;
    }

    nivelActual = 1;
    vidas = 3;
    ultimaCartaJugada = 0;

    mazo.inicializar();
    mazo.barajar();

    for (int i = 0; i < numJugadores; i++) {
        jugadores[i].limpiarMano();
    }

    repartirCartas();
    partidaActiva = true;

    cout << "Partida iniciada.\n";
}

void Juego::repartirCartas() {
    for (int i = 0; i < numJugadores; i++) {
        jugadores[i].limpiarMano();
    }

    int cartasPorJugador = nivelActual;

    for (int i = 0; i < cartasPorJugador; i++) {
        for (int j = 0; j < numJugadores; j++) {
            if (!mazo.vacio()) {
                jugadores[j].agregarCarta(mazo.repartir());
            }
        }
    }
}

void Juego::mostrarEstado() const {
    cout << "\n===== ESTADO DEL JUEGO =====\n";
    cout << "Nivel: " << nivelActual << endl;
    cout << "Vidas: " << vidas << endl;
    cout << "Última carta jugada: " << ultimaCartaJugada << endl;
    cout << "Partida activa: " << (partidaActiva ? "Sí" : "No") << endl;

    for (int i = 0; i < numJugadores; i++) {
        jugadores[i].mostrarMano();
    }
}

int Juego::buscarMenorCarta() const {
    int menor = 101;

    for (int i = 0; i < numJugadores; i++) {
        for (int j = 0; j < jugadores[i].getCantidadCartas(); j++) {
            int valorCarta = jugadores[i].getCarta(j).getValor();
            if (valorCarta < menor) {
                menor = valorCarta;
            }
        }
    }

    return menor;
}

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

bool Juego::jugarCarta(int idJugador, int posicionCarta) {
    if (idJugador < 0 || idJugador >= numJugadores) {
        cout << "ID de jugador inválido.\n";
        return false;
    }

    if (jugadores[idJugador].getCantidadCartas() == 0) {
        cout << "Ese jugador no tiene cartas.\n";
        return false;
    }

    if (posicionCarta < 0 || posicionCarta >= jugadores[idJugador].getCantidadCartas()) {
        cout << "Posición de carta inválida.\n";
        return false;
    }

    Carta cartaJugada = jugadores[idJugador].getCarta(posicionCarta);
    int valorJugada = cartaJugada.getValor();
    int menorGlobal = buscarMenorCarta();

    cout << "Carta jugada: " << valorJugada << endl;
    cout << "Carta mínima real: " << menorGlobal << endl;

    if (valorJugada == menorGlobal) {
        cout << "Jugada correcta.\n";
    } else {
        cout << "Jugada incorrecta. Pierden una vida.\n";
        vidas--;
        eliminarCartasMenores(valorJugada);
    }

    jugadores[idJugador].eliminarCarta(posicionCarta);
    ultimaCartaJugada = valorJugada;

    if (vidas <= 0) {
        cout << "Game Over.\n";
        partidaActiva = false;
    }

    return (valorJugada == menorGlobal);
}

void Juego::jugarCarta() {
    if (!partidaActiva) {
        cout << "No hay partida activa.\n";
        return;
    }

    int idJugador, posicionCarta;

    cout << "Jugador que va a jugar: ";
    cin >> idJugador;

    cout << "Posición de la carta: ";
    cin >> posicionCarta;

    jugarCarta(idJugador, posicionCarta);

    if (verificarVictoriaNivel()) {
        cout << "¡Nivel completado!\n";
    } else if (verificarDerrota()) {
        cout << "¡Has perdido todas las vidas!\n";
    }
}

bool Juego::verificarVictoriaNivel() const {
    for (int i = 0; i < numJugadores; i++) {
        if (jugadores[i].getCantidadCartas() > 0) {
            return false;
        }
    }
    return true;
}

bool Juego::verificarDerrota() const {
    return vidas <= 0;
}

/*==================== GUARDAR / CARGAR ====================*/
Juego::EstadoJuego Juego::obtenerEstado() const {
    EstadoJuego estado;

    estado.numJugadores = numJugadores;
    estado.nivelActual = nivelActual;
    estado.vidas = vidas;
    estado.ultimaCartaJugada = ultimaCartaJugada;
    estado.partidaActiva = partidaActiva;

    for (int i = 0; i < 4; i++) {
        estado.cantidadCartas[i] = jugadores[i].getCantidadCartas();

        for (int j = 0; j < 20; j++) {
            if (j < jugadores[i].getCantidadCartas()) {
                estado.manos[i][j] = jugadores[i].getCarta(j).getValor();
            } else {
                estado.manos[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < 100; i++) {
        estado.mazo[i] = mazo.getCarta(i).getValor();
    }

    estado.topeMazo = mazo.getTope();

    return estado;
}

void Juego::restaurarEstado(const EstadoJuego& estado) {
    numJugadores = estado.numJugadores;
    nivelActual = estado.nivelActual;
    vidas = estado.vidas;
    ultimaCartaJugada = estado.ultimaCartaJugada;
    partidaActiva = estado.partidaActiva;

    for (int i = 0; i < 4; i++) {
        jugadores[i].limpiarMano();

        for (int j = 0; j < estado.cantidadCartas[i]; j++) {
            jugadores[i].agregarCarta(Carta(estado.manos[i][j]));
        }
    }

    for (int i = 0; i < 100; i++) {
        mazo.setCarta(i, Carta(estado.mazo[i]));
    }

    mazo.setTope(estado.topeMazo);
}

void Juego::guardarPartida() const {
    ofstream archivo("partida.dat", ios::binary);

    if (!archivo) {
        cout << "No se pudo abrir el archivo para guardar.\n";
        return;
    }

    EstadoJuego estado = obtenerEstado();
    archivo.write(reinterpret_cast<const char*>(&estado), sizeof(EstadoJuego));
    archivo.close();

    cout << "Partida guardada correctamente.\n";
}

void Juego::cargarPartida() {
    ifstream archivo("partida.dat", ios::binary);

    if (!archivo) {
        cout << "No se encontró una partida guardada.\n";
        return;
    }

    EstadoJuego estado;
    archivo.read(reinterpret_cast<char*>(&estado), sizeof(EstadoJuego));
    archivo.close();

    restaurarEstado(estado);
    cout << "Partida cargada correctamente.\n";
}

/*==================== MAIN ====================*/
int main() {
    srand(time(0));

    Juego juego;
    int opcion;

    do {
        cout << "\n===== THE MIND =====\n";
        cout << "1. Nueva partida\n";
        cout << "2. Mostrar estado\n";
        cout << "3. Jugar carta\n";
        cout << "4. Guardar partida\n";
        cout << "5. Cargar partida\n";
        cout << "6. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                juego.iniciarPartida();
                break;
            case 2:
                juego.mostrarEstado();
                break;
            case 3:
                juego.jugarCarta();
                break;
            case 4:
                juego.guardarPartida();
                break;
            case 5:
                juego.cargarPartida();
                break;
            case 6:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opción inválida.\n";
        }

    } while (opcion != 6);

    return 0;
}