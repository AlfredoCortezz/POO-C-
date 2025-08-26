#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>


// --- Clase Libro ---
class Libro {
public:
    std::string titulo;
    std::string autor;
    int anio;
    bool disponible;

    Libro() : anio(0), disponible(true) {}

    void verDetalles() const {
        std::cout << "Titulo: " << titulo << "\n";
        std::cout << "Autor: " << autor << "\n";
        std::cout << "Publicado en: " << anio << "\n";
        std::cout << "Estado: " << (disponible ? "Disponible" : "Prestado") << "\n";
        std::cout << "______________\n";
    }
};

// Clase Biblioteca
class Biblioteca {
private:
    std::vector<Libro> catalogo;

public:
    // Añadir libro al catálogo
    void agregarLibro(const Libro& nuevo) {
        for (const auto& l : catalogo) {
            if (l.titulo == nuevo.titulo) {
                std::cout << "El libro \"" << nuevo.titulo << "\" ya existe en el catalogo.\n";
                return;
            }
        }
        catalogo.push_back(nuevo);
        std::cout << "Libro ingresado correctamente.\n";
        guardarDatos("catalogo.txt");
    }

    // Mostrar todos los libros
    void mostrarLibros() {
        if (catalogo.empty()) {
            std::cout << "El catalogo esta vacio.\n";
            return;
        }
        std::cout << "\n___Catalogo de la Biblioteca__:\n";
        for (const auto& l : catalogo) {
            l.verDetalles();
        }
    }

    // Buscar libro
    Libro* localizarLibro(const std::string& nombre) {
        for (auto& l : catalogo) {
            if (l.titulo == nombre) return &l;
        }
        return nullptr;
    }

    // Prestar libro
    void prestar(const std::string& nombre) {
        Libro* l = localizarLibro(nombre);
        if (!l) {
            std::cout << "El libro no se encuentra en el catalogo.\n";
        } else if (!l->disponible) {
            std::cout << "El libro ya esta prestado.\n";
        } else {
            l->disponible = false;
            std::cout << "Se presto el libro: " << l->titulo << "\n";
            guardarDatos("catalogo.txt");
        }
    }

    // Devolver libro
    void devolver(const std::string& nombre) {
        Libro* l = localizarLibro(nombre);
        if (!l) {
            std::cout << "Ese libro no existe en la biblioteca.\n";
        } else if (l->disponible) {
            std::cout << "Ese libro ya estaba disponible.\n";
        } else {
            l->disponible = true;
            std::cout << "Se devolvio el libro: " << l->titulo << "\n";
            guardarDatos("catalogo.txt");
        }
    }

    // Guardar en archivo
    void guardarDatos(const std::string& archivo) {
        std::ofstream out(archivo);
        if (!out) {
            std::cout << "Error al guardar datos.\n";
            return;
        }
        for (const auto& l : catalogo) {
            out << l.titulo << ";" << l.autor << ";" << l.anio << ";" << l.disponible << "\n";
        }
        out.close();
    }

    // Cargar desde archivo
    void cargarDatos(const std::string& archivo) {
        std::ifstream in(archivo);
        if (!in) {
            std::cout << "No se encontro archivo previo. Iniciando biblioteca vacia.\n";
            return;
        }
        catalogo.clear();
        std::string linea;
        while (std::getline(in, linea)) {
            Libro l;
            size_t p1 = linea.find(";");
            size_t p2 = linea.find(";", p1 + 1);
            size_t p3 = linea.find(";", p2 + 1);

            l.titulo = linea.substr(0, p1);
            l.autor = linea.substr(p1 + 1, p2 - p1 - 1);
            l.anio = std::stoi(linea.substr(p2 + 1, p3 - p2 - 1));
            l.disponible = (linea.substr(p3 + 1) == "1");
            catalogo.push_back(l);
        }
        in.close();
    }
};

// Main
int main() {
    Biblioteca miBiblioteca;
    miBiblioteca.cargarDatos("catalogo.txt");

    int opcion = 0;
    while (opcion != 5) {
        std::cout << "\n___SISTEMA DE BIBLIOTECA_\n";
        std::cout << "1. Agregar libro\n";
        std::cout << "2. Mostrar catalogo\n";
        std::cout << "3. Prestar libro\n";
        std::cout << "4. Devolver libro\n";
        std::cout << "5. Salir\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;
        std::cin.ignore();

        if (opcion == 1) {
            Libro nuevo;
            std::cout << "Titulo: ";
            std::getline(std::cin, nuevo.titulo);
            std::cout << "Autor: ";
            std::getline(std::cin, nuevo.autor);
            std::cout << "Anio de publicacion: ";
            std::cin >> nuevo.anio;
            std::cin.ignore();
            miBiblioteca.agregarLibro(nuevo);
        }
        else if (opcion == 2) {
            miBiblioteca.mostrarLibros();
        }
        else if (opcion == 3) {
            std::string titulo;
            std::cout << "Titulo del libro a prestar: ";
            std::getline(std::cin, titulo);
            miBiblioteca.prestar(titulo);
        }
        else if (opcion == 4) {
            std::string titulo;
            std::cout << "Titulo del libro a devolver: ";
            std::getline(std::cin, titulo);
            miBiblioteca.devolver(titulo);
        }
        else if (opcion == 5) {
            miBiblioteca.guardarDatos("catalogo.txt");
            std::cout << "Cerrando biblioteca...\n";
        }
        else {
            std::cout << "Opcion invalida, intente de nuevo.\n";
        }
    }

return 0;
}