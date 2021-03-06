/**
@file AStarFunctionsSoulPath.h

En este fichero se implementan las funciones
necesarias para calcular rutas de almas usando A*.


@author �lvaro Valera
@date February, 2016
*/

#pragma once

#ifndef AI_STAR_FUNCTIONS_SOUL_PATH_H_
#define AI_STAR_FUNCTIONS_SOUL_PATH_H_

#include "micropather.h"

namespace AI 
{
	/**
	Clase que hereda de micropather::Graph y que contiene las funciones
	de A* espec�ficas de Galeon.
	*/
	class CAStarFunctionsSoulPath : public micropather::Graph
	{
	public:
		/** 
		Constructor
		*/
		CAStarFunctionsSoulPath(void);
		/** 
		Destructor
		*/
		~CAStarFunctionsSoulPath(void);
		/**
		Devuelve el coste seg�n la heur�stica para llegar desde el estado stateStart hasta stateEnd.
		Para que el camino devuelto por A* sea �ptimo la heur�stica sea aceptable y no sobreestimar 
		la distancia.
		Para la b�squeda de rutas de SoulPaths en el mapa de Tiles utilizaremos como heur�stica la distancia de Manhattan.
		*/
		virtual float LeastCostEstimate( void* stateStart, void* stateEnd );

		/** 
		Devuelve la lista de vecinos de un nodo junto con el coste de llegar desde el nodo actual
		hasta cada uno de ellos.
		*/	
		virtual void AdjacentCost( void* state, std::vector< micropather::StateCost > *adjacent );

		/**
			This function is only used in DEBUG mode - it dumps output to stdout. Since void* 
			aren't really human readable, normally you print out some concise info (like "(1,2)") 
			without an ending newline.
		*/
		virtual void PrintStateInfo( void* state );

	};

} // namespace AI 

#endif //AI_STAR_FUNCTIONS_SOUL_PATH_H_