#ifndef PLACEABLE_H_
#define PLACEABLE_H_

#include <vector>

#include "BaseSubsystems/Math.h"
#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"
#include "Logic/Entity/PlaceableType.h"
#include "Logic/Entity/BuildingType.h"
#include "Logic/Entity/TerrainType.h"

#include <unordered_set>


// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic
{
	class Tile;
	class CTileManager;
}

namespace Logic {
	class CPlaceable : public IComponent{
		RTTI_DECL;
		DEC_FACTORY(CPlaceable);

	// Anotamos la clase como friend de BuildingManager para permitirle acceso al tipo de edificio y de placeable
	friend class CBuildingManager;

	public:
		/**
		Constructor por defecto.
		*/
		CPlaceable();

		/**
		Destructor
		*/
		virtual ~CPlaceable();

		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualizaci�n por frame
		*/
		virtual void tick(unsigned int msecs);

		/** Float this placeable to a given logic position*/
		void floatTo(const Vector3 newOriginPosition, bool showFloating);

		/** Coloca el Placeable en la posici�n original */
		bool place();

		/** Gets occupied tiles depending on the placeable's floor */
		const std::vector<Tile*> getTiles() const;

		/** Gets all adyacent tiles to the placeable's own tiles */
		const std::unordered_set<Tile*> getAdyacentTiles() const;

		/** Checks if it's possible to place all the placeable's floor tiles starting at given origin position. All tiles must be entity free. 
		Returns true if placement is possible, false otherways */
		bool checkPlacementIsPossible(const Vector3 &originPosition) const;

		/** Devuelve un boolean indicando si es posible hacer pasar una SoulPath por el Placeable */
		bool canPassSoulPath() const;

		/** Devuelve un boolean indicando si es posible hacer caminar un alma por el Placeable */
		bool canPassWalkingSoul() const;

		/** Devuelve un boolean indicando si es posible que el Placeable se coloque sobre terreno de recurso */
		bool canBePlacedOnTerrain(TerrainType terrainType) const;

		/** Devuelve un boolean a true si el Placeable corresponde a un edificio */
		bool isBuilding() const;

		/** Devuelve un boolean a true si el Placeable corresponde a una soulpath */
		bool isSoulPath() const;

		virtual bool HandleMessage(const MovePlaceableMessage& msg);

		virtual bool HandleMessage(const CheckValidPositionPlaceableMessage& msg);

		virtual bool HandleMessage(const GetCostPlaceableMessage& msg);

		// Devuelve el PlaceableType de este placeable
		PlaceableType getPlaceableType() const {
			return _placeableType;
		}

		// Devuelve el BuildingType de este placeable
		BuildingType getBuildingType() const {
			return _buildingType;
		}

		// Destruye el placeable con efectos
		void destroyWithEffects();

		/** Elimina las entidades creadas para feedback visual durante la construcci�n */
		void clearFloatingVisualEntities();
		
	private:
		/** Altura a�adida a la posici�n del Placeable para que parezca que est� justo encima */
		const float HEIGHT_ON_TILE = 1.0f;

		/** Altura a�adida a la posici�n del Placeable para que parezca que flota por encima */
		const float HEIGHT_FLOATING_OVER_TILE = 1.5f;

		/** Velocidad a la que se coloca gr�ficamente el edificio */
		const float PLACE_SPEED = 0.0005;
		
		/** Velocidad a la que se desplaza gr�ficamente el edificio */
		const float FLOAT_SPEED = 0.005;

		/** Floor's absolute logic origin position in the matrix */
		Vector3 _floorOriginPosition;

		//costes de fabricaci�n del placeable
		int _gasCost = 0;
		int _mineralCost = 0;

		/** Recompensa en favor de Hades por la construcci�n del edificio */
		int _hadesFavorReward;

		/** Duraci�n (ms) del humo de construcci�n */
		int _smokeConstructionDuration;

		/** Floor's x size */
		int _floorX;

		/** Floor's z size */
		int _floorZ;

		/** Floor's occupied tiles */
		std::vector<Tile*> _tiles;

		/** Tiles adyacent to floor's occupied tiles */
		std::unordered_set<Tile*> _adyacentTiles;

		/** Entidades planas creadas a modo de feedback visual sobre las tiles sobre las que flota el edificio */
		std::vector<CEntity*> _floatingVisualEntities;

		/** Flag a true si el Placeable est� flotando y, por tanto, todav�a no debe estar registrado ni colisionar con otros objetos */
		bool _floating;

		/** Tile manager reference */
		CTileManager* _tileManager;

		/** Tipo del placeable */
		Logic::PlaceableType _placeableType;

		/** Tipo del edificio */
		Logic::BuildingType _buildingType;

		/** Tipos de terrenos aceptados */
		std::unordered_set<Logic::TerrainType> _validTerrainTypes;

		/** Objetivo para calcular ruta */
		CPlaceable* _walkingSoulTarget;

		/** Actualiza el conjunto de tiles adyacentes */
		void updateAdyacentTiles();

		/** Procesa peticiones pendientes de c�lculo de rutas */
		void processWalkingSoulPathRequest();

		//consume los recursos de un edificio a placear
		bool CPlaceable::ConsumeResourcesForConstruction();

		/** Nombre del material por defecto */
		std::string _defaultMaterial;

		/** Posici�n en la que queremos que se haga el place del edificio */
		Vector3 _targetPlacePosition;

		/** Flag para saber si al edificio se le est� haciendo place gr�fico */
		bool _doingGraphicPlace;

		/** Realiza el place animado para la parte gr�fica */
		void doGraphicPlace(int msecs);

		/** Velocidad del place gr�fico */
		float _placeSpeed;

		/** Posici�n en la que queremos que se haga el float del edificio */
		Vector3 _targetFloatPosition;

		/** Flag para saber si el edificio se le est� haciendo float gr�fico */
		bool _doingGraphicFloat;

		/** Realiza el float animado para la parte gr�fica */
		void doGraphicFloat(int msecs);

		/** Velocidad del float gr�fico */
		float _floatSpeed;

		/**
		* Realiza un movimiento gr�fico animado con cierto retardo.
		*
		* @param (in) msecs Tiempo (ms) transcurrido desde la �ltima actualizaci�n
		* @param (in) targetPosition Posici�n objetivo a la que queremos mover
		* @param (in) speed Velocidad a la que queremos que mueva
		* @param (out) Flag donde guardar si hace falta seguir moviendo o no
		*/
		void doDelayedGraphicMovement(int msecs, const Vector3& targetPosition, float speed, bool& moving);

		/**
		* Realiza un movimiento gr�fico inmediato.
		*
		* @param (in) targetPosition Posici�n objetivo a la que queremos mover
		* @param (out) Flag donde guardar si hace falta seguir moviendo o no
		*/
		void doImmediateGraphicMovement(const Vector3& targetPosition, bool& moving);

		const float ZERO_DISTANCE = 0.01f;

		/** Realiza la destrucci�n animada con efectos del edificio */
		void CPlaceable::doGraphicDestroy(int msecs);

		/** Flag a true si el edificio se estr� destruyendo */
		bool _destroying;

		/** Velocidad a la que se destruye (entierra) el edificio */
		float _destructionSpeed;

		/** Duraci�n (ms) de los efectos de destrucci�n del edificio */
		int _destructionDuration;

		/** Posici�n hacia la que tender� el edificio antes de destruirse */
		Vector3 _targetDestroyPosition;

	protected:
		/** Parsea un enum PlaceableType a partir del nombre en texto
		TODO Mover junto al c�digo del enum */
		static PlaceableType parsePlaceableType(const std::string& name);

		/** Parsea un enum BuildingType a partir del nombre en texto
		TODO Mover junto al c�digo del enum */
		static BuildingType parseBuildingType(const std::string& name);

		/** Imprime el texto asociado a un enum BuildingType
		TODO Mover junto al c�digo del enum */
		static std::string printBuildingType(BuildingType type);

		/** Parsea un enum TerrainType a partir del nombre en texto
		TODO Mover junto al c�digo del enum */
		static TerrainType parseTerrainType(const std::string& name);

	}; // class Placeable

	REG_FACTORY(CPlaceable);

} // namespace Logic

#endif // PLACEABLE_H_