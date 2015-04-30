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

struct cout_redirect {
    cout_redirect( std::streambuf * new_buffer )
        : old( std::cout.rdbuf( new_buffer ) )
    { }

    ~cout_redirect( ) {
        std::cout.rdbuf( old );
    }

    void to_stdout( ) {
        std::cout.rdbuf( old );
    }

    void to_other(std::streambuf * new_buffer) {
        std::cout.rdbuf( new_buffer );
    }

private:
    std::streambuf * old;
};

typedef pair<int, int> Coord;

string resolver_bruto(int n, int km, int kb, vector<costoEtapa> costos);

// Implementacion
int main() {
    srand(time(NULL));

    std::stringstream buffer;
    cout_redirect cout_redir(buffer.rdbuf());

    int n_max = 100;
    int costo_max_bmx = 1000;
    int muestras_por_n = 1;
    int muestras_por_km_kb = 1;

    for (int n = 1; n <= n_max; n++) {
        for (int m = 0; m < muestras_por_n; m++) {
            double sum = 0;
            int km = (rand()%n)+1;
            int kb = (rand()%n)+1;
            string program = "echo '";
            program += to_string(n);
            program += " ";
            program += to_string(km);
            program += " ";
            program += to_string(kb);
            program += " ";
            vector<costoEtapa> costos(n, costoEtapa());
            for (int aux = 0; aux < n; aux++) {
                int bmx = (rand()%costo_max_bmx)+1;
                int moto = rand()%bmx;
                int buggy = rand()%bmx;
                program += to_string(bmx);
                program += " ";
                program += to_string(moto);
                program += " ";
                program += to_string(buggy);
                program += " ";
                costos[aux].bmx = bmx;
                costos[aux].moto = moto;
                costos[aux].buggy = buggy;
            }
            string resultado_bruto = resolver_bruto(n, km, kb, costos);
            program += "' | ./ejercicio1";
            system(program.c_str());
            string resultado_ej1 = buffer.str();
            cout_redir.to_stdout();
            if (resultado_bruto == resultado_ej1) {
                cout << "BIEN" << endl;
            } else {
                cout << "MAL" << endl;
            }
            cout_redir.to_other(buffer.rdbuf());
        }
    }

    return 0;
}


string resolver_bruto(int n, int km, int kb, vector<costoEtapa> costos) {
    return "";
}
