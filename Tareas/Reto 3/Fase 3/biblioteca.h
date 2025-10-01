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

    // Métodos auxiliares para validaciones
    bool autorExiste(int id);
    bool libroExiste(int id);
    bool estudianteExiste(int id);
    bool prestamoExiste(int id);
    bool libroDisponible(int id_libro);
    bool validarFecha(const string& fecha);
    
    // Métodos para cargar datos desde archivos
    void cargarAutores();
    void cargarLibros();
    void cargarEstudiantes();
    void cargarPrestamos();
    
    // Métodos para guardar datos en archivos
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
    void actualizarAutor();
    void eliminarAutor();
    
    // CRUD Libros
    void agregarLibro();
    void listarLibros();
    void buscarLibroPorTitulo();
    void actualizarLibro();
    void eliminarLibro();
    
    // CRUD Estudiantes
    void agregarEstudiante();
    void listarEstudiantes();
    void buscarEstudiantePorNombre();
    void actualizarEstudiante();
    void eliminarEstudiante();
    
    // CRUD Prestamos
    void realizarPrestamo();
    void devolverLibro();
    void listarPrestamos();
    void buscarPrestamosPorEstudiante();
    void eliminarPrestamo();
    
    // Menú principal
    void mostrarMenu();
};

#endif