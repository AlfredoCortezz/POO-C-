#include <iostream>
#include <string>

class Deporte {
protected:
    std::string nombre;
    int numJugadores;

public:
    Deporte(const std::string & nombre, int jugadores) {
        this->nombre = nombre;
        this->numJugadores = jugadores;
        std::cout << "Se ha creado un deporte" << std::endl;
    }

    void iniciarPartido() {
        std::cout << "El deporte " << nombre 
                  << " ha comenzado con " << numJugadores 
                  << " jugadores." << std::endl;
    }

    void finalizarPartido() {
        std::cout << "El deporte " << nombre 
                  << " finalizó exitosamente, gracias por ver." << std::endl;
    }
};

// Clase hija: Fútbol
class Futbol : public Deporte {
private:
    std::string liga;

public:
    Futbol(const std::string & nombre, int jugadores, const std::string& liga)
        : Deporte(nombre, jugadores) {
        this->liga = liga;
        std::cout << "Se ha especificado que es un deporte: Futbol." << std::endl;
    }

    void anotarGol() {
        std::cout << "Gol " << this->nombre 
                  << " para abrir el marcador en " << this->liga << "." << std::endl;
    }
};

// Clase hija: Basketball
class Basket : public Deporte {
private:
    std::string equipoEstrella;

public:
    Basket(const std::string & nombre, int jugadores, const std::string & equipo)
        : Deporte(nombre, jugadores) {
        this->equipoEstrella = equipo;
        std::cout << "Se ha especificado que es un deporte: Basketball." << std::endl;
    }

    void encestarTriple() {
        std::cout << "Triple encestado en el partido de " << this->nombre 
                  << " por el equipo " << this->equipoEstrella << "." << std::endl;
    }
};

int main() {
    Futbol miFutbol("Futbol", 12, "La Liga Española");
    miFutbol.iniciarPartido();
    miFutbol.anotarGol();
    miFutbol.finalizarPartido();

    std::cout << "-----------------------------" << std::endl;

    Basket miBasket("Basketball", 5, "Lakers");
    miBasket.iniciarPartido();
    miBasket.encestarTriple();
    miBasket.finalizarPartido();

    return 0;
}

