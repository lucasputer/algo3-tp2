#include <vector>
#include <iostream>
using namespace std;

// Structs y Typedefs
// para indexar los arrays de costos y otras operaciones con vehiculos
#define BMX        0
#define MOTO       1
#define BUGGY      2

// struct que contiene el costo de una etapa con diferentes vehiculos
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
void mostrar_celda(Tabla &tabla, int fila, int columna, vector<costoEtapa> &costos);
celda nueva_celda(Tabla &tabla, int fila, int columna, int vehiculo, vector<costoEtapa> &costos);

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

    // limito la cantidad de motos a la cantidad de etapas
    if(km > n) {
        km = n;
    }
    // limito la cantidad de buggys a la cantidad de etapas
    if(kb > n) {
        kb = n;
    }

    // cargo los costos
    vector<costoEtapa> costos(n, costoEtapa());
    int bmx, moto, buggy;
    for(int i = 0; i < n; i++) {
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
    mostrar_celda(matriz, km, kb, costos);
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
void mostrar_celda(Tabla &tabla, int fila, int columna, vector<costoEtapa> &costos) {
    int n = costos.size();
    vector<int> vehiculos = obtener_vehiculos_celda(tabla, fila, columna, n);

    cout << tabla[fila][columna].tiempo << " ";
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
        cout << vehiculos[i]+1 << " ";
    }
    //cout << "Tiempo total: " << tabla[fila][columna].tiempo << endl;
    cout << endl;
}
