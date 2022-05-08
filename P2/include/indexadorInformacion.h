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
        Fecha(const tm &);
        Fecha(const Fecha &);
        ~Fecha();
        tm getFecha() const;
        Fecha& operator=(const Fecha &);
        bool operator<(const Fecha &);
        bool operator>=(const Fecha &);
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
        InfTermDoc & operator=(const InfTermDoc &);
        // A�adir cuantos m�todos se consideren necesarios para manejar la parte privada de la clase
        void incrementarFt();
        void decrementarFt();
        int getFt() const;
        void setFt(const int &);
        void insertarPosTerm(const int &);
        list<int>& getPosTerm() const;
        list<int> posTerm;
    private:
        int ft;
        // Frecuencia del t�rmino en el documento
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
        // Pone ftc = 0 y vac�a l_docs
        InformacionTermino & operator= (const InformacionTermino &);
        // A�adir cuantos m�todos se consideren necesarios para manejar la parte privada de la clase
        void incrementarFtc();
        int getFtc() const;
        void setFtc(const int &);
        void insertarInfTermDoc(const int &, const InfTermDoc &);
        unordered_map<int, InfTermDoc> getL_docs();
        void restarFtc(const int &);
        void eliminarDoc(const unordered_map<int, InfTermDoc>::iterator &);
        unordered_map<int, InfTermDoc> l_docs;
        // Tabla Hash que se acceder� por el id del documento, devolviendo un objeto de la clase InfTermDoc que contiene 
        // toda la informaci�n de aparici�n del t�rmino en el documento
    private:
        int ftc; // Frecuencia total del t�rmino en la colecci�n
        
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
        // A�adir cuantos m�todos se consideren necesarios para manejar la parte privada de la clase
        void incrementaNumPal(); // Incrementa en uno la variable numTotalPal
        void incrementaNumPal(const int &); // Incrementa en uno la variable numTotalPal
        void incrementaNumPalSinParada(); // Incrementa en uno la variable numTotalPalSinParada
        void incrementaNumPalDiferentes(); // Incrementa en uno la variable numTotalPalDiferente
        void setIdDoc(const int &);
        void setFecha(const Fecha &);
        void setNumPal(const int &val);
        void setNumPalSinParada(const int &val);
        void setNumPalDiferentes(const int &val);
        void setTamBytes(const int &); 
        int getNumPal() const;
        int getNumPalSinParada() const;
        int getNumPalDiferentes() const;
        int getIdDoc() const;
        Fecha getFecha() const;
        int getTamBytes() const;
    private:
        int idDoc;
        // Identificador del documento. El primer documento indexado en la colecci�n ser� el identificador 1
        int numPal;
        // N� total de palabras del documento
        int numPalSinParada;
        // N� total de palabras sin stop-words del documento
        int numPalDiferentes;
        // N� total de palabras diferentes que no sean stop-words (sin acumular la frecuencia de cada una de ellas)
        int tamBytes;
        // Tama�o en bytes del documento
        Fecha fechaModificacion;
        // Atributo correspondiente a la fecha y hora de modificaci�n del documento. El tipo ?Fecha/hora? lo elegir�/implementar� el alumno
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
        // A�adir cuantos m#todos se consideren necesarios para manejar la parte privada de la clase
        void incrementaNumDocs(); // Incrementa en uno la variable numDocs
        void incrementaNumTotalPal(const int &); // Incrementa en uno la variable numTotalPal
        void incrementaNumTotalPalSinParada(const int &); // Incrementa en uno la variable numTotalPalSinParada
        void incrementaNumTotalPalDiferentes(); // Incrementa en uno la variable numTotalPalDiferente
        void sumTamBytes(const int &); // Setter tamBytes
        int getNumDocs() const;
        int getNumTotalPal() const; 
        int getNumTotalPalSinParada() const;
        int getNumTotalPalDiferentes() const;
        int getTamBytes() const;
        void decrementaNumDocs();
        void decrementaNumTotalPalDiferentes();
        void restaNumTotalPal(const int &);
        void restaNumTotalPalSinParada(const int &);
        void restaTamBytes(const int &);
        void setNumDocs(const int &val);
        void setNumTotalPal(const int &val);
        void setNumTotalPalSinParada(const int &val);
        void setNumTotalPalDiferentes(const int &val);
        void setTamBytes(const int &val);
    private:
        int numDocs;
        // N� total de documentos en la colecci�n
        int numTotalPal;
        // N� total de palabras en la colecci�n
        int numTotalPalSinParada;
        // N� total de palabras sin stop-words en la colecci�n
        int numTotalPalDiferentes;
        // N� total de palabras diferentes en la colecci�n que no sean stop-words (sin acumular la frecuencia de cada una de ellas)
        int tamBytes;
        // Tama�o total en bytes de la colecci�n
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
        int getFt() const;
        list<int> getPosTerm() const;
        void incrementarFt();
        void setFt(const int &);
        void insertarPosTerm(const int &);
    private:
        int ft;
        // Frecuencia total del t�rmino en la pregunta
        list<int> posTerm;
        // Solo se almacenar� esta informaci�n si el campo privado del indexador
        // almacenarPosTerm == true;
        // Lista de n�meros de palabra en los que aparece el t�rmino en la
        // pregunta. Los n�meros de palabra comenzar�n desde cero (la primera
        // palabra de la pregunta). Se numerar�n las palabras de parada. Estar�
        // ordenada de menor a mayor posici�n.
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
        // A�adir cuantos m�todos se consideren necesarios para manejar la parte privada de la clase
        void incrementaNumTotalPal(); // Incrementa en uno la variable numTotalPal
        void incrementaNumTotalPalSinParada(); // Incrementa en uno la variable numTotalPalSinParada
        void incrementaNumTotalPalDiferentes(); // Incrementa en uno la variable numTotalPalDiferente
        void setNumTotalPal(const int &);
        void setNumTotalPalSinParada(const int &);
        void setNumTotalPalDiferentes(const int &);
        int getNumTotalPal() const;
        int getNumTotalPalSinParada() const;
        int getNumTotalPalDiferentes() const;
    private:
        int numTotalPal;
        // N� total de palabras en la pregunta
        int numTotalPalSinParada;
        // N� total de palabras sin stop-words en la pregunta
        int numTotalPalDiferentes;
        // N� total de palabras diferentes en la pregunta que no sean stop-words (sin acumular la frecuencia de cada una de ellas)
};  

#endif