#include "EarleyParser.h"

using namespace std;

EarleyParser::EarleyParser(){

}

EarleyParser::~EarleyParser(){
	for( unsigned int i = 0; i <= this -> inputVector.size(); i++ ){
		for( vector<Estado*>::iterator it = this -> chartEarleyParser.at(i).begin(); it != this -> chartEarleyParser.at(i).end(); it++ ){
			delete *it;
		}

	}

	for( vector<Nodo*>::iterator it = this -> returnVector.begin(); it != this -> returnVector.end(); it++ ){
		delete *(it);
	}
}

void EarleyParser::inicializa( string rutaArchivo, bool probabilidadParser ){
	this -> probabilidadParser = probabilidadParser;

	string linea;
	ifstream inputArchivo;

	inputArchivo.open( rutaArchivo.c_str(), fstream::in );

	if( inputArchivo.is_open() ){
		while( inputArchivo.good() ){
			/*
			 * lee una línea (producción) de la gramática de entrada libre de contexto
			 */
			getline( inputArchivo, linea );

			//salta la línea vacía
			if( linea.size() == 0 ) continue;

			cout << "[DEBUG]: " << linea << endl;

			/*
			*extrae tokens de la producción
			*separados por "->"
			*el primer token es el símbolo de terminal
			*el segundo token es la regla de producción
			*la tercera parte es la probabilidad de producción
			 */
			vector< string > tokenVector;
			this -> dividirVector( linea, &tokenVector, "->", false );

			//omite la línea de comentarios
			if( tokenVector.at(0) == "#" ) continue;

			for( unsigned int i = 0; i < tokenVector.size(); i++ ){
				cout << "[DEBUG]: " << "token de linea: [" << tokenVector.at(i) << "]" << endl;
			}

			/*
			 *extrae todos los símbolos en la producción
			 *separados por " "
			*/
			vector< string > produccionTokenVector;
			this -> dividirVector( tokenVector.at(1), &produccionTokenVector, " ", false );
			for( unsigned int i = 0; i < produccionTokenVector.size(); i++ ){
				cout << "[DEBUG]: " << "token de produccion: [" << produccionTokenVector.at(i) << "]" << endl;
			}

			/*
			 * busca en la tabla hash de producciones
			 * si el símbolo no terminal ya estaba insertado
			*/
			EarleyParser::produccionHashTable::iterator got = this -> produccionesEarley.find( tokenVector.at(0) );
			if( got != this -> produccionesEarley.end() ){
				/*
				 * elemento encontrado en la tabla hash ( key tokenVector -> at (0) contenido)
				 * agregue una nueva producción al final de la lista
				*/
				cout << "[DEBUG]: encontro [" << got -> first << "] agregar produccion a la lista existente" << endl << endl;

				got -> second.push_back( Produccion( atof( tokenVector.at(2).c_str() ), produccionTokenVector.size(), produccionTokenVector ) );

			}else{
				/*
				 * elemento no encontrado en la tabla hash (key tokenVector-> at (0) no contenido)
				 * crear una nueva lista y guardarla en un mapa hash
				 * agrega nueva producción a la lista
				*/
				list<Produccion> produccionLista;

				pair< string, list<Produccion> > producciones( tokenVector.at(0), produccionLista );

				pair<EarleyParser::produccionHashTable::iterator, bool> resultadoInsercion;
				resultadoInsercion = this -> produccionesEarley.insert( producciones );
				if( resultadoInsercion.second == true ){
					/*
					 * elemento insertado con éxito
					*/
					resultadoInsercion.first->second.push_back( Produccion( atof( tokenVector.at(2).c_str() ), produccionTokenVector.size(), produccionTokenVector ) );

					cout << "[DEBUG]: nueva lista creada [" << producciones.first << "] y Produccion insertada con éxito" << endl << endl;
				}
			}

		}
		inputArchivo.close();

		/*
		 * comprobar la coherencia de las probabilidades si lo especifica el booleano de entrada
		*/
		if( this -> probabilidadParser == true ){

			for( produccionHashTable::iterator i = this -> produccionesEarley.begin(); i != this -> produccionesEarley.end(); i++ ){

				float probabilidadProduccion = 0;

				cout << "[DEBUG]: comprobando la probabilidad de [" << i -> first << "]" << endl;

				for( list<Produccion>::iterator j = i -> second.begin(); j != i -> second.end(); j++ ){

					probabilidadProduccion += j -> getProbabilidadProduccion();
				}
				if( probabilidadProduccion != 1 ){
					cout << "[DEBUG]: probabilidad de [" << i -> first << "] no suma 1! - MALA ENTRADA DE GRAMÁTICA" << endl;
					exit(1);
				}
			}
		}

	}else{
		cout << "Error: no se puede abrir el archivo de entrada [" << rutaArchivo << "]" << endl;
	}
}

void EarleyParser::crearDiccionario( string rutaArchivo ){
	string linea;
	ifstream inputArchivo;

	inputArchivo.open( rutaArchivo.c_str(), fstream::in );

	if( inputArchivo.is_open() ){
		while( inputArchivo.good() ){
			/*
			 * lee una línea (producción) de entrada de la gramática libre de contexto
			*/
			getline( inputArchivo, linea );

			//salta la línea vacía
			if( linea.size() == 0 ) continue;

			cout << "[DEBUG]: " << linea << endl;

			/*
			 * extrae tokens de la producción
			 * separados por "->"
			 * primer token es una palabra
			 * el segundo token es el no terminal asociado
			*/
			vector< string > tokenVector;
			this -> dividirVector( linea, &tokenVector, "->", false );

			//omite las líneas de comentarios
			if( tokenVector.at(0) == "#" ) continue;

			for( unsigned int i = 0; i < tokenVector.size(); i++ ){
				cout << "[DEBUG]: " << "token de linea: [" << tokenVector.at(i) << "]" << endl;
			}

			/*
			 * busca en la tabla hash del diccionario
			 * si la palabra ya estaba insertada
			*/
			EarleyParser::diccionarioHashTable::iterator got = this -> diccionarioEarley.find( tokenVector.at(0) );
			if( got != this -> diccionarioEarley.end() ){
				/*
     			 * elemento encontrado en la tabla hash (key tokenVector-> at (0) contenido)
				 * ¡error!
				*/

				cout << "[DEBUG]: ¡MAL DICCIONARIO DE ENTRADA!" << endl << endl;
				exit(1);
			}else{
				/*
				 * elemento no encontrado en la tabla hash (key tokenVector-> at (0) no contenido)
				 * guardar cadena en el mapa hash
				*/
				pair< string, string > elemento( tokenVector.at(0), tokenVector.at(1) );

				pair<EarleyParser::diccionarioHashTable::iterator, bool> resultadoInsercion;
				resultadoInsercion = this -> diccionarioEarley.insert( elemento );
				if( resultadoInsercion.second == true ){
					/*
					 * elemento insertado con éxito
					*/
					cout << "[DEBUG]: [" << elemento.first << "][" << elemento.second << "] insertado con exito" << endl << endl;
				}
			}

		}
		inputArchivo.close();

		for( diccionarioHashTable::iterator i = this -> diccionarioEarley.begin(); i != this -> diccionarioEarley.end(); i++ ){

			cout << "[DEBUG]: elemento [" << i -> first << "][" << i -> second << "] en diccionario" << endl;
		}

	}else{
		cout << "Error: no se puede abrir el archivo de entrada [" << rutaArchivo << "]" << endl;
	}
}

void EarleyParser::crearVectorCadenaEntrada( string rutaArchivo ){
	string linea;
	ifstream inputArchivo;
	string inputCadenaAnalizar;

	inputArchivo.open( rutaArchivo.c_str(), fstream::in );

	if( inputArchivo.is_open() ){
		/*
		 *lee una línea (producción) de entrada de la gramática libre de contexto
		 */
		getline( inputArchivo, linea );

		/*
		 * extrae palabras de la cadena de entrada
         * separado por " "
		*/
		this -> dividirVector( linea, & this -> inputVector, " ", false );

		for( unsigned int i = 0; i < this -> inputVector.size(); i++ ){
			cout << "[DEBUG]: " << "token de linea: [" << this -> inputVector.at(i) << "]" << endl;

			EarleyParser::diccionarioHashTable::iterator got = this -> diccionarioEarley.find( this -> inputVector.at(i) );
			if( got != this -> diccionarioEarley.end() ){
				/*
				 * elemento encontrado en la tabla hash (key tokenVector-> en (i) contenido)
				 */
				if( i == 0 )
					inputCadenaAnalizar += got -> second;
				else
					inputCadenaAnalizar = inputCadenaAnalizar + " " + got -> second;

			}else{
				/*
				 * elemento no encontrado en la tabla hash (key tokenVector-> at (i) no contenido)
                 * error en la cadena de entrada, palabra no contenida en el diccionario
				*/
				cout << "[DEBUG]: ¡CADENA DE ENTRADA MALA! Palabra" << this->inputVector.at(i) << " no contenida en el diccionario" << endl << endl;
				exit(1);
			}
		}

		cout << "[DEBUG]: cadena de entrada del parser [" << inputCadenaAnalizar << "]" << endl << endl;

		inputArchivo.close();

		/*
		 * extrae palabras de la cadena de entrada para analizar
         * separadas por " "
		*/
		this -> dividirVector( inputCadenaAnalizar, & this -> inputVector, " ", false );

		for( unsigned int i = 0; i < this -> inputVector.size(); i++ ){
			cout << "[DEBUG]: " << "elemento de vector de entrada: [" << this -> inputVector.at(i) << "]" << endl;
		}


	}else{
		cout << "Error: no se puede abrir el archivo de entrada [" << rutaArchivo << "]" << endl;
	}
}

list<Produccion>* EarleyParser::getProduccion( string noTerminal ){
	/*
	* busca en la tabla hash de producciones
  * si el símbolo no terminal ya estaba insertado
	*/
	EarleyParser::produccionHashTable::iterator got = this -> produccionesEarley.find( noTerminal );
	if( got != this -> produccionesEarley.end() ){
		/*
		 * elemento encontrado en tabla hash (clave noTerminal contenida)
		*/
		//cout << "[DEBUG]: encontro [" << got -> first << "]" << endl;

	}else{
		/*
		 * elemento no encontrado en la tabla hash (clave noTerminal no contenida)
		*/
		//cout << "[DEBUG]: Error no encontrado [" << noTerminal << "] en mapa hash!" << endl << endl;
		return NULL;
	}

	return & got -> second;
}

int EarleyParser::crearChart( string principalNoTerminal ){
	list<Produccion>* produccionPrincipal;

	produccionPrincipal = this -> getProduccion( principalNoTerminal );
	if( produccionPrincipal == NULL ) return -1;
	  /*
	  * proporcionando la semilla del algoritmo de análisis
    * producción principal de frase en la gramática de contexto
	  */
	this->chartEarleyParser.resize( this -> inputVector.size() + 1 );

	cout << "[DEBUG]: tamanio de la lista (numero de producciones con " << principalNoTerminal << " como no terminal) = " << produccionPrincipal -> size() << endl;

	for( list<Produccion>::iterator it = produccionPrincipal -> begin(); it != produccionPrincipal -> end(); it++ ){
		for( unsigned int j = 0; j < it -> getContenidoProduccion().size(); j++ ){
			cout << "[DEBUG]: elemento de lista " << it -> getContenidoProduccion().at(j) << endl;
		}

		Estado *estadoInicial = new Estado(principalNoTerminal, &(*it), NULL, 0, 0 );
		this -> chartEarleyParser[0].push_back( estadoInicial );
	}

	return 1;
}

void EarleyParser::parse(){
	/*
	 * ciclo principal del Earley Parser
	*/
	for( unsigned int i = 0; i <= this -> inputVector.size(); i++ ){
		cout << i << "  *****************************************************" << endl << "*****************************************************" << endl;
		/*
		 * ciclos sobre todo el chart
		*/
		for( unsigned int j = 0; j < this -> chartEarleyParser[i].size(); j++ ){
			if( !( this->chartEarleyParser[i].at(j) -> Completo() ) ){
				/*
				 * el estado no está completo
                 * la producción correspondiente al estado no ha
                 * ha sido analizada completamente
				*/
				if( this -> getProduccion( this -> chartEarleyParser[i].at(j) -> SiguienteSimboloProcesar() ) != NULL )
				{
					/*
					 * El símbolo para procesar no es un terminal y está contenido en el
					 * mapa hash de producción inicializado a partir de la gramática libre de contexto de entrada
					 *
					 * se debe predecir la correcta producción del símbolo
					*/
					this -> predecir( this -> chartEarleyParser[i].at(j), i );

				}else{
					/*
					 * símbolo para procesar es un símbolo de terminal
					 * que no tiene más expansiones
					 *
					 * el símbolo debe escanearse simplemente
					*/
					this -> escanear( this -> chartEarleyParser[i].at(j), i );
				}
			}else{
				/*
				 * el estado está completo
				 * todos los símbolos de la producción correspondiente han sido
   			 * expandidos (no terminales) o escaneados (terminales)
				 */
				this -> completar( this -> chartEarleyParser[i].at(j), i );
			}
		}
	}

	cout << "[DEBUG]: termino de analizar la cadena de entrada!" << endl << endl;
}

int EarleyParser::estadoExiste( int posicionChart, Estado* estadoAnalizar ){
	for( vector<Estado*>::iterator it = this -> chartEarleyParser[ posicionChart].begin(); it != this -> chartEarleyParser[ posicionChart ].end(); it++ ){
		if( (*it) -> getNoTerminal() == estadoAnalizar -> getNoTerminal() &&
			(*it) -> getProgresoCadenaEntrada() == estadoAnalizar -> getProgresoCadenaEntrada() &&
			(*it) -> getPunteroProduccion() == estadoAnalizar -> getPunteroProduccion() &&
			(*it) -> getProgresoProduccion() == estadoAnalizar -> getProgresoProduccion() )
		{
			cout << "[DEBUG]: encontro el mismo estado en la posicion " << it - this -> chartEarleyParser[ posicionChart ].begin() << " de chart[" << posicionChart << "]" << endl;
			return ( it - this -> chartEarleyParser[ posicionChart ].begin());
		}
	}
	return -1;
}

void EarleyParser::predecir( Estado* estadoExpandir, int posicionCadenaEntrada){
	Produccion* produccionEstadoExpandir = estadoExpandir -> getPunteroProduccion();

	if( estadoExpandir -> getProgresoProduccion() >= produccionEstadoExpandir -> getContenidoProduccion().size() ){
		cout << "[ERROR]: desbordamiento en predictor!" << endl;
		exit(1);
	}
	string noTerminalExpandir = produccionEstadoExpandir -> getContenidoProduccion().at( estadoExpandir -> getProgresoProduccion() );

	cout << "[DEBUG]:predictor en expansión[" << noTerminalExpandir << "][" << estadoExpandir -> getProgresoProduccion() << "][" << estadoExpandir -> getProgresoCadenaEntrada() << "]" << endl;

	list<Produccion>* punteroListaProducciones = this -> getProduccion( noTerminalExpandir );

	if( punteroListaProducciones != NULL ){
		for( list<Produccion>::iterator it = punteroListaProducciones -> begin(); it != punteroListaProducciones -> end(); it++ ){
			Estado *estadoInsertar = new Estado( noTerminalExpandir, &(*it), estadoExpandir, posicionCadenaEntrada, 0 );

			cout << "POSICION DE LA CADENA DE ENTRADA: " << posicionCadenaEntrada << " " << this -> inputVector.size() << endl <<endl;

			int posicionEstado = this -> estadoExiste( posicionCadenaEntrada, estadoInsertar );
			if(  posicionEstado < 0 ){
				/*
				* estado ya no está en el chart
				*/

				this -> chartEarleyParser[ posicionCadenaEntrada ].push_back( estadoInsertar );
			}else{
				/*
				*estado ya en el chart
				*empuja hacia atrás el estado del que viene dentro de su vector de estado de retroceso
				*/
				this -> chartEarleyParser[ posicionCadenaEntrada ].at( posicionEstado ) -> getVectorSeguimientoEstado() -> push_back( estadoExpandir );
				delete estadoInsertar;
			}
		}
	}else{
		/*
		 * hacer nada
		 */
		cout << "[DEBUG]: ERROR predicción para expandirse[" << noTerminalExpandir<< "] en estado no encontrado en la tabla hash de predicción " << endl;
		exit(1);
	}
}

void EarleyParser::escanear( Estado* estadoEscanear, int posicionCadenaEntrada ){
    Produccion* produccionEstadoEscanear = estadoEscanear -> getPunteroProduccion();

	if( posicionCadenaEntrada >= this -> inputVector.size() ){
		cout << "[DEBUG]: Falla del escáner, alcance de extremo de la cadena de entrada" << endl;
		return;
	}

	if( this -> inputVector.at( posicionCadenaEntrada ) == produccionEstadoEscanear -> getContenidoProduccion().at( estadoEscanear -> getProgresoProduccion() ) ){
		/*
		* el símbolo a escanear en la producción es el mismo de la cadena de entrada
		* se debe crear un nuevo estado en la siguiente posición del chart
		*
		* debe verificar estados iguales en la tabla y evitar la inserción en ese caso
		*/
		cout << "[DEBUG]: escanear [" << this -> inputVector.at( posicionCadenaEntrada ) << "][" << estadoEscanear -> getProgresoProduccion() << "][" << estadoEscanear -> getProgresoCadenaEntrada() << "]" << endl;

		Estado* estadoInsertar = new Estado( estadoEscanear -> getNoTerminal(), estadoEscanear -> getPunteroProduccion(), estadoEscanear, estadoEscanear -> getProgresoCadenaEntrada(), estadoEscanear -> getProgresoProduccion() + 1 );
		cout << "[DEBUG]: el escáner está insertando el estado en la posición del chart " << posicionCadenaEntrada + 1 << endl;
		this -> chartEarleyParser[ posicionCadenaEntrada + 1 ].push_back( estadoInsertar );
	}else{
		/*
		* hacer nada
		* el símbolo que se va a escanear no coincide con el símbolo en la cadena de entrada
		*/

		cout << "[DEBUG]: escaneo no coincidente - entrada[" << this -> inputVector.at( posicionCadenaEntrada ) << "] produccion[" << produccionEstadoEscanear -> getContenidoProduccion().at( estadoEscanear -> getProgresoProduccion() ) << "]" << endl;
	}

}

void EarleyParser::completar( Estado* estadoCompleto, int posicionCadenaEntrada ){
    cout << "[DEBUG]: completar [" << estadoCompleto -> getNoTerminal() << "][" << estadoCompleto -> getProgresoProduccion() << "][" << estadoCompleto -> getProgresoCadenaEntrada() << "]" << endl;

	if( estadoCompleto -> getProgresoCadenaEntrada() >= this -> chartEarleyParser.size() ){
		cout << "[ERROR]: DESBORDAMIENTO DE COMPLETER" << endl;
		exit(1);
	}
	for( vector<Estado*>::iterator it = this -> chartEarleyParser[ estadoCompleto -> getProgresoCadenaEntrada() ].begin();
			it != this -> chartEarleyParser[ estadoCompleto -> getProgresoCadenaEntrada() ].end(); it++ )
	{
		if( (*it) -> getPunteroProduccion() -> getContenidoProduccion().size() <= (*it) -> getProgresoProduccion() ) continue;
		if( estadoCompleto -> getNoTerminal() == (*it) -> getPunteroProduccion() -> getContenidoProduccion().at( (*it) -> getProgresoProduccion() ) ){
			Estado* estadoInsertar = new Estado( (*it) -> getNoTerminal(), (*it) -> getPunteroProduccion(), estadoCompleto, (*it) -> getProgresoCadenaEntrada(), (*it) -> getProgresoProduccion() + 1 );
			this -> chartEarleyParser[ posicionCadenaEntrada ].push_back( estadoInsertar );
		}
	}
}

vector<Nodo*>* EarleyParser::returnAnalisisArboles( string principalNoTerminal ){
    for( vector<Estado*>::iterator it = this -> chartEarleyParser[ this -> inputVector.size() ].begin();
			it != this -> chartEarleyParser[ this -> inputVector.size() ].end(); it++ )
	{
		if( (*it) -> getNoTerminal() == principalNoTerminal && (*it) -> getProgresoProduccion() == (*it) -> getPunteroProduccion() -> getContenidoProduccion().size() ){
			Estado* estadoMarcador = (*it);
			Nodo* rootArbolAnalisis;

			if( this -> probabilidadParser == true ){
				rootArbolAnalisis =
						new Nodo(
								NULL,
								estadoMarcador -> getNoTerminal(),
								estadoMarcador -> getPunteroProduccion() -> getContenidoProduccion(),
								estadoMarcador -> getPunteroProduccion(),
								estadoMarcador -> getPunteroProduccion() -> getProbabilidadProduccion()
							);
			}else{

				rootArbolAnalisis =
						new Nodo(
								NULL,
								estadoMarcador -> getNoTerminal(),
								estadoMarcador -> getPunteroProduccion() -> getContenidoProduccion(),
								estadoMarcador -> getPunteroProduccion(),
								-1
							);
			}

			vector<Estado*>* test;
			Estado* test2;
			test2 = rootArbolAnalisis -> nodoExpansion( estadoMarcador, & this -> produccionesEarley );
			cout << endl << "terminado de expandir - " << test2 -> getNoTerminal() << endl;

			this -> returnVector.push_back( rootArbolAnalisis );
		}
	}

	return & this->returnVector;

}

void EarleyParser::printProducciones( string principalNoTerminal ){
  cout << "[DEBUG]: Todas las producciones en el chart[" << this -> inputVector.size() << "]" << endl;
	cout << "[DEBUG]: Tamanio del chart " << this -> chartEarleyParser[ this -> inputVector.size() ].size() << endl << endl;

	for( vector<Estado*>::iterator it = this -> chartEarleyParser[ this -> inputVector.size() ].begin();
			it != this -> chartEarleyParser[ this -> inputVector.size() ].end(); it++ )
	{
		cout << "[DEBUG]: tamanio del vector de producción " << (*it) -> getPunteroProduccion() -> getContenidoProduccion().size() << endl;
		cout << "[DEBUG]: - " << (*it) -> getNoTerminal() << " -> ";
		for( unsigned int i = 0; i < (*it) -> getPunteroProduccion() -> getContenidoProduccion().size(); i++ ){
			cout << (*it) -> getPunteroProduccion() -> getContenidoProduccion().at(i) << " ";
		}
		cout << endl;
		cout << "****************************************************************** " << endl << "****************************************************************** " << endl << endl;
	}
}

void EarleyParser::printEstadoSeguimiento( Estado* imprimirEstado ){
	cout << "[DEBUG]: - " << imprimirEstado -> getNoTerminal() << " -> ";
	for( unsigned int i = 0; i < imprimirEstado -> getPunteroProduccion() -> getContenidoProduccion().size(); i++ ){
		cout << imprimirEstado -> getPunteroProduccion() -> getContenidoProduccion().at(i) << " ";
	}

	vector<Estado*>* vectorSeguimientoEstado = imprimirEstado -> getVectorSeguimientoEstado();
	cout << endl << "tamanio del estado del vector de seguimiento: " << vectorSeguimientoEstado -> size() << endl;

	for( unsigned int i = 0; i < vectorSeguimientoEstado -> size(); i++ ){
		if( vectorSeguimientoEstado -> at(i) == NULL ){
			cout << "[DEBUG]: el puntero al siguiente estado es nulo" << endl;
			continue;
		}

		if( vectorSeguimientoEstado -> at(i) -> getImpreso() == false ){
			this -> printEstadoSeguimiento( vectorSeguimientoEstado -> at(i) );
		}
	}

	cout << endl << endl;

	imprimirEstado -> setImpresoTrue();
}

void EarleyParser::dividirVector( const string& cadenaEntrada, vector <string>* vectorSalida, const string& separadorCadena, bool omitirCadenasNulas ){
    unsigned int pos = 0;
    vectorSalida -> clear();

    while( pos < cadenaEntrada.length() ){
        string siguiente = cadenaEntrada.substr( pos );
        unsigned int siguiente_pos = siguiente.find( separadorCadena );

        if( siguiente_pos == static_cast<unsigned int> (std::string::npos) )
            siguiente_pos = siguiente.length();

        if( !omitirCadenasNulas || siguiente_pos > 0 ){
            string paraInsertar = siguiente.substr( 0, siguiente_pos );
            vectorSalida -> push_back( paraInsertar);
        }

        pos += siguiente_pos + separadorCadena.size();
    }
}
