#include "buscador.h"
#include <math.h>
#include <iomanip>
#include <fstream>

// RESULTADO RI

ResultadoRI::ResultadoRI(const double& kvSimilitud, const long int& kidDoc, const int& np)
{
    vSimilitud = kvSimilitud;
    idDoc = kidDoc;
    numPregunta = np;
}

double ResultadoRI::VSimilitud() const
{
    return vSimilitud;
}

long int ResultadoRI::IdDoc() const
{
    return idDoc;
}

int ResultadoRI::getNumPregunta() const
{
    return numPregunta;
}

bool ResultadoRI::operator<(const ResultadoRI& lhs) const
{
    if(numPregunta == lhs.numPregunta)
        return (vSimilitud < lhs.vSimilitud);
    else
        return (numPregunta > lhs.numPregunta);
}

bool ResultadoRI::operator>(const ResultadoRI& lhs) const
{
    return !(*this < lhs);
}

// BUSCADOR 

// Constructor con parametros
Buscador::Buscador(const string& directorioIndexacion, const int& f): IndexadorHash(directorioIndexacion)
{
    this->formSimilitud = f;
    this->c = 2;
    this->k1 = 1.2;
    this->b = 0.75;
}

// Constructor copia
Buscador::Buscador(const Buscador &busc): IndexadorHash(busc)
{
    this->formSimilitud = busc.formSimilitud;
    this->c = busc.c;
    this->k1 = busc.k1;
    this->b = busc.b; 
}

// Destructor
Buscador::~Buscador() {}

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
    return *this;
}

// Funcion para calcular el valor de DFR
ResultadoRI Buscador::calculoSimilitudDFR(const InfDoc &d, const int &nump)
{
    double similitud = 0;
    for(unordered_map<string, InformacionTerminoPregunta>::iterator itPreg = indicePregunta.begin(); itPreg != indicePregunta.end(); itPreg++)
    {
        unordered_map<string, InformacionTermino>::iterator informacionTerm = indice.find(itPreg->first);
        double k = (double)infPregunta.getNumTotalPalSinParada();
        double fT = 0;
        double fTD = 0;
        double nT = 0;
        // Si no se encuentra el índice indexado
        if(informacionTerm != indice.end())
        {
            fT = (double)informacionTerm->second.getFtc();
            unordered_map<int, InfTermDoc>::iterator itL_docs = informacionTerm->second.l_docs.find(d.getIdDoc());
            if(itL_docs != informacionTerm->second.l_docs.end()) fTD = (double)itL_docs->second.getFt();
            nT = (double)informacionTerm->second.l_docs.size();
        }

        double fTQ = (double)itPreg->second.getFt();
        double lD = (double)d.getNumPalSinParada();
        double avr_lD = (double)informacionColeccionDocs.getNumTotalPalSinParada() / (double)informacionColeccionDocs.getNumDocs();
        double N = (double)informacionColeccionDocs.getNumDocs();
        c = this->c;
        double deltaT = (double)fT/(double)N;
        double fTDEstrella = fTD*log2(1+((c*avr_lD)/lD));
        double wTD = (log2(1+deltaT)+fTDEstrella*log2((1+deltaT)/deltaT))*((fT+1)/(nT*(fTDEstrella+1)));
        double wTQ = fTQ/k;
        similitud += wTD * wTQ;
    }
    if(similitud > 0) { return ResultadoRI(similitud, d.getIdDoc(), nump); }
    else { return ResultadoRI(similitud, d.getIdDoc(), 0); }
}

// Funcion para el calculo BM25
ResultadoRI Buscador::calculoSimilitudBM25(const InfDoc &d, const int &nump)
{
    double similitud = 0;
    for(unordered_map<string, InformacionTerminoPregunta>::iterator itPreg = indicePregunta.begin(); itPreg != indicePregunta.end(); itPreg++)
    {
        unordered_map<string, InformacionTermino>::iterator informacionTerm = indice.find(itPreg->first);
        double fQD = 0;
        double nQ = 0;
        if(informacionTerm != indice.end())
        {
            unordered_map<int, InfTermDoc>::iterator itL_docs = informacionTerm->second.l_docs.find(d.getIdDoc());
            if(itL_docs != informacionTerm->second.l_docs.end()) fQD = (double)itL_docs->second.getFt();
            nQ = (double)informacionTerm->second.l_docs.size();
        }
        double D = (double)d.getNumPalSinParada();
        double avgdl = (double)informacionColeccionDocs.getNumTotalPalSinParada() / (double)informacionColeccionDocs.getNumDocs();
        double N = (double)informacionColeccionDocs.getNumDocs();
        

        double IDF = log2((N - nQ + 0.5)/(nQ + 0.5));
        similitud += IDF*((fQD*(this->k1+1))/(fQD + this->k1*(1-this->b+this->b*(D/avgdl))));
    }
    if(similitud > 0) { return ResultadoRI(similitud, d.getIdDoc(), nump); }
    else { return ResultadoRI(similitud, d.getIdDoc(), 0); }
}

// Metodo para buscar en los documentos
bool Buscador::Buscar(const int& numDocumentos, const int& nump)
{
    InformacionPregunta inf;

    if(this->DevuelvePregunta(inf) && !this->indicePregunta.empty())
    {
        docsOrdenados.clear();

        if(this->formSimilitud == 0)
        {
            for(unordered_map<string, InfDoc>::iterator itDocs = indiceDocs.begin(); itDocs != indiceDocs.end(); itDocs++)
            { 
                ResultadoRI aux = calculoSimilitudDFR(itDocs->second, nump);
                if(aux.VSimilitud() != 0) docsOrdenados.insert({aux, aux.VSimilitud()}); 
                if (docsOrdenados.size() > numDocumentos) docsOrdenados.erase(prev(docsOrdenados.end()));
            }
            return true;
        }
        else if(this->formSimilitud == 1)
        {
            for(unordered_map<string, InfDoc>::iterator itDocs = indiceDocs.begin(); itDocs != indiceDocs.end(); itDocs++)
            { 
                ResultadoRI aux = calculoSimilitudBM25(itDocs->second, nump);
                if(aux.VSimilitud() != 0) docsOrdenados.insert({aux, aux.VSimilitud()}); 
                if (docsOrdenados.size() > numDocumentos) docsOrdenados.erase(prev(docsOrdenados.end())); 
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

// Metodo buscar con mas parametros
bool Buscador::Buscar(const string& dirPreguntas, const int& numDocumentos, const int& numPregInicio, const int& numPregFin)
{
    bool devolver = false;
    int numPregInicioAux;
    while(numPregInicioAux != numPregFin)
    {
        fstream fich((dirPreguntas+"/"+to_string(numPregInicioAux)+".txt").c_str(), ios::in);
        if(fich.is_open())
        {
            string aux = "";
            getline(fich, aux);
            this->IndexarPregunta(aux);
            devolver = Buscar(numDocumentos, numPregInicioAux);
        }
        numPregInicioAux++;
    }
    return devolver;
}

// Metodo para buscar en el array de indiceDocs
string Buscador::BuscarDocumento(const int &idDoc) const
{
    for(unordered_map<string, InfDoc>::const_iterator it = indiceDocs.begin(); it != indiceDocs.end(); it++)
    {
        if(it->second.getIdDoc() == idDoc)
        {
            string aux = it->first.substr(it->first.find_last_of("/\\") + 1);
            return aux.substr(0, aux.find_last_of("."));
        } 
    }
    return "";
}

// Impresion del resultado de la busqueda
void Buscador::ImprimirResultadoBusqueda(const int &numDocumentos) const
{   
    int i = 0;
    for(map<ResultadoRI, double>::const_iterator it = docsOrdenados.begin(); (it != docsOrdenados.end() && i != numDocumentos); it++)
    {

        cout << it->first.getNumPregunta() << " ";
        if(DevolverFormulaSimilitud() == 0) { cout << "DRF "; } else { cout << "BM25 "; }
        cout << BuscarDocumento(it->first.IdDoc()) << " ";
        cout << i << " ";
        cout << setprecision(6) << fixed << it->first.VSimilitud() << " ";
        if(it->first.getNumPregunta() == 0) { cout << pregunta; } else { cout << "ConjuntoDePreguntas"; }
        cout << endl;
        i++;
    }
}

// Guarda la salida en un fichero de texto
bool Buscador::ImprimirResultadoBusqueda(const int& numDocumentos, const string& nombreFichero) const
{

    fstream fich(nombreFichero.c_str(), ios::out);
    if(fich.is_open())
    {
        string aux;
        int i = 0;
        for(map<ResultadoRI, double>::const_iterator it = docsOrdenados.begin(); (it != docsOrdenados.end() && i != numDocumentos); it++)
        {
            aux += it->first.getNumPregunta() + " ";
            if(DevolverFormulaSimilitud() == 0) { aux += "DRF "; } else { aux += "BM25 "; }
            aux += BuscarDocumento(it->first.IdDoc()) + " ";
            aux += i + " ";
            aux += it->first.VSimilitud(); 
            aux += " ";
            if(it->first.getNumPregunta() == 0) { aux += pregunta; } else { aux += "ConjuntoDePreguntas"; }
            aux += "\n";
            i++;
        }
        // Escribimos en el fichero y lo guardamos
        fich << aux;
        fich.close();
        return true;
    }
    
    return false;
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