#include "indexadorHash.h"
#include <sys/mman.h>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>


// AUXILIAR: Guarda las palabras de parada en stopWords
void IndexadorHash::GuardarStopWords(const string &fichStopWords)
{
    ifstream fich;
    string stopWord;

    fich.open(fichStopWords.c_str(), ios::in);

    if(fich.is_open())
    {
        while(getline(fich, stopWord))
        {
            this->stopWords.insert(stopWord);
        }
        fich.close();
    }
    else
    {
        cerr << "No se ha podido abrir el fichero: " << fichStopWords << endl;
    }
}

// Contructor
IndexadorHash::IndexadorHash(const string& fichStopWords, const string& delimitadores, const bool& detectComp, const bool& minuscSinAcentos, const string& dirIndice, const int& tStemmer, const bool& almEnDisco, const bool& almPosTerm)
{
    GuardarStopWords(fichStopWords);

    this->tok.DelimitadoresPalabra(delimitadores);
    this->tok.PasarAminuscSinAcentos(minuscSinAcentos);
    this->tok.CasosEspeciales(detectComp);

    if (dirIndice == "")
        this->directorioIndice = get_current_dir_name();
    else 
        this->directorioIndice = dirIndice;

    this->tipoStemmer = tStemmer;
    this->almacenarEnDisco = almEnDisco;
    this->almacenarPosTerm = almPosTerm;
}

// Contructor de copia
IndexadorHash::IndexadorHash(const IndexadorHash &copia)
{
    this->pregunta = copia.pregunta;
    this->indice = copia.indice;
    this->indiceDocs = copia.indiceDocs;
    this->informacionColeccionDocs = copia.informacionColeccionDocs;
    this->pregunta = copia.pregunta;
    this->indicePregunta = copia.indicePregunta;
    this->stopWords = copia.stopWords;
    this->ficheroStopWords = copia.ficheroStopWords;
    this->tok = copia.tok;
    this->directorioIndice = copia.directorioIndice;
    this->tipoStemmer = copia.tipoStemmer;
    this->almacenarEnDisco = copia.almacenarEnDisco;
    this->almacenarPosTerm = copia.almacenarPosTerm;
}

// Destructor
IndexadorHash::~IndexadorHash()
{
    
}

// Operador asignacion
IndexadorHash& IndexadorHash::operator=(const IndexadorHash &copia)
{
    if(this != &copia)
    {
        this->pregunta = copia.pregunta;
        this->indice = copia.indice;
        this->indiceDocs = copia.indiceDocs;
        this->informacionColeccionDocs = copia.informacionColeccionDocs;
        this->pregunta = copia.pregunta;
        this->indicePregunta = copia.indicePregunta;
        this->stopWords = copia.stopWords;
        this->ficheroStopWords = copia.ficheroStopWords;
        this->tok = copia.tok;
        this->directorioIndice = copia.directorioIndice;
        this->tipoStemmer = copia.tipoStemmer;
        this->almacenarEnDisco = copia.almacenarEnDisco;
        this->almacenarPosTerm = copia.almacenarPosTerm;
    }
    return *this;
}

// Metodo para indexar documento
bool IndexadorHash::IndexarDocumento(const string &documento, int idDoc)
{
    int fd = open(documento.c_str(), O_RDONLY);
    // Si el documento se ha abierto
    if(fd != -1)
    {
        InfDoc informacionDocumento;
        list<string> tokens;
        list<string>::iterator it;
        stemmerPorter stemmer;
        int posicionTerminoDocumento = 0;

        // Abrimos el archivo con mmap
        int len = lseek(fd, 0, SEEK_END);   // len también es el tamaño del documento
        char *mbuf = (char*) mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);  
        string infoDoc(mbuf), linea;
        stringstream ss(infoDoc);

        // Cerramos el documento
        close(fd);

        // Si se trata de un documento que no se encuentra indexado, incrementamos el numero de documentos indexados y le asignamos un nuevo idDoc
        if(idDoc == 0)
        {
            informacionColeccionDocs.incrementaNumDocs();
            informacionDocumento.setIdDoc(informacionColeccionDocs.getNumDocs()); 
        }
        // Si se trata de un documento ya indexado, se le asigna el idDoc del documento anteriormente indexado
        else
        {
            informacionDocumento.setIdDoc(idDoc); 
        }

        informacionDocumento.setTamBytes(len);
        // Sacamos información del documento
        struct stat documentFile;
        stat(documento.c_str(), &documentFile);
        Fecha fechaDocumento(*(gmtime(&(documentFile.st_mtime))));
        informacionDocumento.setFecha(fechaDocumento); // Guardamos la fecha del documento

        unordered_map<string, InformacionTermino>::iterator iteradorIndice;


        // Leemos línea a línea
        while(getline(ss, linea, '\n'))
        {
            tok.Tokenizar(linea, tokens);

            for(it = tokens.begin(); it != tokens.end(); it++)
            {
                stemmer.stemmer(*it, tipoStemmer);
                informacionDocumento.incrementaNumPal();

                if(stopWords.find(*it) == stopWords.end())  // Verificamos que no el stem no es una stopword
                {
                    informacionDocumento.incrementaNumPalSinParada();
                    iteradorIndice = indice.find(*it);

                    if(iteradorIndice == indice.end()) // Si el termino no esta indexado
                    {
                        InformacionTermino infoTermino;
                        InfTermDoc informacionTerminoDocumento;

                        informacionDocumento.incrementaNumPalDiferentes();
                        informacionColeccionDocs.incrementaNumTotalPalDiferentes();

                        // Inicializamos la información del termino en el documento
                        informacionTerminoDocumento.setFt(1);
                        informacionTerminoDocumento.insertarPosTerm(posicionTerminoDocumento);

                        // Actualizamos la información del termino
                        infoTermino.setFtc(1);
                        infoTermino.insertarInfTermDoc(informacionDocumento.getIdDoc(), informacionTerminoDocumento);
                        // Insertamos en el indice
                        indice.insert({*it, infoTermino});
                    }
                    else    // Si ya esta indexado
                    {
                        // Sacamos la InformacionTerminoDocumento
                        unordered_map<int, InfTermDoc>::iterator itInformacionTerminoDocumento = iteradorIndice->second.getL_docs().find(idDoc);  

                        if(itInformacionTerminoDocumento != iteradorIndice->second.getL_docs().end())   // Si ya esta en el documento
                        {
                            itInformacionTerminoDocumento->second.incrementarFt();
                            itInformacionTerminoDocumento->second.insertarPosTerm(posicionTerminoDocumento);
                        }
                        else    // Si existe pero no en el documento antiguo
                        {
                            informacionDocumento.incrementaNumPalDiferentes();
                            InfTermDoc informacionTerminoDocumento;

                            // Inicializamos la información del termino en el documento
                            informacionTerminoDocumento.setFt(1);
                            informacionTerminoDocumento.insertarPosTerm(posicionTerminoDocumento);
                            iteradorIndice->second.insertarInfTermDoc(informacionDocumento.getIdDoc(), informacionTerminoDocumento);
                        }

                        iteradorIndice->second.incrementarFtc();
                    }
                }
                else
                {
                    posicionTerminoDocumento++;
                }
            }
        }

        informacionColeccionDocs.incrementaNumTotalPal(informacionDocumento.getNumPal());
        informacionColeccionDocs.incrementaNumTotalPalSinParada(informacionDocumento.getNumPalSinParada());
        informacionColeccionDocs.sumTamBytes(informacionDocumento.getTamBytes());
        indiceDocs.insert({documento, informacionDocumento});

        return true;
    }
    // El documento no se ha podido abrir
    else
    {
        cerr << "No se pudo abrir el documento: " << documento << endl;
        return false;
    }
}

// Metodo para indexar desde un fichero con los documentos
bool IndexadorHash::Indexar(const string &ficheroDocumentos)
{
    int fd = open(ficheroDocumentos.c_str(), O_RDONLY);

    if(fd != -1)
    {
        // Abrimos el archivo con mmap
        int len = lseek(fd, 0, SEEK_END);   // len también es el tamaño del documento
        char *mbuf = (char*) mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);  
        string contFicheroDocs(mbuf), documento;
        stringstream ss(contFicheroDocs);

        // Estructura stat para sacar la fecha de modificación de los documentos
        struct stat infoDoc;
        
        // Cerramos el documento
        close(fd);

        // Leemos línea a línea del fichero
        while(getline(ss, documento, '\n'))
        {
            // Guardamos el iterador del indice que queremos buscar en nuestro hash de documentos indexados
            unordered_map<string, InfDoc>::iterator itIndiceDocumento = indiceDocs.find(documento);

            // Sacamos información del documento
            stat(documento.c_str(), &infoDoc);
            Fecha fechaDocumento(*(gmtime(&(infoDoc.st_mtime))));

            // Verificamos si existe el documento en nuestro indice de documentos
            if(itIndiceDocumento != indiceDocs.end())
            {
                int idDocIndexar = itIndiceDocumento->second.getIdDoc();

                // Eliminamos la indexación del fichero antiguo
                if(!BorraDoc(documento))
                {
                    cerr << "No se ha podido borrar la indexación antigua del documento: " << documento << endl;
                    return false;
                }

                // Si la fecha del documento a indexar es posterior al documento ya indexado volvemos a indexarlo
                if(itIndiceDocumento->second.getFecha() < fechaDocumento)
                {
                    return IndexarDocumento(documento, idDocIndexar);
                }
            }
            else
            {
                return IndexarDocumento(documento);
            }
        }
    }
    else
    {
        cerr << "No se ha podido abrir el documento: " << ficheroDocumentos << endl;
        return false;
    }
}

// Método para indexar un directorio
bool IndexadorHash::IndexarDirectorio(const string& dirAIndexar)
{
    struct stat dir;
    // Compruebo la existencia del directorio
    int err=stat(dirAIndexar.c_str(), &dir);
    if(err==-1 || !S_ISDIR(dir.st_mode))
        return false;
    else {
        // Hago una lista en un fichero con find>fich
        string cmd="find "+dirAIndexar+" -follow |sort > .lista_fich_index";
        system(cmd.c_str());
        return Indexar(".lista_fich_index");
    }
}

// Método que indexa la pregunta
bool IndexadorHash::IndexarPregunta(const string &preg)
{

}

// Devuelve el número de palabras indexadas
int IndexadorHash::NumPalIndexadas() const
{
    return this->indice.size();
}

// Devuelve el contenido de ficheroStopwords
string IndexadorHash::DevolverFichPalParada() const
{
    return this->ficheroStopWords;
}

// Lista por pantalla las palabras de parada
void IndexadorHash::ListarPalParada() const
{
    for(auto it = stopWords.begin(); it != stopWords.end(); it++)
    {
        cout << (*it) << endl;
    }
}

// Devuelve el número de palabras de parada
int IndexadorHash::NumPalParada() const
{
    return stopWords.size();
}

// Devuelve los delimitadores
string IndexadorHash::DevolverDelimitadores() const
{
    return tok.DelimitadoresPalabra();
}

// Devuelve la variable casos especiales del tokenizador
bool IndexadorHash::DevolverCasosEspeciales() const
{
    return tok.CasosEspeciales();
}

// Devuelve la variable pasar a minusculas del tokenizador
bool IndexadorHash::DevolverPasarAminuscSinAcentos() const
{
    return tok.PasarAminuscSinAcentos();
}

// Devuelve la variable almacenarPosTerm
bool IndexadorHash::DevolverAlmacenarPosTerm() const
{
    return almacenarPosTerm;
}

// Devuelve el directorio donde se almacena la indexacion
string IndexadorHash::DevolverDirIndice() const
{
    return directorioIndice;
}

// Devuelve la variable tipoSteming
int IndexadorHash::DevolverTipoStemming() const
{
    return tipoStemmer;
}

// Devuelve la variable almacenaEnDisco
bool IndexadorHash::DevolverAlmEnDisco() const
{
    return almacenarEnDisco;
}

