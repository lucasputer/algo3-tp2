#include <vector>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
using namespace std;

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

string resolver_bruto(int n, int km, int kb, vector<costoEtapa>& costos);
vector<int> bfs_bruto(int n, int h, int km_aux, int kb_aux,
    vector<int> vehiculos, int km, int kb, vector<costoEtapa>& costos);
int tiempo_total_aux(vector<int> vehiculos, vector<costoEtapa> &costos);
string resolver_ej1(int n, int km, int kb, vector<costoEtapa>& costos);


/********************** EJERCICIO 1 (sin main) ************************************/

// celda de la Tabla
struct celda {

    celda() : tiempo(0), vehiculo(BMX), etapa(0), antecesor(-1,-1) {}

    // antecesor son las coordenadas de la celda sobre la cual se basa esta solucion
    pair<int,int> antecesor;
    // la etapa que se modifica en esta celda
    int etapa;
    // el vehiculo que se agrega en esta celda
    int vehiculo;
    // el tiempo total de la carrera que se obtiene al poner el vehiculo en la etapa
    int tiempo;
};

typedef vector<celda> Vec;
typedef vector<Vec> Tabla;

// Prototipado de funciones
int tiempo_total(Tabla &tabla, int fila, int columna, vector<costoEtapa> &costos);
vector<int> obtener_vehiculos_celda(Tabla &tabla, int fila, int columna, int n);
string mostrar_celda(Tabla &tabla, int fila, int columna, vector<costoEtapa> &costos);
celda nueva_celda(Tabla &tabla, int fila, int columna, int vehiculo, vector<costoEtapa> &costos);

string resolver_ej1(int n, int km, int kb, vector<costoEtapa>& costos) {

    // creo la tabla de celdaes parciales
    Tabla matriz(km+1, Vec(kb+1, celda()));

    // La celda (0,0) ya tiene inicializada todas las etapas usando BMX
    // Calculamos la sumatoria de los tiempos para la celda inicial
    matriz[0][0].tiempo = tiempo_total(matriz, 0, 0, costos);
    //mostrar_celda(matriz, 0, 0, costos);

    // calculo la primera fila
    for(int i = 1; i <= kb; i++) {
        matriz[0][i] = nueva_celda(matriz, 0, i-1, BUGGY, costos);
    }
    //mostrar_celda(matriz, 0, 1, costos);

    // calculo la primera columna
    for(int i = 1; i <= km; i++) {
        matriz[i][0] = nueva_celda(matriz, i-1, 0, MOTO, costos);
    }
    //mostrar_celda(matriz, 1, 0, costos);
    //mostrar_celda(matriz, 2, 0, costos);

    // calculo todas las celdas internas de la tabla
    for(int i = 1; i <= km; i++) {
        for(int j = 1; j <= kb; j++) {
            // comparo agregar una moto a la celda superior contra
            // agregar un buggy a la celda de la izquierda
            celda aux_moto = nueva_celda(matriz, i-1, j, MOTO, costos);
            celda aux_buggy = nueva_celda(matriz, i, j-1, BUGGY, costos);
            // y elijo la que minimiza el tiempo total del rally
            if (aux_moto.tiempo < aux_buggy.tiempo) {
                matriz[i][j] = aux_moto;
            } else {
                matriz[i][j] = aux_buggy;
            }
            //mostrar_celda(matriz, i, j, costos);
        }
    }

    // muestro la ultima celda de la matriz que tiene la solucion
    return mostrar_celda(matriz, km, kb, costos);
}

/**
 * Crea una nueva celda agregando un vehiculo a una celda base.
 * La nueva celda tiene un tiempo total menor o igual que la celda base.
 *
 * param tabla: la matriz de soluciones parciales.
 * param fila: la fila de la celda base en la tabla.
 * param columna: la columna de la celda base en la tabla.
 * param costos: vector de costos para cada vehiculo en cada etapa.
 */
celda nueva_celda(Tabla &tabla, int fila, int columna, int vehiculo, vector<costoEtapa> &costos) {
    int n = costos.size();
    vector<int> vehiculos = obtener_vehiculos_celda(tabla, fila, columna, n);

    int min_etapa = -1;
    int min_tiempo = tabla[fila][columna].tiempo;

    for(int i = 0; i < n; i++) {
        int tiempo = tabla[fila][columna].tiempo - costos[i].costo(vehiculos[i]) +  costos[i].costo(vehiculo);
        if (tiempo < min_tiempo) {
            min_tiempo = tiempo;
            min_etapa = i;
        }
    }

    if (min_etapa != -1) {
        celda nueva;
        nueva.tiempo = min_tiempo;
        nueva.etapa = min_etapa;
        nueva.vehiculo = vehiculo;
        nueva.antecesor.first = fila;
        nueva.antecesor.second = columna;
        return nueva;
    } else {
        celda nueva;
        nueva.tiempo = tabla[fila][columna].tiempo;
        nueva.etapa = tabla[fila][columna].etapa;
        nueva.vehiculo = tabla[fila][columna].vehiculo;
        nueva.antecesor.first = fila;
        nueva.antecesor.second = columna;
        return nueva;
    }
}

/**
 * Devuelve el tiempo total que toma la solucion parcial para una celda dada.
 *
 * param tabla: la matriz de soluciones parciales.
 * param fila: la fila de la celda en la tabla.
 * param columna: la columna de la celda en la tabla.
 * param costos: vector de costos para cada vehiculo en cada etapa.
 */
int tiempo_total(Tabla &tabla, int fila, int columna, vector<costoEtapa> &costos) {
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

/**
 * Devuelve los vehiculos de la solucion parcial para una celda dada.
 * Esto se realiza navegando hacia atras desde la celda dada hasta la celda (0,0)
 * usando los atributos "antecesor".
 *
 * param tabla: la matriz de soluciones parciales.
 * param fila: la fila de la celda en la tabla.
 * param columna: la columna de la celda en la tabla.
 * param n: la cantidad de etapas del problema.
 */
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

/**
 * Muestra por stdout la solucion parcial para una celda dada.
 *
 * param tabla: la matriz de soluciones parciales.
 * param fila: la fila de la celda en la tabla.
 * param columna: la columna de la celda en la tabla.
 * param costos: vector de costos para cada vehiculo en cada etapa.
 */
string mostrar_celda(Tabla &tabla, int fila, int columna, vector<costoEtapa> &costos) {
    int n = costos.size();
    vector<int> vehiculos = obtener_vehiculos_celda(tabla, fila, columna, n);


    //cout << tabla[fila][columna].tiempo << " ";
    string aux = to_string(tabla[fila][columna].tiempo);
    aux += " ";
    for(int i = 0; i < n; i++) {
        string v = "";
        if (vehiculos[i] == MOTO) {
            v = "Motocross";
        } else if (vehiculos[i] == BUGGY) {
            v = "Buggy";
        } else {
            v = "BMX";
        }
        //cout << "Etapa " << i+1 << ": " << v << endl;
        //cout << vehiculos[i]+1 << " ";
        aux += to_string(vehiculos[i]+1);
        aux += " ";
    }
    //cout << "Tiempo total: " << tabla[fila][columna].tiempo << endl;
    //cout << endl;
    aux += "\n";
    return aux;
}


/********************** fin EJERCICIO 1 ************************************/


// Implementacion
int main() {
    srand(time(NULL));

    int n_max = 10;
    int costo_max = 10000;
    int muestras_por_n = 1000;

    for (int n = 1; n <= n_max; n++) {
        cout << n << endl;
        for (int m = 0; m < muestras_por_n; m++) {
            int km = (rand()%n)+1;
            int kb = (rand()%n)+1;
            vector<costoEtapa> costos(n, costoEtapa());
            for (int aux = 0; aux < n; aux++) {
                int bmx = (rand()%costo_max)+1;
                int moto = (rand()%costo_max)+1;
                int buggy = (rand()%costo_max)+1;
                costos[aux].bmx = bmx;
                costos[aux].moto = moto;
                costos[aux].buggy = buggy;
            }
            string resultado_bruto = resolver_bruto(n, km, kb, costos);
            string resultado_ej1 = resolver_ej1(n, km, kb, costos);
            if (resultado_bruto.compare(resultado_ej1) == 0) {
                //cout << "BIEN" << endl;
            } else {
                // Veo si de verdad difierien en los tiempos, o solamente es una diferencia de orden
                std::stringstream  stream1(resultado_bruto);
                std::string tiempo_bruto;
                stream1 >> tiempo_bruto;

                std::stringstream  stream2(resultado_ej1);
                std::string tiempo_ej1;
                stream2 >> tiempo_ej1;

                if (tiempo_bruto.compare(tiempo_ej1) != 0) {
                    cout << "\nDIFERENCIA ENCONTRADA" << endl;
                    cout << "Problema: " << n << " " << km << " " << kb << endl;
                    for(int i = 0; i < n; i++) {
                        cout << costos[i].costo(BMX) << " ";
                        cout << costos[i].costo(MOTO) << " ";
                        cout << costos[i].costo(BUGGY) << endl;
                    }
                    cout << "Bruto: " << resultado_bruto;
                    cout << "Ej1: " << resultado_ej1;
                }
            }
        }
    }

    return 0;
}

int tiempo_total_aux(vector<int> vehiculos, vector<costoEtapa> &costos) {
    int n = vehiculos.size();
    int tiempo = 0;
    for(int i = 0; i < n; i++) {
       tiempo = tiempo + costos[i].costo(vehiculos[i]);
    }
    return tiempo;
}

string resolver_bruto(int n, int km, int kb, vector<costoEtapa>& costos) {
    // solucion inicial
    vector<int> vehiculos(n, BMX);

    vector<int> sol = bfs_bruto(n, 0, 0, 0, vehiculos, km, kb, costos);

    //cout << tabla[fila][columna].tiempo << " ";
    string aux = to_string(tiempo_total_aux(sol, costos));
    aux += " ";
    for(int i = 0; i < n; i++) {
        string v = "";
        if (sol[i] == MOTO) {
            v = "Motocross";
        } else if (sol[i] == BUGGY) {
            v = "Buggy";
        } else {
            v = "BMX";
        }
        //cout << "Etapa " << i+1 << ": " << v << endl;
        //cout << vehiculos[i]+1 << " ";
        aux += to_string(sol[i]+1);
        aux += " ";
    }
    //cout << "Tiempo total: " << tabla[fila][columna].tiempo << endl;
    //cout << endl;
    aux += "\n";
    return aux;
}

vector<int> bfs_bruto(int n, int h, int km_aux, int kb_aux,
    vector<int> vehiculos, int km, int kb, vector<costoEtapa>& costos) {

    if (h >= n || (km_aux >= km && kb_aux >= kb)) {
        return vehiculos;
    }

    vector<int> sol_bmx = bfs_bruto(n, h+1, km_aux, kb_aux, vehiculos, km, kb, costos);
    vector<int> sol_moto = sol_bmx;
    if (km_aux < km) {
        vehiculos[h] = MOTO;
        sol_moto = bfs_bruto(n, h+1, km_aux + 1, kb_aux, vehiculos, km, kb, costos);
    }
    vector<int> sol_buggy = sol_bmx;
    if (kb_aux < kb) {
        vehiculos[h] = BUGGY;
        sol_buggy = bfs_bruto(n, h+1, km_aux, kb_aux + 1, vehiculos, km, kb, costos);
    }

    int tiempo_bmx = tiempo_total_aux(sol_bmx, costos);
    int tiempo_moto = tiempo_total_aux(sol_moto, costos);
    int tiempo_buggy = tiempo_total_aux(sol_buggy, costos);

    // elijo el minimo
    if (tiempo_bmx < tiempo_moto) {
        if (tiempo_bmx < tiempo_buggy) {
            return sol_bmx;
        } else {
            return sol_buggy;
        }
    } else {
        if (tiempo_moto < tiempo_buggy) {
            return sol_moto;
        } else {
            return sol_buggy;
        }
    }

}
