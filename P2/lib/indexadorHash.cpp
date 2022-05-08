#include "indexadorHash.h"
#include <sys/mman.h>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysinfo.h>


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
    this->ficheroStopWords = fichStopWords;

    this->tok = Tokenizador();
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

// Constructor por defecto
IndexadorHash::IndexadorHash()
{
    this->ficheroStopWords = "./StopWordsEspanyol.txt";
    GuardarStopWords(ficheroStopWords);
    this->tok = Tokenizador();
    this->directorioIndice = get_current_dir_name();
    this->tipoStemmer = 0;
    this->almacenarEnDisco = false;
    this->almacenarPosTerm = true;
}

// Constructor a raíz de una indexación previa
IndexadorHash::IndexadorHash(const string &directorioIndexacion)
{
    RecuperarIndexacion(directorioIndexacion);
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
    this->indicesEnDisco = copia.indicesEnDisco;
}

// Destructor
IndexadorHash::~IndexadorHash()
{
    VaciarIndicePreg();
    this->informacionColeccionDocs.~InfColeccionDocs();
    this->pregunta = "";
    this->infPregunta.~InformacionPregunta();
    this->stopWords.clear();
    this->ficheroStopWords = "";
    //this->tok.~Tokenizador();
    this->directorioIndice = "";
    this->tipoStemmer = 0;
    this->almacenarEnDisco = false;
    this->almacenarPosTerm = false; 
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

// Guardar el índice en disco
bool IndexadorHash::GuardarIndiceDisco()
{
    struct stat dir;
    string guardar = "";

    int err = stat(this->directorioIndice.c_str(), &dir);
    // Si no existe el directorio se crea
    if(err==-1 || !S_ISDIR(dir.st_mode))
        system(("mkdir "+this->directorioIndice).c_str());

    fstream fich((directorioIndice+"/indiceDisco" + to_string(this->indicesDisco)).c_str(), ios::out);

    if(fich.is_open())
    {
        // Guardamos indice
        for(unordered_map<string, InformacionTermino>::const_iterator it = indice.begin(); it != indice.end(); it++)
        {
            guardar += it->first + " ";
            guardar += to_string(it->second.getFtc()) + " ";
            for(unordered_map<int, InfTermDoc>::const_iterator itIndiceTerm = it->second.l_docs.begin(); itIndiceTerm != it->second.l_docs.end(); itIndiceTerm++)
            {
                guardar += to_string(itIndiceTerm->first) + " ";
                guardar += to_string(itIndiceTerm->second.getFt()) + " ";
                
                for(list<int>::const_iterator itPosTerm = itIndiceTerm->second.posTerm.begin(); itPosTerm != itIndiceTerm->second.posTerm.end(); itPosTerm++)
                {
                    guardar += to_string(*itPosTerm) + ",";
                }
                guardar += "\t";
            }
            guardar += "\n";
        }
        // Incrementamos la variable de veces que se ha guardado en disco
        this->indicesDisco++;
        // Insertamos el fichero donde se ha guardado la primera indexacion
        this->indicesEnDisco.push_back((directorioIndice+ "/indiceDisco" + to_string(this->indicesDisco)));

        // Escribimos en el fichero y lo guardamos
        fich << guardar;
        fich.close();

        return true;
    }
    return false;
}

// Guardar el índiceDocs en disco
bool IndexadorHash::GuardarIndiceDocsDisco()
{
    struct stat dir;
    string guardar = "";

    int err = stat(this->directorioIndice.c_str(), &dir);
    // Si no existe el directorio se crea
    if(err==-1 || !S_ISDIR(dir.st_mode))
        system(("mkdir "+this->directorioIndice).c_str());

    fstream fich((directorioIndice+"/indiceDocsDisco"+to_string(this->indicesDocsDisco)).c_str(), ios::out);

    if(fich.is_open())
    {
        // Guardamos el indiceDocs
        for(unordered_map<string, InfDoc>::const_iterator it = indiceDocs.begin(); it != indiceDocs.end(); it++)
        {
            guardar += it->first + ",";
            guardar += to_string(it->second.getIdDoc()) + ",";
            guardar += to_string(it->second.getNumPal()) + ",";
            guardar += to_string(it->second.getNumPalSinParada()) + ",";
            guardar += to_string(it->second.getNumPalDiferentes()) + ",";
            guardar += to_string(it->second.getTamBytes()) + ",";
            struct tm fecha = it->second.getFecha().getFecha();
            guardar += to_string(fecha.tm_sec) + ",";
            guardar += to_string(fecha.tm_min) + ",";
            guardar += to_string(fecha.tm_hour) + ",";
            guardar += to_string(fecha.tm_mday) + ",";
            guardar += to_string(fecha.tm_mon) + ",";
            guardar += to_string(fecha.tm_year) + ", ";
        }
        
        // Incrementamos la variable de veces que se ha guardado en disco indiceDocs
        this->indicesDocsDisco++;
        // Insertamos el fichero donde se ha guardado la primera indexacion
        this->indicesDocsEnDisco.push_back((directorioIndice+ "/indiceDocsDisco" + to_string(this->indicesDocsDisco)));

        // Escribimos en el fichero y lo guardamos
        fich << guardar;
        fich.close();

        return true;
    }
    return false;
}

// Metodo para indexar documento
bool IndexadorHash::IndexarDocumento(const string &documento, int idDoc)
{
    int fd = open(documento.c_str(), O_RDONLY);
    // Si el documento se ha abierto
    if(fd != -1)
    {
        // Abrimos el archivo con mmap
        int len = lseek(fd, 0, SEEK_END);   // len también es el tamaño del documento
        if(len < 0) // Si la longitud es menor que cero significa que es un documento vacio
            return true;
        // Guardamos el texto del documento en *mbuf
        char *mbuf = (char*) mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);  

        InfDoc informacionDocumento;
        list<string> tokens;
        list<string>::iterator it;
        stemmerPorter stemmer;
        int posicionTerminoDocumento = 0;
    
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

        // Tokenizamos el fichero entero
        tok.Tokenizar(mbuf, tokens);
        // Recorremos los tokens o terminos del documento
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
                    // Si almacenarPosTerm es true almacenamos la posicion del termino
                    if(almacenarPosTerm) informacionTerminoDocumento.insertarPosTerm(posicionTerminoDocumento);

                    // Actualizamos la información del termino
                    infoTermino.setFtc(1);
                    infoTermino.insertarInfTermDoc(informacionDocumento.getIdDoc(), informacionTerminoDocumento);
                    // Insertamos en el indice
                    indice.insert({*it, infoTermino});
                }
                else    // Si ya esta indexado
                {
                    // Sacamos la InformacionTerminoDocumento
                    unordered_map<int, InfTermDoc>::iterator itInformacionTerminoDocumento = iteradorIndice->second.l_docs.find(informacionDocumento.getIdDoc());  

                    if(itInformacionTerminoDocumento != iteradorIndice->second.l_docs.end())   // Si ya se encuentra en el documento actual
                    {
                        itInformacionTerminoDocumento->second.incrementarFt();
                        // Si almacenarPosTerm es true almacenamos la posicion del termino
                        if(almacenarPosTerm) itInformacionTerminoDocumento->second.insertarPosTerm(posicionTerminoDocumento);
                    }
                    else    // Si esta indexado pero no en el documento actual
                    {
                        informacionDocumento.incrementaNumPalDiferentes();
                        InfTermDoc informacionTerminoDocumento;

                        // Inicializamos la información del termino en el documento
                        informacionTerminoDocumento.setFt(1);
                        // Si almacenarPosTerm es true almacenamos la posicion del termino
                        if(almacenarPosTerm) informacionTerminoDocumento.insertarPosTerm(posicionTerminoDocumento);
                        iteradorIndice->second.insertarInfTermDoc(informacionDocumento.getIdDoc(), informacionTerminoDocumento);
                    }

                    iteradorIndice->second.incrementarFtc();
                }
            }                  
            posicionTerminoDocumento++;
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
    bool devolver = false;

    if(fd != -1)
    {
        // Abrimos el archivo con mmap
        int len = lseek(fd, 0, SEEK_END);   // len también es el tamaño del documento
        char *mbuf = (char*) mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);  
        string documento;
        stringstream ss(mbuf);

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
                // Si la fecha del documento a indexar es posterior al documento ya indexado volvemos a indexarlo
                if(itIndiceDocumento->second.getFecha() < fechaDocumento)
                {
                    // Eliminamos la indexación del fichero antiguo
                    if(!BorraDoc(documento))
                    {
                        cerr << "No se ha podido borrar la indexación antigua del documento: " << documento << endl;
                        return false;
                    }
                    devolver = IndexarDocumento(documento, idDocIndexar);
                }
            }
            else
            {
                devolver = IndexarDocumento(documento);
            }
            // Si la variable almacenarEnDisco está a true, almacenamos la indexacion en el disco duro
            if(almacenarEnDisco)
            {
                struct sysinfo memInfo; 
                sysinfo (&memInfo);
                // Sacamos la cantidad de memoria total
                long long totalPhysMem = memInfo.totalram; 
                totalPhysMem *= memInfo.mem_unit;

                // Sacamos la cantidad de memoria utilizada
                long long physMemUsed = memInfo.totalram - memInfo.freeram; 
                physMemUsed *= memInfo.mem_unit;

                // Si la memoria que estamos usando es mayor o igual a la mitad de la memoria total vamos a guardar los indices en memoria principal
                if(physMemUsed >= totalPhysMem/2)
                {
                    // Guardamos en disco
                    GuardarIndiceDisco();
                    GuardarIndiceDocsDisco();
                    // Eliminamos de la memoria principal para que no ocupe memoria
                    this->indice.clear();
                    this->indiceDocs.clear();
                }
            }
        }

        return devolver;
    }
    else
    {
        cerr << "No se ha podido abrir el documento: " << ficheroDocumentos << endl;
        return devolver;
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

// Metodo para guardar la indexación en el disco duro
bool IndexadorHash::GuardarIndexacion() const
{
    struct stat dir;
    string guardar = "";

    int err = stat(this->directorioIndice.c_str(), &dir);
    // Si no existe el directorio se crea
    if(err==-1 || !S_ISDIR(dir.st_mode))
        system(("mkdir "+this->directorioIndice).c_str());

    fstream fich((directorioIndice+"/indexacion").c_str(), ios::out);

    if(fich.is_open())
    {
        // Empezamos a guardar términos
        guardar += ficheroStopWords + "\n"; // Fichero de stopwords
        for(unordered_set<string>::const_iterator it = stopWords.begin(); it != stopWords.end(); it++)  // Palabras de parada
        {
            guardar += *it + ",";
        }
        guardar += "\n";
        guardar += to_string(tipoStemmer) + "\n";  // Guardamos tipoStemmer
        guardar += to_string(almacenarEnDisco) + "\n"; // Guardamos almacenarEnDisco
        guardar += to_string(almacenarPosTerm) + "\n"; // Guardamos almacenarPosTermino
        guardar += directorioIndice + "\n"; // Guardamos directorioIndice
        // Guardamos el tokenizador utilizado
        guardar += tok.DelimitadoresPalabra() + "\n";
        guardar += to_string(tok.CasosEspeciales()) + "\n";
        guardar += to_string(tok.PasarAminuscSinAcentos()) + "\n"; 
        // Guardamos el infPregunta
        guardar += to_string(infPregunta.getNumTotalPal()) + "\n";
        guardar += to_string(infPregunta.getNumTotalPalDiferentes()) + "\n";
        guardar += to_string(infPregunta.getNumTotalPalSinParada()) + "\n";
        // Guardamos el indicePregunta
        for(unordered_map<string, InformacionTerminoPregunta>::const_iterator it = indicePregunta.begin(); it != indicePregunta.end(); it++)
        {
            guardar += it->first + " ";
            guardar += to_string(it->second.getFt()) + " ";
            for(list<int>::const_iterator itPosTerm = it->second.posTerm.begin(); itPosTerm != it->second.posTerm.end(); itPosTerm++)
            {
                guardar += to_string(*itPosTerm) + ",";
            }
            guardar += "\t";
        }
        guardar += "\n";
        // Guardamos la pregunta indexada
        guardar += pregunta + "\n";
        // Guardamos infColeccionDocs
        guardar += to_string(informacionColeccionDocs.getNumDocs()) + "\n";
        guardar += to_string(informacionColeccionDocs.getNumTotalPal()) + "\n";
        guardar += to_string(informacionColeccionDocs.getNumTotalPalSinParada()) + "\n";
        guardar += to_string(informacionColeccionDocs.getNumTotalPalDiferentes()) + "\n";
        guardar += to_string(informacionColeccionDocs.getTamBytes()) + "\n";
        // Guardamos el indiceDocs
        for(unordered_map<string, InfDoc>::const_iterator it = indiceDocs.begin(); it != indiceDocs.end(); it++)
        {
            guardar += it->first + ",";
            guardar += to_string(it->second.getIdDoc()) + ",";
            guardar += to_string(it->second.getNumPal()) + ",";
            guardar += to_string(it->second.getNumPalSinParada()) + ",";
            guardar += to_string(it->second.getNumPalDiferentes()) + ",";
            guardar += to_string(it->second.getTamBytes()) + ",";
            struct tm fecha = it->second.getFecha().getFecha();
            guardar += to_string(fecha.tm_sec) + ",";
            guardar += to_string(fecha.tm_min) + ",";
            guardar += to_string(fecha.tm_hour) + ",";
            guardar += to_string(fecha.tm_mday) + ",";
            guardar += to_string(fecha.tm_mon) + ",";
            guardar += to_string(fecha.tm_year) + ", ";
        }
        guardar += "\n";
        // Guardamos indice
        for(unordered_map<string, InformacionTermino>::const_iterator it = indice.begin(); it != indice.end(); it++)
        {
            guardar += it->first + " ";
            guardar += to_string(it->second.getFtc()) + " ";
            for(unordered_map<int, InfTermDoc>::const_iterator itIndiceTerm = it->second.l_docs.begin(); itIndiceTerm != it->second.l_docs.end(); itIndiceTerm++)
            {
                guardar += to_string(itIndiceTerm->first) + " ";
                guardar += to_string(itIndiceTerm->second.getFt()) + " ";
                
                for(list<int>::const_iterator itPosTerm = itIndiceTerm->second.posTerm.begin(); itPosTerm != itIndiceTerm->second.posTerm.end(); itPosTerm++)
                {
                    guardar += to_string(*itPosTerm) + ",";
                }
                guardar += "\t";
            }
            guardar += "\n";
        }
        
        // Escribimos en el fichero y lo guardamos
        fich << guardar;
        fich.close();
        return true;
    }
    
    return false;
}

// Método para recuperar la indexación guardada en el disco duro
bool IndexadorHash::RecuperarIndexacion(const string &directorioIndexacion)
{
    int fd = open((directorioIndexacion+"/indexacion").c_str(), O_RDONLY);
    // Si el documento se ha abierto
    if(fd != -1)
    {
        // Abrimos el archivo con mmap
        int len = lseek(fd, 0, SEEK_END);   // len también es el tamaño del documento
        char *mbuf = (char*) mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);  
        stringstream ss(mbuf);
        string aux;
        close(fd);

        // Primero cogemos el directorio de las stopwords
        getline(ss, this->ficheroStopWords, '\n');
        // Guardamos todas las stopword
        getline(ss, aux, '\n');
        
        // Leemos y guardamos cada stopword
        stringstream ss2(aux);
        while(getline(ss2, aux, ','))
        {
            this->stopWords.insert(aux);
        }
        // TipoStemmer
        getline(ss, aux, '\n');
        this->tipoStemmer = stoi(aux);
        // AlmacenaEnDisco
        getline(ss, aux, '\n');
        this->almacenarEnDisco = (aux == "1");
        // Almacena posTerm
        getline(ss, aux, '\n');
        this->almacenarPosTerm = (aux == "1");
        // Guardamos el directorioIndice
        getline(ss, this->directorioIndice, '\n');
        // Cogemos los datos del tokenizador
        getline(ss, aux, '\n');
        this->tok.DelimitadoresPalabra(aux);
        getline(ss, aux, '\n');
        this->tok.CasosEspeciales((aux == "1"));
        getline(ss, aux, '\n');
        this->tok.PasarAminuscSinAcentos((aux == "1"));
        // Cogemos la información de infPregunta
        getline(ss, aux, '\n');
        infPregunta.setNumTotalPal(stoi(aux));
        getline(ss, aux, '\n');
        infPregunta.setNumTotalPalDiferentes(stoi(aux));
        getline(ss, aux, '\n');
        infPregunta.setNumTotalPalSinParada(stoi(aux));
        // Cogemos el indicePregunta
        getline(ss, aux, '\n');
        if(aux != "")
        {
            ss2 = stringstream(aux);
            while(getline(ss2, aux, '\t'))
            {
                InformacionTerminoPregunta infTerminoPregunta;
                string terminoPregunta;
                stringstream ss3(aux); 
                getline(ss3, terminoPregunta, ' ');
                getline(ss3, aux, ' ');
                infTerminoPregunta.setFt(stoi(aux));
        
                ss3 = stringstream(aux);
                while(getline(ss3, aux, ','))
                {
                    infTerminoPregunta.insertarPosTerm(stoi(aux));
                }
                this->indicePregunta.insert({terminoPregunta, infTerminoPregunta});
            }
        }
        // Cogemos la pregunta
        getline(ss, this->pregunta, '\n');   
        // Cogemos los datos de informacionColeccionDocs
        getline(ss, aux, '\n');
        this->informacionColeccionDocs.setNumDocs(stoi(aux));
        getline(ss, aux, '\n');
        this->informacionColeccionDocs.setNumTotalPal(stoi(aux));
        getline(ss, aux, '\n');
        this->informacionColeccionDocs.setNumTotalPalSinParada(stoi(aux));
        getline(ss, aux, '\n');
        this->informacionColeccionDocs.setNumTotalPalDiferentes(stoi(aux));
        getline(ss, aux, '\n');
        this->informacionColeccionDocs.setTamBytes(stoi(aux));
        // Cogemos la informacion de indiceDocs
        getline(ss, aux, '\n');
        ss2 = stringstream(aux);
        // Cogemos la info de cada documento
        while(getline(ss2, aux, ' '))
        {   
            stringstream ss3(aux);
            InfDoc inf;
            string nomDoc;
            getline(ss3, nomDoc, ',');
            getline(ss3, aux, ',');
            inf.setIdDoc(stoi(aux));
            getline(ss3, aux, ',');
            inf.setNumPal(stoi(aux));
            getline(ss3, aux, ',');
            inf.setNumPalSinParada(stoi(aux));
            getline(ss3, aux, ',');
            inf.setNumPalDiferentes(stoi(aux));
            getline(ss3, aux, ',');
            inf.setTamBytes(stoi(aux));
            struct tm fecha;
            getline(ss3, aux, ',');
            fecha.tm_sec = stoi(aux);
            getline(ss3, aux, ',');
            fecha.tm_min = stoi(aux);
            getline(ss3, aux, ',');
            fecha.tm_hour = stoi(aux);
            getline(ss3, aux, ',');
            fecha.tm_mday = stoi(aux);
            getline(ss3, aux, ',');
            fecha.tm_mon = stoi(aux);
            getline(ss3, aux, ',');
            fecha.tm_year = stoi(aux);

            inf.setFecha(fecha);
            this->indiceDocs.insert({nomDoc, inf});
        }
        // Cogemos la informacion de los terminos indice
        while(getline(ss, aux, '\n'))
        {
            string nomIndice;
            InformacionTermino inf;
            ss2 = stringstream(aux);

            getline(ss2, nomIndice, ' ');
            getline(ss2, aux, ' ');
            inf.setFtc(stoi(aux));
            // Cogemos la coleccion de l_docs
            while(getline(ss2, aux, '\t'))
            {
                int idDoc;
                InfTermDoc infTerm;
                stringstream ss3(aux);
                getline(ss3, aux, ' ');
                idDoc = stoi(aux);
                getline(ss3, aux, ' ');
                infTerm.setFt(stoi(aux));
                // Leemos la informacion termino documento
                while(getline(ss3, aux, ','))
                {
                    infTerm.posTerm.push_back(stoi(aux));
                }
                inf.l_docs.insert({idDoc, infTerm});
            }
            // Insertamos en el indice
            this->indice.insert({nomIndice, inf});
        }

        return true;
    }
    return true;
}

// Método para imprimir la indexacion
void IndexadorHash::ImprimirIndexacion() const
{
    cout << "Terminos indexados: " << endl;
    for(unordered_map<string, InformacionTermino>::const_iterator it = indice.begin(); it != indice.end(); it++)
    {
        cout << it->first << "\t" << it->second << endl;
    }

    cout << "Documentos indexados: " << endl;
    for(unordered_map<string, InfDoc>::const_iterator it = indiceDocs.begin(); it != indiceDocs.end(); it++)
    {
        cout << it->first << "\t" << it->second << endl;
    }
}

// Método que indexa la informacion de la pregunta
bool IndexadorHash::IndexarPregunta(const string &preg)
{
    int posicionTerminoPregunta = 0;
    infPregunta = InformacionPregunta();
    indicePregunta.clear();

    if(!preg.empty())
    {
        this->pregunta = preg;

        list<string> tokensPregunta;
        tok.Tokenizar(preg, tokensPregunta);
        stemmerPorter stemmer;

        // Recorremos todos los tokens
        for(list<string>::iterator it = tokensPregunta.begin(); it != tokensPregunta.end(); it++)
        {
            infPregunta.incrementaNumTotalPal();
            stemmer.stemmer(*it, tipoStemmer);

            if(stopWords.find(*it) == stopWords.end())
            {
                unordered_map<string, InformacionTerminoPregunta>::iterator iteradorIndicePregunta = indicePregunta.find(*it);

                // Si el termino ya se encuentra indexado
                if(iteradorIndicePregunta != indicePregunta.end())
                {
                    iteradorIndicePregunta->second.incrementarFt();
                    iteradorIndicePregunta->second.insertarPosTerm(posicionTerminoPregunta);
                }
                else    // Si no esta indexado
                {
                    InformacionTerminoPregunta informacionTermPreg;
                    informacionTermPreg.setFt(1);
                    informacionTermPreg.insertarPosTerm(posicionTerminoPregunta);

                    indicePregunta.insert({*it, informacionTermPreg});

                    infPregunta.incrementaNumTotalPalDiferentes();
                }
                infPregunta.incrementaNumTotalPalSinParada();
            }
            posicionTerminoPregunta++;
        }

        // Miramos si la pregunta estaba compuesta por solo palabras de parada
        if(infPregunta.getNumTotalPalSinParada() == 0)
        {
            cerr << "Pregunta con todo términos de parada" << endl;
            return false;
        }
        else
        {
            return true;
        }
    }

    cerr << "La pregunta no tiene términos" << endl;
    return false;
}

// Metodo que devuelve true o false en funcion de si la pregunta esta indexada y devuelve la pregunta
bool IndexadorHash::DevuelvePregunta(string& preg) const
{
    if(!pregunta.empty())
    {
        preg = pregunta;
        return true;
    }
    return false;
}

// Metodo que devuelve true o false si existe la pregunta indexada
bool IndexadorHash::DevuelvePregunta(const string& word, InformacionTerminoPregunta& inf) const
{
    // Aplicamos stemming y pasar a minusculas la palabra
    string wordAux = aplicarStemYMayus(word);
    unordered_map<string, InformacionTerminoPregunta>::const_iterator it = indicePregunta.find(wordAux);
    
    if(it != indicePregunta.end())
    {
        inf = it->second;
        return true;
    }
    inf.~InformacionTerminoPregunta();
    return false;
}

// Metodo que devuelve true o false en funcion de si la pregunta esta indexada y devuelve la pregunta
bool IndexadorHash::DevuelvePregunta(InformacionPregunta& inf) const
{
    if(!pregunta.empty())
    {
        inf = infPregunta;
        return true;
    }
    inf.~InformacionPregunta();
    return false;
}

// Imprime la indexacion de la pregunta
void IndexadorHash::ImprimirIndexacionPregunta()
{
    cout << "Pregunta indexada: " << pregunta << endl;
    cout << "Terminos indexados en la pregunta: " << endl;
    // A continuación aparecerá un listado del contenido de indicePregunta
    for(unordered_map<string, InformacionTerminoPregunta>::iterator it = indicePregunta.begin(); it != indicePregunta.end(); it++)
    {
        cout << it->first << "\t" << it->second << endl; 
    }
    cout << "Informacion de la pregunta: " << infPregunta << endl;
}

// Imprime pregunta
void IndexadorHash::ImprimirPregunta()
{
    cout << "Pregunta indexada: " << pregunta << endl;
    cout << "Informacion de la pregunta: " << infPregunta << endl;
}

// Devuelve true si word esta indexada y devuelve la informacion
bool IndexadorHash::Devuelve(const string& word, InformacionTermino& inf) const
{
    string wordAux = aplicarStemYMayus(word);
    unordered_map<string, InformacionTermino>::const_iterator it = indice.find(wordAux);

    if(it != indice.end())
    {
        inf = it->second;
        return true;
    }
    inf.~InformacionTermino();
    return false;
}

// Devuelve true si word esta indexada y aparece en el documento nomDoc, devolviendo la informacion
bool IndexadorHash::Devuelve(const string& word, const string& nomDoc, InfTermDoc& infDoc) const
{
    string wordAux = aplicarStemYMayus(word);
    unordered_map<string, InformacionTermino>::const_iterator iteradorIndice = indice.find(wordAux);

    // Miramos si la palabra esta indexada
    if(iteradorIndice != indice.end())
    {
        unordered_map<string, InfDoc>::const_iterator iteradorIndiceDocs = indiceDocs.find(nomDoc);
        
        // Miramos si el documento esta indexado
        if(iteradorIndiceDocs != indiceDocs.end())
        {
            unordered_map<int, InfTermDoc>::const_iterator itInformacionTerminoDocumento = iteradorIndice->second.l_docs.find(iteradorIndiceDocs->second.getIdDoc());
            // Miramos si el termino tiene el documento en su lista de documentos
            if(itInformacionTerminoDocumento != iteradorIndice->second.l_docs.end())
            {
                infDoc = itInformacionTerminoDocumento->second;
                return true;
            }
        }
    }
    infDoc.~InfTermDoc();
    return false;
}

// Devuelve true si word aparece indexado
bool IndexadorHash::Existe(const string& word) const
{
    string wordAux = aplicarStemYMayus(word);
    return indice.find(wordAux) != indice.end();
}

// Devuelve true si la palabra se borra de la indexacion
bool IndexadorHash::Borra(const string& word)
{
    string wordAux = aplicarStemYMayus(word);
    return indice.erase(wordAux) != 0;
}

// Devuelve true si nomdoc esta indexado y se logra borrar
bool IndexadorHash::BorraDoc(const string &nomDoc)
{
    unordered_map<string, InfDoc>::iterator iteradorIndiceDocs = indiceDocs.find(nomDoc);   // Buscamos el documento en el indice de documentos

    if(iteradorIndiceDocs != indiceDocs.end()) 
    {
        int idDoc = iteradorIndiceDocs->second.getIdDoc();
        unordered_map<string, InformacionTermino>::iterator iteradorIndice = indice.begin();

        while(iteradorIndice != indice.end())
        {
            unordered_map<int, InfTermDoc>::iterator iteradorLDocs = iteradorIndice->second.l_docs.find(idDoc);
            if(iteradorLDocs != iteradorIndice->second.l_docs.end())
            {
                iteradorIndice->second.restarFtc(iteradorLDocs->second.getFt());
                iteradorIndice->second.l_docs.erase(iteradorLDocs);

                if(iteradorIndice->second.l_docs.empty())
                {
                    iteradorIndice = indice.erase(iteradorIndice);
                    informacionColeccionDocs.decrementaNumTotalPalDiferentes();
                }
                else
                {
                    iteradorIndice++;
                }
            }
            else
            {
                iteradorIndice++;
            }
        }
        // Restamos los datos de la coleccion de documentos
        informacionColeccionDocs.decrementaNumDocs();
		informacionColeccionDocs.restaNumTotalPal(iteradorIndiceDocs->second.getNumPal());
		informacionColeccionDocs.restaNumTotalPalSinParada(iteradorIndiceDocs->second.getNumPalSinParada());
		informacionColeccionDocs.restaTamBytes(iteradorIndiceDocs->second.getTamBytes());

		indiceDocs.erase(iteradorIndiceDocs);
		return true;
    }   
    
    return false;
}

// Vacia todos los indices de la pregunta actual
void IndexadorHash::VaciarIndicePreg()
{
    indice.clear();
    indiceDocs.clear();
    indicePregunta.clear();
}

// Devuelve true si esta indexado y se actualiza
bool IndexadorHash::Actualiza(const string& word, const InformacionTermino& inf)
{
    if(Existe(word))
    {
        indice[aplicarStemYMayus(word)] = inf;
        return true;
    }
    cerr << "ERROR: En actualiza no existe la palabra " << word <<endl;
    return false;
}

// Sera true si inserta la informacion del termino si word no estaba indexado
bool IndexadorHash::Inserta(const string& word, const InformacionTermino& inf)
{
    if(!Existe(word))
    {
        indice.insert({aplicarStemYMayus(word), inf});

        return true;
    }
    return false;
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
    for(unordered_set<string>::const_iterator it = stopWords.begin(); it != stopWords.end(); it++)
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

// Mostrar por pantalla: infoColeccionDocs
void IndexadorHash::ListarInfColeccDocs() const
{
    cout << informacionColeccionDocs << endl;
}        

// Muestra por pantalla los términos indexados
void IndexadorHash::ListarTerminos() const
{
    for(unordered_map<string, InformacionTermino>::const_iterator it = indice.begin(); it != indice.end(); it++)
    {
        cout << it->first << "\t" << it->second << endl;
    }
}

// Muestra por pantalla los términos indexados del documento nomDoc
bool IndexadorHash::ListarTerminos(const string& nomDoc) const
{
    unordered_map<string, InfDoc>::const_iterator iteradorDocumentos = indiceDocs.find(nomDoc);

    if(iteradorDocumentos != indiceDocs.end())
    {
        for(unordered_map<string, InformacionTermino>::const_iterator iteradorIndice = indice.begin(); iteradorIndice != indice.end(); iteradorIndice++)
        {
            if(iteradorIndice->second.l_docs.find(iteradorDocumentos->second.getIdDoc()) != iteradorIndice->second.l_docs.end())
            {
                cout << iteradorIndice->first << "\t" << iteradorIndice->second << endl;
            }
        }
        return true;
    }   
    return false;
}

// Muestra por pantalla la lista de documentos indexados
void IndexadorHash::ListarDocs() const
{
    for(unordered_map<string, InfDoc>::const_iterator it = indiceDocs.begin(); it != indiceDocs.end(); it++)
    {
        cout << it->first << "\t" << it->second << endl;
    }
}
    
// Muestra por pantalla la lista de documentos con nomDoc y devuelve true si lo encuentra
bool IndexadorHash::ListarDocs(const string& nomDoc) const
{
    unordered_map<string, InfDoc>::const_iterator iteradorDocumentos = indiceDocs.find(nomDoc);

    if(iteradorDocumentos != indiceDocs.end())
    {
        cout << iteradorDocumentos->first << "\t" << iteradorDocumentos->second << endl;
        return true;
    }
    else
    {
        //cerr << "ERROR: No existe el documento: " << nomDoc << endl;
        return false;
    }
}

// Metodo para aplicar stemming y pasar a minúsculas una palabra
string IndexadorHash::aplicarStemYMayus(string word) const
{
    if(this->DevolverPasarAminuscSinAcentos())
    {
        word = this->tok.convertirSinMayusSinAcen(word);
    }
    stemmerPorter stemmer;
    stemmer.stemmer(word, tipoStemmer);

    return word;
}