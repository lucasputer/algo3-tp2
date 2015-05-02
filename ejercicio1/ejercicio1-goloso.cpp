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

// Prototipado de funciones
vector<int> resolver(int n, int km, int kb, vector<costoEtapa>& costos);
int tiempo_total(vector<int> &vehiculos, vector<costoEtapa> &costos);

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

    vector<int> sol = resolver(n, km, kb, costos);

    // muestro la solucion por stdout
    string aux = to_string(tiempo_total(sol, costos));
    aux += " ";
    for(int i = 0; i < n; i++) {
        aux += to_string(sol[i]+1);
        aux += " ";
    }
    aux += "\n";
    cout << aux;
}

vector<int> resolver(int n, int km, int kb, vector<costoEtapa>& costos) {
    // inicializo solucion inicial
    int km_aux = 0;
    int kb_aux = 0;
    vector<int> sol_actual(n, BMX);
    int tiempo_actual = tiempo_total(sol_actual, costos);

    bool seguir = true;
    while (seguir) {
        seguir = false;
        int tiempo_min = tiempo_actual;
        int etapa_min = -1;
        int vehiculo_min = -1;

        for(int i = 0; i < n; i++) {
            int tiempo_bmx = tiempo_actual - costos[i].costo(sol_actual[i]) + costos[i].costo(BMX);
            int tiempo_moto = tiempo_actual - costos[i].costo(sol_actual[i]) + costos[i].costo(MOTO);
            int tiempo_buggy = tiempo_actual - costos[i].costo(sol_actual[i]) + costos[i].costo(BUGGY);

            // elijo el minimo de los 3 tiempos
            if (tiempo_bmx < tiempo_min && tiempo_bmx <= tiempo_moto && tiempo_bmx <= tiempo_buggy) {
                tiempo_min = tiempo_bmx;
                etapa_min = i;
                vehiculo_min = BMX;
            } else if (tiempo_moto < tiempo_min && tiempo_moto <= tiempo_bmx
                            && (tiempo_moto <= tiempo_buggy || kb_aux >= kb)
                            && km_aux < km) {
                tiempo_min = tiempo_moto;
                etapa_min = i;
                vehiculo_min = MOTO;
            }  else if (tiempo_buggy < tiempo_min && tiempo_buggy <= tiempo_bmx
                            && (tiempo_buggy <= tiempo_moto || km_aux >= km)
                            && kb_aux < kb) {
                tiempo_min = tiempo_buggy;
                etapa_min = i;
                vehiculo_min = BUGGY;
            }
        }

        // asigno el nuevo vehiculo
        if (etapa_min != -1) {
            seguir = true;
            tiempo_actual = tiempo_min;
            sol_actual[etapa_min] = vehiculo_min;
            if (vehiculo_min == MOTO) {
                km_aux++;
            } else if (vehiculo_min == BUGGY) {
                kb_aux++;
            }
        }
    }
    return sol_actual;
}

 int tiempo_total(vector<int> &vehiculos, vector<costoEtapa> &costos) {
    int n = costos.size();
    int tiempo = 0;
    for(int i = 0; i < n; i++) {
       tiempo = tiempo + costos[i].costo(vehiculos[i]);
    }
    return tiempo;
}
