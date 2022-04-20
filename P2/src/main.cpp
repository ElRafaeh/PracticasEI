#include <iostream> 
#include <string>
#include <list> 
#include "tokenizador.h"
#include "indexadorInformacion.h"

using namespace std;

///////// Comprobaci�n de que vac�e la lista resultado

void imprimirListaSTL(const list<string>& cadena)
{
    list<string>::const_iterator itCadena;
    for(itCadena=cadena.begin();itCadena!=cadena.end();itCadena++)
    {
        cout << (*itCadena) << ", ";
    }
    cout << endl;
}

int
main(void)
{
    Fecha fecha;
    tm pene = fecha.getFecha();
    cout << asctime(&pene) << endl;
    fecha.~Fecha();
    pene = fecha.getFecha();
    cout << asctime(&pene) << endl;
}
