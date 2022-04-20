#include "indexadorInformacion.h"


  ////////////////////////////
 //      Clase Fecha       //
////////////////////////////

// Constructor por defecto
Fecha::Fecha()
{
    time_t timer;
    time(&timer);
    this->fecha = *(localtime(&timer));
}

// Destructor de la clase fecha
Fecha::~Fecha()
{
    tm aux = {0};
    aux.tm_sec = 0;			/* Seconds.	[0-60] (1 leap second) */
    aux.tm_min = 0;			/* Minutes.	[0-59] */
    aux.tm_hour = 0;			/* Hours.	[0-23] */
    aux.tm_mday = 1;			/* Day.		[1-31] */
    aux.tm_mon = 0;			/* Month.	[0-11] */
    aux.tm_year = 1900;			/* Year	- 1900.  */

    this->fecha = aux;
}

tm Fecha::getFecha() const
{
    return this->fecha;
}


  //////////////////////////////
 //     Clase InfTermDoc     //
//////////////////////////////

// Constructor por defecto
InfTermDoc::InfTermDoc()
{
    this->ft = 0;
}

// Constructor de copia
InfTermDoc::InfTermDoc(const InfTermDoc &copia)
{
    this->ft = copia.ft;
    this->posTerm = copia.posTerm;
}

// Destructor
InfTermDoc::~InfTermDoc()
{
    this->ft = 0;
    this->posTerm.clear();
}

// Operador de asignación
InfTermDoc& InfTermDoc::operator=(const InfTermDoc &copia)
{
    if(this != &copia)
    {
        this->~InfTermDoc();
        this->ft = copia.ft;
        this->posTerm = copia.posTerm;   
    }
    return *this;
}

// Operador salida
ostream& operator<<(ostream& s, const InfTermDoc& p) 
{
    s << "ft: " << p.ft;
    // A continuación se mostrarían todos los elementos de p.posTerm (?posicion TAB posicion TAB ... posicion, es decir nunca finalizará en un TAB?): 
    //s << "\t" << posicion;
    return s;
}

  //////////////////////////////////
 //   Clase InformacionTermino   //
//////////////////////////////////

// Constructor por defecto
InformacionTermino::InformacionTermino()
{
    this->ftc = 0;
}

// Contructor copia
InformacionTermino::InformacionTermino(const InformacionTermino &copia)
{
    this->ftc = copia.ftc;
    this->l_docs = copia.l_docs;
}

// Destructor
InformacionTermino::~InformacionTermino()
{
    this->ftc = 0;
    this->l_docs.clear(); 
}

// Operador asignacion
InformacionTermino& InformacionTermino::operator=(const InformacionTermino &copia)
{
    if(this != &copia)
    {
        this->~InformacionTermino();
        this->ftc = copia.ftc;
        this->l_docs = copia.l_docs;
    }
    return *this;
}

// Operador salida
ostream& operator<<(ostream& s, const InformacionTermino& p) 
{
    s << "Frecuencia total: " << p.ftc << "\tfd: " << p.l_docs.size();
    // A continuación se mostraráan todos los elementos de p.l_docs: 
    // s << "\tId.Doc: " << idDoc << "\t" << InfTermDoc;
    return s;
}

  //////////////////////////////
 //       Clase InfDoc       //
//////////////////////////////

// Contructor por defecto
InfDoc::InfDoc()
{
    this->idDoc = 0;
    this->numPal = 0;
    this->numPalSinParada = 0;
    this->numPalDiferentes = 0;
    this->tamBytes = 0;
    this->fechaModificacion = Fecha();
}

// Contructor copia
InfDoc::InfDoc(const InfDoc &copia)
{
    this->idDoc = copia.idDoc;
    this->numPal = copia.numPal;
    this->numPalSinParada = copia.numPalSinParada;
    this->numPalDiferentes = copia.numPalDiferentes;
    this->tamBytes = copia.tamBytes;
    this->fechaModificacion = copia.fechaModificacion;
}

// Destructor
InfDoc::~InfDoc()
{
    this->idDoc = 0;
    this->numPal = 0;
    this->numPalSinParada = 0;
    this->numPalDiferentes = 0;
    this->tamBytes = 0;
    this->fechaModificacion.~Fecha();
}

// Operador salida
ostream& operator<<(ostream& s, const InfDoc& p) 
{
    s << "idDoc:" << p.idDoc << "\tnumPal: " << p.numPal << "\tnumPalSinParada: " << p.numPalSinParada << "\tnumPalDiferentes: " << p.numPalDiferentes << "\ttamBytes: " << p.tamBytes;
    return s;
}

  //////////////////////////////////
 //    Clase InfColeccionDocs    //
//////////////////////////////////

// Contructor por defecto

// Operador salida
ostream& operator<<(ostream &s, const InfColeccionDocs &p)
{
    s << "numDocs: " << p.numDocs << "\tnumTotalPal: " << p.numTotalPal << "\tnumTotalPalSinParada: " << p.numTotalPalSinParada << "\tnumTotalPalDiferentes: " << p.numTotalPalDiferentes << "\ttamBytes: " << p.tamBytes;
    return s;
}

  ////////////////////////////////////////
 //  Clase informacionTerminoPregunta  //
////////////////////////////////////////

// Constructor por defecto

// Operador salida
ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p) 
{
    s << "ft: " << p.ft;
    // A continuaci#n se mostrar#an todos los elementos de p.posTerm (?posicion TAB posicion TAB ... posicion, es decir nunca finalizar# en un TAB?): s << ?\t? << posicion;
    return s;
}

  //////////////////////////////////
 //   Clase informacionPregunta  //
//////////////////////////////////

// Contructor por defecto

// Operador salida
ostream& operator<<(ostream &s, const InformacionPregunta &p)
{
    s << "numTotalPal: " << p.numTotalPal << "\tnumTotalPalSinParada: " << p.numTotalPalSinParada << "\tnumTotalPalDiferentes: " << p.numTotalPalDiferentes;
    return s;
}