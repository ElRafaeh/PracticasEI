#include <iostream>
#include "tokenizador.h"

using namespace std;

int main()
{
    Tokenizador tokenizador;
    cout << tokenizador.DelimitadoresPalabra() << endl;
    string a = "tumama";

    tokenizador.AnyadirDelimitadoresPalabra(a);

    cout << tokenizador.DelimitadoresPalabra() << endl;

    tokenizador.DelimitadoresPalabra(a);

    cout << tokenizador.DelimitadoresPalabra() << endl;
    
    return 0;
}