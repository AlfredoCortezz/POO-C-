#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>

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

    bool autorExiste(int id);
    bool libroExiste(int id);
    bool estudianteExiste(int id);
    bool prestamoExiste(int id);
    bool libroDisponible(int id_libro);
    void cargarAutores();
    void cargarLibros();
    void cargarEstudiantes();
    void cargarPrestamos();
    void guardarAutores();
    void guardarLibros();
    void guardarEstudiantes();
    void guardarPrestamos();

public:
    BibliotecaDB();
    
    void agregarAutor();
    void listarAutores();
    void buscarAutorPorId();
    void eliminarAutor();
    
    void agregarLibro();
    void listarLibros();
    void buscarLibroPorTitulo();
    void eliminarLibro();
    
    void agregarEstudiante();
    void listarEstudiantes();
    void buscarEstudiantePorNombre();
    void eliminarEstudiante();
    
    void realizarPrestamo();
    void devolverLibro();
    void listarPrestamos();
    void buscarPrestamosPorEstudiante();
    
    void mostrarMenu();
};

// constructor que carga los datos al iniciar
BibliotecaDB::BibliotecaDB() {
    cargarAutores();
    cargarLibros();
    cargarEstudiantes();
    cargarPrestamos();
    cout << "Sistema de biblioteca iniciado..." << endl;
}

// verifica si un autor existe por su ID
bool BibliotecaDB::autorExiste(int id) {
    for (const auto& autor : autores) {
        if (autor.id == id) return true;
    }
    return false;
}

// verifica si un libro existe por su ID
bool BibliotecaDB::libroExiste(int id) {
    for (const auto& libro : libros) {
        if (libro.id == id) return true;
    }
    return false;
}

// verifica si un estudiante existe por su ID
bool BibliotecaDB::estudianteExiste(int id) {
    for (const auto& estudiante : estudiantes) {
        if (estudiante.id == id) return true;
    }
    return false;
}

// verifica si un prestamo existe por su ID
bool BibliotecaDB::prestamoExiste(int id) {
    for (const auto& prestamo : prestamos) {
        if (prestamo.id == id) return true;
    }
    return false;
}

// verifica si un libro esta disponible (no prestado)
bool BibliotecaDB::libroDisponible(int id_libro) {
    for (const auto& prestamo : prestamos) {
        if (prestamo.id_libro == id_libro && prestamo.activo) {
            return false;
        }
    }
    return true;
}

// carga los autores desde el archivo
void BibliotecaDB::cargarAutores() {
    ifstream archivo("autores.txt");
    if (!archivo.is_open()) {
        cout << "No hay archivo de autores, se creara uno nuevo" << endl;
        return;
    }
    
    string linea;
    int contador = 0;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        
        stringstream ss(linea);
        string token;
        vector<string> tokens;
        
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() == 3) {
            Autor autor;
            autor.id = stoi(tokens[0]);
            autor.nombre = tokens[1];
            autor.nacionalidad = tokens[2];
            autores.push_back(autor);
            contador++;
        }
    }
    archivo.close();
    cout << "Cargados " << contador << " autores" << endl;
}

// carga los libros desde el archivo
void BibliotecaDB::cargarLibros() {
    ifstream archivo("libros.txt");
    if (!archivo.is_open()) {
        cout << "No hay archivo de libros, se creara uno nuevo" << endl;
        return;
    }
    
    string linea;
    int contador = 0;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        
        stringstream ss(linea);
        string token;
        vector<string> tokens;
        
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() == 5) {
            Libro libro;
            libro.id = stoi(tokens[0]);
            libro.titulo = tokens[1];
            libro.isbn = tokens[2];
            libro.ano = stoi(tokens[3]);
            libro.id_autor = stoi(tokens[4]);
            libros.push_back(libro);
            contador++;
        }
    }
    archivo.close();
    cout << "Cargados " << contador << " libros" << endl;
}

// carga los estudiantes desde el archivo - ESTA ES LA PARTE QUE ARREGLE
void BibliotecaDB::cargarEstudiantes() {
    ifstream archivo("estudiantes.txt");
    if (!archivo.is_open()) {
        cout << "No hay archivo de estudiantes, se creara uno nuevo" << endl;
        return;
    }
    
    string linea;
    int contador = 0;
    while (getline(archivo, linea)) {
        if (linea.empty() || linea.find_first_not_of(' ') == string::npos) {
            continue;
        }
        
        stringstream ss(linea);
        string token;
        vector<string> tokens;
        
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() == 3) {
            try {
                Estudiante estudiante;
                estudiante.id = stoi(tokens[0]);
                estudiante.nombre = tokens[1];
                estudiante.grado = stoi(tokens[2]);
                estudiantes.push_back(estudiante);
                contador++;
            } catch (const exception& e) {
                cout << "Error en linea de estudiantes: " << linea << endl;
            }
        }
    }
    archivo.close();
    cout << "Cargados " << contador << " estudiantes" << endl;
}

// carga los prestamos desde el archivo
void BibliotecaDB::cargarPrestamos() {
    ifstream archivo("prestamos.txt");
    if (!archivo.is_open()) {
        cout << "No hay archivo de prestamos, se creara uno nuevo" << endl;
        return;
    }
    
    string linea;
    int contador = 0;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        
        stringstream ss(linea);
        string token;
        vector<string> tokens;
        
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() == 6) {
            Prestamo prestamo;
            prestamo.id = stoi(tokens[0]);
            prestamo.fecha_prestamo = tokens[1];
            prestamo.fecha_devolucion = tokens[2];
            prestamo.id_libro = stoi(tokens[3]);
            prestamo.id_estudiante = stoi(tokens[4]);
            prestamo.activo = (tokens[5] == "1");
            prestamos.push_back(prestamo);
            contador++;
        }
    }
    archivo.close();
    cout << "Cargados " << contador << " prestamos" << endl;
}

// guarda los autores en el archivo
void BibliotecaDB::guardarAutores() {
    ofstream archivo("autores.txt");
    for (const auto& autor : autores) {
        archivo << autor.id << "," << autor.nombre << "," << autor.nacionalidad << endl;
    }
    archivo.close();
}

// guarda los libros en el archivo
void BibliotecaDB::guardarLibros() {
    ofstream archivo("libros.txt");
    for (const auto& libro : libros) {
        archivo << libro.id << "," << libro.titulo << "," << libro.isbn << "," 
                << libro.ano << "," << libro.id_autor << endl;
    }
    archivo.close();
}

// guarda los estudiantes en el archivo
void BibliotecaDB::guardarEstudiantes() {
    ofstream archivo("estudiantes.txt");
    for (const auto& estudiante : estudiantes) {
        archivo << estudiante.id << "," << estudiante.nombre << "," << estudiante.grado << endl;
    }
    archivo.close();
}

// guarda los prestamos en el archivo
void BibliotecaDB::guardarPrestamos() {
    ofstream archivo("prestamos.txt");
    for (const auto& prestamo : prestamos) {
        archivo << prestamo.id << "," << prestamo.fecha_prestamo << "," << prestamo.fecha_devolucion << ","
                << prestamo.id_libro << "," << prestamo.id_estudiante << "," << (prestamo.activo ? "1" : "0") << endl;
    }
    archivo.close();
}

// agrega un nuevo autor
void BibliotecaDB::agregarAutor() {
    Autor autor;
    cout << "\n=== AGREGAR AUTOR ===" << endl;
    
    cout << "ID: ";
    cin >> autor.id;
    
    if (autorExiste(autor.id)) {
        cout << "Error: Ya existe un autor con ese ID." << endl;
        return;
    }
    
    cin.ignore();
    cout << "Nombre: ";
    getline(cin, autor.nombre);
    
    cout << "Nacionalidad: ";
    getline(cin, autor.nacionalidad);
    
    autores.push_back(autor);
    guardarAutores();
    cout << "Autor agregado correctamente" << endl;
}

// muestra todos los autores
void BibliotecaDB::listarAutores() {
    cout << "\n=== LISTA DE AUTORES ===" << endl;
    if (autores.empty()) {
        cout << "No hay autores registrados" << endl;
        return;
    }
    
    for (const auto& autor : autores) {
        cout << "ID: " << autor.id << " | Nombre: " << autor.nombre 
             << " | Nacionalidad: " << autor.nacionalidad << endl;
    }
}

// busca un autor por su ID
void BibliotecaDB::buscarAutorPorId() {
    int id;
    cout << "\n=== BUSCAR AUTOR POR ID ===" << endl;
    cout << "ID del autor: ";
    cin >> id;
    
    for (const auto& autor : autores) {
        if (autor.id == id) {
            cout << "Autor encontrado:" << endl;
            cout << "ID: " << autor.id << " | Nombre: " << autor.nombre 
                 << " | Nacionalidad: " << autor.nacionalidad << endl;
            return;
        }
    }
    cout << "No se encontro autor con ese ID" << endl;
}

// elimina un autor por su ID
void BibliotecaDB::eliminarAutor() {
    int id;
    cout << "\n=== ELIMINAR AUTOR ===" << endl;
    cout << "ID del autor a eliminar: ";
    cin >> id;
    
    for (auto it = autores.begin(); it != autores.end(); ++it) {
        if (it->id == id) {
            autores.erase(it);
            guardarAutores();
            cout << "Autor eliminado" << endl;
            return;
        }
    }
    cout << "No se encontro autor con ese ID" << endl;
}

// agrega un nuevo libro
void BibliotecaDB::agregarLibro() {
    Libro libro;
    cout << "\n=== AGREGAR LIBRO ===" << endl;
    
    cout << "ID: ";
    cin >> libro.id;
    
    if (libroExiste(libro.id)) {
        cout << "Error: Ya existe un libro con ese ID" << endl;
        return;
    }
    
    cin.ignore();
    cout << "Titulo: ";
    getline(cin, libro.titulo);
    
    cout << "ISBN: ";
    getline(cin, libro.isbn);
    
    cout << "Año: ";
    cin >> libro.ano;
    
    cout << "ID Autor: ";
    cin >> libro.id_autor;
    
    if (!autorExiste(libro.id_autor)) {
        cout << "Error: No existe autor con ese ID" << endl;
        return;
    }
    
    libros.push_back(libro);
    guardarLibros();
    cout << "Libro agregado correctamente" << endl;
}

// muestra todos los libros
void BibliotecaDB::listarLibros() {
    cout << "\n=== LISTA DE LIBROS ===" << endl;
    if (libros.empty()) {
        cout << "No hay libros registrados" << endl;
        return;
    }
    
    for (const auto& libro : libros) {
        string nombreAutor = "No encontrado";
        for (const auto& autor : autores) {
            if (autor.id == libro.id_autor) {
                nombreAutor = autor.nombre;
                break;
            }
        }
        
        string disponible = libroDisponible(libro.id) ? "Si" : "No";
        
        cout << "ID: " << libro.id << " | Titulo: " << libro.titulo 
             << " | ISBN: " << libro.isbn << " | Año: " << libro.ano 
             << " | Autor: " << nombreAutor << " | Disponible: " << disponible << endl;
    }
}

// busca libros por titulo
void BibliotecaDB::buscarLibroPorTitulo() {
    string titulo;
    cout << "\n=== BUSCAR LIBRO POR TITULO ===" << endl;
    cin.ignore();
    cout << "Titulo a buscar: ";
    getline(cin, titulo);
    
    bool encontrado = false;
    for (const auto& libro : libros) {
        if (libro.titulo.find(titulo) != string::npos) {
            if (!encontrado) {
                cout << "Libros encontrados:" << endl;
                encontrado = true;
            }
            
            string nombreAutor = "No encontrado";
            for (const auto& autor : autores) {
                if (autor.id == libro.id_autor) {
                    nombreAutor = autor.nombre;
                    break;
                }
            }
            
            string disponible = libroDisponible(libro.id) ? "Si" : "No";
            
            cout << "ID: " << libro.id << " | Titulo: " << libro.titulo 
                 << " | Autor: " << nombreAutor << " | Disponible: " << disponible << endl;
        }
    }
    
    if (!encontrado) {
        cout << "No hay libros con ese titulo" << endl;
    }
}

// elimina un libro por su ID
void BibliotecaDB::eliminarLibro() {
    int id;
    cout << "\n=== ELIMINAR LIBRO ===" << endl;
    cout << "ID del libro a eliminar: ";
    cin >> id;
    
    for (auto it = libros.begin(); it != libros.end(); ++it) {
        if (it->id == id) {
            libros.erase(it);
            guardarLibros();
            cout << "Libro eliminado" << endl;
            return;
        }
    }
    cout << "No se encontro libro con ese ID" << endl;
}

// agrega un nuevo estudiante
void BibliotecaDB::agregarEstudiante() {
    Estudiante estudiante;
    cout << "\n=== AGREGAR ESTUDIANTE ===" << endl;
    
    cout << "ID: ";
    cin >> estudiante.id;
    
    if (estudianteExiste(estudiante.id)) {
        cout << "Error: Ya existe un estudiante con ese ID" << endl;
        return;
    }
    
    cin.ignore();
    cout << "Nombre: ";
    getline(cin, estudiante.nombre);
    
    cout << "Grado: ";
    cin >> estudiante.grado;
    
    estudiantes.push_back(estudiante);
    guardarEstudiantes();
    cout << "Estudiante agregado correctamente" << endl;
}

// muestra todos los estudiantes - ESTO YA DEBERIA FUNCIONAR BIEN
void BibliotecaDB::listarEstudiantes() {
    cout << "\n=== LISTA DE ESTUDIANTES ===" << endl;
    if (estudiantes.empty()) {
        cout << "No hay estudiantes registrados" << endl;
        return;
    }
    
    cout << "Total de estudiantes: " << estudiantes.size() << endl;
    
    for (const auto& estudiante : estudiantes) {
        cout << "ID: " << estudiante.id << " | Nombre: " << estudiante.nombre 
             << " | Grado: " << estudiante.grado << endl;
    }
}

// busca estudiantes por nombre
void BibliotecaDB::buscarEstudiantePorNombre() {
    string nombre;
    cout << "\n=== BUSCAR ESTUDIANTE POR NOMBRE ===" << endl;
    cin.ignore();
    cout << "Nombre a buscar: ";
    getline(cin, nombre);
    
    bool encontrado = false;
    for (const auto& estudiante : estudiantes) {
        if (estudiante.nombre.find(nombre) != string::npos) {
            if (!encontrado) {
                cout << "Estudiantes encontrados:" << endl;
                encontrado = true;
            }
            cout << "ID: " << estudiante.id << " | Nombre: " << estudiante.nombre 
                 << " | Grado: " << estudiante.grado << endl;
        }
    }
    
    if (!encontrado) {
        cout << "No hay estudiantes con ese nombre" << endl;
    }
}

// elimina un estudiante por su ID
void BibliotecaDB::eliminarEstudiante() {
    int id;
    cout << "\n=== ELIMINAR ESTUDIANTE ===" << endl;
    cout << "ID del estudiante a eliminar: ";
    cin >> id;
    
    for (auto it = estudiantes.begin(); it != estudiantes.end(); ++it) {
        if (it->id == id) {
            estudiantes.erase(it);
            guardarEstudiantes();
            cout << "Estudiante eliminado" << endl;
            return;
        }
    }
    cout << "No se encontro estudiante con ese ID" << endl;
}

// realiza un nuevo prestamo
void BibliotecaDB::realizarPrestamo() {
    Prestamo prestamo;
    cout << "\n=== REALIZAR PRESTAMO ===" << endl;
    
    cout << "ID Prestamo: ";
    cin >> prestamo.id;
    
    if (prestamoExiste(prestamo.id)) {
        cout << "Error: Ya existe un prestamo con ese ID" << endl;
        return;
    }
    
    cout << "ID Libro: ";
    cin >> prestamo.id_libro;
    
    if (!libroExiste(prestamo.id_libro)) {
        cout << "Error: No existe libro con ese ID" << endl;
        return;
    }
    
    if (!libroDisponible(prestamo.id_libro)) {
        cout << "Error: El libro no esta disponible" << endl;
        return;
    }
    
    cout << "ID Estudiante: ";
    cin >> prestamo.id_estudiante;
    
    if (!estudianteExiste(prestamo.id_estudiante)) {
        cout << "Error: No existe estudiante con ese ID" << endl;
        return;
    }
    
    cin.ignore();
    cout << "Fecha prestamo (YYYY-MM-DD): ";
    getline(cin, prestamo.fecha_prestamo);
    
    cout << "Fecha devolucion (YYYY-MM-DD): ";
    getline(cin, prestamo.fecha_devolucion);
    
    prestamo.activo = true;
    prestamos.push_back(prestamo);
    guardarPrestamos();
    cout << "Prestamo realizado correctamente" << endl;
}

// devuelve un libro prestado
void BibliotecaDB::devolverLibro() {
    int id_prestamo;
    cout << "\n=== DEVOLVER LIBRO ===" << endl;
    cout << "ID del prestamo: ";
    cin >> id_prestamo;
    
    for (auto& prestamo : prestamos) {
        if (prestamo.id == id_prestamo && prestamo.activo) {
            prestamo.activo = false;
            guardarPrestamos();
            cout << "Libro devuelto correctamente" << endl;
            return;
        }
    }
    cout << "Error: No hay prestamo activo con ese ID" << endl;
}

// muestra todos los prestamos
void BibliotecaDB::listarPrestamos() {
    cout << "\n=== LISTA DE PRESTAMOS ===" << endl;
    if (prestamos.empty()) {
        cout << "No hay prestamos registrados" << endl;
        return;
    }
    
    for (const auto& prestamo : prestamos) {
        string tituloLibro = "No encontrado";
        for (const auto& libro : libros) {
            if (libro.id == prestamo.id_libro) {
                tituloLibro = libro.titulo;
                break;
            }
        }
        
        string nombreEstudiante = "No encontrado";
        for (const auto& estudiante : estudiantes) {
            if (estudiante.id == prestamo.id_estudiante) {
                nombreEstudiante = estudiante.nombre;
                break;
            }
        }
        
        cout << "ID: " << prestamo.id << " | Libro: " << tituloLibro 
             << " | Estudiante: " << nombreEstudiante 
             << " | Fecha prestamo: " << prestamo.fecha_prestamo 
             << " | Fecha devolucion: " << prestamo.fecha_devolucion 
             << " | Activo: " << (prestamo.activo ? "Si" : "No") << endl;
    }
}

// busca prestamos por estudiante
void BibliotecaDB::buscarPrestamosPorEstudiante() {
    int id_estudiante;
    cout << "\n=== BUSCAR PRESTAMOS POR ESTUDIANTE ===" << endl;
    cout << "ID del estudiante: ";
    cin >> id_estudiante;
    
    if (!estudianteExiste(id_estudiante)) {
        cout << "Error: No existe estudiante con ese ID" << endl;
        return;
    }
    
    bool encontrado = false;
    for (const auto& prestamo : prestamos) {
        if (prestamo.id_estudiante == id_estudiante) {
            if (!encontrado) {
                cout << "Prestamos del estudiante:" << endl;
                encontrado = true;
            }
            
            string tituloLibro = "No encontrado";
            for (const auto& libro : libros) {
                if (libro.id == prestamo.id_libro) {
                    tituloLibro = libro.titulo;
                    break;
                }
            }
            
            cout << "ID Prestamo: " << prestamo.id << " | Libro: " << tituloLibro 
                 << " | Fecha prestamo: " << prestamo.fecha_prestamo 
                 << " | Fecha devolucion: " << prestamo.fecha_devolucion 
                 << " | Activo: " << (prestamo.activo ? "Si" : "No") << endl;
        }
    }
    
    if (!encontrado) {
        cout << "No hay prestamos para este estudiante" << endl;
    }
}

// muestra el menu principal
void BibliotecaDB::mostrarMenu() {
    int opcion;
    
    while (true) {
        cout << "\n=== SISTEMA DE BIBLIOTECA ===" << endl;
        cout << "1. Gestion de Autores" << endl;
        cout << "2. Gestion de Libros" << endl;
        cout << "3. Gestion de Estudiantes" << endl;
        cout << "4. Gestion de Prestamos" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        
        switch (opcion) {
            case 1: {
                int opcionAutor;
                cout << "\n=== GESTION DE AUTORES ===" << endl;
                cout << "1. Agregar Autor" << endl;
                cout << "2. Listar Autores" << endl;
                cout << "3. Buscar Autor por ID" << endl;
                cout << "4. Eliminar Autor" << endl;
                cout << "0. Volver" << endl;
                cout << "Seleccione: ";
                cin >> opcionAutor;
                
                switch (opcionAutor) {
                    case 1: agregarAutor(); break;
                    case 2: listarAutores(); break;
                    case 3: buscarAutorPorId(); break;
                    case 4: eliminarAutor(); break;
                    case 0: break;
                    default: cout << "Opcion invalida" << endl;
                }
                break;
            }
            case 2: {
                int opcionLibro;
                cout << "\n=== GESTION DE LIBROS ===" << endl;
                cout << "1. Agregar Libro" << endl;
                cout << "2. Listar Libros" << endl;
                cout << "3. Buscar Libro por Titulo" << endl;
                cout << "4. Eliminar Libro" << endl;
                cout << "0. Volver" << endl;
                cout << "Seleccione: ";
                cin >> opcionLibro;
                
                switch (opcionLibro) {
                    case 1: agregarLibro(); break;
                    case 2: listarLibros(); break;
                    case 3: buscarLibroPorTitulo(); break;
                    case 4: eliminarLibro(); break;
                    case 0: break;
                    default: cout << "Opcion invalida" << endl;
                }
                break;
            }
            case 3: {
                int opcionEstudiante;
                cout << "\n=== GESTION DE ESTUDIANTES ===" << endl;
                cout << "1. Agregar Estudiante" << endl;
                cout << "2. Listar Estudiantes" << endl;
                cout << "3. Buscar Estudiante por Nombre" << endl;
                cout << "4. Eliminar Estudiante" << endl;
                cout << "0. Volver" << endl;
                cout << "Seleccione: ";
                cin >> opcionEstudiante;
                
                switch (opcionEstudiante) {
                    case 1: agregarEstudiante(); break;
                    case 2: listarEstudiantes(); break;
                    case 3: buscarEstudiantePorNombre(); break;
                    case 4: eliminarEstudiante(); break;
                    case 0: break;
                    default: cout << "Opcion invalida" << endl;
                }
                break;
            }
            case 4: {
                int opcionPrestamo;
                cout << "\n=== GESTION DE PRESTAMOS ===" << endl;
                cout << "1. Realizar Prestamo" << endl;
                cout << "2. Devolver Libro" << endl;
                cout << "3. Listar Todos los Prestamos" << endl;
                cout << "4. Buscar Prestamos por Estudiante" << endl;
                cout << "0. Volver" << endl;
                cout << "Seleccione: ";
                cin >> opcionPrestamo;
                
                switch (opcionPrestamo) {
                    case 1: realizarPrestamo(); break;
                    case 2: devolverLibro(); break;
                    case 3: listarPrestamos(); break;
                    case 4: buscarPrestamosPorEstudiante(); break;
                    case 0: break;
                    default: cout << "Opcion invalida" << endl;
                }
                break;
            }
            case 0:
                cout << "Saliendo del sistema... Hasta luego!" << endl;
                return;
            default:
                cout << "Opcion invalida, intenta de nuevo" << endl;
        }
    }
}

// MAIN FUNCTION - todo en un solo archivo
int main() {
    BibliotecaDB biblioteca;
    biblioteca.mostrarMenu();
    return 0;
}