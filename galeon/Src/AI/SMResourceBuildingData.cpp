#include "SMResourceBuildingData.h"
#include "Logic/ResourcesManager.h"

#include <iostream>
#include <cassert>

using namespace Logic;

namespace AI {
	/** Registra el tipo de recurso dado para ser almacenado */
	void CSMResourceBuildingData::registerStoredResourceType(ResourceType type) {
		// Inicializamos los recursos almacenados de ese tipo a 0
		_storedResources[type] = 0;

		// �dem para los recursos reservables
		_reservedResources[type] = 0;
	}

	/** Devuelve true o false seg�n si el tipo de recurso es almacenable en esta entidad o no */
	bool CSMResourceBuildingData::isResourceTypeStored(ResourceType type){
		return (_storedResources.count(type) > 0);
	}

	/** Devuelve la cantidad de recursos almacenados del tipo dado */
	int CSMResourceBuildingData::getStoredResources(ResourceType type) {
		if (!isResourceTypeStored(type)){
			return 0;
		}

		return _storedResources[type];
	}

	/** Devuelve la cantidad de recursos disponibles del tipo dado = Recursos existentes menos los reservados */
	int CSMResourceBuildingData::getAvailableResources(ResourceType type) {
		if (!isResourceTypeStored(type)){
			return 0;
		}

		return _storedResources[type] - _reservedResources[type];
	}

	/** Modifica los recursos del tipo dado seg�n la cantidad positiva o negativa indicada.
	Devuelve true o false seg�n si la operaci�n se realiz� correctamente o no */
	bool CSMResourceBuildingData::changeStoredResources(ResourceType type, int quantity){
		if (!isResourceTypeStored(type)){
			return 0;
		}

		int aux = _storedResources[type] + quantity;

		// Controlamos que el nuevo valor no sobrepase los l�mites
		if (aux < 0){
			std::cout << "Discarded changing " << quantity << " resources because there are only stored " << _storedResources[type] << std::endl;
			return false;
		}

		else if (aux > (int)_maxResources){
			std::cout << "Discarded changing " << quantity << " resources because there are already stored " << _storedResources[type] << " (limit = " << _maxResources << ")" << std::endl;
			return false;
		}

		// Registramos el nuevo valor con el cambio aplicado
		_storedResources[type] = aux;

		// Notificamos al ResourcesManager
		// TODO Atajo temporal para obtener el ResourcesManager
		Logic::ResourcesManager* resourcesManager = ResourcesManager::getSingletonPtr();
		resourcesManager->changeResources(type, quantity);

		return true;
	}

	/** Reserva los recursos del tipo dado seg�n la cantidad positiva indicada. 
	Con el flag allowPartial a true se permiten reservas parciales, esto es, se reserva todo lo que haya disponible aunque no llegue a lo solicitado.
	En finallyReserved se almacena la cantidad finalmente reservada.
	Devuelve true o false seg�n si la operaci�n se realiz� correctamente o no */
	bool CSMResourceBuildingData::reserveResources(ResourceType type, int quantity, bool allowPartial, int& finallyReserved){
		if (!isResourceTypeStored(type)){
			return 0;
		}

		if (quantity <= 0){
			assert(false && "Reserved quantity must be > 0");
			return false;
		}

		int aux = _reservedResources[type] + quantity;

		// Si se sobrepasa la cantidad disponible para reservar
		if (aux > getAvailableResources(type)){
			// Fallamos si no se deseaba reserva parcial
			if (!allowPartial){
				std::cout << "Discarded reserving " << quantity << " resources because there are only available " << getAvailableResources(type) << std::endl;
				finallyReserved = 0;
				return false;
			}

			// Si se permiten, reservamos hasta donde sea posible
			else{
				finallyReserved = getAvailableResources(type);
				_reservedResources[type] = _reservedResources[type] + finallyReserved;
				return true;
			}
		}

		_reservedResources[type] = aux;
		finallyReserved = quantity;

		return true;
	}

	/** Libera los recursos reservados del tipo dado seg�n la cantidad positiva indicada.
	Devuelve true o false seg�n si la operaci�n se realiz� correctamente o no */
	bool CSMResourceBuildingData::freeReservedResources(ResourceType type, int quantity){
		if (!isResourceTypeStored(type)){
			return 0;
		}

		if (quantity <= 0){
			assert(false && "Freed quantity must be > 0");
			return false;
		}

		int aux = _reservedResources[type] - quantity;

		// Controlamos que no se libere m�s de lo que estaba reservado
		if (aux < 0){
			assert(false && "Can't free resources that weren't reserved");
			return false;
		}

		_reservedResources[type] = aux;
		return true;
	}

	/** Reclama los recursos previamente reservados del tipo dado en la cantidad indicada
	Devuelve true o false seg�n si la operaci�n se realiz� correctamente o no */
	bool CSMResourceBuildingData::claimReservedResources(ResourceType type, int quantity){
		if (!changeStoredResources(type, -quantity))
			return false;
			
		return freeReservedResources(type, quantity);
	}

	std::unordered_set<ResourceType>& CSMResourceBuildingData::getProvidedResources() {
		return _providedResources;
	}

	/*void CSMResourceBuildingData::registerStoredResourceType(std::unordered_set<ResourceType>& providedResources) {
		providedResources = _providedResources;
	}*/

	unsigned int CSMResourceBuildingData::getMaxResources() const{
		return _maxResources;
	}

	void CSMResourceBuildingData::setMaxResources(unsigned int maxResources){
		_maxResources = maxResources;
	}

	unsigned int CSMResourceBuildingData::getInitialResources() const{
		return _initialResources;
	}

	void CSMResourceBuildingData::setInitialResources(unsigned int initialResources){
		_initialResources = initialResources;
	}

	void CSMResourceBuildingData::initResources(){
		for (auto it = _storedResources.cbegin(); it != _storedResources.cend(); ++it){
			ResourceType type = it->first;
			changeStoredResources(type, _initialResources);
		}
	}

	/** Limpia los recursos almacenados */
	void CSMResourceBuildingData::cleanResources() {
		for (auto it = _storedResources.cbegin(); it != _storedResources.cend(); ++it){
			ResourceType type = it->first;
			int quantity = it->second;

			if (quantity > 0)
				changeStoredResources(type, -quantity);
		}
	}
}
