#include <iostream>
#include "../include/tokenizador.h"


using namespace std;

int main()
{
    Tokenizador tokenizador("...///sssdddxxsxzzzrrr", true, false);
    tokenizador.PasarAminuscSinAcentos(true);
    list<string> tokens;
    tokenizador.Tokenizar("h��l M� ��� ll�am� pedro", tokens);
    cout << tokenizador << endl;
    tokenizador.AnyadirDelimitadoresPalabra("@/.");
    cout << tokenizador << endl;
    tokenizador.AnyadirDelimitadoresPalabra("@/.");
    cout << tokenizador << endl;
    return 0;
}