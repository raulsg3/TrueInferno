//---------------------------------------------------------------------------
// EmptyManagerTemplate.h
//---------------------------------------------------------------------------

/**
THIS IS A TEMPLATE OF AN EMPTY SINGLETON MANAGER, CONTAINING ONLY THE
MANAGER INSTANCE AND ALL REQUIRED FUNCTIONS FOR INITIALIZATION AND
DESTRUCTION OF STATIC RESOURCES.

TO CREATE A NEW MANAGER FROM THIS, SIMPLY:
1) COPY-PASTE THIS CODE.
2) CLOSE COMMENTS.
3) CHANGE #ifndef, #define, THE CLASS NAME AND EVERY OTHER THING YOU NEED.
4) REMEMBER ALSO TO REVIEW AND CHANGE COMMENTS IF NEEDED.
5) ADD MANAGER LOGIC IN Logic::CServer
*/

/**
@file TileManager.h

Contiene la declaraci�n del gestor de la matriz de tiles que da forma
al mapa.

@see Logic::CTileManager

@author Ra�l Segura
@date Enero, 2016
/

#ifndef __Logic_TileManager_H
#define __Logic_TileManager_H

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic
{
}

/**
Namespace que engloba la l�gica del juego. Engloba desde el mapa l�gico
contenedor de todas las entidades del juego hasta las propias entidades,
sus componentes, mensajes, factorias de entidades y componentes, etc.
(para m�s informaci�n ver @ref LogicGroup).

@author David Llans�
@date Agosto, 2010
/
namespace Logic
{
	/**
	Manager singleton que gestiona la lectura y actualizaci�n de la matriz
	de tiles del mapa.

	@ingroup logicGroup

	@author Ra�l Segura
	@date Enero, 2016
	/
	class CTileManager
	{
	public:
		/**
		Devuelve la �nica instancia de la clase.

		@return Puntero a la instancia de la clase.
		/
		static Logic::CTileManager *getSingletonPtr() { return _instance; }

		/**
		Inicializa la instancia y los recursos est�ticos.

		@return true si la inicializaci�n se hizo correctamente.
		/
		static bool Init();

		/**
		Libera la propia instancia y los recursos est�ticos.
		Debe llamarse al finalizar la aplicaci�n.
		/
		static void Release();

	protected:

		/**
		Constructor.
		/
		CTileManager();

		/**
		Destructor.
		/
		virtual ~CTileManager();

		/**
		Segunda fase de la construcci�n del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones
		est�ticas.

		@return true si la inicializaci�n se hizo correctamente.
		/
		bool open();

		/**
		Segunda fase de la destrucci�n del objeto. Sirve para hacer liberar
		los recursos de la propia instancia, la liberaci�n de los recursos
		est�ticos se hace en Release().
		/
		void close();

	private:
		/**
		�nica instancia de la clase.
		/
		static CTileManager *_instance;

	}; // class TileManager

} // namespace Logic

#endif // __Logic_TileManager_H

*/