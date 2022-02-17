#include "tokenizador.h"
#include <fstream>
#include <algorithm>

// Operador salida
ostream& operator<<(ostream& os, const Tokenizador& tokenizador)
{
    os << "DELIMITADORES: " << tokenizador.delimiters <<  " TRATA CASOS ESPECIALES: " << tokenizador.casosEspeciales << " PASAR A MINUSCULAS Y SIN ACENTOS: " << tokenizador.pasarAminuscSinAcentos; 

    return os;
}

// Funcion auxiliar que elimina duplicados de un string
string Tokenizador::eliminaDuplicados(const string &aEliminar)
{
    string aux = aEliminar;

    for(int i = 0; i < aux.size(); i++)
    {
        for(int j = 0; j < aux.size(); j++)
        {
            if(aux[i] == aux[j])
            {
                aux.erase(aux.begin()+j);
            }
        }
    }

    return aux;
}

// Funcion auxiliar de copia
void Tokenizador::copia(const Tokenizador& copia)
{
    this->delimiters = eliminaDuplicados(copia.delimiters);
    this->casosEspeciales = copia.casosEspeciales;
    this->pasarAminuscSinAcentos = copia.pasarAminuscSinAcentos;
}

// Constructor
Tokenizador::Tokenizador(const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos)
{
    this->delimiters = eliminaDuplicados(delimitadoresPalabra);
    this->casosEspeciales = kcasosEspeciales;
    this->pasarAminuscSinAcentos = minuscSinAcentos;
}

// Constructor de copia
Tokenizador::Tokenizador(const Tokenizador& copia)
{
    this->copia(copia);
}

// Constructor por defecto de la clase
Tokenizador::Tokenizador()
{
    this->delimiters = ",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
    this->casosEspeciales = true;
    this->pasarAminuscSinAcentos = false;
}

// Destructor de la clase
Tokenizador::~Tokenizador()
{
    delimiters.clear();
}

// Operador de asignacion
Tokenizador& Tokenizador::operator=(const Tokenizador& tokenizadorParam)
{
    if(this != &tokenizadorParam)
    {
        this->copia(tokenizadorParam);
    }

    return *this;
}

// Versión del tokenizador vista en CLASE
void Tokenizador::Tokenizar(const string& str, list<string>& tokens) const
{
    if(pasarAminuscSinAcentos)
    {
        
    }

    // Comprueba si queremos tokenizar con los casos especiales o no 
    if(!casosEspeciales)
    {
        string::size_type lastPos = str.find_first_not_of(delimiters,0);
        string::size_type pos = str.find_first_of(delimiters,lastPos);
        while(string::npos != pos || string::npos != lastPos)
        {
            tokens.push_back(str.substr(lastPos, pos - lastPos));
            lastPos = str.find_first_not_of(delimiters, pos);
            pos = str.find_first_of(delimiters, lastPos);
        }
    }
    else
    {
        //Hacer mi tokenizar con casos especiales
    }
}

// Versión del tokenizador de ficheros CLASE
bool Tokenizador::Tokenizar(const string& NomFichEntr, const string& NomFichSal) const
{
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

// Cambia delimiters por nuevoDelimiters
void Tokenizador::DelimitadoresPalabra(const string& nuevoDelimiters)
{
    this->delimiters = eliminaDuplicados(nuevoDelimiters);
}

// Añade delimitadores al final de delimiters
void Tokenizador::AnyadirDelimitadoresPalabra(const string& nuevoDelimiters)
{
    this->delimiters += nuevoDelimiters;
    this->delimiters = eliminaDuplicados(this->delimiters);     
}

// Devuelve delimiters
string Tokenizador::DelimitadoresPalabra() const
{
    return this->delimiters;
}

// Cambia casosEspeciales
void Tokenizador::CasosEspeciales(const bool& nuevoCasosEspeciales)
{
    casosEspeciales = nuevoCasosEspeciales;
}

// Devuelve la variable casos especiales
bool Tokenizador::CasosEspeciales() 
{
    return casosEspeciales;
}

// Cambia la variable pasaMinusculas...
void Tokenizador::PasarAminuscSinAcentos(const bool& nuevoPasarAminuscSinAcentos)
{
    this->pasarAminuscSinAcentos = nuevoPasarAminuscSinAcentos;
}

// Devuelve la variable pasaMinusculas...
bool Tokenizador::PasarAminuscSinAcentos()
{
    return pasarAminuscSinAcentos;
}