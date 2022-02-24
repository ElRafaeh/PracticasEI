#include <iostream>
#include "../include/tokenizador.h"

using namespace std;

int main()
{
    Tokenizador tokenizador;
    tokenizador.PasarAminuscSinAcentos(true);
    list<string> tokens;
    tokenizador.Tokenizar("hola Yo Me llamo Ráfa", tokens);
    cout << tokenizador << endl;

    return 0;
}