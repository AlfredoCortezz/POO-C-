#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <regex>
#include <limits>
#include <cctype>

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

    bool autorExiste(int id) {
        return any_of(autores.begin(), autores.end(), [id](const Autor& a) { return a.id == id; });
    }

    bool libroExiste(int id) {
        return any_of(libros.begin(), libros.end(), [id](const Libro& l) { return l.id == id; });
    }

    bool estudianteExiste(int id) {
        return any_of(estudiantes.begin(), estudiantes.end(), [id](const Estudiante& e) { return e.id == id; });
    }

    bool prestamoExiste(int id) {
        return any_of(prestamos.begin(), prestamos.end(), [id](const Prestamo& p) { return p.id == id; });
    }

    bool libroDisponible(int id_libro) {
        return none_of(prestamos.begin(), prestamos.end(), 
                      [id_libro](const Prestamo& p) { return p.id_libro == id_libro && p.activo; });
    }

    bool esBisiesto(int anio) {
        return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
    }

    bool fechaEsValida(int anio, int mes, int dia) {
        if (mes < 1 || mes > 12 || dia < 1) return false;
        
        int diasEnMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (mes == 2 && esBisiesto(anio)) diasEnMes[1] = 29;
        
        return dia <= diasEnMes[mes - 1];
    }

    bool validarFecha(const string& fecha) {
        regex fechaRegex("^\\d{4}-\\d{2}-\\d{2}$");
        if (!regex_match(fecha, fechaRegex)) return false;
        
        try {
            int anio = stoi(fecha.substr(0, 4));
            int mes = stoi(fecha.substr(5, 2));
            int dia = stoi(fecha.substr(8, 2));
            
            if (anio < 1900 || anio > 2100) return false;
            return fechaEsValida(anio, mes, dia);
        } catch (const exception& e) {
            return false;
        }
    }

    int generarIdAutor() {
        if (autores.empty()) return 1;
        return max_element(autores.begin(), autores.end(), 
                         [](const Autor& a, const Autor& b) { return a.id < b.id; })->id + 1;
    }

    int generarIdLibro() {
        if (libros.empty()) return 1;
        return max_element(libros.begin(), libros.end(), 
                         [](const Libro& a, const Libro& b) { return a.id < b.id; })->id + 1;
    }

    int generarIdEstudiante() {
        if (estudiantes.empty()) return 101;
        return max_element(estudiantes.begin(), estudiantes.end(), 
                         [](const Estudiante& a, const Estudiante& b) { return a.id < b.id; })->id + 1;
    }

    int generarIdPrestamo() {
        if (prestamos.empty()) return 1001;
        return max_element(prestamos.begin(), prestamos.end(), 
                         [](const Prestamo& a, const Prestamo& b) { return a.id < b.id; })->id + 1;
    }

    int leerEntero(const string& mensaje) {
        int valor;
        while (true) {
            cout << mensaje;
            cin >> valor;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Error: Entrada invalida. Por favor ingrese un numero entero." << endl;
            } else {
                cin.ignore();
                return valor;
            }
        }
    }

    string leerCadena(const string& mensaje) {
        string valor;
        cout << mensaje;
        getline(cin, valor);
        
        if (!valor.empty()) {
            valor[0] = toupper(valor[0]);
            for (size_t i = 1; i < valor.length(); i++) {
                if (valor[i-1] == ' ') valor[i] = toupper(valor[i]);
                else valor[i] = tolower(valor[i]);
            }
        }
        return valor;
    }

    void pausar() {
        cout << "\nPresione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    bool confirmarAccion(const string& mensaje) {
        cout << mensaje << " (s/n): ";
        char respuesta;
        cin >> respuesta;
        cin.ignore();
        return tolower(respuesta) == 's';
    }

    void crearArchivosPredeterminados() {
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

    void cargarAutores() {
        ifstream archivo("autores.txt");
        if (!archivo.is_open()) {
            cout << "Error: No se pudo abrir el archivo autores.txt" << endl;
            return;
        }
        
        string linea;
        while (getline(archivo, linea)) {
            if (linea.empty()) continue;
            
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
                } catch (const exception& e) {
                    cout << "Error en linea de autores: " << linea << endl;
                }
            }
        }
        archivo.close();
    }

    void cargarLibros() {
        ifstream archivo("libros.txt");
        if (!archivo.is_open()) {
            cout << "Error: No se pudo abrir el archivo libros.txt" << endl;
            return;
        }
        
        string linea;
        while (getline(archivo, linea)) {
            if (linea.empty()) continue;
            
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
                } catch (const exception& e) {
                    cout << "Error en linea de libros: " << linea << endl;
                }
            }
        }
        archivo.close();
    }

    void cargarEstudiantes() {
        ifstream archivo("estudiantes.txt");
        if (!archivo.is_open()) {
            cout << "Error: No se pudo abrir el archivo estudiantes.txt" << endl;
            return;
        }
        
        string linea;
        while (getline(archivo, linea)) {
            if (linea.empty()) continue;
            
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
                } catch (const exception& e) {
                    cout << "Error en linea de estudiantes: " << linea << endl;
                }
            }
        }
        archivo.close();
    }

    void cargarPrestamos() {
        ifstream archivo("prestamos.txt");
        if (!archivo.is_open()) {
            cout << "Error: No se pudo abrir el archivo prestamos.txt" << endl;
            return;
        }
        
        string linea;
        while (getline(archivo, linea)) {
            if (linea.empty()) continue;
            
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
                } catch (const exception& e) {
                    cout << "Error en linea de prestamos: " << linea << endl;
                }
            }
        }
        archivo.close();
    }

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
    BibliotecaDB() {
        crearArchivosPredeterminados();
        cargarAutores();
        cargarLibros();
        cargarEstudiantes();
        cargarPrestamos();
        cout << "Sistema de biblioteca iniciado correctamente" << endl;
    }
    
    void agregarAutor() {
        cout << "\n===== AGREGAR AUTOR =====" << endl;
        
        Autor autor;
        autor.id = generarIdAutor();
        autor.nombre = leerCadena("Nombre: ");
        autor.nacionalidad = leerCadena("Nacionalidad: ");
        
        autores.push_back(autor);
        guardarAutores();
        cout << "Autor agregado correctamente (ID: " << autor.id << ")" << endl;
        pausar();
    }

    void listarAutores() {
        cout << "\n===== LISTA DE AUTORES =====" << endl;
        if (autores.empty()) {
            cout << "No hay autores registrados" << endl;
            pausar();
            return;
        }
        
        printf("%-4s %-30s %-20s\n", "ID", "NOMBRE", "NACIONALIDAD");
        cout << string(60, '=') << endl;
        
        for (const auto& autor : autores) {
            printf("%-4d %-30s %-20s\n", autor.id, autor.nombre.c_str(), autor.nacionalidad.c_str());
        }
        cout << "\nTotal: " << autores.size() << " autores" << endl;
        pausar();
    }

    void buscarAutorPorId() {
        cout << "\n===== BUSCAR AUTOR POR ID =====" << endl;
        int id = leerEntero("Ingrese ID del autor: ");
        
        auto it = find_if(autores.begin(), autores.end(), [id](const Autor& a) { return a.id == id; });
        if (it != autores.end()) {
            cout << "\nAutor encontrado:" << endl;
            cout << "ID: " << it->id << endl;
            cout << "Nombre: " << it->nombre << endl;
            cout << "Nacionalidad: " << it->nacionalidad << endl;
        } else {
            cout << "No se encontro autor con ID " << id << endl;
        }
        pausar();
    }

    void actualizarAutor() {
        cout << "\n===== ACTUALIZAR AUTOR =====" << endl;
        int id = leerEntero("ID del autor a actualizar: ");
        
        auto it = find_if(autores.begin(), autores.end(), [id](const Autor& a) { return a.id == id; });
        if (it != autores.end()) {
            cout << "\nAutor actual: " << it->nombre << " (" << it->nacionalidad << ")" << endl;
            
            it->nombre = leerCadena("Nuevo nombre: ");
            it->nacionalidad = leerCadena("Nueva nacionalidad: ");
            
            guardarAutores();
            cout << "Autor actualizado correctamente" << endl;
        } else {
            cout << "No se encontro autor con ID " << id << endl;
        }
        pausar();
    }

    void eliminarAutor() {
        cout << "\n===== ELIMINAR AUTOR =====" << endl;
        int id = leerEntero("ID del autor a eliminar: ");
        
        auto it = find_if(autores.begin(), autores.end(), [id](const Autor& a) { return a.id == id; });
        if (it != autores.end()) {
            bool tieneLibros = any_of(libros.begin(), libros.end(), 
                                    [id](const Libro& l) { return l.id_autor == id; });
            
            if (tieneLibros) {
                cout << "Error: No se puede eliminar. El autor tiene libros asociados." << endl;
            } else if (confirmarAccion("Esta seguro de eliminar este autor")) {
                autores.erase(it);
                guardarAutores();
                cout << "Autor eliminado correctamente" << endl;
            } else {
                cout << "Eliminacion cancelada" << endl;
            }
        } else {
            cout << "No se encontro autor con ID " << id << endl;
        }
        pausar();
    }

    void agregarLibro() {
        cout << "\n===== AGREGAR LIBRO =====" << endl;
        
        Libro libro;
        libro.id = generarIdLibro();
        libro.titulo = leerCadena("Titulo: ");
        cout << "ISBN: ";
        getline(cin, libro.isbn);
        libro.anio = leerEntero("Anio de publicacion: ");
        
        cout << "\nLista de autores disponibles:" << endl;
        listarAutores();
        
        libro.id_autor = leerEntero("ID del autor: ");
        
        if (!autorExiste(libro.id_autor)) {
            cout << "Error: No existe autor con ese ID" << endl;
            pausar();
            return;
        }
        
        libros.push_back(libro);
        guardarLibros();
        cout << "Libro agregado correctamente (ID: " << libro.id << ")" << endl;
        pausar();
    }

    void listarLibros() {
        cout << "\n===== CATALOGO DE LIBROS =====" << endl;
        if (libros.empty()) {
            cout << "No hay libros registrados" << endl;
            pausar();
            return;
        }
        
        printf("%-3s %-25s %-15s %-6s %-20s %-12s\n", 
               "ID", "TITULO", "ISBN", "AÑO", "AUTOR", "DISPONIBLE");
        cout << string(90, '=') << endl;
        
        for (const auto& libro : libros) {
            string nombreAutor = "No encontrado";
            auto autorIt = find_if(autores.begin(), autores.end(), 
                                 [&libro](const Autor& a) { return a.id == libro.id_autor; });
            if (autorIt != autores.end()) nombreAutor = autorIt->nombre;
            
            string disponible = libroDisponible(libro.id) ? "Si" : "No";
            
            printf("%-3d %-25s %-15s %-6d %-20s %-12s\n", 
                   libro.id, libro.titulo.c_str(), libro.isbn.c_str(), 
                   libro.anio, nombreAutor.c_str(), disponible.c_str());
        }
        pausar();
    }

    void buscarLibroPorTitulo() {
        cout << "\n===== BUSCAR LIBRO POR TITULO =====" << endl;
        string titulo = leerCadena("Titulo a buscar: ");
        
        bool encontrado = false;
        for (const auto& libro : libros) {
            string tituloLower = libro.titulo;
            transform(tituloLower.begin(), tituloLower.end(), tituloLower.begin(), ::tolower);
            string busquedaLower = titulo;
            transform(busquedaLower.begin(), busquedaLower.end(), busquedaLower.begin(), ::tolower);
            
            if (tituloLower.find(busquedaLower) != string::npos) {
                if (!encontrado) {
                    cout << "Libros encontrados:" << endl;
                    printf("%-3s %-25s %-20s %-12s\n", "ID", "TITULO", "AUTOR", "DISPONIBLE");
                    cout << string(70, '-') << endl;
                    encontrado = true;
                }
                
                string nombreAutor = "No encontrado";
                auto autorIt = find_if(autores.begin(), autores.end(), 
                                     [&libro](const Autor& a) { return a.id == libro.id_autor; });
                if (autorIt != autores.end()) nombreAutor = autorIt->nombre;
                
                string disponible = libroDisponible(libro.id) ? "Si" : "No";
                
                printf("%-3d %-25s %-20s %-12s\n", 
                       libro.id, libro.titulo.c_str(), nombreAutor.c_str(), disponible.c_str());
            }
        }
        
        if (!encontrado) {
            cout << "No se encontraron libros con ese titulo" << endl;
        }
        pausar();
    }

    void actualizarLibro() {
        cout << "\n===== ACTUALIZAR LIBRO =====" << endl;
        int id = leerEntero("ID del libro: ");
        
        auto it = find_if(libros.begin(), libros.end(), [id](const Libro& l) { return l.id == id; });
        if (it != libros.end()) {
            cout << "\nLibro actual: " << it->titulo << " (ISBN: " << it->isbn << ")" << endl;
            
            it->titulo = leerCadena("Nuevo titulo: ");
            cout << "Nuevo ISBN: ";
            getline(cin, it->isbn);
            it->anio = leerEntero("Nuevo anio: ");
            
            cout << "\nLista de autores disponibles:" << endl;
            listarAutores();
            it->id_autor = leerEntero("Nuevo ID autor: ");
            
            if (!autorExiste(it->id_autor)) {
                cout << "Error: No existe autor con ese ID" << endl;
                return;
            }
            
            guardarLibros();
            cout << "Libro actualizado correctamente" << endl;
        } else {
            cout << "No se encontro libro con ID " << id << endl;
        }
        pausar();
    }

    void eliminarLibro() {
        cout << "\n===== ELIMINAR LIBRO =====" << endl;
        int id = leerEntero("ID del libro a eliminar: ");
        
        auto it = find_if(libros.begin(), libros.end(), [id](const Libro& l) { return l.id == id; });
        if (it != libros.end()) {
            if (!libroDisponible(id)) {
                cout << "Error: No se puede eliminar. El libro esta actualmente prestado." << endl;
            } else if (confirmarAccion("Esta seguro de eliminar este libro")) {
                libros.erase(it);
                guardarLibros();
                cout << "Libro eliminado correctamente" << endl;
            } else {
                cout << "Eliminacion cancelada" << endl;
            }
        } else {
            cout << "No se encontro libro con ID " << id << endl;
        }
        pausar();
    }

    void agregarEstudiante() {
        cout << "\n===== AGREGAR ESTUDIANTE =====" << endl;
        
        Estudiante estudiante;
        estudiante.id = generarIdEstudiante();
        estudiante.nombre = leerCadena("Nombre: ");
        estudiante.grado = leerEntero("Grado: ");
        
        estudiantes.push_back(estudiante);
        guardarEstudiantes();
        cout << "Estudiante agregado correctamente (ID: " << estudiante.id << ")" << endl;
        pausar();
    }

    void listarEstudiantes() {
        cout << "\n===== LISTA DE ESTUDIANTES =====" << endl;
        if (estudiantes.empty()) {
            cout << "No hay estudiantes registrados" << endl;
            pausar();
            return;
        }
        
        printf("%-4s %-25s %-6s\n", "ID", "NOMBRE", "GRADO");
        cout << string(40, '=') << endl;
        
        for (const auto& estudiante : estudiantes) {
            printf("%-4d %-25s %-6d\n", estudiante.id, estudiante.nombre.c_str(), estudiante.grado);
        }
        cout << "\nTotal: " << estudiantes.size() << " estudiantes" << endl;
        pausar();
    }

    void buscarEstudiantePorNombre() {
        cout << "\n===== BUSCAR ESTUDIANTE POR NOMBRE =====" << endl;
        string nombre = leerCadena("Nombre a buscar: ");
        
        bool encontrado = false;
        for (const auto& estudiante : estudiantes) {
            string nombreLower = estudiante.nombre;
            transform(nombreLower.begin(), nombreLower.end(), nombreLower.begin(), ::tolower);
            string busquedaLower = nombre;
            transform(busquedaLower.begin(), busquedaLower.end(), busquedaLower.begin(), ::tolower);
            
            if (nombreLower.find(busquedaLower) != string::npos) {
                if (!encontrado) {
                    cout << "Estudiantes encontrados:" << endl;
                    printf("%-4s %-25s %-6s\n", "ID", "NOMBRE", "GRADO");
                    cout << string(40, '-') << endl;
                    encontrado = true;
                }
                printf("%-4d %-25s %-6d\n", estudiante.id, estudiante.nombre.c_str(), estudiante.grado);
            }
        }
        
        if (!encontrado) {
            cout << "No se encontraron estudiantes con ese nombre" << endl;
        }
        pausar();
    }

    void actualizarEstudiante() {
        cout << "\n===== ACTUALIZAR ESTUDIANTE =====" << endl;
        int id = leerEntero("ID del estudiante: ");
        
        auto it = find_if(estudiantes.begin(), estudiantes.end(), [id](const Estudiante& e) { return e.id == id; });
        if (it != estudiantes.end()) {
            cout << "\nEstudiante actual: " << it->nombre << " (Grado: " << it->grado << ")" << endl;
            
            it->nombre = leerCadena("Nuevo nombre: ");
            it->grado = leerEntero("Nuevo grado: ");
            
            guardarEstudiantes();
            cout << "Estudiante actualizado correctamente" << endl;
        } else {
            cout << "No se encontro estudiante con ID " << id << endl;
        }
        pausar();
    }

    void eliminarEstudiante() {
        cout << "\n===== ELIMINAR ESTUDIANTE =====" << endl;
        int id = leerEntero("ID del estudiante a eliminar: ");
        
        auto it = find_if(estudiantes.begin(), estudiantes.end(), [id](const Estudiante& e) { return e.id == id; });
        if (it != estudiantes.end()) {
            bool tienePrestamosActivos = any_of(prestamos.begin(), prestamos.end(), 
                                              [id](const Prestamo& p) { return p.id_estudiante == id && p.activo; });
            
            if (tienePrestamosActivos) {
                cout << "Error: No se puede eliminar. El estudiante tiene prestamos activos." << endl;
            } else if (confirmarAccion("Esta seguro de eliminar este estudiante")) {
                estudiantes.erase(it);
                guardarEstudiantes();
                cout << "Estudiante eliminado correctamente" << endl;
            } else {
                cout << "Eliminacion cancelada" << endl;
            }
        } else {
            cout << "No se encontro estudiante con ID " << id << endl;
        }
        pausar();
    }

    void realizarPrestamo() {
        cout << "\n===== REALIZAR PRESTAMO =====" << endl;
        
        Prestamo prestamo;
        prestamo.id = generarIdPrestamo();
        
        cout << "\nLibros disponibles:" << endl;
        bool hayLibrosDisponibles = false;
        for (const auto& libro : libros) {
            if (libroDisponible(libro.id)) {
                if (!hayLibrosDisponibles) {
                    printf("%-3s %-25s %-20s\n", "ID", "TITULO", "AUTOR");
                    cout << string(50, '-') << endl;
                    hayLibrosDisponibles = true;
                }
                
                string nombreAutor = "No encontrado";
                auto autorIt = find_if(autores.begin(), autores.end(), 
                                     [&libro](const Autor& a) { return a.id == libro.id_autor; });
                if (autorIt != autores.end()) nombreAutor = autorIt->nombre;
                
                printf("%-3d %-25s %-20s\n", libro.id, libro.titulo.c_str(), nombreAutor.c_str());
            }
        }
        
        if (!hayLibrosDisponibles) {
            cout << "No hay libros disponibles para prestamo" << endl;
            pausar();
            return;
        }
        
        prestamo.id_libro = leerEntero("\nID del libro: ");
        
        if (!libroExiste(prestamo.id_libro)) {
            cout << "Error: No existe libro con ese ID" << endl;
            pausar();
            return;
        }
        
        if (!libroDisponible(prestamo.id_libro)) {
            cout << "Error: El libro no esta disponible" << endl;
            pausar();
            return;
        }
        
        cout << "\nLista de estudiantes:" << endl;
        listarEstudiantes();
        prestamo.id_estudiante = leerEntero("ID del estudiante: ");
        
        if (!estudianteExiste(prestamo.id_estudiante)) {
            cout << "Error: No existe estudiante con ese ID" << endl;
            pausar();
            return;
        }
        
        string fecha;
        do {
            cout << "Fecha de prestamo (YYYY-MM-DD): ";
            getline(cin, fecha);
            if (!validarFecha(fecha)) {
                cout << "Error: Formato de fecha invalido. Use YYYY-MM-DD con una fecha valida." << endl;
            }
        } while (!validarFecha(fecha));
        prestamo.fecha_prestamo = fecha;
        
        do {
            cout << "Fecha de devolucion (YYYY-MM-DD): ";
            getline(cin, fecha);
            if (!validarFecha(fecha)) {
                cout << "Error: Formato de fecha invalido. Use YYYY-MM-DD con una fecha valida." << endl;
            }
        } while (!validarFecha(fecha));
        prestamo.fecha_devolucion = fecha;
        
        prestamo.activo = true;
        prestamos.push_back(prestamo);
        guardarPrestamos();
        cout << "Prestamo realizado correctamente (ID Prestamo: " << prestamo.id << ")" << endl;
        pausar();
    }

    void devolverLibro() {
        cout << "\n===== DEVOLVER LIBRO =====" << endl;
        int id_prestamo = leerEntero("ID del prestamo: ");
        
        auto it = find_if(prestamos.begin(), prestamos.end(), 
                         [id_prestamo](const Prestamo& p) { return p.id == id_prestamo && p.activo; });
        if (it != prestamos.end()) {
            string tituloLibro = "No encontrado";
            auto libroIt = find_if(libros.begin(), libros.end(), 
                                 [it](const Libro& l) { return l.id == it->id_libro; });
            if (libroIt != libros.end()) tituloLibro = libroIt->titulo;
            
            cout << "Devolviendo libro: " << tituloLibro << endl;
            it->activo = false;
            guardarPrestamos();
            cout << "Libro devuelto correctamente" << endl;
        } else {
            cout << "Error: No hay prestamo activo con ese ID" << endl;
        }
        pausar();
    }

    void listarPrestamos() {
        cout << "\n===== LISTA DE PRESTAMOS =====" << endl;
        if (prestamos.empty()) {
            cout << "No hay prestamos registrados" << endl;
            pausar();
            return;
        }
        
        printf("%-5s %-25s %-20s %-15s %-15s %-6s\n", 
               "ID", "LIBRO", "ESTUDIANTE", "PRESTAMO", "DEVOLUCION", "ACTIVO");
        cout << string(100, '=') << endl;
        
        for (const auto& prestamo : prestamos) {
            string tituloLibro = "No encontrado";
            auto libroIt = find_if(libros.begin(), libros.end(), 
                                 [&prestamo](const Libro& l) { return l.id == prestamo.id_libro; });
            if (libroIt != libros.end()) tituloLibro = libroIt->titulo;
            
            string nombreEstudiante = "No encontrado";
            auto estudianteIt = find_if(estudiantes.begin(), estudiantes.end(), 
                                      [&prestamo](const Estudiante& e) { return e.id == prestamo.id_estudiante; });
            if (estudianteIt != estudiantes.end()) nombreEstudiante = estudianteIt->nombre;
            
            printf("%-5d %-25s %-20s %-15s %-15s %-6s\n", 
                   prestamo.id, tituloLibro.c_str(), nombreEstudiante.c_str(),
                   prestamo.fecha_prestamo.c_str(), prestamo.fecha_devolucion.c_str(),
                   prestamo.activo ? "Si" : "No");
        }
        pausar();
    }

    void buscarPrestamosPorEstudiante() {
        cout << "\n===== BUSCAR PRESTAMOS POR ESTUDIANTE =====" << endl;
        int id_estudiante = leerEntero("ID del estudiante: ");
        
        if (!estudianteExiste(id_estudiante)) {
            cout << "Error: No existe estudiante con ese ID" << endl;
            pausar();
            return;
        }
        
        bool encontrado = false;
        for (const auto& prestamo : prestamos) {
            if (prestamo.id_estudiante == id_estudiante) {
                if (!encontrado) {
                    cout << "Prestamos del estudiante:" << endl;
                    printf("%-5s %-25s %-15s %-15s %-6s\n", 
                           "ID", "LIBRO", "PRESTAMO", "DEVOLUCION", "ACTIVO");
                    cout << string(70, '-') << endl;
                    encontrado = true;
                }
                
                string tituloLibro = "No encontrado";
                auto libroIt = find_if(libros.begin(), libros.end(), 
                                     [&prestamo](const Libro& l) { return l.id == prestamo.id_libro; });
                if (libroIt != libros.end()) tituloLibro = libroIt->titulo;
                
                printf("%-5d %-25s %-15s %-15s %-6s\n", 
                       prestamo.id, tituloLibro.c_str(), prestamo.fecha_prestamo.c_str(),
                       prestamo.fecha_devolucion.c_str(), prestamo.activo ? "Si" : "No");
            }
        }
        
        if (!encontrado) {
            cout << "No hay prestamos para este estudiante" << endl;
        }
        pausar();
    }

    void eliminarPrestamo() {
        cout << "\n===== ELIMINAR PRESTAMO =====" << endl;
        int id = leerEntero("ID del prestamo a eliminar: ");
        
        auto it = find_if(prestamos.begin(), prestamos.end(), [id](const Prestamo& p) { return p.id == id; });
        if (it != prestamos.end()) {
            if (it->activo) {
                cout << "Error: No se puede eliminar un prestamo activo. Debe devolver el libro primero." << endl;
            } else if (confirmarAccion("Esta seguro de eliminar este prestamo")) {
                prestamos.erase(it);
                guardarPrestamos();
                cout << "Prestamo eliminado correctamente" << endl;
            } else {
                cout << "Eliminacion cancelada" << endl;
            }
        } else {
            cout << "No se encontro prestamo con ese ID" << endl;
        }
        pausar();
    }

    void mostrarMenu() {
        int opcion;
        
        while (true) {
            cout << "\n";
            cout << "==========================================" << endl;
            cout << "          SISTEMA DE BIBLIOTECA          " << endl;
            cout << "==========================================" << endl;
            cout << "1. Gestion de Autores" << endl;
            cout << "2. Gestion de Libros" << endl;
            cout << "3. Gestion de Estudiantes" << endl;
            cout << "4. Gestion de Prestamos" << endl;
            cout << "0. Salir del Sistema" << endl;
            cout << "==========================================" << endl;
            
            opcion = leerEntero("Seleccione una opcion: ");
            
            switch (opcion) {
                case 1: {
                    int opcionAutor;
                    cout << "\n===== GESTION DE AUTORES =====" << endl;
                    cout << "1. Agregar Autor" << endl;
                    cout << "2. Listar Autores" << endl;
                    cout << "3. Buscar por ID" << endl;
                    cout << "4. Actualizar Autor" << endl;
                    cout << "5. Eliminar Autor" << endl;
                    cout << "0. Volver al Menu Principal" << endl;
                    
                    opcionAutor = leerEntero("Seleccione: ");
                    
                    switch (opcionAutor) {
                        case 1: agregarAutor(); break;
                        case 2: listarAutores(); break;
                        case 3: buscarAutorPorId(); break;
                        case 4: actualizarAutor(); break;
                        case 5: eliminarAutor(); break;
                        case 0: break;
                        default: cout << "Opcion invalida" << endl; pausar();
                    }
                    break;
                }
                case 2: {
                    int opcionLibro;
                    cout << "\n===== GESTION DE LIBROS =====" << endl;
                    cout << "1. Agregar Libro" << endl;
                    cout << "2. Listar Libros" << endl;
                    cout << "3. Buscar por Titulo" << endl;
                    cout << "4. Actualizar Libro" << endl;
                    cout << "5. Eliminar Libro" << endl;
                    cout << "0. Volver al Menu Principal" << endl;
                    
                    opcionLibro = leerEntero("Seleccione: ");
                    
                    switch (opcionLibro) {
                        case 1: agregarLibro(); break;
                        case 2: listarLibros(); break;
                        case 3: buscarLibroPorTitulo(); break;
                        case 4: actualizarLibro(); break;
                        case 5: eliminarLibro(); break;
                        case 0: break;
                        default: cout << "Opcion invalida" << endl; pausar();
                    }
                    break;
                }
                case 3: {
                    int opcionEstudiante;
                    cout << "\n===== GESTION DE ESTUDIANTES =====" << endl;
                    cout << "1. Agregar Estudiante" << endl;
                    cout << "2. Listar Estudiantes" << endl;
                    cout << "3. Buscar por Nombre" << endl;
                    cout << "4. Actualizar Estudiante" << endl;
                    cout << "5. Eliminar Estudiante" << endl;
                    cout << "0. Volver al Menu Principal" << endl;
                    
                    opcionEstudiante = leerEntero("Seleccione: ");
                    
                    switch (opcionEstudiante) {
                        case 1: agregarEstudiante(); break;
                        case 2: listarEstudiantes(); break;
                        case 3: buscarEstudiantePorNombre(); break;
                        case 4: actualizarEstudiante(); break;
                        case 5: eliminarEstudiante(); break;
                        case 0: break;
                        default: cout << "Opcion invalida" << endl; pausar();
                    }
                    break;
                }
                case 4: {
                    int opcionPrestamo;
                    cout << "\n===== GESTION DE PRESTAMOS =====" << endl;
                    cout << "1. Realizar Prestamo" << endl;
                    cout << "2. Devolver Libro" << endl;
                    cout << "3. Listar Prestamos" << endl;
                    cout << "4. Buscar por Estudiante" << endl;
                    cout << "5. Eliminar Prestamo" << endl;
                    cout << "0. Volver al Menu Principal" << endl;
                    
                    opcionPrestamo = leerEntero("Seleccione: ");
                    
                    switch (opcionPrestamo) {
                        case 1: realizarPrestamo(); break;
                        case 2: devolverLibro(); break;
                        case 3: listarPrestamos(); break;
                        case 4: buscarPrestamosPorEstudiante(); break;
                        case 5: eliminarPrestamo(); break;
                        case 0: break;
                        default: cout << "Opcion invalida" << endl; pausar();
                    }
                    break;
                }
                case 0:
                    cout << "\nGracias por usar el Sistema de Biblioteca. ¡Hasta pronto!" << endl;
                    return;
                default:
                    cout << "Opcion invalida, por favor intente nuevamente" << endl;
                    pausar();
            }
        }
    }
};

int main() {
    BibliotecaDB biblioteca;
    biblioteca.mostrarMenu();
    return 0;
}