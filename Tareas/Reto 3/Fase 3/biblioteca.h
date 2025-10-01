#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

struct Autor {
    int id;
    string nombre;
    string nacionalidad;
};

struct Libro {
    int id;
    string titulo;
    string isbn;
    int ano;
    int id_autor;
};

struct Estudiante {
    int id;
    string nombre;
    int grado;
};

struct Prestamo {
    int id;
    string fecha_prestamo;
    string fecha_devolucion;
    int id_libro;
    int id_estudiante;
    bool activo;
};

class BibliotecaDB {
private:
    vector<Autor> autores;
    vector<Libro> libros;
    vector<Estudiante> estudiantes;
    vector<Prestamo> prestamos;

    // metodos auxiliares
    bool autorExiste(int id);
    bool libroExiste(int id);
    bool estudianteExiste(int id);
    bool prestamoExiste(int id);
    bool libroDisponible(int id_libro);
    
    // carga de datos
    void cargarAutores();
    void cargarLibros();
    void cargarEstudiantes();
    void cargarPrestamos();
    
    // guardado de datos
    void guardarAutores();
    void guardarLibros();
    void guardarEstudiantes();
    void guardarPrestamos();

public:
    BibliotecaDB();
    
    // CRUD Autores
    void agregarAutor();
    void listarAutores();
    void buscarAutorPorId();
    void eliminarAutor();
    
    // CRUD Libros
    void agregarLibro();
    void listarLibros();
    void buscarLibroPorTitulo();
    void eliminarLibro();
    
    // CRUD Estudiantes
    void agregarEstudiante();
    void listarEstudiantes();
    void buscarEstudiantePorNombre();
    void eliminarEstudiante();
    
    // CRUD Prestamos
    void realizarPrestamo();
    void devolverLibro();
    void listarPrestamos();
    void buscarPrestamosPorEstudiante();
    
    // Menu principal
    void mostrarMenu();
};

#endif