#include "LACheckNewConsumption.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerGeneratorData.h"
#include "Logic\ResourcesManager.h"
#include "Logic\Entity\Components\Billboard.h"

namespace AI {
	RTTI_IMPL(CLACheckNewConsumption, CLatentAction);

	bool CLACheckNewConsumption::HandleMessage(const ResourceMessage& msg) {
		// Rechazamos lo que no sean mensajes de informaci�n de recursos de coke
		if (msg._type != MessageType::RESOURCES_INFO || msg._resourceType != ResourceType::COKE)
			return false;

		// No se aceptan peticiones simult�neas
		if (_received)
			return false;

		_received = true;

		// Guardamos datos en la memoria compartida de la SM
		_smData.setCurrentCoke(msg._available);

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLACheckNewConsumption::OnStart() {
		// Inicializamos
		_received = false;
		_smData.setCurrentCoke(0);

		// Preparamos un mensaje para saber cu�nto coke tenemos
		ResourceMessage m;
		m.assembleResourcesAsk(ResourceType::COKE, _entity->getEntityID());

		// Repetimos hasta que se acepte el mensaje
		if (m.Dispatch(*_entity))
			// Nos suspendemos hasta que llegue la respuesta
			return LAStatus::SUSPENDED;

		else
			return LAStatus::READY;
	}

	CLatentAction::LAStatus CLACheckNewConsumption::OnRun(unsigned int msecs) {
		int totalConsumption = _smData.getTotalConsumptionUnits();
		int newConsumption = _smData.getNewConsumptionUnits();
		int currentCoke = _smData.getCurrentCoke();

		// Si las reservas soportan un ciclo completo del nuevo consumo total: �xito
		if (currentCoke >= (totalConsumption + newConsumption)){
			_smData.setNewConsumerAccepted(true);

			// Marcamos que ya no necesitamos coke
			IconMessage m(MessageType::ICON_DELETE, Billboard::getResourceIcon(ResourceType::COKE));
			bool result = m.Dispatch(*_entity);
			assert(result && "Can't remove resource icon");

			return LAStatus::SUCCESS;
		}
		
		// Si no, fallo
		else{
			_smData.setNewConsumerAccepted(false);

			// Marcamos que necesitamos coke
			IconMessage m(MessageType::ICON_ADD, Billboard::getResourceIcon(ResourceType::COKE));
			bool result = m.Dispatch(*_entity);
			assert(result && "Can't add resource icon");

			return LAStatus::FAIL;
		}
	}

}