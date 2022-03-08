#include <iostream> 
#include <string>
#include <list> 
#include "tokenizador.h"

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
    Tokenizador a("-#", true, false);
    list<string> tokens;
    a.DelimitadoresPalabra("/ ");
    a.Tokenizar("MS-DOS p1 p2", tokens);
    imprimirListaSTL(tokens);
    // La lista de tokens a devolver debería contener (MS-DOS no se detectaría
    // como multipalabra, sino como token normal ya que el – no es delimitador): "MS-
    // DOS, p1, p2"
    a.Tokenizar("pal1 -MS-DOS p1 p2", tokens);
    imprimirListaSTL(tokens);
    // La lista de tokens a devolver debería contener (no se detectaría como
    // multipalabra, sino como token normal ya que el – no es delimitador, no quita
    // el primer guion porque no está entre los delimitadores): "pal1, -MS-DOS, p1, p2"
    a.Tokenizar("pal1 MS-DOS#p3 p1 p2", tokens);
    imprimirListaSTL(tokens);
    // La lista de tokens a devolver debería contener (no separa #p3 porque # no
    // está entre los delimitadores): "pal1, MS-DOS#p3, p1, p2"
    a.Tokenizar("pal1#MS-DOS#p3 p1 p2", tokens);
    imprimirListaSTL(tokens);
    // La lista de tokens a devolver debería contener: "pal1#MS-DOS#p3, p1, p2"
    a.Tokenizar("pal1 -MS-DOS- p1 p2", tokens);
    imprimirListaSTL(tokens);
    // La lista de tokens a devolver debería contener (no se detectaría como
    // multipalabra, sino como token normal ya que el – no es delimitador): "pal1, -MS-DOS-, p1, p2"
    a.Tokenizar("pal1 MS-DOS--TRES p1 p2", tokens);
    imprimirListaSTL(tokens);
    // La lista de tokens a devolver debería contener (no se detectaría como
    // multipalabra, sino como token normal ya que el – no es delimitador): "pal1, MS-DOS--TRES, p1, p2"
    a.Tokenizar("pal1 MS-DOS-TRES--- p1 p2", tokens);
    imprimirListaSTL(tokens);
    // La lista de tokens a devolver debería contener (no se detectaría como
    // multipalabra, sino como token normal ya que el – no es delimitador): "pal1, MS-DOS-TRES---, p1, p2"
}