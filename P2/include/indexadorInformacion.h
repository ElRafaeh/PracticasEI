#ifndef __INDEXADORINFORMACION__
#define __INDEXADORINFORMACION__

#include <unordered_map>
#include <list>
#include <iostream>
#include <ostream>
#include <list>
#include <ctime>

using namespace std;

  //////////////////////////////////
 //         Clase Fecha          //
//////////////////////////////////
class Fecha
{
    public:
        Fecha();
        Fecha(const Fecha &);
        ~Fecha();
        tm getFecha() const;
    private:
        tm fecha;
};

  //////////////////////////////////
 //       Clase InfTermDoc       //
//////////////////////////////////
class InfTermDoc {
    friend ostream& operator<<(ostream& s, const InfTermDoc& p);
    public:
        InfTermDoc (const InfTermDoc &);
        InfTermDoc ();
        // Inicializa ft = 0
        ~InfTermDoc ();
        // Pone ft = 0
        InfTermDoc & operator= (const InfTermDoc &);
        // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
    private:
        int ft;
        // Frecuencia del término en el documento
        list<int> posTerm;
        // Solo se almacenaría esta información si el campo privado del indexador almacenarPosTerm == true
        // Lista de números de palabra en los que aparece el término en el documento. Los números de palabra comenzarán desde cero (la primera
        //palabra del documento). Se numerarán las palabras de parada. Estará ordenada de menor a mayor posición.
};

  //////////////////////////////////
 //   Clase InformacionTermino   //
//////////////////////////////////
class InformacionTermino {
    friend ostream& operator<<(ostream& s, const InformacionTermino& p);
    public:
        InformacionTermino(const InformacionTermino &);
        InformacionTermino();
        // Inicializa ftc = 0
        ~InformacionTermino ();
        // Pone ftc = 0 y vacía l_docs
        InformacionTermino & operator= (const InformacionTermino &);
        // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
    private:
        int ftc; // Frecuencia total del término en la colección
        unordered_map<int, InfTermDoc> l_docs;
        // Tabla Hash que se accederá por el id del documento, devolviendo un objeto de la clase InfTermDoc que contiene 
        // toda la información de aparición del término en el documento
};

  //////////////////////////////////
 //         Clase InfDoc         //
//////////////////////////////////
class InfDoc {
    friend ostream& operator<<(ostream& s, const InfDoc& p);
    public:
        InfDoc (const InfDoc &);
        InfDoc ();
        ~InfDoc ();
        InfDoc & operator= (const InfDoc &);
        // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
    private:
        int idDoc;
        // Identificador del documento. El primer documento indexado en la colección será el identificador 1
        int numPal;
        // Nº total de palabras del documento
        int numPalSinParada;
        // Nº total de palabras sin stop-words del documento
        int numPalDiferentes;
        // Nº total de palabras diferentes que no sean stop-words (sin acumular la frecuencia de cada una de ellas)
        int tamBytes;
        // Tamaño en bytes del documento
        Fecha fechaModificacion;
        // Atributo correspondiente a la fecha y hora de modificación del documento. El tipo ?Fecha/hora? lo elegirá/implementará el alumno
};

  //////////////////////////////////
 //    Clase InfColeccionDocs    //
//////////////////////////////////
class InfColeccionDocs {
    friend ostream& operator<<(ostream& s, const InfColeccionDocs& p);
    public:
        InfColeccionDocs (const InfColeccionDocs &);
        InfColeccionDocs ();
        ~InfColeccionDocs ();
        InfColeccionDocs & operator= (const InfColeccionDocs &);
        // A#adir cuantos m#todos se consideren necesarios para manejar la parte privada de la clase
    private:
        int numDocs;
        // N# total de documentos en la colecci#n
        int numTotalPal;
        // N# total de palabras en la colecci#n
        int numTotalPalSinParada;
        // N# total de palabras sin stop-words en la colecci#n
        int numTotalPalDiferentes;
        // N# total de palabras diferentes en la colecci#n que no sean stop-words (sin acumular la frecuencia de cada una de ellas)
        int tamBytes;
        // Tama#o total en bytes de la colecci#n
};

  ////////////////////////////////////////
 //  Clase informacionTerminoPregunta  //
////////////////////////////////////////
class InformacionTerminoPregunta {
    friend ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p);
    public:
        InformacionTerminoPregunta (const InformacionTerminoPregunta&);
        InformacionTerminoPregunta ();
        ~InformacionTerminoPregunta ();
        InformacionTerminoPregunta & operator=(const InformacionTerminoPregunta &);
        // A#adir cuantos m#todos se consideren necesarios para manejar la parte privada de la clase
    private:
        int ft;
        // Frecuencia total del t#rmino en la pregunta
        list<int> posTerm;
        // Solo se almacenar# esta informaci#n si el campo privado del indexador
        //almacenarPosTerm == true;
        // Lista de n#meros de palabra en los que aparece el t#rmino en la
        // pregunta. Los n#meros de palabra comenzar#n desde cero (la primera
        // palabra de la pregunta). Se numerar#n las palabras de parada. Estar#
        // ordenada de menor a mayor posici#n.
};

  //////////////////////////////////
 //   Clase informacionPregunta  //
//////////////////////////////////
class InformacionPregunta {
    friend ostream& operator<<(ostream& s, const InformacionPregunta& p);
    public:
    InformacionPregunta (const InformacionPregunta &);
        InformacionPregunta ();
        ~InformacionPregunta ();
        InformacionPregunta & operator= (const InformacionPregunta &);
        // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
    private:
        int numTotalPal;
        // Nº total de palabras en la pregunta
        int numTotalPalSinParada;
        // Nº total de palabras sin stop-words en la pregunta
        int numTotalPalDiferentes;
        // Nº total de palabras diferentes en la pregunta que no sean stop-words (sin acumular la frecuencia de cada una de ellas)
};
    

#endif