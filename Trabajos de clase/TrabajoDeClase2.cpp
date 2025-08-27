#include <iostream>
#include <vector>
#include <string>
using namespace std; //esto me salio en starflow


//Crea una clase base llamada Vehiculo.
class Vehiculo {
protected:
    string marca;
    string modelo;
    int anio;
    double precioBase;

public:
    Vehiculo(string m, string mod, int a, double p)
        : marca(m), modelo(mod), anio(a), precioBase(p) {}

    virtual ~Vehiculo() {} 

    virtual double calcularPrecioFinal() const {
        return precioBase; 
    }

    virtual void mostrarInfo() const {
        cout << "Marca: " << marca
             << ", Modelo: " << modelo
             << ", Año: " << anio
             << ", Precio final: $" << calcularPrecioFinal()
             << endl;
    }
};

//2.Crea dos clases derivadas: Auto y Moto
//La clase Auto debe heredar de Vehiculo y tener un atributo adicional:
//int numeroPuertas
class Auto : 
public Vehiculo {
private:
    int numeroPuertas; //atributo adicional pedido

public:
    Auto(string m, string mod, int a, double p, int puertas)
        : Vehiculo(m, mod, a, p), numeroPuertas(puertas) {}


//Cada clase derivada (Auto y Moto) debe anular el método calcularPrecioFinal() para implementar su propia lógica de impuestos:
//Para un Auto, el impuesto es del 15% del precio base:

    double calcularPrecioFinal() const override {
        return precioBase * 1.15; // 15% de impuesto
    }

    void mostrarInfo() const override {
        cout << "Auto -> Marca: " << marca
             << ", Modelo: " << modelo
             << ", Año: " << anio
             << ", Puertas: " << numeroPuertas
             << ", Precio final: $" << calcularPrecioFinal()
             << endl;
    }
};

//Clase derivada moto
//La clase Moto debe heredar de Vehiculo y tener un atributo adicional:
//int cilindrada
class Moto : 
public Vehiculo {
private: 
    int cilindrada; //atributo adiconal
public:
    Moto(string m, string mod, int a, double p, int cil)
        : Vehiculo(m, mod, a, p), cilindrada(cil) {}
// Para una Moto, el impuesto es del 10% del precio base.
    double calcularPrecioFinal() const override {
        return precioBase * 1.10; // 10% de impuesto
    }

    void mostrarInfo() const override {
        cout << "Moto -> Marca: " << marca
             << ", Modelo: " << modelo
             << ", Año: " << anio
             << ", Cilindrada: " << cilindrada << "cc"
             << ", Precio final: $" << calcularPrecioFinal()
             << endl;
    }
};

//En la función main(), haz lo siguiente:
//Crea un vector de punteros a Vehiculo (vector<Vehiculo*>).
int main() {
    vector<Vehiculo*> concesionariaaa;

    //Crea un objeto Auto y un objeto Moto y agrégalos al vector. Puedes inicializarlos con valores de ejemplo.
    concesionariaaa.push_back(new Auto("Toyota", "Corolla", 2022, 20000, 4));
    concesionariaaa.push_back(new Moto("Yamaha", "R3", 2021, 8000, 321));

    //Recorre el vector y, para cada vehículo, imprime su marca, modelo y su precio final.
    for (auto v : concesionariaaa) {
        v->mostrarInfo();
    }

    // Liberar memoria
    for (auto v : concesionariaaa) {
        delete v;
    }

    return 0;
}