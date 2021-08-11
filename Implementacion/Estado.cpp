#include "Estado.h"

using namespace std;

Estado::Estado( string noTerminal, Produccion* punteroProduccion, Estado* seguimientoEstado, int inputProgresoCadena, int progresoProduccion )
:progresoProduccion(0), impreso(false){

	this -> noTerminal = noTerminal;
	this -> punteroProduccion = punteroProduccion;
	this -> vectorSeguimientoEstado.push_back( seguimientoEstado );
	this -> inputProgresoCadena = inputProgresoCadena;
	this -> progresoProduccion = progresoProduccion;
}

Estado::~Estado(){}

vector <Estado*>* Estado::getVectorSeguimientoEstado(){
	return &( this -> vectorSeguimientoEstado );
}

int Estado::getProgresoCadenaEntrada(){
	return this -> inputProgresoCadena;
}

const string Estado::getNoTerminal()
{
	return this -> noTerminal;
}

Produccion* Estado::getPunteroProduccion() const{
	if( this -> punteroProduccion == NULL ){
		cout << "[ERROR]: El puntero de la produccion esta vacio" << endl;
		exit(1);
	}

	return this -> punteroProduccion;
}

int Estado::getProgresoProduccion() const{
	return this -> progresoProduccion;
}

bool Estado::Completo(){
	if( this -> getProgresoProduccion() == this -> getPunteroProduccion() -> getContenidoProduccion().size() )
		return true;
	else
		return false;
}

string Estado::SiguienteSimboloProcesar(){
	return this -> getPunteroProduccion() -> getContenidoProduccion().at( this -> progresoProduccion );
}

bool Estado::getImpreso(){
	return this -> impreso;
}

void Estado::setImpresoTrue(){
	this -> impreso = true;
}
