#include <iostream>
#include "../include/tokenizador.h"

using namespace std;

int main()
{
    Tokenizador tokenizador;
    
    cout << tokenizador.eliminaDuplicados("abaababbba") << endl;

    return 0;
}