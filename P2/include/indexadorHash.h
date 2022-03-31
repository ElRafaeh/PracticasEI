#ifndef __INDEXADORHASH__
#define __INDEXADORHASH__

#include <iostream>

// Clase indexadorHash
class IndexadorHash {
    friend ostream& operator<<(ostream& s, const IndexadorHash& p) {
    s << "Fichero con el listado de palabras de parada: " << p.ficheroStopWords << endl;
    s << "Tokenizador: " << p.tok << endl;
    s << "Directorio donde se almacenara el indice generado: " p.directorioIndice << endl;
    s << "Stemmer utilizado: " << p.tipoStemmer << endl;
    s << "Informacion de la coleccion indexada: " p.informacionColeccionDocs << endl;
    s << "Se almacenara parte del indice en disco duro: " p.almacenarEnDisco << endl;
    s << "Se almacenaran las posiciones de los terminos: " p.almacenarPosTerm;
    return s;
}

#endif