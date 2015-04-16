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

struct celda {

    celda() : tiempo(0), vehiculo(BMX), etapa(0), antecesor(-1,-1) {}

    int tiempo;
    int vehiculo;
    int etapa;
    pair<int,int> antecesor;
};

typedef vector<celda> Vec;
typedef vector<Vec> Tabla;

// Prototipado de funciones
int tiempo_total(Tabla &tabla, int fila, int columna, vector<costoEtapa> costos);
vector<int> obtener_vehiculos_celda(Tabla &tabla, int fila, int columna, int n);
void mostrar_celda(Tabla &tabla, int fila, int columna, vector<costoEtapa> costos);
celda agregar_vehiculo(Tabla &tabla, int fila, int columna, int vehiculo, vector<costoEtapa> costos);

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

    if(km > n) {
        km = n;
    }
    if(kb > n) {
        kb = n;
    }

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

    // creo la tabla de celdaes parciales
    Tabla matriz(km+1, Vec(kb+1, celda()));

    // La celda (0,0) ya tiene inicializada todas las etapas usando BMX
    // Calculamos la sumatoria de los tiempos para la celda inicial
    matriz[0][0].tiempo = tiempo_total(matriz, 0, 0, costos);
    //mostrar_celda(matriz, 0, 0, costos);

    // calculo la primera fila
    for(int i = 1; i <= kb; i++) {
        matriz[0][i] = agregar_vehiculo(matriz, 0, i-1, BUGGY, costos);
    }
    //mostrar_celda(matriz, 0, 1, costos);

    // calculo la primera columna
    for(int i = 1; i <= km; i++) {
        matriz[i][0] = agregar_vehiculo(matriz, i-1, 0, MOTO, costos);
    }

    //mostrar_celda(matriz, 1, 0, costos);
    //mostrar_celda(matriz, 2, 0, costos);

    // calculo todas las celdas internas de la tabla
    for(int i = 1; i <= km; i++) {
        for(int j = 1; j <= kb; j++) {
            celda aux_moto = agregar_vehiculo(matriz, i-1, j, MOTO, costos);
            celda aux_buggy = agregar_vehiculo(matriz, i, j-1, BUGGY, costos);
            if (aux_moto.tiempo < aux_buggy.tiempo) {
                matriz[i][j] = aux_moto;
            } else {
                matriz[i][j] = aux_buggy;
            }
            //mostrar_celda(matriz, i, j, costos);
        }
    }

    mostrar_celda(matriz, km, kb, costos);
}

// , agrega ese nuevo vehiculo a la celda y la devuelve
celda agregar_vehiculo(Tabla &tabla, int fila, int columna, int vehiculo, vector<costoEtapa> costos) {
    int n = costos.size();
    vector<int> vehiculos = obtener_vehiculos_celda(tabla, fila, columna, n);

    int min_etapa = -1;
    int min_tiempo = -1;

    for(int i = 0; i < n; i++) {
        int tiempo = tabla[fila][columna].tiempo - costos[i].costo(vehiculos[i]) +  costos[i].costo(vehiculo);
        if (tiempo < min_tiempo || min_tiempo == -1) {
            min_tiempo = tiempo;
            min_etapa = i;
        }
    }

    celda nueva;
    nueva.tiempo = min_tiempo;
    nueva.etapa = min_etapa;
    nueva.vehiculo = vehiculo;
    nueva.antecesor.first = fila;
    nueva.antecesor.second = columna;
    return nueva;
}


int tiempo_total(Tabla &tabla, int fila, int columna, vector<costoEtapa> costos) {
    int n = costos.size();
    vector<int> vehiculos = obtener_vehiculos_celda(tabla, fila, columna, n);
    int tiempo = 0;
    for(int i = 0; i < n; i++) {
        int aux = 0;
        if (vehiculos[i] == MOTO) {
            aux =  costos[i].costo(MOTO);
        } else if (vehiculos[i] == BUGGY) {
            aux = costos[i].costo(BUGGY);
        } else {
            aux = costos[i].costo(BMX);
        }
       tiempo = tiempo + aux;
    }
    return tiempo;
}

vector<int> obtener_vehiculos_celda(Tabla &tabla, int fila, int columna, int n) {
    celda aux = tabla[fila][columna];
    vector<int> vehiculos = vector<int>(n, BMX);
    vector<bool> cambios = vector<bool>(n, false);
    int counter = fila+columna;

    vehiculos[aux.etapa] = aux.vehiculo;
    cambios[aux.etapa] = true;
    counter--;
    while (counter > 0) {
        int f = aux.antecesor.first;
        int c = aux.antecesor.second;
        aux = tabla[f][c];
        if(cambios[aux.etapa] == false) {
            vehiculos[aux.etapa] = aux.vehiculo;
            cambios[aux.etapa] = true;
        }
        counter--;
    }
    return vehiculos;
}

// muestra por stdout los vehiculos para cada etapa y el tiempo total de la celda
void mostrar_celda(Tabla &tabla, int fila, int columna, vector<costoEtapa> costos) {
    int n = costos.size();
    vector<int> vehiculos = obtener_vehiculos_celda(tabla, fila, columna, n);
    for(int i = 0; i < n; i++) {
        string v = "";
        if (vehiculos[i] == MOTO) {
            v = "Motocross";
        } else if (vehiculos[i] == BUGGY) {
            v = "Buggy";
        } else {
            v = "BMX";
        }
        cout << "Etapa " << i+1 << ": " << v << endl;
    }
    cout << "Tiempo total: " << tabla[fila][columna].tiempo << endl;
}
