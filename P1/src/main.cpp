#include <iostream>
#include "../include/tokenizador.h"

using namespace std;

int main()
{
    Tokenizador tokenizador("...///sssdddxxsxzzzrrr", true, false);
    tokenizador.PasarAminuscSinAcentos(true);
    list<string> tokens;
    tokenizador.Tokenizar("hola Yo Me llamo Ráfa", tokens);
    cout << tokenizador << endl;
    tokenizador.AnyadirDelimitadoresPalabra("@/.");
    cout << tokenizador << endl;
    tokenizador.AnyadirDelimitadoresPalabra("@/.");
    cout << tokenizador << endl;
    return 0;
}