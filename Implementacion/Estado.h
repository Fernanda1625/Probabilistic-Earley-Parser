#ifndef ESTADO_H_
#define ESTADO_H_

#include "Produccion.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

using namespace std;

class Estado{
	public:

		Estado( string noTerminal, Produccion* punteroProduccion, Estado* seguimientoEstado, int inputProgresoCadena, int progresoProduccion );

		virtual ~Estado();

		vector <Estado*>* getVectorSeguimientoEstado();
		const string getNoTerminal();

		Produccion* getPunteroProduccion() const;
		int getProgresoProduccion() const;
		int getProgresoCadenaEntrada();

		bool Completo();
		string SiguienteSimboloProcesar();

		bool getImpreso();
		void setImpresoTrue();

	private:

		explicit Estado() : punteroProduccion( NULL ) {}

		string noTerminal;
		Produccion* punteroProduccion;

		vector <Estado*> vectorSeguimientoEstado;

		int progresoProduccion;
		int inputProgresoCadena;

		bool impreso;

};

#endif /* ESTADO_H_ */
