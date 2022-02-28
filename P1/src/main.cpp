#include <iostream>
#include "../include/tokenizador.h"


using namespace std;


void multiplabra(const string delimiters, const string& str, list<string> &tokens, string::size_type &lastPos,string::size_type &pos, const string::size_type inicio)
{
    if(str[pos] == '-')
    {
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
        if (pos != string::npos && str[pos] == '-')
        {
            multiplabra(delimiters, str, tokens, lastPos, pos, inicio);
        }
    }
    tokens.push_back(str.substr(inicio, pos - inicio));
}

int main()
{
    Tokenizador tokenizador;
    tokenizador.CasosEspeciales(true);
    list<string> tokens;
    tokenizador.Tokenizar("catedraTelefonicaUA@iuii.ua.es p1 p2", tokens);
    cout << tokenizador << endl;
    tokenizador.AnyadirDelimitadoresPalabra("@/.");
    cout << tokenizador << endl;
    tokenizador.AnyadirDelimitadoresPalabra("@/.");
    cout << tokenizador << endl;

    string delimiters = tokenizador.DelimitadoresPalabra();
    string str = "MS-DOS";


    string s = "p0 http://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date=22-01-2013&newspaper=catedraTelefonicaUA@iuii.ua.es http://ua.es p1 p2";
    string::size_type pene = s.find("http:", 6);
    cout << endl;

    return 0;
}




