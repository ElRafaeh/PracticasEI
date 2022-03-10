#ifndef __TOKENIZADOR__
#define __TOKENIZADOR__

#include <iostream>
#include <list>

using namespace std;


class Tokenizador
{
	friend ostream& operator<<(ostream&, const Tokenizador&);
		// cout << ?DELIMITADORES: ? << delimiters << ? TRATA CASOS ESPECIALES:? << casosEspeciales << ? PASAR A MINUSCULAS Y SIN ACENTOS: ? << pasarAminuscSinAcentos;
		// Aunque se modifique el almacenamiento de los delimitadores por temas de eficiencia, 
		// el campo delimiters se imprimir? con el string le?do en el tokenizador (tras las modificaciones y eliminaci?n de los caracteres repetidos correspondientes)

	public:
		Tokenizador (const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos);
			// Inicializa delimiters a delimitadoresPalabra filtrando que no se introduzcan delimitadores repetidos (de izquierda a derecha, en cuyo
			// caso se eliminar?an los que hayan sido repetidos por la derecha); casosEspeciales a kcasosEspeciales; pasarAminuscSinAcentos a minuscSinAcentos
		Tokenizador (const Tokenizador&);
		Tokenizador ();
			// Inicializa delimiters=",;:.-/+*\\ '\"{}[]()<>?!??&#=\t\n\r@"; casosEspeciales a true; pasarAminuscSinAcentos a false
		~Tokenizador (); 
			// Pone delimiters=""
		Tokenizador& operator= (const Tokenizador&);
			// Operador igualdad
		void TokenizarSinCasosEspeciales(const string& str, list<string>& tokens) const; 
			//Funci?n para tokenizar sin casos especiales activados
		void TokenizarCasosEspeciales(const string& str, list<string>& tokens, const string &delimiters) const; 
			//Funci?n para tokenizar con casos especiales activados
		void Tokenizar (string str, list<string>& tokens) const;
			// Tokeniza str devolviendo el resultado en tokens. La lista tokens se
			// vaciar? antes de almacenar el resultado de la tokenizaci?n.
		bool Tokenizar (const string& i, const string& f) const;
			// Tokeniza el fichero i guardando la salida en el fichero f (una  palabra en cada l?nea del fichero). Devolver? true si se realiza la
			// tokenizaci?n de forma correcta; false en caso contrario enviando a cerr  el mensaje correspondiente (p.ej. que no exista el archivo i)
		bool Tokenizar (const string & i) const;
			// Tokeniza el fichero i guardando la salida en un fichero de nombre i a?adi?ndole extensi?n .tk (sin eliminar previamente la extensi?n de i
			// por ejemplo, del archivo pp.txt se generar?a el resultado en pp.txt.tk), y que contendr? una palabra en cada l?nea del fichero. Devolver? true si
			// se realiza la tokenizaci?n de forma correcta; false en caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no exista el archivo i)
		bool TokenizarListaFicheros (const string& i) const;
			// Tokeniza el fichero i que contiene un nombre de fichero por l?nea guardando la salida en ficheros (uno por cada l?nea de i) cuyo nombre
			// ser? el le?do en i a?adi?ndole extensi?n .tk, y que contendr? una palabra en cada l?nea del fichero le?do en i. Devolver? true si se
			// realiza la tokenizaci?n de forma correcta de todos los archivos que contiene i; devolver? false en caso contrario enviando a cerr el mensaje
			// correspondiente (p.ej. que no exista el archivo i, o que se trate de un directorio, enviando a ?cerr? los archivos de i que no existan o que
			// sean directorios; luego no se ha de interrumpir la ejecuci?n si hay alg?n archivo en i que no exista)
		bool TokenizarDirectorio (const string& i) const;
			// Tokeniza todos los archivos que contenga el directorio i, incluyendo los de los subdirectorios, guardando la salida en ficheros cuyo nombre
			// ser? el de entrada a?adi?ndole extensi?n .tk, y que contendr? una palabra en cada l?nea del fichero. Devolver? true si se realiza la
			// tokenizaci?n de forma correcta de todos los archivos; devolver? false en caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no
			// exista el directorio i, o los ficheros que no se hayan podido tokenizar)
		void DelimitadoresPalabra(const string& nuevoDelimiters);
			// Cambia ?delimiters? por ?nuevoDelimiters? comprobando que no hayan delimitadores repetidos (de izquierda a derecha), en cuyo caso se
			// eliminar?an los que hayan sido repetidos (los nuevos delimitadores que se van analizando) tanto en ?nuevoDelimiters? como los que ya estuviesen
			// en ?delimiters?
		void AnyadirDelimitadoresPalabra(const string& nuevoDelimiters); //
			// A?ade al final de ?delimiters? los nuevos delimitadores que aparezcan en ?nuevoDelimiters? (no se almacenar?n caracteres repetidos)
		string DelimitadoresPalabra() const;
			// Devuelve ?delimiters?
		void CasosEspeciales(const bool& nuevoCasosEspeciales);
			// Cambia la variable privada ?casosEspeciales?
		bool CasosEspeciales();
			// Devuelve el contenido de la variable privada ?casosEspeciales?
		void PasarAminuscSinAcentos (const bool& nuevoPasarAminuscSinAcentos);
			// Cambia la variable privada ?pasarAminuscSinAcentos?. Atenci?n al formato de codificaci?n del corpus (comando ?file? de Linux). Para la
			// correcci?n de la pr?ctica se utilizar? el formato actual (ISO-8859).
		bool PasarAminuscSinAcentos ();
			// Devuelve el contenido de la variable privada ?pasarAminuscSinAcentos? 
	private:
		string delimiters; 
			// Delimitadores de t?rminos. Aunque se modifique la forma de almacenamiento interna para mejorar la eficiencia, este
			// campo debe permanecer para indicar el orden en que se introdujeron los delimitadores
		bool casosEspeciales;
			// Si true detectar? palabras compuestas y casos especiales. Sino, trabajar? al igual que el algoritmo propuesto en la secci?n ?Versi?n del
			// tokenizador vista en clase?
		bool pasarAminuscSinAcentos;
			// Si true pasar? el token a min?sculas y quitar? acentos, antes de realizar la tokenizaci?n
		string eliminaDuplicados(const string &conDuplicados) const; 
			// Funcion auxiliar que elimina duplicados de un string
		void copia(const Tokenizador&); 
			// Funcion auxiliar para copiar el tokenizador pasado
		string convertirSinMayusSinAcen(string) const;
			// Funcion auxiliar para quitar mayusculas y acentos a un string pasado
		bool casoUrl(list<string> &tokens, const string &str, string::size_type &pos, string::size_type &lastPos, const string &delim, const string &delimiters) const;
			// Funci?n que verifica si se trata de un caso Url o no, y mete el tokens con la URL
		bool casoDecimal(list<string> &tokens, const string &str, string::size_type &pos, string::size_type &lastPos, const string &delim,const string &delimiters) const;
			// Funcion que verifica si se trata de un caso decimal o no y almacena en tokens
		bool casoEmail(list<string> &tokens, const string &str, string::size_type &pos, string::size_type &lastPos, const string &delim, const string &delimiters) const;
			// Funcion que verifica si se trata de un caso email o no y almacena en 
		bool casoAcronimoYMulti(const char, list<string> &tokens, const string &str, string::size_type &pos, string::size_type &lastPos, const string &delim, const string &delimiters) const;
			// Funcion que verifica si se trata de un caso de acronimo o multipalabra y almacena el token
		string quitarEspeciales(const string &especiales, const string &delimiters) const;
			// Funcion que devuelve los delimitadores habiendole quitados los delimitadores pasador por argumento

};


#endif