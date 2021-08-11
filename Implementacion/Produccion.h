#ifndef PRODUCCION_H_
#define PRODUCCION_H_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

using namespace std;

class Produccion{

	public:
		Produccion( float probabilidadProduccion, int tamanioProduccion, vector < string > contenidoProduccion );

		virtual ~Produccion();

		float getProbabilidadProduccion();
		int getTamanioProduccion();
		vector < string > getContenidoProduccion();

	private:

		explicit Produccion() {}

		float probabilidadProduccion;
		int tamanioProduccion;
		vector < string > contenidoProduccion;
};

#endif /* PRODUCCION_H_ */
