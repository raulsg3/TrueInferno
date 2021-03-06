/**
@file Entity.h

Contiene la declaraci�n de la clase Entity, que representa una entidad
de juego. Es una colecci�n de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#ifndef __Logic_Entity_H
#define __Logic_Entity_H

#include "BaseSubsystems/Math.h"

#include "Logic/Entity/Message.h"

#include "Logic/Maps/EntityID.h"

#include <vector>
#include <string>

#include "Component.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Map
{
	class CEntity;
}

namespace Logic 
{
	class CMap;
	class CEntityFactory;
}

// Declaraci�n de la clase
namespace Logic 
{
	/**
	Clase que representa una entidad en el entorno virtual. Las entidades
	son meros contenedores de componentes, y su funcionamiento depende
	de cu�les tenga. Adem�s, la clase contiene una serie de atributos que
	pueden ser accedidos desde los componentes (id, nombre, etc.).
	<p>
	Tras la creaci�n de la entidad, donde se inicializan los valores de la
	entidad a su valor por defecto, se invocar� a su m�todo spawn() en el
	que se inicializar�n los atributos de la entidad con los valores leidos 
	del fichero del mapa. Tras esto, en alg�n momento, cuando se active
	el mapa se llamar� al m�todo activate() de la entidad. En este momento
	los componentes har�n visibles los componentes gr�ficos y dem�s ya que 
	apartir de ah� es cuando la entidad empieza su funci�n siendo actualizada 
	por el tick() en cada vuelta de bucle.

    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llans�
	@date Agosto, 2010
	*/
	class CEntity : public MessageHandler
	{
	protected:

		/**
		Clase amiga que crea y destruye objetos de la clase.
		*/
		friend class CEntityFactory;

		/**
		Constructor protegido de la clase (para crear CEntity debe
		utilizarse la factor�a CEntityFactory). El constructor
		no hace m�s que poner el identificador �nico de la entidad
		, pues la inicializaci�n efectiva se hace en el m�todo spawn().
		
		@param entityID Identificador �nico de la entidad.
		*/
		CEntity(TEntityID entityID);

		/**
		Destructor de la clase. Es un m�todo protegido pues para
		eliminar CEntity debe utilizarse la factor�a
		CEntityFactory. El destructor invoca al destructor de
		cada componente.
		Cuando el destructor es invocado, �ste _ya ha sido desenganchado
		del mapa al que perteneci�_, por lo que los destructores de los
		componentes no pueden utilizar el mapa.
		*/
		~CEntity();

		/**
		Inicializaci�n del objeto Logic, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Map::CEntity). Avisar� a los componentes
		de la entidad para que se inicialicen.

		@param map Mapa Logic en el que se registrar� la entidad.
		@param entity Informaci�n de construcci�n de la entidad le�da del
		fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		bool spawn(CMap *map, const Map::CEntity *entity);

	public:

		/**
		Activa la entidad. Esto significa que el mapa ha sido activado.
		<p>
		El m�todo llama al activate() de todos los componentes para que
		se den por enterados y hagan lo que necesiten.
		 
		@return Devuelve true si todo fue bien.
		*/
		bool activate();

		/**
		Desactiva la entidad. Esto significa que el mapa ha sido desactivado.
		<p>
		El m�todo llama al deactivate() de todos los componentes para que
		se den por enterados y hagan lo que necesiten.
		*/
		void deactivate();

		/**
		Funci�n llamada en cada frame para que se realicen las funciones
		de actualizaci�n adecuadas.
		<p>
		Llamar� a los m�todos tick() de todos sus componentes.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs);

		/**
		M�todo que a�ade un nuevo componente a la lista de la entidad.

		@param component Componente a a�adir.
		*/
		void addComponent(IComponent* component);

		/**
		M�todo que quita un componente de la lista.

		El componente es eliminado de la lista de componentes de la
		entidad, pero <b>no</b> es desactivado ni eliminado (con delete);
		la responsabilidad de esa tarea se deja al invocante.

		@param component Componente a borrar.
		@return true si se borr� el componente (false si el componente
		no estaba en el objeto).
		*/
		bool removeComponent(IComponent* component);
		
		/**
		M�todo que destruye todos los componentes de una entidad.
		*/
		void destroyAllComponents();

		// Para cada tipo de mensaje se hace la gesti�n apropiada.
		bool HandleMessage(const Message& msg);
		bool HandleMessage(const TransformMessage& msg);
		bool HandleMessage(const PositionMessage& msg);
		bool HandleMessage(const RotationMessage& msg);
		bool HandleMessage(const DimensionsMessage& msg);
		bool HandleMessage(const ColorMessage& msg);
		bool HandleMessage(const MaterialMessage& msg);
		bool HandleMessage(const AnimationMessage& msg);
		bool HandleMessage(const ControlMessage& msg);
		bool HandleMessage(const PhysicMessage& msg);
		bool HandleMessage(const TouchMessage& msg);
		bool HandleMessage(const DamageMessage& msg);
		bool HandleMessage(const WorkerMessage& msg);
		bool HandleMessage(const WalkSoulPathMessage& msg);
		bool HandleMessage(const HellQuartersMessage& msg);
		bool HandleMessage(const MovePlaceableMessage& msg);
		bool HandleMessage(const SoulSenderMessage& msg);
		bool HandleMessage(const SoulMessage& msg);
		bool HandleMessage(const CheckValidPositionPlaceableMessage& msg);
		bool HandleMessage(const SoulBurnMessage& msg);
		bool HandleMessage(const ResourceMessage& msg);
		bool HandleMessage(const GetCostPlaceableMessage& msg);
		bool HandleMessage(const LogisticsMessage& msg);
		bool HandleMessage(const ToggleMessage& msg);
		bool HandleMessage(const PowerMessage& msg);
		bool HandleMessage(const ConsumptionMessage& msg);
		bool HandleMessage(const IconMessage& msg);
		bool HandleMessage(const SoundMessage& msg);
		bool HandleMessage(const ParticleMessage& msg);

		/**
		Devuelve el identificador �nico de la entidad.

		@return Identificador.
		*/
		Logic::TEntityID getEntityID() const { return _entityID; }

		/**
		Devuelve el mapa donde est� la entidad.

		@return Puntero al mapa que contiene la entidad.
		*/
		CMap *getMap() { return _map; }

		/**
		Devuelve el mapa donde est� la entidad.

		@return Puntero al mapa que contiene la entidad.
		*/
		const CMap *getMap() const { return _map; }

		/**
		Devuelve el nombre de la entidad.

		@return Nombre de la entidad.
		*/
		const std::string &getName() const { return _name; }

		/**
		Devuelve el tipo de la entidad. Este atributo es leido de
		la entidad del mapa en spawn().

		@return Tipo de la entidad.
		*/
		const std::string &getType() const { return _type; }

		/**
		Establece la matriz de transformaci�n de la entidad (posici�n,
		rotaci�n y escala). Avisa a los componentes del cambio.

		@param transform Nueva matriz de transformaci�n de la entidad.
		*/
		bool setTransform(const Matrix4& transform);

		/**
		Establece la posici�n de la entidad. Avisa a los componentes
		del cambio.

		@param position Nueva posici�n de la entidad.
		*/
		bool setPosition(const Vector3 &position);

		/**
		Establece la rotaci�n de la entidad. Avisa a los componentes
		del cambio.

		@param rotation Nueva rotaci�n de la entidad.
		*/
		bool setRotation(const Vector3 &rotation);

		/**
		Establece las dimensiones de la entidad. Avisa a los componentes
		del cambio.

		@param dimensions Nuevas dimensiones de la entidad.
		*/
		bool setDimensions(const Vector3 &dimensions);

		/**
		Devuelve la matriz de transformaci�n de la entidad.

		@return Matriz de transformaci�n de la entidad en el entorno.
		*/
		Matrix4 getTransform() const;

		/**
		Devuelve la posici�n de la entidad.
		<p>
		La posici�n es inicialmente le�da del mapa (si no aparece,
		se colocar� a (0, 0, 0)), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Posici�n de la entidad en el entorno.
		*/
		Vector3 getPosition() const;

		/**
		Devuelve la rotaci�n de la entidad.

		@return Rotaci�n de la entidad en el entorno.
		*/
		Vector3 getRotation() const;

		/**
		Devuelve las dimensiones de la entidad.

		@return Dimensiones de la entidad.
		*/
		Vector3 getDimensions() const;

		/**
		Devuelve las dimensiones del modelo de la entidad.

		@return Dimensiones del modelo de la entidad.
		*/
		Vector3 getMeshDimensions() const;

		/**
		Indica si la entidad se encuentra activa.

		@return true si la entidad est� activa.
		*/
		bool isActivated() {return _activated;}

		/** Magic method to get a Component from an Entity */
		template <class ComponentClass>
		ComponentClass* getComponent(){
			for (auto it = _components.cbegin(); it != _components.cend(); ++it){
				if ((*it)->GetRTTI().IsExactly(ComponentClass::rtti))
					return (ComponentClass*)(*it);
			}

			return nullptr;
		}

		friend void swap(CEntity& first, CEntity& second) // nothrow
		{
			// enable ADL (not necessary in our case, but good practice)
			using std::swap;

			// by swapping the members of two classes,
			// the two classes are effectively swapped
			swap(first._name, second._name);
			swap(first._activated, second._activated);
			swap(first._components, second._components);
			swap(first._dimensions, second._dimensions);
			swap(first._entityID, second._entityID);
			swap(first._isPlayer, second._isPlayer);
			swap(first._map, second._map);
			swap(first._meshDimensions, second._meshDimensions);
			swap(first._position, second._position);
			swap(first._rotation, second._rotation);
			swap(first._type, second._type);
		}

		CEntity& operator=(CEntity other) // (1)
		{
			swap(*this, other); // (2)

			return *this;
		}

		bool operator==(const CEntity& rhs) const
		{
			return _name == rhs._name;
		}

	protected:

		/**
		Actualiza el valor de todos los par�metros que forman parte del
		transform de la entidad (_position, _rotation y _dimensions).
		Avisa a los componentes del cambio.

		@param transform Nueva matriz de transformaci�n.
		*/
		void updateTransformValuesFromMatrix(const Matrix4 &transform);

		/**
		Construye la matriz de transformaci�n de la entidad y env�a un
		mensaje para avisar a los componentes del cambio.
		*/
		bool sendTransformMessage();

		/**
		Clase amiga que puede modificar los atributos (en concreto se 
		usa para modificar el mapa.
		*/
		friend class CMap;

		/**
		Identificador l�ico de la entidad.
		*/
		Logic::TEntityID _entityID;

		/**
		Tipo para la lista de componentes.
		*/
		typedef std::vector<IComponent*> TComponentList;

		/**
		Lista de los componentes de la entidad.
		*/
		TComponentList _components;

		/**
		Indica si la entidad est� activa.
		*/
		bool _activated;

		/**
		Tipo de la entidad declarado en el archivo blueprints.
		*/
		std::string _type;

		/**
		Nombre de la entidad.
		*/
		std::string _name;

		/**
		Mapa l�gico donde est� la entidad.
		*/
		Logic::CMap *_map;

		/**
		Posici�n de la entidad.
		*/
		Vector3 _position;

		/**
		Rotaci�n de la entidad con respecto a los ejes X (pitch), Y (yaw)
		y Z (roll). Radianes!
		*/
		Vector3 _rotation;

		/**
		Dimensiones de la entidad.
		*/
		Vector3 _dimensions;

		/**
		Dimensiones del modelo de la entidad.
		*/
		Vector3 _meshDimensions = Vector3(100.f, 100.f, 100.f);

		/**
		Atributo que indica si la entidad es el jugador; por defecto
		es false a no ser que se lea otra cosa de los atributos.
		*/
		bool _isPlayer;

	}; // class CEntity

} // namespace Logic

#endif // __Logic_Entity_H
