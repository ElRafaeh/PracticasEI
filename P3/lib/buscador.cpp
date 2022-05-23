#include "buscador.h"


// Constructor con parametros
Buscador::Buscador(const string& directorioIndexacion, const int& f):IndexadorHash(directorioIndexacion)
{
    this->formSimilitud = f;
    this->c = 2;
    this->k1 = 1.2;
    this->b = 0.75;
}

// Constructor copia
Buscador::Buscador(const Buscador &busc): IndexadorHash(busc.DevolverDirIndice())
{
    this->formSimilitud = busc.formSimilitud;
    this->c = busc.c;
    this->k1 = busc.k1;
    this->b = busc.b; 
}

// Operador asignacion
Buscador& Buscador::operator=(const Buscador &busc)
{
    if(&busc != this)
    {
        this->formSimilitud = busc.formSimilitud;
        this->c = busc.c;
        this->k1 = busc.k1;
        this->b = busc.b; 
    }
}

// Metodo para buscar en los documentos
bool Buscador::Buscar(const int& numDocumentos = 99999)
{
    InformacionPregunta inf;

    if(this->DevuelvePregunta(inf))
    {
        
    }
}

// Destructor
Buscador::~Buscador()
{
    this->formSimilitud = 0;
    this->c = 0;
    this->k1 = 0;
    this->b = 0;
}

// Getter
int Buscador::DevolverFormulaSimilitud() const
{
    return this->formSimilitud;
}

// Setter
bool Buscador::CambiarFormulaSimilitud(const int& f)
{
    if(f == 0 || f == 1)
    {
        this->formSimilitud = f;
        return true;
    }
    return false;
}

// Setter
void Buscador::CambiarParametrosDFR(const double &kc)
{
    this->c = kc;
}

//Getter
double Buscador::DevolverParametrosDFR() const
{
    return this->c;
}

// Pone parametro BM25
void Buscador::CambiarParametrosBM25(const double& kk1, const double& kb)
{
    this->k1 = kk1;
    this->b = kb;
}

// Getter
void Buscador::DevolverParametrosBM25(double& kk1, double& kb) const
{
    kk1 = this->k1;
    kb = this->b;
}