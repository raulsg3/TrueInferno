#include "LAAcceptOrRejectNewConsumer.h"

#include "Logic\Entity\Message.h"
#include "Logic/ResourcesManager.h"
#include "Logic\Maps\Map.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {
	RTTI_IMPL(CLAAcceptOrRejectNewConsumer, CLatentAction);
	
	CLatentAction::LAStatus CLAAcceptOrRejectNewConsumer::OnStart() {
		// Inicializamos
		_consumerNotified = false;
		_consumptionChangeNotified = false;
		_firstConsumer = false;
		_firstConsumerNotified = false;

		// A�adimos al consumidor a la lista de conexiones de consumidores del PowerGenerator
		if (_smData.getNewConsumerAccepted()){
			Logic::Consumer consumer;
			consumer.id = _smData.getNewConsumer();
			consumer.consumption = _smData.getNewConsumptionUnits();
			consumer.period = _smData.getNewConsumptionPeriod();

			bool added = _smData.addConsumer(consumer);

			// Si se a�adi� consumidor y la lista tiene un �nico consumidor es que es el primero
			if (added && _smData.getNumConsumers() == 1)
				_firstConsumer = true;
		}

		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAAcceptOrRejectNewConsumer::OnRun(unsigned int msecs) {
		/* Hacemos 3 cosas:
		* 1) Notificar a la entidad del consumidor de su conexi�n/desconexi�n
		* 2) Notificar a nuestra entidad si era el primer consumidor o el �ltimo para empezar o parar el consumo
		* 3) Notificar a nuestra entidad del cambio en el consumo (NOTA: despu�s de haber notificado el comienzo del consumo)
		*/

		// Notificamos al consumidor de la conexi�n/desconexi�n si no lo hab�amos hecho ya
		if (!_consumerNotified){
			// Verificamos que existe el consumidor
			CEntity *consumer = _entity->getMap()->getEntityByID(_smData.getNewConsumer());
			if (consumer != nullptr){
				// Preparamos el mensaje con la conexi�n/desconexi�n
				PowerMessage powerMsg;
				powerMsg.assemblePowerAttachmentInfo(_entity->getEntityID(), _smData.getNewConsumerAccepted(), _smData.getNewConsumptionUnits(), _smData.getNewConsumptionPeriod());

				// Reintentamos el env�o hasta que se acepte
				if (powerMsg.Dispatch(*consumer)){
					_consumerNotified = true;
					_smData.setNewConsumer(EntityID::UNASSIGNED);
				}

				else
					return LAStatus::RUNNING;
			}

			else{
				// Damos por notificado al consumidor porque ha desaparecido
				_consumerNotified = true;
			}
		}

		// Si era el primer consumidor y todav�a no lo hemos notificado
		if (_firstConsumer && !_firstConsumerNotified){
			// Ordenamos que empiece el consumo
			ConsumptionMessage m;
			m.assembleConsumptionStart(ResourceType::COKE);

			// Reintentamos el env�o hasta que se acepte
			if (m.Dispatch(*_entity))
				_firstConsumerNotified = true;

			else
				return LAStatus::RUNNING;
		}

		// Notificamos el cambio de consumo en el caso de nuevos consumidores si no estaba hecho ya
		if (!_consumptionChangeNotified && _smData.getNewConsumerAccepted()){
			// Avisamos del cambio del consumo
			ConsumptionMessage consumptionMsg;

			// El cambio es positivo si es una conexi�n y negativo en caso contrario
			int consumptionChange = (_smData.getNewConsumerAccepted() ? 1 : -1) * _smData.getNewConsumptionUnits();

			consumptionMsg.assembleConsumptionChange(ResourceType::COKE, consumptionChange, _smData.getNewConsumptionPeriod());

			if (consumptionMsg.Dispatch(*_entity))
				_consumptionChangeNotified = true;

			else
				return LAStatus::RUNNING;
		}

		return LAStatus::SUCCESS;
	}

}