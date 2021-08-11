#include "EarleyParser.h"
#include "Estado.h"

using namespace std;

int main( int argc, char** argv ){
	if( argc < 6 ){

		cout << endl << "Uso: ./main 'Archivo de gramática de libre de contexto' 'Archivo de diccionario de entrada' 'Archivo de cadena de entrada' 'Producción principal' 'Análisis de probabilidad? (y / n)'" << endl << endl;
		exit(1);
	}

	EarleyParser myEarleyParser;
	bool probabilidadParser;

	int comprobarFuncionamiento;

	if( string( argv[5] ).compare("y") == 0 )
		probabilidadParser = true;
	else
		probabilidadParser = false;

	myEarleyParser.inicializa( string( argv[1] ), probabilidadParser );
	cout << "*****************************************************" << endl << "*****************************************************" << endl;

	myEarleyParser.crearDiccionario( string( argv[2] ) );
	cout << "*****************************************************" << endl << "*****************************************************" << endl;

	myEarleyParser.crearVectorCadenaEntrada( string( argv[3] ) );
	cout << "*****************************************************" << endl << "*****************************************************" << endl;

	string principalNoTerminal = string( argv[4] );
	comprobarFuncionamiento = myEarleyParser.crearChart( principalNoTerminal );
	if( comprobarFuncionamiento < 0 ) exit(1);
	cout << "*****************************************************" << endl << "*****************************************************" << endl;

	myEarleyParser.parse();
	cout << "*****************************************************" << endl << "*****************************************************" << endl;

	myEarleyParser.printProducciones( principalNoTerminal );
	cout << "*****************************************************" << endl << "*****************************************************" << endl;

	vector<Nodo*>* vectorAnalisisRootArboles;

	vectorAnalisisRootArboles = myEarleyParser.returnAnalisisArboles( principalNoTerminal );
	cout << "*****************************************************" << endl << "*****************************************************" << endl;
	for( vector<Nodo*>::iterator it = vectorAnalisisRootArboles->begin(); it != vectorAnalisisRootArboles->end(); it++ ){
		float prob;
		prob = (*it) -> print( probabilidadParser );
		cout << "La probabilidad del arbol impreso es: [" << prob << "]" << endl;
		cout << "*****************************************************" << endl << "*****************************************************" << endl << endl;
	}
	return 0;
}