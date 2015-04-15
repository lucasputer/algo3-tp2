#include <functional>
#include <algorithm>
#include <vector>
#include <iostream>
#include <time.h>
using namespace std;

// Structs y Typedefs
// para indexar los arrays de costos y otras operaciones con vehiculos
#define BMX        0
#define MOTO       1
#define BUGGY      2

struct costoEtapa {

    costoEtapa() : bmx(0), moto(0), buggy(0) {}

    costoEtapa(int t1, int t2, int t3) : bmx(t1), moto(t2), buggy(t3) {}

    int costo(int vehiculo) {
        if (vehiculo == BMX) {
            return bmx;
        } else if (vehiculo == MOTO) {
            return moto;
        } else {
            return buggy;
        }
    }

    int bmx;
    int moto;
    int buggy;
};

struct solucion {

    solucion(int n) : tiempo(0), vehiculos(n, BMX) {}

    int tiempo;
    vector<int> vehiculos;
};

typedef vector<solucion> Vec;
typedef vector<Vec> Tabla;

// Prototipado de funciones
int tiempo_total(vector<int> s, vector<costoEtapa> costos);
void mostrar_solucion(solucion s, vector<costoEtapa> costos);
solucion agregar_vehiculo(int vehiculo, solucion s, vector<costoEtapa> costos);

// Implementacion. Contiene el cargado de input más la resolución del ejercicio.
int main() {
    // cantidad de etapas
    int n;
    cin >> n;
    // cantidad de veces que puedo usar la moto
    int km;
    cin >> km;
    // cantidad de veces que puedo usar el buggy
    int kb;
    cin >> kb;

    // cargo los costos
    vector<costoEtapa> costos(n, costoEtapa());
    for(int i = 0; i < n; i++) {
        int bmx, moto, buggy;
        cin >> bmx;
        cin >> moto;
        cin >> buggy;
        costoEtapa costo = costoEtapa(bmx, moto, buggy);
        costos[i] = costo;
    }

    // creo la tabla de soluciones parciales
    Tabla matriz(km+1, Vec(kb+1, solucion(n)));
    // La solucion (0,0) ya tiene inicializada todas las etapas usando BMX
    // Calculamos la sumatoria de los tiempos para la solucion inicial
    matriz[0][0].tiempo = tiempo_total(matriz[0][0].vehiculos, costos);
    //mostrar_solucion(matriz[0][0], costos);

    // calculo la primera fila
    for(int i = 1; i <= kb; i++) {
        matriz[0][i] = agregar_vehiculo(BUGGY, matriz[0][i-1], costos);
    }
    // calculo la primera columna
    for(int i = 1; i <= km; i++) {
        matriz[i][0] = agregar_vehiculo(MOTO, matriz[i-1][0], costos);
    }

    /*mostrar_solucion(matriz[0][1], costos);
    mostrar_solucion(matriz[1][0], costos);
    mostrar_solucion(matriz[2][0], costos);*/

    // calculo todas las celdas internas de la tabla
    for(int i = 1; i <= km; i++) {
        for(int j = 1; j <= kb; j++) {
            solucion aux_moto = agregar_vehiculo(MOTO, matriz[i-1][j], costos);
            solucion aux_buggy = agregar_vehiculo(BUGGY, matriz[i][j-1], costos);
            if (aux_moto.tiempo < aux_buggy.tiempo) {
                matriz[i][j] = aux_moto;
            } else {
                matriz[i][j] = aux_buggy;
            }
        }
    }

    mostrar_solucion(matriz[km][kb], costos);
}

// Dado un vehiculo, una solucion de partida y los costos de los vehiculos para cada etapa
// , agrega ese nuevo vehiculo a la solucion y la devuelve
solucion agregar_vehiculo(int vehiculo, solucion s, vector<costoEtapa> costos) {
    int n = costos.size();

    int min_etapa = -1;
    int min_tiempo = -1;

    for(int i = 0; i < n; i++) {
        int tiempo = s.tiempo - costos[i].costo(s.vehiculos[i]) +  costos[i].costo(vehiculo);
        if (tiempo < min_tiempo || min_tiempo == -1) {
            min_tiempo = tiempo;
            min_etapa = i;
        }
    }

    solucion nueva = s;
    nueva.vehiculos[min_etapa] = vehiculo;
    nueva.tiempo = min_tiempo;
    return nueva;
}

// Dado un vector de vehiculos y los costos de los vehiculos para cada etapa
// devuelve el costo de realizar el rally.
int tiempo_total(vector<int> s, vector<costoEtapa> costos) {
    int n = s.size();
    int tiempo = 0;
    for(int i = 0; i < n; i++) {
        tiempo = tiempo + costos[i].costo(s[i]);
    }
    return tiempo;
}

// Dada una solucion y los costos de los vehiculos para cada etapa
// muestra por stdout los vehiculos para cada etapa y el tiempo total de la solucion
void mostrar_solucion(solucion s, vector<costoEtapa> costos) {
    int n = costos.size();
    for(int i = 0; i < n; i++) {
        string v = "";
        if (s.vehiculos[i] == BMX) {
            v = "BMX";
        } else if (s.vehiculos[i] == MOTO) {
            v = "Motocross";
        } else {
            v = "Buggy";
        }
        cout << "Etapa " << i+1 << ": " << v << endl;
    }
    cout << "Tiempo total: " << s.tiempo << endl;
}
