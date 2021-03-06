//---------------------------------------------------------------------------
// TileManager.cpp
//---------------------------------------------------------------------------

/**
@file TileManager.cpp

Contiene la implementaci�n del gestor de la matriz de tiles.

@see Logic::CTileManager

@author Ra�l Segura
@date Enero, 2016
*/

#include "TileManager.h"

#include "Map/MapParser.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Tile.h"

#include <cassert>
#include <fstream>

namespace Logic {

	CTileManager* CTileManager::_instance = nullptr;
	const int CTileManager::SIZE_X = 45;
	const int CTileManager::SIZE_Z = 45;
	const std::string CTileManager::TERRAIN_MAP_FILE = "media/maps/terrain_map.txt";

	//--------------------------------------------------------

	CTileManager::CTileManager()
	{
		_instance = this;
		_tiles = nullptr;

	} // CTileManager

	//--------------------------------------------------------

	CTileManager::~CTileManager()
	{
		assert(_instance);
		_instance = 0;

		// Free tile matrix
		if (_tiles){
			for (int x = 0; x < SIZE_X; ++x){
				if (_tiles[x]){
					delete(_tiles[x]);
					_tiles[x] = nullptr;
				}
			}
			delete(_tiles);
			_tiles = nullptr;
		}

	} // ~CTileManager

	//--------------------------------------------------------

	bool CTileManager::Init()
	{
		assert(!_instance && "Segunda inicializaci�n de Logic::CTileManager no permitida!");

		new CTileManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CTileManager::Release()
	{
		assert(_instance && "Logic::CTileManager no est� inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CTileManager::loadInitialMatrix(CMap *map)
	{
		// Coge la Map::CEntity "Tile" le�da del fichero de mapa a modo de prefab.
		// @TODO Hacerlo en Map::CParser mediante una funci�n gen�rica que reciba el nombre de la Map::CEntity.
		Map::CMapParser::TEntityList mapEntityList =
			Map::CMapParser::getSingletonPtr()->getEntityList();

		Map::CMapParser::TEntityList::const_iterator it, end;
		it = mapEntityList.begin();
		end = mapEntityList.end();

		// Itera sobre la lista de entidades del mapa hasta encontrar el prefab "Tile".
		for (; it != end; it++) {
			if ((*it)->getType() == "Tile") {
				tileMapEntity = *it;
			}
		}

		assert(tileMapEntity && "Map::CEntity Tile not found");

		// Genera todas las Logic::CEntity tiles de la matriz a partir de la
		// Map::CEntity "Tile" le�da del fichero de mapa.
		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();

		std::string tileBaseName = tileMapEntity->getStringAttribute("name");
		Vector3 tileBasePosition = tileMapEntity->getVector3Attribute("position");
		Vector3 tileBaseDimensions = tileMapEntity->getVector3Attribute("dimensions");

		// Reserva de memoria para la matriz de tiles.
		_tiles = new Tile**[SIZE_X];
		for (int x = 0; x < SIZE_X; ++x) {
			_tiles[x] = new Tile*[SIZE_Z];
			for (int z = 0; z < SIZE_Z; ++z) {
				_tiles[x][z] = nullptr;
			}
		}

		// Lectura de los tiles del mapa.
		// Se leen en el mismo orden en el que luego se les asigna el terreno, pero no es necesario.
		for (int x = 0; x < SIZE_X; ++x) {
			for (int z = 0; z < SIZE_Z; ++z) {
				// Cambia la posici�n.
				Vector3 tilePosition(tileBasePosition);

				tilePosition.x = (float) x; // *tileBaseDimensions.x;
				tilePosition.y = (float) 0; // Matriz de tiles 2D
				tilePosition.z = (float) z; // *tileBaseDimensions.z;

				// Construye la nueva posici�n como un Vector3 representado por un
				// string con formato: "x y z".
				// @TODO Esto deber�a hacerse en Map::MapEntity.
				std::stringstream newPosition;
				newPosition << tilePosition.x << " " << tilePosition.y << " " << tilePosition.z;
				tileMapEntity->setAttribute("position", newPosition.str());

				// Cambia el nombre (debe ser �nico!).
				std::stringstream newTileName;
				newTileName << tileBaseName;
				newTileName << "_" << tilePosition.x << "_" << tilePosition.z;
				tileMapEntity->setName(newTileName.str());

				// Crea una nueva Logic::CEntity tile.
				CEntity *entityTile = entityFactory->createEntity(tileMapEntity, map);
				assert(entityTile && "Failed to create entity Tile[X,Z]");
			}
		}

		// Procesa la configuraci�n inicial del terreno.
		loadTerrain(TERRAIN_MAP_FILE);
		
		return true;
	} // loadInitialMatrix

	//--------------------------------------------------------

	Tile* CTileManager::getRandomTile(){
		int x = rand() % SIZE_X;
		int z = rand() % SIZE_Z;
		return _tiles[x][z];
	}

	//--------------------------------------------------------

	Tile* CTileManager::getNearestTile(const Vector3 &pos) const{
		float minSqrDist = FLT_MAX;
		Tile* nearestTile = nullptr;

		for (int x = 0; x < SIZE_X; ++x)
			for (int z = 0; z < SIZE_Z; ++z){
				Tile *tile = _tiles[x][z];
				float sqrDist = pos.squaredDistance(tile->getEntity()->getPosition());
				if (sqrDist < minSqrDist){
					minSqrDist = sqrDist;
					nearestTile = tile;
				}
			}

		return nearestTile;
	}

	//--------------------------------------------------------

	Tile* CTileManager::getTile(const Vector3 &position){
		// Check bounds
		if (position.x < 0 || position.x >= SIZE_X || position.z < 0 || position.z >= SIZE_Z)
			return nullptr;

		return _tiles[(int)position.x][(int)position.z];

	} // getTile

	//--------------------------------------------------------

	void CTileManager::printDebugInfo(){
		for (int x = 0; x < SIZE_X; ++x) {
			for (int z = 0; z < SIZE_Z; ++z) {
				_tiles[x][z]->printDebugInfo();
				std::cout << std::endl;
			}
		}
	}

	//--------------------------------------------------------

	Tile* CTileManager::getTileByName(std::string tileName){
		for (int x = 0; x < SIZE_X; ++x) {
			for (int z = 0; z < SIZE_Z; ++z) {
				if (_tiles[x][z]->getEntity()->getName() == tileName)
					return _tiles[x][z];
			}
		}

		return nullptr;
	}

	//--------------------------------------------------------

	void CTileManager::registerTile(Tile *tile) {
		Vector3 position = tile->getLogicPosition();
		_tiles[(int)(position.x)][(int)(position.z)] = tile;

	} // registerTile

	//--------------------------------------------------------

	void CTileManager::loadTerrain(const std::string &filename) {
		// Open file
		std::ifstream file(filename);

		// For each line
		std::string line;
		int z = 0;

		while (z < SIZE_Z && std::getline(file, line)) {
			// Read line
			std::stringstream ss(line);
			
			// For each token in line delimited by whitespace
			int x = 0;
			std::string item;

			while (x < SIZE_X && std::getline(ss, item, ' ')) {
				assert((item.size() == 1) && "Only tokens of 1 char are allowed in the terrain map");
				
				TerrainType terrainType;
				char terrainTypeChar(item[0]);

				switch (terrainTypeChar) {
				case '0':
					terrainType = TerrainType::Empty;
					break;
				case '1':
					terrainType = TerrainType::Mineral;
					break;
				case '2':
					terrainType = TerrainType::Gas;
					break;
				default:
					assert("Unknown terrain type char: " + terrainType);
				}

				// Actualizamos el terreno del Tile
				bool result = _tiles[x][z]->changeTerrain(terrainType);
				assert(result && "Can't change tile's terrain");

				++x;
			} // while x

			++z;
		} // while z

		file.close();

	} // loadTerrain

	//--------------------------------------------------------

	bool CTileManager::open() {
		return true;
	} // open

	//--------------------------------------------------------

	void CTileManager::close() {
	} // close

} // namespace Logic
