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

// Constructor con parametros
Fecha::Fecha(const tm &tiempo)
{
    this->fecha = tiempo;
}

// Contructor de copia
Fecha::Fecha(const Fecha &fech)
{
    this->fecha = fech.fecha;
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

// Operador menor
bool Fecha::operator<(const Fecha &fech)
{
    tm aux = fech.fecha;

    if (difftime (mktime(&this->fecha), mktime(&aux)) < 0)
        return true;
    else
        return false;
}

// Operador asignacion
Fecha& Fecha::operator=(const Fecha &fech)
{
    if(this != &fech)
    {
        this->fecha = fech.getFecha();
    }

    return *this;
}

// Operador mayor igual
bool Fecha::operator>=(const Fecha &fech)
{
    return !(*this < fech);
}

  //////////////////////////////
 //     Clase InfTermDoc     //
//////////////////////////////

// Constructor por defecto
InfTermDoc::InfTermDoc()
{
    this->ft = 0;
}

// Setter Ft
void InfTermDoc::setFt(const int &valor)
{
    this->ft = valor;
}

// Constructor de copia
InfTermDoc::InfTermDoc(const InfTermDoc &copia)
{
    this->ft = copia.ft;
    this->posTerm = copia.posTerm;
}

// Getter de Ft
int InfTermDoc::getFt() const
{
    return this->ft;
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

// Incrementa Ft
void InfTermDoc::incrementarFt()
{
    this->ft++;
}

// Inserta en posterm la posicion
void InfTermDoc::insertarPosTerm(const int &pos)
{
    this->posTerm.push_back(pos);
}

// Decrementa Ft
void InfTermDoc::decrementarFt()
{
    this->ft--;
}

// Operador salida
ostream& operator<<(ostream& s, const InfTermDoc& p) 
{
    s << "ft: " << p.ft;
    // A continuación se mostrarían todos los elementos de p.posTerm (?posicion TAB posicion TAB ... posicion, es decir nunca finalizará en un TAB?): 
    for(list<int>::const_iterator it = p.posTerm.begin(); it != p.posTerm.end(); it++)
    {
        s << "\t" << *it;
    }
        
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

// Getter
int InformacionTermino::getFtc() const
{
    return this->ftc;
}

// Setter ftc
void InformacionTermino::setFtc(const int &valor)
{
    this->ftc = valor;
}

// Restar a ftc
void InformacionTermino::restarFtc(const int &valor)
{
    this->ftc -= valor;
}

// Eliminar documento
void InformacionTermino::eliminarDoc(const  unordered_map<int, InfTermDoc>::iterator &it)
{
    this->l_docs.erase(it);
}

// Destructor
InformacionTermino::~InformacionTermino()
{
    this->ftc = 0;
    this->l_docs.clear(); 
}

// Incrementamos ftc
void InformacionTermino::incrementarFtc()
{
    this->ftc++;
}

// Metodo para insertar la información del termino en l_docs
void InformacionTermino::insertarInfTermDoc(const int &idDoc, const InfTermDoc &infoTerminoDocumento)
{
    l_docs.insert({idDoc, infoTerminoDocumento});
}

// Devuelve l_docs
unordered_map<int, InfTermDoc> InformacionTermino::getL_docs()
{
    return this->l_docs;
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
    for(unordered_map<int, InfTermDoc>::const_iterator it = p.l_docs.begin(); it != p.l_docs.end(); it++)
    {
        s << "\tId.Doc: " << it->first << "\t" << it->second;
    }
    
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

// Operador asignación
InfDoc& InfDoc::operator=(const InfDoc &copia)
{
    if(this != &copia)
    {
        this->idDoc = copia.idDoc;
        this->numPal = copia.numPal;
        this->numPalSinParada = copia.numPalSinParada;
        this->numPalDiferentes = copia.numPalDiferentes;
        this->tamBytes = copia.tamBytes;
        this->fechaModificacion = copia.fechaModificacion;
    }
    return *this;
}

// Incrementa en uno la variable numPal
void InfDoc::incrementaNumPal()
{
    this->numPal++;
} 

// Incrementa en numPal la variable numPal
void InfDoc::incrementaNumPal(const int &numPal)
{
    this->numPal += numPal;
} 

// Incrementa en uno la variable numPalSinParada
void InfDoc::incrementaNumPalSinParada()
{
    this->numPalSinParada++;
}

// Incrementa en uno la variable numPalDiferente
void InfDoc::incrementaNumPalDiferentes()
{
    this->numPalDiferentes++;
}

// Setter tamBytes
void InfDoc::setTamBytes(const int &tamBytes)
{
    this->tamBytes = tamBytes;
} 

// Setter Fecha
void InfDoc::setFecha(const Fecha &fech)
{
    this->fechaModificacion = fech;
}

void InfDoc::setNumPal(const int &val)
{
    this->numPal = val;
}

void InfDoc::setNumPalSinParada(const int &val)
{
    this->numPalSinParada = val;
}

void InfDoc::setNumPalDiferentes(const int &val)
{
    this->numPalDiferentes = val;
}


// Setter idDoc
void InfDoc::setIdDoc(const int &id)
{
    this->idDoc = id;
}

// Getter
int InfDoc::getIdDoc() const
{
    return this->idDoc;
}

// Getter
int InfDoc::getNumPal() const
{
    return this->numPal;
}

// Getter
int InfDoc::getNumPalSinParada() const
{
    return this->numPalSinParada;
}

// Getter
int InfDoc::getNumPalDiferentes() const
{
    return this->numPalDiferentes;
}

// Getter
int InfDoc::getTamBytes() const
{
    return this->tamBytes;
}

// Getter 
Fecha InfDoc::getFecha() const
{
    return this->fechaModificacion;
}


// Operador salida
ostream& operator<<(ostream& s, const InfDoc& p) 
{
    s << "idDoc: " << p.idDoc << "\tnumPal: " << p.numPal << "\tnumPalSinParada: " << p.numPalSinParada << "\tnumPalDiferentes: " << p.numPalDiferentes << "\ttamBytes: " << p.tamBytes;
    return s;
}

  //////////////////////////////////
 //    Clase InfColeccionDocs    //
//////////////////////////////////

// Contructor por defecto
InfColeccionDocs::InfColeccionDocs()
{
    this->numDocs = 0;
    this->numTotalPal = 0;
    this->numTotalPalDiferentes = 0;
    this->numTotalPalSinParada = 0;
    this->tamBytes = 0;
}

// Contructor copia
InfColeccionDocs::InfColeccionDocs(const InfColeccionDocs &copia)
{
    this->numDocs = copia.numDocs;
    this->numTotalPal = copia.numTotalPal;
    this->numTotalPalDiferentes = copia.numTotalPalDiferentes;
    this->numTotalPalSinParada = copia.numTotalPalSinParada;
    this->tamBytes = copia.tamBytes;
}

// Destructor
InfColeccionDocs::~InfColeccionDocs()
{
    this->numDocs = 0;
    this->numTotalPal = 0;
    this->numTotalPalDiferentes = 0;
    this->numTotalPalSinParada = 0;
    this->tamBytes = 0;
}

// Operador asignacion
InfColeccionDocs& InfColeccionDocs::operator=(const InfColeccionDocs &copia)
{
    if(this != &copia)
    {
        this->numDocs = copia.numDocs;
        this->numTotalPal = copia.numTotalPal;
        this->numTotalPalDiferentes = copia.numTotalPalDiferentes;
        this->numTotalPalSinParada = copia.numTotalPalSinParada;
        this->tamBytes = copia.tamBytes;
    }
    return *this;
}

// Incrementa en uno la variable numDocs
void InfColeccionDocs::incrementaNumDocs()
{ 
    numDocs++;
}

// Incrementa en uno la variable numTotalPal
void InfColeccionDocs::incrementaNumTotalPal(const int &palabras)
{
    numTotalPal += palabras;
} 

// Incrementa en uno la variable numTotalPalSinParada
void InfColeccionDocs::incrementaNumTotalPalSinParada(const int &palabras)
{
    numTotalPalSinParada += palabras;
}

// Incrementa en uno la variable numTotalPalDiferente
void InfColeccionDocs::incrementaNumTotalPalDiferentes()
{
    numTotalPalDiferentes++;
}

// Setter tamBytes
void InfColeccionDocs::sumTamBytes(const int &tamBytes)
{
    this->tamBytes += tamBytes;
} 

// Getter
int InfColeccionDocs::getNumDocs() const
{
    return this->numDocs;
}

// Getter
int InfColeccionDocs::getNumTotalPal() const
{
    return this->numTotalPal;
}

// Getter
int InfColeccionDocs::getNumTotalPalSinParada() const
{
    return this->numTotalPalSinParada;
}

// Getter
int InfColeccionDocs::getNumTotalPalDiferentes() const
{
    return this->numTotalPalDiferentes;
}

// Getter
int InfColeccionDocs::getTamBytes() const
{
    return this->tamBytes;
}

// Decrementa numDocs
void InfColeccionDocs::decrementaNumDocs()
{
    this->numDocs--;
}

// Decrementa numTotalPalDiferentes
void InfColeccionDocs::decrementaNumTotalPalDiferentes()
{
    this->numTotalPalDiferentes--;
}

// Resta a num total pal
void InfColeccionDocs::restaNumTotalPal(const int &valor)
{
    this->numTotalPal -= valor;
}

// Setters
void InfColeccionDocs::setNumDocs(const int &val)
{
    this->numDocs = val;
}

void InfColeccionDocs::setNumTotalPal(const int &val)
{
    this->numTotalPal = val;
}

void InfColeccionDocs::setNumTotalPalSinParada(const int &val)
{
    this->numTotalPalSinParada = val;
}

void InfColeccionDocs::setNumTotalPalDiferentes(const int &val)
{
    this->numTotalPalDiferentes = val;
}

void InfColeccionDocs::setTamBytes(const int &val)
{
    this->tamBytes = val;
}


// Resta al numTotalPalSinParada
void InfColeccionDocs::restaNumTotalPalSinParada(const int &valor)
{
    this->numTotalPalSinParada -= valor;
}

// Resta al tamaño de bytes
void InfColeccionDocs::restaTamBytes(const int &valor)
{
    this->tamBytes -= valor;
}

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
InformacionTerminoPregunta::InformacionTerminoPregunta()
{
    this->ft = 0;
}

// Constructor de copia
InformacionTerminoPregunta::InformacionTerminoPregunta(const InformacionTerminoPregunta &copia)
{
    this->ft = copia.ft;
    this->posTerm = copia.posTerm;
}

// Destructor
InformacionTerminoPregunta::~InformacionTerminoPregunta()
{
    this->ft = 0;
    this->posTerm.clear();
}

// Operador asignacion
InformacionTerminoPregunta& InformacionTerminoPregunta::operator=(const InformacionTerminoPregunta &copia)
{
    if(this != &copia)
    {
        this->ft = copia.ft;
        this->posTerm = copia.posTerm;
    }
    return *this;
}

// Getter
int InformacionTerminoPregunta::getFt() const
{
    return this->ft;
}

// Getter
list<int> InformacionTerminoPregunta::getPosTerm() const
{
    return this->posTerm;
}

// Incrementa ft
void InformacionTerminoPregunta::incrementarFt()
{
    this->ft++;
}

// Setter ft
void InformacionTerminoPregunta::setFt(const int &valor)
{
    this->ft = valor;
}

// Setea posTerm
void InformacionTerminoPregunta::insertarPosTerm(const int &pos)
{
    this->posTerm.push_back(pos);
}

// Operador salida
ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p) 
{
    s << "ft: " << p.ft;
    for(list<int>::const_iterator it = p.posTerm.begin(); it != p.posTerm.end(); it++)
    {
        s << "\t" << *it;
    }
    
    return s;
}

  //////////////////////////////////
 //   Clase informacionPregunta  //
//////////////////////////////////

// Contructor por defecto
InformacionPregunta::InformacionPregunta()
{
    this->numTotalPal = 0;
    this->numTotalPalDiferentes = 0;
    this->numTotalPalSinParada = 0;
}

// Contructor copia
InformacionPregunta::InformacionPregunta(const InformacionPregunta &copia)
{
    this->numTotalPal = copia.numTotalPal;
    this->numTotalPalDiferentes = copia.numTotalPalDiferentes;
    this->numTotalPalSinParada = copia.numTotalPalSinParada;
}

// Destructor
InformacionPregunta::~InformacionPregunta()
{
    this->numTotalPal = 0;
    this->numTotalPalDiferentes = 0;
    this->numTotalPalSinParada = 0;
}

// Operador asignacion
InformacionPregunta& InformacionPregunta::operator=(const InformacionPregunta &copia)
{
    if(this != &copia)
    {
        this->numTotalPal = copia.numTotalPal;
        this->numTotalPalDiferentes = copia.numTotalPalDiferentes;
        this->numTotalPalSinParada = copia.numTotalPalSinParada;
    }
    return *this;
}

void InformacionPregunta::incrementaNumTotalPal() // Incrementa en uno la variable numTotalPal
{
    this->numTotalPal++;
}

void InformacionPregunta::incrementaNumTotalPalSinParada() // Incrementa en uno la variable numTotalPalSinParada
{
    this->numTotalPalSinParada++;
}

void InformacionPregunta::incrementaNumTotalPalDiferentes() // Incrementa en uno la variable numTotalPalDiferente
{
    this->numTotalPalDiferentes++;
}   

int InformacionPregunta::getNumTotalPal() const
{
    return this->numTotalPal;
}

int InformacionPregunta::getNumTotalPalSinParada() const
{
    return this->numTotalPalSinParada;
}

int InformacionPregunta::getNumTotalPalDiferentes() const
{
    return this->numTotalPalDiferentes;
}

void InformacionPregunta::setNumTotalPal(const int &val)
{
    this->numTotalPal = val;
}

void InformacionPregunta::setNumTotalPalSinParada(const int &val)
{
    this->numTotalPalSinParada = val;
}

void InformacionPregunta::setNumTotalPalDiferentes(const int &val)
{
    this->numTotalPalDiferentes = val;
}

// Operador salida
ostream& operator<<(ostream &s, const InformacionPregunta &p)
{
    s << "numTotalPal: " << p.numTotalPal << "\tnumTotalPalSinParada: " << p.numTotalPalSinParada << "\tnumTotalPalDiferentes: " << p.numTotalPalDiferentes;
    return s;
}