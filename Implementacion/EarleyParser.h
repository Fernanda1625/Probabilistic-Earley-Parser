#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
#include <string.h>
#include <list>

#include "Produccion.h"
#include "Estado.h"
#include "Nodo.h"

using namespace std;

class EarleyParser{

	public:

		EarleyParser();

		virtual ~EarleyParser();

		void inicializa( string rutaArchivo, bool probabilidadParser );
		void crearDiccionario( string rutaArchivo );
		void crearVectorCadenaEntrada( string rutaArchivo );

		int crearChart( string principalNoTerminal );
		void parse();

		void printProducciones( string principalNoTerminal );
		vector<Nodo*>* returnAnalisisArboles( string principalNoTerminal );

	private:

		bool probabilidadParser;

		typedef unordered_map < string, list<Produccion> > produccionHashTable;
		produccionHashTable produccionesEarley;

		typedef unordered_map < string, string > diccionarioHashTable;
		diccionarioHashTable diccionarioEarley;

		vector<vector< Estado* >> chartEarleyParser;

		vector < string > inputVector;

		vector <Nodo*> returnVector;

		list<Produccion>* getProduccion( string noTerminal );

		void predecir( Estado* estadoExpandir, int posicionCadenaEntrada);
		void escanear( Estado* estadoEscanear, int posicionCadenaEntrada );
		void completar( Estado* estadoCompleto, int posicionCadenaEntrada );

		int estadoExiste( int posicionChart, Estado* verificarEstado );

		void printEstadoSeguimiento( Estado* imprimirEstado );

		void dividirVector( const string& cadenaEntrada, vector <string>* vectorSalida, const string& separadorCadena, bool omitirCadenasNulas );

};
