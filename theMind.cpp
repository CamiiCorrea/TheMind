#include <iostream>
#include <ctime>
#include <cstdlib>
/*Clase carta que representa una carta en el juego*/
class Carta {
private:
    int valor;

public:
    Carta(int v = 0) : valor(v) {}

    int getValor() const { return valor; }
    void setValor(int v) { valor = v; }
};

/*Creamos la clase Jugador, que tendrá un arreglo 
 dinámico de cartas, la cantidad de cartas en la mano 
y un identificador único para cada jugador.*/

class Jugador {
private:
    Carta mano [20]; // Máximo 20 cartas por jugador
    int cantidadCartas;
    int id;

public:
    Jugador(int idJugador = 0);

    void agregarCarta(Carta c);
    void eliminarCarta(int posicion);
    void mostrarMano() const;

    int getCantidadCartas() const;
    Carta getCarta(int pos) const;
    int getId() const;
};

/*Crea las cartas del mazo del 1 al 100*/
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
};

/*Controla la logica general del juego*/
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
    bool verificarVictoriaNivel() const;
    bool verificarDerrota() const;
    int buscarMenorCarta() const;
};

/*PARTE 1: Crear la partida*/

/*Validaciones para el juego*/
int Juego::buscarMenorCarta() const {
    int menor = 101; // Mayor que cualquier carta posible
    for (int i = 0; i < numJugadores; ++i) {
        for (int j = 0; j < jugadores[i].getCantidadCartas(); ++j) {
            int valorCarta = jugadores[i].getCarta(j).getValor();
            if (valorCarta < menor) {
                menor = valorCarta;
            }
        }
    }
    return menor;
}

Juego::Juego(int nJugadores) {
    if (nJugadores < 2 || nJugadores > 4) {
        std::cout << "Número de jugadores inválido. Se establecerá a 2 por defecto." << std::endl;
        numJugadores = 2;
    } else {
        numJugadores = nJugadores;
    }

    nivelActual = 1;
    vidas = 3;
    ultimaCartaJugada = 0;
    partidaActiva = false;

    for (int i = 0; i < numJugadores; ++i) {
        jugadores[i] = Jugador(i);
    }
}

/* Validaciones para Juego*/
bool Juego::jugarCarta(int idJugador, int posicionCarta) {
    if (idJugador < 0 || idJugador >= numJugadores) {
        std::cout << "ID de jugador inválido." << std::endl;
        return false;
    }

    if (jugadores[idJugador].getCantidadCartas() == 0) {
        std::cout << "El jugador no tiene cartas." << std::endl;
        return false;
    }

    if (posicionCarta < 0 || posicionCarta >= jugadores[idJugador].getCantidadCartas()) {
        std::cout << "Posición de carta inválida." << std::endl;
        return false;
    }

    Carta cartaJugada = jugadores[idJugador].getCarta(posicionCarta);
    int valorJugada = cartaJugada.getValor();
    int menorGlobal = buscarMenorCarta();

    if (valorJugada == menorGlobal) {
        std::cout << "Carta jugada correctamente." << std::endl;
        jugadores[idJugador].eliminarCarta(posicionCarta);
        ultimaCartaJugada = valorJugada;
        return true;
    } else {
        std::cout << "Carta jugada incorrectamente. Pierdes una vida." << std::endl;
        vidas--;
        jugadores[idJugador].eliminarCarta(posicionCarta);
        ultimaCartaJugada = valorJugada;
        return false;
    }
}

void Juego::iniciarPartida() {
    mazo.inicializar();
    mazo.barajar();
    repartirCartas();
    partidaActiva = true;
}

void Juego::repartirCartas() {
    int cartasPorJugador = nivelActual; // Nivel 1: 1 cartas, Nivel 2: 2 cartas, etc.
    for (int i = 0; i < numJugadores; ++i) {
        for (int j = 0; j < cartasPorJugador; ++j) {
            if (!mazo.vacio()) {
                jugadores[i].agregarCarta(mazo.repartir());
            }
        }
    }
}

void Juego::mostrarEstado() const {
    std::cout << "\n---Estado del Juego---\n" << std::endl;
    std::cout << "\nNivel: " << nivelActual << " \n| Vidas: " << vidas << " \n| Última carta jugada: " << ultimaCartaJugada << std::endl;
    for (int i = 0; i < numJugadores; ++i) {
        jugadores[i].mostrarMano();
    }
}

bool Juego::verificarVictoriaNivel() const {
    for (int i = 0; i < numJugadores; ++i) {
        if (jugadores[i].getCantidadCartas() > 0) {
            return false; // Aún quedan cartas por jugar
        }
    }
    return true; // Todos los jugadores han jugado todas sus cartas
}

bool Juego::verificarDerrota() const {
    return vidas <= 0;
}

/*Validaciones para el jugador*/
Jugador::Jugador(int idJugador){
    id = idJugador;
    cantidadCartas = 0;
}

void Jugador::agregarCarta(Carta c) {
    if (cantidadCartas < 20) {
        mano[cantidadCartas] = c;
        cantidadCartas++;
    } else {
        std::cout << "No se pueden agregar más cartas a la mano." << std::endl;
    }
}

void Jugador::eliminarCarta(int posicion) {
    if (posicion < 0 || posicion >= cantidadCartas) {
        std::cout << "Posición de carta inválida." << std::endl;
        return;
    }
    for (int i = posicion; i < cantidadCartas - 1; ++i) {
        mano[i] = mano[i + 1];
    }
    cantidadCartas--;
}

void Jugador::mostrarMano() const {
    std::cout << "Jugador " << id << " tiene las siguientes cartas: ";
    for (int i = 0; i < cantidadCartas; ++i) {
        std::cout << mano[i].getValor() << " ";
    }
    std::cout << std::endl;
}

int Jugador::getCantidadCartas() const {
    return cantidadCartas;
}
Carta Jugador::getCarta(int pos) const {
    if (pos < 0 || pos >= cantidadCartas) {
        std::cout << "Posición de carta inválida." << std::endl;
        return Carta(-1); // Retorna una carta inválida
    }
    return mano[pos];
}
int Jugador::getId() const {
    return id;
}

/*Validaciones para el mazo*/
Mazo::Mazo() {
    tope = 0;
    inicializar();
}

void Mazo::inicializar() {
    for (int i = 0; i < 100; ++i) {
        cartas[i] = Carta(i + 1);
    }
    tope = 100;
}

void Mazo::barajar(){
    for(int i = 0; i < tope; ++i) {
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
        std::cout << "El mazo está vacío." << std::endl;
        return Carta(-1); // Retorna una carta inválida
    }
}

bool Mazo::vacio() const {
    return tope == 0;
}


int main(){
    srand(time(0));

    Juego juego(2); // Crear un juego con 2 jugadores
    juego.iniciarPartida();
    juego.mostrarEstado();

    int idJugador, posicionCarta;

    std::cout << "Ingrese el ID del jugador (0 o 1): ";
    std::cin >> idJugador;

    std::cout << "Ingrese la posición de la carta a jugar: ";
    std::cin >> posicionCarta;

    juego.jugarCarta(idJugador, posicionCarta);
    juego.mostrarEstado();

     if (juego.verificarVictoriaNivel()) {
        std::cout << "¡Nivel completado! Avanzando al siguiente nivel." << std::endl;
    } else if (juego.verificarDerrota()) {
        std::cout << "¡Has perdido todas tus vidas! Fin del juego." << std::endl;
    }

    return 0;
}
