#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <regex>

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
    int anio;
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

    // Verifica si un autor existe por su ID
    bool autorExiste(int id) {
        for (const auto& autor : autores) {
            if (autor.id == id) return true;
        }
        return false;
    }

    // Verifica si un libro existe por su ID
    bool libroExiste(int id) {
        for (const auto& libro : libros) {
            if (libro.id == id) return true;
        }
        return false;
    }

    // Verifica si un estudiante existe por su ID
    bool estudianteExiste(int id) {
        for (const auto& estudiante : estudiantes) {
            if (estudiante.id == id) return true;
        }
        return false;
    }

    // Verifica si un prestamo existe por su ID
    bool prestamoExiste(int id) {
        for (const auto& prestamo : prestamos) {
            if (prestamo.id == id) return true;
        }
        return false;
    }

    // Verifica si un libro esta disponible (no prestado)
    bool libroDisponible(int id_libro) {
        for (const auto& prestamo : prestamos) {
            if (prestamo.id_libro == id_libro && prestamo.activo) {
                return false;
            }
        }
        return true;
    }

    // Valida el formato de fecha (YYYY-MM-DD)
    bool validarFecha(const string& fecha) {
        regex fechaRegex("\\d{4}-\\d{2}-\\d{2}");
        if (!regex_match(fecha, fechaRegex)) return false;
        // Validacion adicional para anio, mes y dia
        try {
            int anio = stoi(fecha.substr(0, 4));
            int mes = stoi(fecha.substr(5, 2));
            int dia = stoi(fecha.substr(8, 2));
            if (anio < 1900 || anio > 2025 || mes < 1 || mes > 12 || dia < 1 || dia > 31) {
                return false;
            }
            return true;
        } catch (const exception& e) {
            return false;
        }
    }

    // Crea archivos predeterminados si no existen
    void crearArchivosPredeterminados() {
        // Verificar y crear autores.txt si no existe
        ifstream archivoAutores("autores.txt");
        if (!archivoAutores.is_open()) {
            ofstream nuevoArchivo("autores.txt");
            nuevoArchivo << "1,Gabriel Garcia Marquez,Colombiana\n"
                        << "2,Isabel Allende,Chilena\n"
                        << "3,Mario Vargas Llosa,Peruana\n"
                        << "4,Julio Cortazar,Argentina\n"
                        << "5,Jorge Luis Borges,Argentina\n"
                        << "6,Pablo Neruda,Chilena\n"
                        << "7,Octavio Paz,Mexicana\n"
                        << "8,Carlos Fuentes,Mexicana\n";
            nuevoArchivo.close();
            cout << "Archivo autores.txt creado con datos predeterminados" << endl;
        }
        archivoAutores.close();

        // Verificar y crear libros.txt si no existe
        ifstream archivoLibros("libros.txt");
        if (!archivoLibros.is_open()) {
            ofstream nuevoArchivo("libros.txt");
            nuevoArchivo << "1,Cien Anios de Soledad,978-8437604947,1967,1\n"
                        << "2,El Amor en los Tiempos del Colera,978-0307474728,1985,1\n"
                        << "3,La Casa de los Espiritus,978-8403093806,1982,2\n"
                        << "4,La Ciudad y los Perros,978-8420471836,1963,3\n"
                        << "5,Rayuela,978-8432216420,1963,4\n"
                        << "6,Ficciones,978-8420674701,1944,5\n"
                        << "7,Veinte Poemas de Amor,978-8432216536,1924,6\n"
                        << "8,El Laberinto de la Soledad,978-6071601753,1950,7\n"
                        << "9,La Region Mas Transparente,978-6073119060,1958,8\n"
                        << "10,Cronica de una Muerte Anunciada,978-0307474599,1981,1\n";
            nuevoArchivo.close();
            cout << "Archivo libros.txt creado con datos predeterminados" << endl;
        }
        archivoLibros.close();

        // Verificar y crear estudiantes.txt si no existe
        ifstream archivoEstudiantes("estudiantes.txt");
        if (!archivoEstudiantes.is_open()) {
            ofstream nuevoArchivo("estudiantes.txt");
            nuevoArchivo << "101,Maria Gonzalez,10\n"
                        << "102,Carlos Rodriguez,11\n"
                        << "103,Ana Martinez,9\n"
                        << "104,Luis Hernandez,12\n"
                        << "105,Sofia Lopez,10\n"
                        << "106,Diego Perez,11\n"
                        << "107,Elena Ramirez,9\n"
                        << "108,Javier Torres,12\n";
            nuevoArchivo.close();
            cout << "Archivo estudiantes.txt creado con datos predeterminados" << endl;
        }
        archivoEstudiantes.close();

        // Verificar y crear prestamos.txt si no existe
        ifstream archivoPrestamos("prestamos.txt");
        if (!archivoPrestamos.is_open()) {
            ofstream nuevoArchivo("prestamos.txt");
            nuevoArchivo << "1001,2024-03-15,2024-03-25,1,101,0\n"
                        << "1002,2024-03-18,2024-03-28,3,102,1\n"
                        << "1003,2024-03-20,2024-03-30,5,103,1\n"
                        << "1004,2024-03-10,2024-03-20,2,104,0\n"
                        << "1005,2024-03-22,2024-04-01,7,105,1\n";
            nuevoArchivo.close();
            cout << "Archivo prestamos.txt creado con datos predeterminados" << endl;
        }
        archivoPrestamos.close();
    }

    // Carga los autores desde el archivo autores.txt
    void cargarAutores() {
        ifstream archivo("autores.txt");
        if (!archivo.is_open()) {
            cout << "Error: No se pudo abrir el archivo autores.txt" << endl;
            return;
        }
        
        string linea;
        int contador = 0;
        while (getline(archivo, linea)) {
            if (linea.empty() || linea.find_first_not_of(' ') == string::npos) continue;
            
            stringstream ss(linea);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            
            if (tokens.size() == 3) {
                try {
                    Autor autor;
                    autor.id = stoi(tokens[0]);
                    autor.nombre = tokens[1];
                    autor.nacionalidad = tokens[2];
                    autores.push_back(autor);
                    contador++;
                } catch (const exception& e) {
                    cout << "Error en linea de autores: " << linea << endl;
                }
            }
        }
        archivo.close();
        cout << "Cargados " << contador << " autores" << endl;
    }

    // Carga los libros desde el archivo libros.txt
    void cargarLibros() {
        ifstream archivo("libros.txt");
        if (!archivo.is_open()) {
            cout << "Error: No se pudo abrir el archivo libros.txt" << endl;
            return;
        }
        
        string linea;
        int contador = 0;
        while (getline(archivo, linea)) {
            if (linea.empty() || linea.find_first_not_of(' ') == string::npos) continue;
            
            stringstream ss(linea);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            
            if (tokens.size() == 5) {
                try {
                    Libro libro;
                    libro.id = stoi(tokens[0]);
                    libro.titulo = tokens[1];
                    libro.isbn = tokens[2];
                    libro.anio = stoi(tokens[3]);
                    libro.id_autor = stoi(tokens[4]);
                    libros.push_back(libro);
                    contador++;
                } catch (const exception& e) {
                    cout << "Error en linea de libros: " << linea << endl;
                }
            }
        }
        archivo.close();
        cout << "Cargados " << contador << " libros" << endl;
    }

    // Carga los estudiantes desde el archivo estudiantes.txt
    void cargarEstudiantes() {
        ifstream archivo("estudiantes.txt");
        if (!archivo.is_open()) {
            cout << "Error: No se pudo abrir el archivo estudiantes.txt" << endl;
            return;
        }
        
        string linea;
        int contador = 0;
        while (getline(archivo, linea)) {
            if (linea.empty() || linea.find_first_not_of(' ') == string::npos) continue;
            
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

    // Carga los prestamos desde el archivo prestamos.txt
    void cargarPrestamos() {
        ifstream archivo("prestamos.txt");
        if (!archivo.is_open()) {
            cout << "Error: No se pudo abrir el archivo prestamos.txt" << endl;
            return;
        }
        
        string linea;
        int contador = 0;
        while (getline(archivo, linea)) {
            if (linea.empty() || linea.find_first_not_of(' ') == string::npos) continue;
            
            stringstream ss(linea);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            
            if (tokens.size() == 6) {
                try {
                    Prestamo prestamo;
                    prestamo.id = stoi(tokens[0]);
                    prestamo.fecha_prestamo = tokens[1];
                    prestamo.fecha_devolucion = tokens[2];
                    prestamo.id_libro = stoi(tokens[3]);
                    prestamo.id_estudiante = stoi(tokens[4]);
                    prestamo.activo = (tokens[5] == "1");
                    prestamos.push_back(prestamo);
                    contador++;
                } catch (const exception& e) {
                    cout << "Error en linea de prestamos: " << linea << endl;
                }
            }
        }
        archivo.close();
        cout << "Cargados " << contador << " prestamos" << endl;
    }

    // Guarda los autores en el archivo autores.txt
    void guardarAutores() {
        ofstream archivo("autores.txt");
        if (!archivo.is_open()) {
            cout << "Error: No se pudo guardar autores.txt" << endl;
            return;
        }
        for (const auto& autor : autores) {
            archivo << autor.id << "," << autor.nombre << "," << autor.nacionalidad << endl;
        }
        archivo.close();
    }

    // Guarda los libros en el archivo libros.txt
    void guardarLibros() {
        ofstream archivo("libros.txt");
        if (!archivo.is_open()) {
            cout << "Error: No se pudo guardar libros.txt" << endl;
            return;
        }
        for (const auto& libro : libros) {
            archivo << libro.id << "," << libro.titulo << "," << libro.isbn << ","
                    << libro.anio << "," << libro.id_autor << endl;
        }
        archivo.close();
    }

    // Guarda los estudiantes en el archivo estudiantes.txt
    void guardarEstudiantes() {
        ofstream archivo("estudiantes.txt");
        if (!archivo.is_open()) {
            cout << "Error: No se pudo guardar estudiantes.txt" << endl;
            return;
        }
        for (const auto& estudiante : estudiantes) {
            archivo << estudiante.id << "," << estudiante.nombre << "," << estudiante.grado << endl;
        }
        archivo.close();
    }

    // Guarda los prestamos en el archivo prestamos.txt
    void guardarPrestamos() {
        ofstream archivo("prestamos.txt");
        if (!archivo.is_open()) {
            cout << "Error: No se pudo guardar prestamos.txt" << endl;
            return;
        }
        for (const auto& prestamo : prestamos) {
            archivo << prestamo.id << "," << prestamo.fecha_prestamo << "," << prestamo.fecha_devolucion << ","
                    << prestamo.id_libro << "," << prestamo.id_estudiante << "," << (prestamo.activo ? "1" : "0") << endl;
        }
        archivo.close();
    }

public:
    // Constructor que inicializa la base de datos cargando los datos
    BibliotecaDB() {
        crearArchivosPredeterminados();
        cargarAutores();
        cargarLibros();
        cargarEstudiantes();
        cargarPrestamos();
        cout << "Sistema de biblioteca iniciado correctamente" << endl;
    }
    
    // Agrega un nuevo autor al sistema
    void agregarAutor() {
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

    // Lista todos los autores registrados
    void listarAutores() {
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

    // Busca un autor por su ID
    void buscarAutorPorId() {
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

    // Actualiza los datos de un autor existente
    void actualizarAutor() {
        int id;
        cout << "\n=== ACTUALIZAR AUTOR ===" << endl;
        cout << "ID del autor: ";
        cin >> id;
        
        for (auto& autor : autores) {
            if (autor.id == id) {
                cin.ignore();
                cout << "Nuevo nombre (anterior: " << autor.nombre << "): ";
                getline(cin, autor.nombre);
                cout << "Nueva nacionalidad (anterior: " << autor.nacionalidad << "): ";
                getline(cin, autor.nacionalidad);
                guardarAutores();
                cout << "Autor actualizado correctamente" << endl;
                return;
            }
        }
        cout << "No se encontro autor con ese ID" << endl;
    }

    // Elimina un autor por su ID
    void eliminarAutor() {
        int id;
        cout << "\n=== ELIMINAR AUTOR ===" << endl;
        cout << "ID del autor a eliminar: ";
        cin >> id;
        
        for (auto it = autores.begin(); it != autores.end(); ++it) {
            if (it->id == id) {
                // Verificar si el autor esta asociado a algun libro
                for (const auto& libro : libros) {
                    if (libro.id_autor == id) {
                        cout << "Error: El autor esta asociado a uno o mas libros" << endl;
                        return;
                    }
                }
                autores.erase(it);
                guardarAutores();
                cout << "Autor eliminado" << endl;
                return;
            }
        }
        cout << "No se encontro autor con ese ID" << endl;
    }

    // Agrega un nuevo libro al sistema
    void agregarLibro() {
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
        
        cout << "Anio: ";
        cin >> libro.anio;
        
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

    // Lista todos los libros registrados
    void listarLibros() {
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
                 << " | ISBN: " << libro.isbn << " | Anio: " << libro.anio 
                 << " | Autor: " << nombreAutor << " | Disponible: " << disponible << endl;
        }
    }

    // Busca libros por titulo (busqueda parcial)
    void buscarLibroPorTitulo() {
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

    // Actualiza los datos de un libro existente
    void actualizarLibro() {
        int id;
        cout << "\n=== ACTUALIZAR LIBRO ===" << endl;
        cout << "ID del libro: ";
        cin >> id;
        
        for (auto& libro : libros) {
            if (libro.id == id) {
                cin.ignore();
                cout << "Nuevo titulo (anterior: " << libro.titulo << "): ";
                getline(cin, libro.titulo);
                cout << "Nuevo ISBN (anterior: " << libro.isbn << "): ";
                getline(cin, libro.isbn);
                cout << "Nuevo anio (anterior: " << libro.anio << "): ";
                cin >> libro.anio;
                cout << "Nuevo ID autor (anterior: " << libro.id_autor << "): ";
                cin >> libro.id_autor;
                
                if (!autorExiste(libro.id_autor)) {
                    cout << "Error: No existe autor con ese ID" << endl;
                    return;
                }
                
                guardarLibros();
                cout << "Libro actualizado correctamente" << endl;
                return;
            }
        }
        cout << "No se encontro libro con ese ID" << endl;
    }

    // Elimina un libro por su ID
    void eliminarLibro() {
        int id;
        cout << "\n=== ELIMINAR LIBRO ===" << endl;
        cout << "ID del libro a eliminar: ";
        cin >> id;
        
        for (auto it = libros.begin(); it != libros.end(); ++it) {
            if (it->id == id) {
                // Verificar si el libro esta asociado a algun prestamo activo
                for (const auto& prestamo : prestamos) {
                    if (prestamo.id_libro == id && prestamo.activo) {
                        cout << "Error: El libro esta actualmente prestado" << endl;
                        return;
                    }
                }
                libros.erase(it);
                guardarLibros();
                cout << "Libro eliminado" << endl;
                return;
            }
        }
        cout << "No se encontro libro con ese ID" << endl;
    }

    // Agrega un nuevo estudiante al sistema
    void agregarEstudiante() {
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

    // Lista todos los estudiantes registrados
    void listarEstudiantes() {
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

    // Busca estudiantes por nombre (busqueda parcial)
    void buscarEstudiantePorNombre() {
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

    // Actualiza los datos de un estudiante existente
    void actualizarEstudiante() {
        int id;
        cout << "\n=== ACTUALIZAR ESTUDIANTE ===" << endl;
        cout << "ID del estudiante: ";
        cin >> id;
        
        for (auto& estudiante : estudiantes) {
            if (estudiante.id == id) {
                cin.ignore();
                cout << "Nuevo nombre (anterior: " << estudiante.nombre << "): ";
                getline(cin, estudiante.nombre);
                cout << "Nuevo grado (anterior: " << estudiante.grado << "): ";
                cin >> estudiante.grado;
                guardarEstudiantes();
                cout << "Estudiante actualizado correctamente" << endl;
                return;
            }
        }
        cout << "No se encontro estudiante con ese ID" << endl;
    }

    // Elimina un estudiante por su ID
    void eliminarEstudiante() {
        int id;
        cout << "\n=== ELIMINAR ESTUDIANTE ===" << endl;
        cout << "ID del estudiante a eliminar: ";
        cin >> id;
        
        for (auto it = estudiantes.begin(); it != estudiantes.end(); ++it) {
            if (it->id == id) {
                // Verificar si el estudiante tiene prestamos activos
                for (const auto& prestamo : prestamos) {
                    if (prestamo.id_estudiante == id && prestamo.activo) {
                        cout << "Error: El estudiante tiene prestamos activos" << endl;
                        return;
                    }
                }
                estudiantes.erase(it);
                guardarEstudiantes();
                cout << "Estudiante eliminado" << endl;
                return;
            }
        }
        cout << "No se encontro estudiante con ese ID" << endl;
    }

    // Realiza un nuevo prestamo
    void realizarPrestamo() {
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
        if (!validarFecha(prestamo.fecha_prestamo)) {
            cout << "Error: Formato de fecha invalido (use YYYY-MM-DD)" << endl;
            return;
        }
        
        cout << "Fecha devolucion (YYYY-MM-DD): ";
        getline(cin, prestamo.fecha_devolucion);
        if (!validarFecha(prestamo.fecha_devolucion)) {
            cout << "Error: Formato de fecha invalido (use YYYY-MM-DD)" << endl;
            return;
        }
        
        prestamo.activo = true;
        prestamos.push_back(prestamo);
        guardarPrestamos();
        cout << "Prestamo realizado correctamente" << endl;
    }

    // Devuelve un libro prestado
    void devolverLibro() {
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

    // Lista todos los prestamos registrados
    void listarPrestamos() {
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

    // Busca prestamos por ID de estudiante
    void buscarPrestamosPorEstudiante() {
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

    // Elimina un prestamo por su ID
    void eliminarPrestamo() {
        int id;
        cout << "\n=== ELIMINAR PRESTAMO ===" << endl;
        cout << "ID del prestamo a eliminar: ";
        cin >> id;
        
        for (auto it = prestamos.begin(); it != prestamos.end(); ++it) {
            if (it->id == id) {
                prestamos.erase(it);
                guardarPrestamos();
                cout << "Prestamo eliminado" << endl;
                return;
            }
        }
        cout << "No se encontro prestamo con ese ID" << endl;
    }

    // Muestra el menu principal del sistema
    void mostrarMenu() {
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
                    cout << "4. Actualizar Autor" << endl;
                    cout << "5. Eliminar Autor" << endl;
                    cout << "0. Volver" << endl;
                    cout << "Seleccione: ";
                    cin >> opcionAutor;
                    
                    switch (opcionAutor) {
                        case 1: agregarAutor(); break;
                        case 2: listarAutores(); break;
                        case 3: buscarAutorPorId(); break;
                        case 4: actualizarAutor(); break;
                        case 5: eliminarAutor(); break;
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
                    cout << "4. Actualizar Libro" << endl;
                    cout << "5. Eliminar Libro" << endl;
                    cout << "0. Volver" << endl;
                    cout << "Seleccione: ";
                    cin >> opcionLibro;
                    
                    switch (opcionLibro) {
                        case 1: agregarLibro(); break;
                        case 2: listarLibros(); break;
                        case 3: buscarLibroPorTitulo(); break;
                        case 4: actualizarLibro(); break;
                        case 5: eliminarLibro(); break;
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
                    cout << "4. Actualizar Estudiante" << endl;
                    cout << "5. Eliminar Estudiante" << endl;
                    cout << "0. Volver" << endl;
                    cout << "Seleccione: ";
                    cin >> opcionEstudiante;
                    
                    switch (opcionEstudiante) {
                        case 1: agregarEstudiante(); break;
                        case 2: listarEstudiantes(); break;
                        case 3: buscarEstudiantePorNombre(); break;
                        case 4: actualizarEstudiante(); break;
                        case 5: eliminarEstudiante(); break;
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
                    cout << "5. Eliminar Prestamo" << endl;
                    cout << "0. Volver" << endl;
                    cout << "Seleccione: ";
                    cin >> opcionPrestamo;
                    
                    switch (opcionPrestamo) {
                        case 1: realizarPrestamo(); break;
                        case 2: devolverLibro(); break;
                        case 3: listarPrestamos(); break;
                        case 4: buscarPrestamosPorEstudiante(); break;
                        case 5: eliminarPrestamo(); break;
                        case 0: break;
                        default: cout << "Opcion invalida" << endl;
                    }
                    break;
                }
                case 0:
                    cout << "Saliendo del sistema... ¡Hasta luego!" << endl;
                    return;
                default:
                    cout << "Opcion invalida, intenta de nuevo" << endl;
            }
        }
    }
};

// Funcion principal del programa
int main() {
    BibliotecaDB biblioteca;
    biblioteca.mostrarMenu();
    return 0;
}