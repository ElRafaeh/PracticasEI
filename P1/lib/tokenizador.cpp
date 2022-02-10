#include "tokenizador.h"
#include <fstream>


// Versión del tokenizador vista en CLASE
void Tokenizador::Tokenizar(const string& str, list<string>& tokens) const{
    string::size_type lastPos = str.find_first_not_of(delimiters,0);
    string::size_type pos = str.find_first_of(delimiters,lastPos);
    while(string::npos != pos || string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

// Versión del tokenizador de ficheros CLASE
bool Tokenizador::Tokenizar(const string& NomFichEntr, const string& NomFichSal) const{
    ifstream entry;
    ofstream exit;
    string cadena;
    list<string> tokens;

    entry.open(NomFichEntr.c_str());
    
    if(!entry) {
        cerr << "ERROR: No existe el archivo: " << NomFichEntr << endl;
        return false;
    }
    else
    {
        while(!entry.eof())
        {
            cadena="";
            getline(entry, cadena);
            if(cadena.length()!=0)
            {
                Tokenizar(cadena, tokens);
            }
        }
    }

    entry.close();
    exit.open(NomFichSal.c_str());
    list<string>::iterator itS;
    
    for(itS= tokens.begin();itS!= tokens.end();itS++)
    {
        exit << (*itS) << endl;
    }

    exit.close();

    return true;
} 

// Versión acceso a
