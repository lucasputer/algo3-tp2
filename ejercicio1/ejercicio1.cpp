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

    solucion() : tiempo(0) {}

    int tiempo;
};

typedef vector<solucion> Vec;
typedef vector<Vec> Tabla;

// Prototipado de funciones
int tiempo_total(vector<pair <int,int> > &vehiculos, int fila, int columna, vector<costoEtapa> costos);
void mostrar_solucion(solucion s, vector< pair<int,int> > &vehiculos, int fila, int columna, vector<costoEtapa> costos);
solucion agregar_vehiculo(int vehiculo, solucion s, vector<costoEtapa> costos, vector< pair<int,int> > &vehiculos, int fila, int columna);
solucion sim_agregar_vehiculo(int vehiculo, solucion s, vector<costoEtapa> costos, vector< pair<int,int> > &vehiculos, int fila, int columna);

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
    Tabla matriz(km+1, Vec(kb+1, solucion()));
    vector< pair<int,int> > vehiculos_fila(n, pair<int,int>(BMX, 0));

    // La solucion (0,0) ya tiene inicializada todas las etapas usando BMX
    // Calculamos la sumatoria de los tiempos para la solucion inicial
    matriz[0][0].tiempo = tiempo_total(vehiculos_fila, 0, 0, costos);
    //mostrar_solucion(matriz[0][0], costos);
    //mostrar_solucion(matriz[0][0], vehiculos_fila, 0, 0, costos);

    // calculo la primera fila
    for(int i = 1; i <= kb; i++) {
        matriz[0][i] = agregar_vehiculo(BUGGY, matriz[0][i-1], costos, vehiculos_fila, 0, i);
    }
    //mostrar_solucion(matriz[0][1], vehiculos_fila, 0, 1, costos);

    // calculo la primera columna
    /*for(int i = 1; i <= km; i++) {
        matriz[i][0] = agregar_vehiculo(MOTO, matriz[i-1][0], costos);
    }*/

    /*mostrar_solucion(matriz[0][1], costos);
    mostrar_solucion(matriz[1][0], costos);
    mostrar_solucion(matriz[2][0], costos);*/

    // calculo todas las celdas internas de la tabla
    for(int i = 1; i <= km; i++) {
        for(int j = 0; j <= kb; j++) {
            if (j == 0) {
                matriz[i][0] = agregar_vehiculo(MOTO, matriz[i-1][0], costos, vehiculos_fila, i, 0);
            } else {
                solucion aux_moto = sim_agregar_vehiculo(MOTO, matriz[i-1][j], costos, vehiculos_fila, i, j);
                solucion aux_buggy = sim_agregar_vehiculo(BUGGY, matriz[i][j-1], costos, vehiculos_fila, i, j);
                if (aux_moto.tiempo < aux_buggy.tiempo) {
                    matriz[i][j] = aux_moto;
                    agregar_vehiculo(MOTO, matriz[i-1][j], costos, vehiculos_fila, i, j);
                } else {
                    matriz[i][j] = aux_buggy;
                    agregar_vehiculo(BUGGY, matriz[i][j-1], costos, vehiculos_fila, i, j);
                }
            }
            cout << i << j << endl;
            mostrar_solucion(matriz[i][j], vehiculos_fila, i, j, costos);
            cout << endl;
        }
    }

    //mostrar_solucion(matriz[km][kb], vehiculos_fila, km, kb, costos);
}

// Dado un vehiculo, una solucion de partida y los costos de los vehiculos para cada etapa
// , agrega ese nuevo vehiculo a la solucion y la devuelve
solucion agregar_vehiculo(int vehiculo, solucion s, vector<costoEtapa> costos, vector< pair<int,int> > &vehiculos, int fila, int columna) {
    int n = costos.size();

    int min_etapa = -1;
    int min_tiempo = -1;

    for(int i = 0; i < n; i++) {
        int tiempo = 0;
        if (vehiculos[i].first == MOTO) {
            if (vehiculos[i].second < fila || (vehiculo == BUGGY && vehiculos[i].second <= fila)) {
                tiempo = s.tiempo - costos[i].costo(MOTO) +  costos[i].costo(vehiculo);
            } else {
                tiempo = s.tiempo - costos[i].costo(BMX) +  costos[i].costo(vehiculo);
            }
        } else if (vehiculos[i].first == BUGGY) {
            if (vehiculos[i].second < columna || (vehiculo == MOTO && vehiculos[i].second <= columna)) {
                tiempo = s.tiempo - costos[i].costo(BUGGY) +  costos[i].costo(vehiculo);
            } else {
                tiempo = s.tiempo - costos[i].costo(BMX) +  costos[i].costo(vehiculo);
            }
        } else {
            tiempo = s.tiempo - costos[i].costo(BMX) +  costos[i].costo(vehiculo);
        }

        if (tiempo < min_tiempo || min_tiempo == -1) {
            min_tiempo = tiempo;
            min_etapa = i;
        }
    }

    solucion nueva = s;
    vehiculos[min_etapa].first = vehiculo;
    if (vehiculo == MOTO) {
        vehiculos[min_etapa].second = fila;
    } else {
        vehiculos[min_etapa].second = columna;
    }
    nueva.tiempo = min_tiempo;
    return nueva;
}

solucion sim_agregar_vehiculo(int vehiculo, solucion s, vector<costoEtapa> costos, vector< pair<int,int> > &vehiculos, int fila, int columna) {
    int n = costos.size();

    int min_etapa = -1;
    int min_tiempo = -1;

    for(int i = 0; i < n; i++) {
        int tiempo = 0;
        if (vehiculos[i].first == MOTO) {
            if (vehiculos[i].second < fila || (vehiculo == BUGGY && vehiculos[i].second <= fila)) {
                tiempo = s.tiempo - costos[i].costo(MOTO) +  costos[i].costo(vehiculo);
            } else {
                tiempo = s.tiempo - costos[i].costo(BMX) +  costos[i].costo(vehiculo);
            }
        } else if (vehiculos[i].first == BUGGY) {
            if (vehiculos[i].second < columna || (vehiculo == MOTO && vehiculos[i].second <= columna)) {
                tiempo = s.tiempo - costos[i].costo(BUGGY) +  costos[i].costo(vehiculo);
            } else {
                tiempo = s.tiempo - costos[i].costo(BMX) +  costos[i].costo(vehiculo);
            }
        } else {
            tiempo = s.tiempo - costos[i].costo(BMX) +  costos[i].costo(vehiculo);
        }

        if (tiempo < min_tiempo || min_tiempo == -1) {
            min_tiempo = tiempo;
            min_etapa = i;
        }
    }

    solucion nueva = s;
    nueva.tiempo = min_tiempo;
    return nueva;
}
// Dado un vector de vehiculos y los costos de los vehiculos para cada etapa
// devuelve el costo de realizar el rally.
int tiempo_total(vector< pair<int,int> > &vehiculos, int fila, int columna, vector<costoEtapa> costos) {
    int n = costos.size();
    int tiempo = 0;
    for(int i = 0; i < n; i++) {
        int aux = 0;
        if (vehiculos[i].first == MOTO) {
            if (vehiculos[i].second <= fila) {
                aux =  costos[i].costo(MOTO);
            } else {
                aux = costos[i].costo(BMX);
            }
        } else if (vehiculos[i].first == BUGGY) {
            if (vehiculos[i].second <= columna) {
                aux = costos[i].costo(BUGGY);
            } else {
                aux = costos[i].costo(BMX);
            }
        } else {
            aux = costos[i].costo(BMX);
        }
       tiempo = tiempo + aux;
    }
    return tiempo;
}

// Dada una solucion y los costos de los vehiculos para cada etapa
// muestra por stdout los vehiculos para cada etapa y el tiempo total de la solucion
void mostrar_solucion(solucion s, vector< pair<int,int> > &vehiculos, int fila, int columna, vector<costoEtapa> costos) {
    int n = costos.size();
    for(int i = 0; i < n; i++) {
        string v = "";
        if (vehiculos[i].first == MOTO) {
            if (vehiculos[i].second <= fila) {
                v = "Motocross";
            } else {
                v = "BMX";
            }
        } else if (vehiculos[i].first == BUGGY) {
            if (vehiculos[i].second <= columna) {
                v = "Buggy";
            } else {
                v = "BMX";
            }
        } else {
            v = "BMX";
        }
        cout << "Etapa " << i+1 << ": " << v << endl;
    }
    cout << "Tiempo total: " << s.tiempo << endl;
}
