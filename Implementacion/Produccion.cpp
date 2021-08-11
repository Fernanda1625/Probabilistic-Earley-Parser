#include "Produccion.h"

using namespace std;

Produccion::Produccion( float probabilidadProduccion, int tamanioProduccion, vector < string > contenidoProduccion ){
	this -> probabilidadProduccion = probabilidadProduccion;
	this -> tamanioProduccion = tamanioProduccion;
	this -> contenidoProduccion = contenidoProduccion;
}

Produccion::~Produccion(){}

vector < string > Produccion::getContenidoProduccion(){
	return this -> contenidoProduccion;
}

int Produccion::getTamanioProduccion(){
	return this -> tamanioProduccion;
}

float Produccion::getProbabilidadProduccion(){
	return this -> probabilidadProduccion;
}
