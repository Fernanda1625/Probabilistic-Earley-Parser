#include "Produccion.h"
#include "Estado.h"

#include <unordered_map>
#include <list>

#ifndef NODO_H_
#define NODO_H_

using namespace std;

class Nodo{

 public:

		Nodo( Nodo* padre, string elementoGramatica, vector <string> produccionNodo, Produccion* punteroProduccion, float probabilidadProduccion );

		virtual ~Nodo();

		Estado* nodoExpansion( Estado* punteroExpandirEstado, unordered_map< string, list<Produccion>>* produccionesEarley );

		float print( bool printProbabilidades );

		Produccion* getPunteroProduccion();

	private:

		explicit Nodo() {}

		string elementoGramatica;
		vector <string> produccion;
		Nodo* padre;

		vector <Nodo*> hijos;

		float probabilidadProduccion;

		Produccion* punteroProduccion;

};

#endif /* NODO_H_ */
