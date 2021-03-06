#include "Toggleable.h"

#include <iostream>

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/IconType.h"
#include "Logic/Entity/Components/Billboard.h"

namespace Logic {
	RTTI_ROOT_IMPL(CToggleable);
	IMP_FACTORY(CToggleable);

	bool CToggleable::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		/*
		if (entityInfo->hasAttribute("initiallyEnabled")){
			_enabled = entityInfo->getBoolAttribute("initiallyEnabled");
		}
		else
			_enabled = false;

		_update = false;
		_newEnabled = _enabled;
		*/

		// Guardamos en cadena de caracteres los requisitos iniciales de la entidad (si hay alguno)
		if (entityInfo->hasAttribute("requirements")){
			_requirementsStr = entityInfo->getStringAttribute("requirements");

			// Todav�a no los activamos porque necesitamos que est�n spawneados el resto de componentes
		}

		return true;
	} // spawn

	bool CToggleable::activate() {
		if (!IComponent::activate())
			return false;

		// Guardamos el set con los requisitos iniciales de la entidad (si hay alguno)
		if (!_requirementsStr.empty()){
			std::istringstream ss(_requirementsStr);
			std::string item;

			// Para cada elemento separado por coma
			while (std::getline(ss, item, ',')) {
				// Almacenamos el requisito
				addRequirement(parseLogicRequirement(item));
			}

			_requirementsStr = "";
		}

		return true;
	}

	void CToggleable::tick(unsigned int msecs){
		/*
		// Si hay que actualizar
		if (_update){
			// Actualizamos
			_enabled = _newEnabled;

			std::cout << "Logic enabled = " << _enabled << std::endl;

			// Limpiamos el flag de actualizaci�n
			_update = false;
		}
		*/
	} // tick

	bool CToggleable::isLogicEnabled(const std::set<LogicRequirement>& skippedRequirements) const {
		// OJO con confundir el argumento del set skippedRequirements con el propio _skippedRequirements que tiene el componente CToggleable!

		// Si no hay requisitos por satisfacer estamos habilitados
		if (_requirements.empty())
			return true;

		// Si el conjunto de requisitos a ignorar contiene el elemento especial 'All' estamos habilitados
		if (skippedRequirements.count(LogicRequirement::All) > 0)
			return true;

		// Buscamos si hay alg�n requisito pendiente que NO est� entre los ignorados y fallamos
		for (auto it = _requirements.cbegin(); it != _requirements.cend(); ++it){
			LogicRequirement requirement = *it;
			if (skippedRequirements.count(requirement) == 0)
				return false;
		}
		
		// True porque todos los requisitos pendientes est�n ignorados
		return true;
	}


	bool CToggleable::HandleMessage(const ToggleMessage& msg){
		// Solicitudes de cambio
		if (msg._type == MessageType::TOGGLE_REQUEST)
			return handleToggleRequest(msg);

		// Solicitudes de comprobaci�n de estado
		else if (msg._type == MessageType::TOGGLE_CHECK)
			return handleCheckStatus(msg);

		return false;
	}

	bool CToggleable::handleToggleRequest(const ToggleMessage& msg){
		/*
		// Si no hab�a actualizaci�n en curso
		if (!_update){
		// El nuevo valor de habilitado viene dado por el mensaje
		_newEnabled = msg._enable;

		// Marcamos que estamos actualizando
		_update = true;
		}

		// Si s� hab�a actualizaci�n en curso
		else{
		// Habilitaremos si y s�lo todos los cambios son para habilitar
		_newEnabled &= msg._enable;
		}
		*/

		// Si tenemos que a�adir requisito
		if (msg._add) {
			addRequirement(msg._requirement);
		}

		// Si no, tenemos que eliminar requisito
		else {
			removeRequirement(msg._requirement);
		}

		return true;
	}

	bool CToggleable::handleCheckStatus(const ToggleMessage& msg){
		bool enabled = _requirements.empty();
		ToggleMessage m(enabled);
		m.Dispatch(*_entity);

		return true;
	}


	bool CToggleable::addRequirement(LogicRequirement requirement){
		// Chequeamos si es el primero en a�adirse
		bool wasEmpty = false;
		if (_requirements.empty()){
			wasEmpty = true;
		}

		// Chequeamos que un mismo requisito no se intente a�adir varias veces
		bool added = (_requirements.emplace(requirement)).second;
		assert(added && "Adding requirement more than once");

		// Si se ha a�adido algo y la lista de requisitos estaba vac�a: Fue el primero en a�adirse ==> La entidad acaba de ser desactivada l�gicamente
		if (added && wasEmpty){
			ToggleMessage m(false);
			bool result = m.Dispatch(*_entity);
			assert(result && "Unhandled ToggleMessage to disable entity");
		}

		// Activamos el icono correspondiente a la necesidad, si es que hay alguno configurado
		IconType::IconType desiredIcon = Billboard::getLogicRequirementIcon(requirement);
		if (desiredIcon != IconType::NONE) {
			IconMessage m2(MessageType::ICON_ADD, desiredIcon);
			bool result = m2.Dispatch(*_entity);
			assert(result && "Can't set icon for logicRequirement");
		}

		//std::cout << "Logic: Added '" << printLogicRequirement(requirement) << "' requirement" << std::endl;

		return added;
	}

	bool CToggleable::removeRequirement(LogicRequirement requirement){
		// Chequeamos que el requisito exista antes de intentar eliminarlo
		bool removed = _requirements.erase(requirement) != 0;
		assert(removed && "Removing requirement that doesn't exist");

		// Si se ha borrado algo y la lista de requisitos ahora est� vac�a: Fue el �ltimo por eliminar ==> La entidad acaba de ser activada l�gicamente
		if (removed && _requirements.empty()){
			ToggleMessage m(true);
			const bool result = m.Dispatch(*_entity);
			assert(result && "Unhandled ToggleMessage to enable entity");
		}

		// Eliminamos el icono correspondiente a la necesidad
		IconMessage m2(MessageType::ICON_DELETE, Billboard::getLogicRequirementIcon(requirement));
		const bool result = m2.Dispatch(*_entity);
		assert(result && "Can't delete icon for logicRequirement");

		//std::cout << "Logic: Removed '" << printLogicRequirement(requirement) << "' requirement. " << _requirements.size() << " requirements left" << std::endl;

		return removed;
	}


} // namespace Logic