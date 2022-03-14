#include "tokenizador.h"
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>

// Operador salida
ostream& operator<<(ostream& os, const Tokenizador& tokenizador)
{
    os << "DELIMITADORES: " << tokenizador.delimiters <<  " TRATA CASOS ESPECIALES: " << tokenizador.casosEspeciales << " PASAR A MINUSCULAS Y SIN ACENTOS: " << tokenizador.pasarAminuscSinAcentos; 

    return os;
}

// Funcion auxiliar que elimina duplicados de un string
string Tokenizador::eliminaDuplicados(string aEliminar) const
{
    unordered_map<int, char> stringHash;
    string::iterator it;
    string aux = aEliminar;
    
    for(it = aEliminar.begin(); it != aEliminar.end(); it++)
    {
        if(!(stringHash.insert(make_pair(*it, *it)).second))
        {
            aux.erase(aux.begin() + aux.find_last_of(*it));
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
    this->delimiters = eliminaDuplicados(this->delimiters);
}

// Constructor de copia
Tokenizador::Tokenizador(const Tokenizador& copia)
{
    this->copia(copia);
    this->delimiters = eliminaDuplicados(this->delimiters);
}

// Constructor por defecto de la clase
Tokenizador::Tokenizador()
{
    this->delimiters = ",;:.-/+*\\ '\"{}[]()<>?!??&#=\t\n\r@";
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

// Funcion que devuelve el car?cter pasado a minuscula
char normalizarCaracter(char car)
{
    char aux;

    switch (car)
    {   
        case '\300'...'\305': case '\340'...'\345':
            return 'a';
            break;
        case '\310'...'\313': case '\350'...'\353':
            return 'e';
            break;
        case '\314'...'\317': case '\354'...'\357':
            return 'i';
            break;
        case '\321':
            return '\361';
        case '\322'...'\326': case '\362'...'\366':
            return 'o';
            break;
        case '\331'...'\334': case '\371'...'\374':
            return 'u';
            break;
        default:
            return car;
            break;
    }
}


// Funcion auxiliar que pasa un string pasado a otro sin minusculas y sin acentos
string Tokenizador::convertirSinMayusSinAcen(string str) const
{
    string minusculas;
    string::iterator it;

    for(it = str.begin(); it != str.end(); it++)
    {
        minusculas += tolower(normalizarCaracter(*it));               
    }

    return minusculas;
}

// Funcion tokenizar sin casos especiales
void Tokenizador::TokenizarSinCasosEspeciales(const string& str, list<string>& tokens) const
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

// Funcion que devuelve los delimitadores habiendole quitados los delimitadores pasador por argumento
string Tokenizador::quitarEspeciales(const string &especiales, const string &delimiters) const
{
    string aux = delimiters;
    string::size_type pos = aux.find_first_of(especiales);

    while(pos != string::npos)
    {
        aux.erase(aux.begin() + pos);
        pos = aux.find_first_of(especiales);
    }

    return aux;
}

// Funci?n para sacar url
bool Tokenizador::casoUrl(list<string> &tokens, const string &str, string::size_type &pos, string::size_type &lastPos, const string &delimitadoresUrl, const string &delimiters) const
{
    if (str.find("http:", lastPos) == lastPos || str.find("https:", lastPos) == lastPos || str.find("ftp:", lastPos) == lastPos)
    {
        char siguienteAHtpp = str[str.find_first_of(":", lastPos) + 1];
        bool sigueCaracter = (delimitadoresUrl.find(siguienteAHtpp) && siguienteAHtpp != '\0');

        // Si despu?s de los dos puntos no le sigue ningun caracter, no se considera URL
        if(!sigueCaracter) return false;

        pos = str.find_first_of(delimitadoresUrl, lastPos);
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);

        return true;
    }

    return false;
}

// Funcion para verificar si es decimal o no y guardar el token correspondiente
bool Tokenizador::casoDecimal(list<string> &tokens, const string &str, string::size_type &pos, string::size_type &lastPos, const string &delimitersDecimal, const string &delimiters) const
{
    string::size_type posAnterior = delimiters.find(str[pos-1]), posPosterior = delimiters.find(str[pos+1]), posAux= str.find_first_of(delimitersDecimal, lastPos), almacenaPrimerLastPos = lastPos, almacenaPrimerPos = pos;;
    string puntoComa = ".,", tokenAcumulador, numeros = "0123456789";

    while((puntoComa.find(str[pos]) != string::npos || puntoComa.find(str[almacenaPrimerLastPos-1]) != string::npos) && (posPosterior == string::npos) && (posAnterior == string::npos) && (str[pos+1] != '\0') && (str[pos-1] != '\0'))
    {         
        string siguienteAacumular = str.substr(lastPos, pos-lastPos);

        // Miramos si el n?mero empieza por punto o coma para a?adirle el 0 del principio
        if (puntoComa.find(str[almacenaPrimerLastPos-1]) != string::npos && lastPos == almacenaPrimerLastPos && str[almacenaPrimerLastPos-1] != '\0')
        {
            string ceroComa = "0";
            ceroComa += str[almacenaPrimerLastPos-1];
            tokenAcumulador += ceroComa;
        }

        // Miramos si el substring de despues del punto tiene algun caracter distinto a un n?mero
        if(siguienteAacumular.find_first_not_of(numeros) != string::npos) 
        { 
            lastPos = almacenaPrimerLastPos;
            pos = almacenaPrimerPos;
            return false;
        }

        if(str[pos] == '.' || str[pos] == ',')
        {
            tokenAcumulador += siguienteAacumular + str[pos];
        }
        else
        {
            tokenAcumulador += siguienteAacumular;
            tokens.push_back(tokenAcumulador);
            lastPos = str.find_first_not_of(delimiters, pos);
            pos = str.find_first_of(delimiters, lastPos);
            return true;
        }

        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);

        posAnterior = delimiters.find(str[pos-1]);
        posPosterior = delimiters.find(str[pos+1]);

        if (pos == posAux || posPosterior != string::npos || posAnterior != string::npos || str[pos+1] == '\0')
        {
            siguienteAacumular = str.substr(lastPos, pos-lastPos);
            string caracteresFinales = "%$", caracterFinal;
            bool almacenaCaracter = false;

            if(siguienteAacumular.find_first_not_of(numeros) != string::npos) 
            { 
                if (siguienteAacumular.find_first_of(caracteresFinales) != siguienteAacumular.size()-1)
                {
                    lastPos = almacenaPrimerLastPos;
                    pos = almacenaPrimerPos;
                    return false;
                }
                else
                {
                    caracterFinal = siguienteAacumular[siguienteAacumular.size()-1];
                    almacenaCaracter = true;
                    siguienteAacumular.erase(siguienteAacumular.begin() + siguienteAacumular.size()-1);
                }
            }

            tokenAcumulador += siguienteAacumular;
            tokens.push_back(tokenAcumulador);

            if(almacenaCaracter)
            {
                tokens.push_back(caracterFinal);
            }

            lastPos = str.find_first_not_of(delimiters, pos);
            pos = str.find_first_of(delimiters, lastPos);
            
            return true;
        }
    }

    if(puntoComa.find(str[almacenaPrimerLastPos-1]) != string::npos)
    {
        string ceroComa = "0", siguienteAacumular = str.substr(lastPos, pos-lastPos);
        ceroComa += str[almacenaPrimerLastPos-1];

        // Miramos si el substring de despu?s del punto tiene algun caracter distinto a un n?mero
        if(siguienteAacumular.find_first_not_of(numeros) != string::npos) 
        { 
            lastPos = almacenaPrimerLastPos;
            pos = almacenaPrimerPos;
            return false;
        }

        tokens.push_back(ceroComa + siguienteAacumular);
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
        return true;
    }

    return false;
}

// Funcion para verificar si es email o no y guardar el token correspondiente
bool Tokenizador::casoEmail(list<string> &tokens, const string &str, string::size_type &pos, string::size_type &lastPos, const string &delimitadoresEmail, const string &delimiters) const
{
    string::size_type posAnterior = delimiters.find(str[pos-1]), posPosterior = delimiters.find(str[pos+1]);
    bool condicionMenosDeUnDelimitador = (str[str.find_first_of(delimitadoresEmail, pos+1)] == ' ') || (str[str.find_first_of(delimitadoresEmail, pos+1)] == '\n') || (str[str.find_first_of(delimitadoresEmail, pos+1)] == '\0');

    if((str[pos] == '@') && (posPosterior == string::npos) && (posAnterior == string::npos) && (str[pos+1] != '\0') && condicionMenosDeUnDelimitador)  
    {   
        string::size_type posAux = str.find_first_of(delimitadoresEmail, pos+1), siguienteDelim = str.find_first_of(delimiters, pos+1);

        if(delimiters.find(str[siguienteDelim+1]) != string::npos)
        {
            posAux = siguienteDelim;
        }

        tokens.push_back(str.substr(lastPos, posAux - lastPos)); 

        lastPos = str.find_first_not_of(delimiters, posAux);    
        pos = str.find_first_of(delimiters, lastPos);

        return true;
    }

    return false;
}

// Funcion para verificar si es acronimo o multipalabra o no y guardar el token correspondiente
bool Tokenizador::casoAcronimoYMulti(const char car, list<string> &tokens, const string &str, string::size_type &pos, string::size_type &lastPos, const string &delimitadoresAcronimOMulti, const string &delimiters) const
{
    string::size_type posAnterior = delimiters.find(str[pos-1]), posPosterior = delimiters.find(str[pos+1]), posAux= str.find_first_of(delimitadoresAcronimOMulti, lastPos);
    string tokenAcumulador;

    while((str[pos] == car) && (posPosterior == string::npos) && (posAnterior == string::npos) && (str[pos+1] != '\0') && (str[pos-1] != '\0'))
    {         
        tokenAcumulador += str.substr(lastPos, pos-lastPos) + str[pos];

        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
   
        posAnterior = delimiters.find(str[pos-1]);
        posPosterior = delimiters.find(str[pos+1]);

        if (pos == posAux || posPosterior != string::npos || posAnterior != string::npos || str[pos+1] == '\0')
        {
            tokenAcumulador += str.substr(lastPos, pos-lastPos);
            tokens.push_back(tokenAcumulador);
            lastPos = str.find_first_not_of(delimiters, pos);
            pos = str.find_first_of(delimiters, lastPos);
            
            return true;
        }
    }

    return false;
}

// Funcion tokenizar con casos especiales
void Tokenizador::TokenizarCasosEspeciales(const string &str, list<string> &tokens, const string &delimiters) const
{
    string::size_type lastPos = str.find_first_not_of(delimiters,0);
    string::size_type pos = str.find_first_of(delimiters,lastPos);
    string delimitadoresUrl = quitarEspeciales("_:/.?&-=#@", delimiters), delimitadoresDecimal = quitarEspeciales(".,%$", delimiters), delimitadoresEmail = quitarEspeciales(".-_", delimiters), delimitadoresAcronim = quitarEspeciales(".", delimiters), delimitadoresMulti = quitarEspeciales("-", delimiters);
    
    while(string::npos != pos || string::npos != lastPos)
    {
        if(!casoUrl(tokens, str, pos, lastPos, delimitadoresUrl, delimiters))
        {
            if(!casoDecimal(tokens, str, pos, lastPos, delimitadoresDecimal, delimiters))
            {
                if(!casoEmail(tokens, str, pos, lastPos, delimitadoresEmail, delimiters))
                {
                    if(!casoAcronimoYMulti('.', tokens, str, pos, lastPos, delimitadoresAcronim, delimiters))
                    {
                        if(!casoAcronimoYMulti('-', tokens, str, pos, lastPos, delimitadoresMulti, delimiters))
                        {
                            tokens.push_back(str.substr(lastPos, pos - lastPos));
                            lastPos = str.find_first_not_of(delimiters, pos);
                            pos = str.find_first_of(delimiters, lastPos);
                        }
                    }
                }
            }
        }
    }
}

// Funcion tokenizar que devuelve una lista de tokens
void Tokenizador::Tokenizar(string str, list<string>& tokens) const
{
    // Borramos la lista de tokens
    tokens.clear();

    // Hacemos una copia de la variable privada delimiters y le metemos los delimitadors espacio y salto de l�nea
    string delimiters = eliminaDuplicados(this->delimiters + " \n");

    // Comprobamos si la variable booleana para pasar a minuscula y sin acentos esta activa o no
    if(pasarAminuscSinAcentos) { str = convertirSinMayusSinAcen(str); }

    // Comprueba si queremos tokenizar con los casos especiales o no 
    casosEspeciales ? TokenizarCasosEspeciales(str, tokens, delimiters) : TokenizarSinCasosEspeciales(str, tokens);  
}

// Versi?n del tokenizador de ficheros CLASE
bool Tokenizador::Tokenizar(const string& NomFichEntr, const string& NomFichSal) const
{
    ifstream entry;
    ofstream exit;
    string cadena;
    list<string> tokens;
    string aux = "";
    
    entry.open(NomFichEntr.c_str());
    if(!entry) {
        cerr << "ERROR: No existe el archivo: " << NomFichEntr << endl;
        return false;
    }
    else
    {
        while(getline(entry, cadena))
        {
            list<string>::iterator itS;

            if(cadena.length()!=0)
            {
                Tokenizar(cadena, tokens);
            }

            for(itS= tokens.begin();itS!= tokens.end();itS++)
            {
                aux += (*itS) + "\n";
            }

            cadena="";
        }
    }

    entry.close(); 
    exit.open((NomFichSal).c_str());
    if (exit)
    {
        exit << aux;
        exit.close();
    }
    return true;
} 

// Funcion tokenizar un fichero y ponerle el mismo nombre al de salida que entrada
bool Tokenizador::Tokenizar(const string &i) const
{
    return Tokenizar(i, i+".tk");
}

// Funcion para tokenizar una lista de ficheros
bool Tokenizador::TokenizarListaFicheros(const string &i) const
{
    ifstream entry;
    string cadena;
    list<string> tokens;
    bool devolver = true;

    entry.open(i.c_str());

    if(!entry) {
        cerr << "ERROR: No existe el archivo: " << i << endl;
        return false;
    }
    else
    {
        while(getline(entry, cadena))
        {
            if(!Tokenizar(cadena))
            {
                devolver = false;
            }
            cadena = "";
        }
    }
    entry.close();

    return devolver;
}

// Funcion para tokenizar un directorio
bool Tokenizador::TokenizarDirectorio (const string& dirAIndexar) const {
    struct stat dir;
    // Compruebo la existencia del directorio
    int err=stat(dirAIndexar.c_str(), &dir);
    if(err==-1 || !S_ISDIR(dir.st_mode))
        return false;
    else {
        // Hago una lista en un fichero con find>fich
        string cmd="find "+dirAIndexar+" -follow |sort > .lista_fich";
        system(cmd.c_str());
        return TokenizarListaFicheros(".lista_fich");
    }
}

// Cambia delimiters por nuevoDelimiters
void Tokenizador::DelimitadoresPalabra(const string& nuevoDelimiters)
{
    this->delimiters = eliminaDuplicados(nuevoDelimiters);
}

// A?ade delimitadores al final de delimiters
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