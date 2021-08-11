#include "Nodo.h"

using namespace std;

Nodo::Nodo( Nodo* padre, string elementoGramatica, vector <string> produccionNodo, Produccion* punteroProduccion, float probabilidadProduccion ){
	this -> padre = padre;
	this -> elementoGramatica = elementoGramatica;
	this -> produccion = produccionNodo;
	this -> probabilidadProduccion = probabilidadProduccion;
	this -> punteroProduccion = punteroProduccion;
}

Nodo::~Nodo(){
	for( vector <Nodo*>::iterator i = this -> hijos.begin(); i != this -> hijos.end(); i++ ){
		delete (*i);
	}
}

Produccion* Nodo::getPunteroProduccion(){
	return this -> punteroProduccion;
}

Estado* Nodo::nodoExpansion( Estado* actualEstadoExpandir, unordered_map < string, list<Produccion> >* produccionesEarley ){
	Estado* siguienteEstadoExpandir;

	cout << endl << "Actualmente en expansion de - " << actualEstadoExpandir -> getNoTerminal();
	for( unsigned int i = 0; i < actualEstadoExpandir->getPunteroProduccion() -> getContenidoProduccion().size(); i++ ){
		cout << " " << actualEstadoExpandir -> getPunteroProduccion() -> getContenidoProduccion().at(i) << " ";
	}

	for( unsigned int i = this -> produccion.size(); i > 0; i-- ){
		unordered_map < string, list<Produccion> >::iterator got = produccionesEarley -> find( this->produccion.at( i - 1 ) );
		if( got != produccionesEarley -> end() ){
	        /*
			*el elemento contenido en la producción en la posición actual
			*del bucle es NO TERMINAL y debe ampliarse
			*/
			if( actualEstadoExpandir -> getVectorSeguimientoEstado() -> size() > 1 ){
				cout << endl << "VECTOR DE SEGUIMIENTO MÁS GRANDE QUE 1" << endl;

				for( unsigned j = 0; j < actualEstadoExpandir -> getVectorSeguimientoEstado() -> size(); j++ ){
					Estado* estado = actualEstadoExpandir -> getVectorSeguimientoEstado() -> at(j);
					cout << endl << "[DEBUG]: En 0 -" << estado -> getNoTerminal() << " size: " << estado -> getPunteroProduccion() -> getContenidoProduccion().size() << endl;
					for( unsigned int k = 0; k < estado -> getPunteroProduccion() -> getContenidoProduccion().size(); k++ ){
						cout << estado -> getPunteroProduccion() -> getContenidoProduccion().at(k) << " ";
					}
					cout << endl;
				}

				for( unsigned j = 0; j < actualEstadoExpandir -> getVectorSeguimientoEstado() -> size(); j++ ){
					if( this -> padre -> getPunteroProduccion() == actualEstadoExpandir -> getVectorSeguimientoEstado() -> at(j) -> getPunteroProduccion() ){
						actualEstadoExpandir = actualEstadoExpandir -> getVectorSeguimientoEstado()->at(j);
						cout << "[DEBUG]: puntero de producción coincidente en la posición " << j << endl;
						break;
					}

					if( j == actualEstadoExpandir -> getVectorSeguimientoEstado() -> size() - 1 )
					{
						cout << "[ERROR]: ¡No hay puntero de producción correspondiente a la producción en este nodo!" << endl;
						exit(1);
					}
				}

			} else{
				siguienteEstadoExpandir = actualEstadoExpandir -> getVectorSeguimientoEstado() -> at(0);
			}

			Nodo* hijo;
			if( this -> probabilidadProduccion >= 0 ){
				hijo =	new Nodo(
								this,
								siguienteEstadoExpandir -> getNoTerminal(),
								siguienteEstadoExpandir -> getPunteroProduccion() -> getContenidoProduccion(),
								siguienteEstadoExpandir -> getPunteroProduccion(),
								siguienteEstadoExpandir -> getPunteroProduccion() -> getProbabilidadProduccion()
							);
			} else{

				hijo = new Nodo(
								this,
								siguienteEstadoExpandir -> getNoTerminal(),
								siguienteEstadoExpandir -> getPunteroProduccion() -> getContenidoProduccion(),
								siguienteEstadoExpandir -> getPunteroProduccion(),
								-1
							);
			}

			this -> hijos.insert( this -> hijos.begin(), hijo );

			actualEstadoExpandir = hijo -> nodoExpansion( siguienteEstadoExpandir, produccionesEarley );

			cout << endl << "Expansion terminada - " << siguienteEstadoExpandir -> getNoTerminal() << endl;
      

		}else{

			Nodo* hijo;
			if( this -> probabilidadProduccion >= 0 )
			{
				/*
				* los nodos hoja solo tienen probabilidades
				* si las probabilidades de los símbolos terminales son
				* especificados en la entrada
				*/


				/*
				* NULL se inserta porque los símbolos terminales
				* no tienen una producción
				*/
				hijo =	new Nodo(
								this,
								this -> produccion.at( i - 1 ),
								vector <string>(),
								NULL,
								1
							);
			}else{

				hijo = new Nodo(
								this,
								this -> produccion.at( i - 1 ),
								vector <string>(),
								NULL,
								-1
							);
			}

			/*
			* insertar al principio del vector
			*/
			this -> hijos.insert( this -> hijos.begin(), hijo);

			if( actualEstadoExpandir -> getVectorSeguimientoEstado() -> size() > 1 ){

				cout << endl << "VECTOR DE SEGUIMIENTO MÁS GRANDE QUE 1" << endl;

				for( unsigned j = 0; j < actualEstadoExpandir -> getVectorSeguimientoEstado() -> size(); j++ ){
					Estado* estado = actualEstadoExpandir -> getVectorSeguimientoEstado()->at(j);
					cout << endl << "[DEBUG]: en 0 -" << estado->getNoTerminal() << " Tamanio: " << estado -> getPunteroProduccion() -> getContenidoProduccion().size() << endl;
					for( unsigned int k = 0; k < estado -> getPunteroProduccion() -> getContenidoProduccion().size(); k++ ){
						cout << estado -> getPunteroProduccion() -> getContenidoProduccion().at(k) << " ";
					}
					cout << endl;
				}

				for( unsigned j = 0; j < actualEstadoExpandir -> getVectorSeguimientoEstado() -> size(); j++ ){
					if( this -> padre -> getPunteroProduccion() == actualEstadoExpandir -> getVectorSeguimientoEstado() -> at(j) -> getPunteroProduccion() ){
						actualEstadoExpandir = actualEstadoExpandir -> getVectorSeguimientoEstado() -> at(j);
						cout << "[DEBUG]: puntero de producción coincidente en la posición " << j << endl;
						break;
					}

					if( j == actualEstadoExpandir -> getVectorSeguimientoEstado() -> size() - 1 ){
						cout << "[ERROR]: ningún puntero de producción coincidente correspondiente a la producción en este nodo! " << endl;
						exit(1);
					}
				}

			}else{
				actualEstadoExpandir = actualEstadoExpandir -> getVectorSeguimientoEstado() -> at(0);
			}

			if( actualEstadoExpandir -> getVectorSeguimientoEstado() -> size() > 1 ){

				cout << endl << "VECTOR DE SEGUIMIENTO MÁS GRANDE QUE 1" << endl;

				for( unsigned j = 0; j < actualEstadoExpandir -> getVectorSeguimientoEstado() -> size(); j++ ){
					Estado* estado = actualEstadoExpandir -> getVectorSeguimientoEstado() -> at(j);
					cout << endl << "[DEBUG]: en 0 -" << estado -> getNoTerminal() << " tamanio: " << estado -> getPunteroProduccion() -> getContenidoProduccion().size() << endl;
					for( unsigned int k = 0; k < estado -> getPunteroProduccion() -> getContenidoProduccion().size(); k++ ){
						cout << estado -> getPunteroProduccion() -> getContenidoProduccion().at(k) << " ";
					}
					cout << endl;
				}

				for( unsigned j = 0; j < actualEstadoExpandir -> getVectorSeguimientoEstado() -> size(); j++ ){
					if( this -> padre -> getPunteroProduccion() == actualEstadoExpandir -> getVectorSeguimientoEstado() -> at(j) -> getPunteroProduccion() ){
						actualEstadoExpandir = actualEstadoExpandir -> getVectorSeguimientoEstado() -> at(j);
						cout << "[DEBUG]: puntero de producción coincidente en la posición " << j << endl;
						break;
					}else{
						cout << "[DEBUG]: sin puntero de producción coincidente en la posición " << j << endl;
					}

					if( j == actualEstadoExpandir -> getVectorSeguimientoEstado() -> size() - 1){
						cout << "[ERROR]: ¡No hay puntero de producción coincidente correspondiente a la producción en este nodo!" << endl;
						exit(1);
					}
				}

			}else{
				actualEstadoExpandir = actualEstadoExpandir -> getVectorSeguimientoEstado() -> at(0);
			}
		}
	}
	return actualEstadoExpandir;
}

float Nodo::print( bool printProbabilidades ){
	float prob = -1;

	cout << "Nodo [" << this -> elementoGramatica << "]{";

	if( printProbabilidades == true ){

		if( this -> hijos.size() == 0 ){
			/*
			 * Estamos en un nodo de hoja
			 */
			prob = this -> probabilidadProduccion;

		}else{

			prob = this -> probabilidadProduccion;
			for( vector <Nodo*>::iterator it = this -> hijos.begin(); it != this -> hijos.end(); it++ ){
				prob = prob * (*it) -> print( printProbabilidades );
			}
		}
	}else{

		for( vector<Nodo*>::iterator it = this -> hijos.begin(); it != this -> hijos.end(); it++ ){
			prob = (*it) -> print( printProbabilidades);
		}
	}
	cout << "prob [" << prob << "]}";
cout << endl;
	return prob;
}
