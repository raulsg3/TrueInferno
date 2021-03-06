//---------------------------------------------------------------------------
// Entity.h
//---------------------------------------------------------------------------

/**
@file Entity.h

Contiene la declaraci�n de la clase que representa una entidad gr�fica.

@see Graphics::CEntity

@author David Llans�
@date Julio, 2010
*/

#ifndef __Graphics_Entity_H
#define __Graphics_Entity_H

#include "BaseSubsystems/Math.h"
#include <string>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre 
{
	class Entity;
	class SceneNode;
}

namespace Graphics 
{
	class CScene;
}

namespace Logic
{
	enum ParticleType;
}

// Factor para oscurecer los colores. Cuanto m�s cerca de 0 m�s oscuro
#define CHANGE_COLOR_FACTOR 0.4

namespace Graphics 
{
	/**
	Clase que representa una entidad gr�fica. Contiene una referencia a
	una entidad de Ogre y al Nodo que la contiene.
	Oculta los detalles escabrosos del motor gr�fico.
	<p>
	Al construir la clase solo se almacenan los atributos necesarios para
	la carga de las entidades de Ogre. Para cargar la entidad en Ogre y que 
	sea reenderizada, �sta debe a�adirse a una escenada de Ogre mediante el 
	m�todo attachToScene(). Solo la clase CScene puede invocar los m�todos 
	para a�adir o eliminar entidades de una escena y por tanto forzar su 
	carga o descarga en el motor de Ogre. Esto deriva de que en Ogre la carga
	de entidades se hace por las escenas, por lo que en nuestra arquitectura
	no permitimos entidades cargadas que no pertenezcan a ninguna escena.
	Al a�adir una entidad a una escena se fuerza su carga en Ogre invocando a 
	load.
	<p>
	Todas las modificaciones que se deseen hacer sobre los atributos de la
	entidad gr�fica (posici�n, orientaci�n, escala...) se deber�n hacer una 
	vez que las estructuras de Ogre han sido cargadas. De la misma manera 
	la entidad debe estar cargada para poder acceder a ellos.
	<p>
	Pueden implementarse clases derivadas de �stas, que permitan
	modelos con distintas caracter�sticas. Por ejemplo, �sta clase
	no permite especificar la animaci�n utilizada. Si queremos que podamos
	guardar un modelo gr�fico, y poder cambiar la animaci�n actual,
	se utilizar� la clase derivada correspondiente.
	
	@ingroup graphicsGroup

	@author David Llans�
	@date Julio, 2010
	*/
	class CEntity 
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		@param meshDimensions Dimensiones del modelo que debe cargarse.
		*/
		CEntity(const std::string &name, const std::string &mesh, const Vector3 &meshDimensions);

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CEntity();

		/**
		Cambia la matriz de transformaci�n de la entidad (posici�n,
		orientaci�n y escala).

		@param transform Referencia a la matriz de transformaci�n con la 
		que debe posicionarse, orientarse y escalarse la entidad.
		*/
		void setTransform(const Matrix4 &transform);

		/**
		Cambia la posici�n de la entidad.

		@param position Nueva posici�n para el nodo que contiene la entidad 
		de Ogre.
		*/
		void setPosition(const Vector3& position);

		/**
		Cambia la rotaci�n de la entidad.

		@param rotation Nueva rotaci�n para el nodo que contiene la entidad
		de Ogre.
		*/
		void setRotation(const Vector3& rotation);

		/**
		Cambia las dimensiones de la entidad.

		@param dimensions Nuevas dimensiones para el nodo que contiene la
		entidad de Ogre.
		*/
		void setDimensions(const Vector3& dimensions);

		/**
		Escala la entidad.

		@param scale Valores de la escala en los diferentes ejes.
		*/
		void setScale(const Vector3 &scale);

		/**
		Escala la entidad.

		@param scale Valor de la escala para los 3 ejes.
		*/
		void setScale(float scale);

		/**
		 Establece la propiedad visible de la entidad. Si
		 la entidad est� puesta invisible, no deber�a dibujarse.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no est� relacionada con si la entidad est�
		 dentro del campo de visi�n de la c�mara o no.
		 <p>
		 El m�todo cambia la propiedad visible a todas las 
		 subentidades o nodos que contenga _entityNode.

		 @param visible Nuevo valor de la propiedad visible.
		 */
		void setVisible(bool visible);

		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no est� relacionada con si la entidad est�
		 dentro del campo de visi�n de la c�mara o no.

		 @return Cierto si la entidad es visible (est� activa 
		 para ser reenderizada).
		*/
		const bool getVisible();

		/**
		Devuelve el valor de la escala de la entidad.
		@return Valores de la escala en los diferentes ejes.
		*/
		const Vector3 &getScale();

		/**
		Cambia el color de la entidad.

		@param color Valores (r,g,b) del color interpolados entre 0 y 1.
		@param technique Nombre de la t�cnica en la que cambiar el color.
		@param pass Nombre de la pasada en la que cambiar el color.
		*/
		void setDiffuseColor(const Vector3 &color);
		void setDiffuseColor(const Vector3 &color, std::string technique, std::string pass);

		void setEmissiveColor(const Vector3 &color);
		void setEmissiveColor(const Vector3 &color, std::string technique, std::string pass);

		/**
		Devuelve el color de la entidad.

		@param technique Nombre de la t�cnica de la que obtener el color.
		@param pass Nombre de la pasada de la que obtener el color.
		@return Valores (r,g,b) del color interpolados entre 0 y 1.
		*/
		Vector3 getDiffuseColor();
		Vector3 getDiffuseColor(std::string technique, std::string pass);

		Vector3 getEmissiveColor();
		Vector3 getEmissiveColor(std::string technique, std::string pass);

		/**
		* Cambia el color para oscurecerlo aplicando el factor dado (0,1). Cuanto m�s cerca de 0, m�s oscuro.
		*/
		void makeDarkerColor(float factor = CHANGE_COLOR_FACTOR);

		/**
		* Cambia el color para aclararlo aplicando el factor dado (0,1). Cuanto m�s cerca de 0, m�s claro.
		*/
		void makeClearerColor(float factor = CHANGE_COLOR_FACTOR);

		/**
		Cambia el material de la entidad.
		@param material Nombre del material.
		*/
		void setMaterialName(const std::string &materialName);

		/**
		A�ade un par�metro de entrada al material de la entidad.

		@param index �ndice del par�metro.
		@param param Par�metro de entrada.
		*/
		void setCustomParameter(int index, Vector4 param);

		/**
		Devuelve el material de la entidad.
		@return Nombre del material.
		*/
		const std::string &getMaterialName();

		/**
		Establece la propiedad castShadows de la entidad.
		@param castShadows Nuevo valor de la propiedad castShadows.
		*/
		void setCastShadows(bool castShadows);

		/**
		Devuelve la entidad de Ogre.
		@return puntero a la entidad de Ogre.
		*/
		Ogre::Entity* getEntity() {return _entity;}

		/** Crea o rearranca el sistema de part�culas del tipo dado para esta entidad */
		void startParticles(Logic::ParticleType particleType);

		/** Detiene el sistema de part�culas del tipo dado para esta entidad */
		void stopParticles(Logic::ParticleType particleType);

	protected:

		// CScene es la �nica que puede a�adir o eliminar entidades de una 
		// escena y por tanto cargar o descargar entidades.
		// Por otro lado cada entidad debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;

		/**
		A�ade la entidad al SceneManager pasado por par�metro. Si la entidad
		no est� cargada se fuerza su carga.

		@param sceneMgr Gestor de la escena de Ogre a la que se quiere a�adir
		la entidad.
		@return true si la entidad se pudo cargar y a�adir a la escena.
		*/
		bool attachToScene(CScene *scene);

		/**
		Descarga una entidad de la escena en la que se encuentra cargada.

		@return true si la entidad se descargo y elimin� de la escena
		correctamente. Si la entidad no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();
		
		/**
		Carga la entidad gr�fica correspondiente al nombre _mesh. No hace 
		comprobaci�nes de si la entidad est� ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la entidad pudo crear los objetos necesarios en Ogre
		o si la entidad ya estaba cargada.
		*/
		bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaci�nes de si la entidad est� o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		void unload();
		
		/**
		Actualiza el estado de la entidad cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs N�mero de segundos transcurridos desde la �ltima llamada.
		*/
		virtual void tick(float secs);
		
		/**
		Nodo que contiene la entidad de Ogre.
		*/
		Ogre::SceneNode *_entityNode;

		/**
		Entidad de Ogre.
		*/
		Ogre::Entity *_entity;

		/**
		Quiz�s es mejor meter la clase CScene

		Controla todos los elementos Ogre de una escena. Su equivalente
		en la l�gica del juego ser�a el mapa o nivel. 
		*/
		CScene *_scene;

		/**
		Nombre de la entidad.
		*/
		std::string _name;

		/**
		Nombre del modelo de la entidad.
		*/
		std::string _mesh;

		/**
		Dimensiones del modelo de la entidad.
		*/
		Vector3 _meshDimensions;

		/**
		Indica si la entidad ha sido cargada en el motor gr�fico.
		*/
		bool _loaded;
	
	private:
		/** Obtiene el nombre de plantilla para el tipo de sistema part�culas dado */
		static std::string getParticleSystemTemplateName(Logic::ParticleType particleType);

		/** Obtiene el identificador �nico del tipo de sistema part�culas dado para esta entidad */
		std::string CEntity::getParticleSystemId(Logic::ParticleType particleType);

	}; // class CEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
