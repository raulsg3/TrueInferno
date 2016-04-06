#include "LAAttachToGenerator.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerConsumerData.h"

namespace AI {
	bool CLAAttachToGenerator::HandleMessage(const PowerMessage& msg) {
		// Rechazamos lo que no sean mensajes de informaci�n de conexi�n a generador
		if (msg._type != MessageType::POWER_ATTACHMENT_INFO)
			return false;

		// No se aceptan peticiones simult�neas
		if (_received)
			return false;

		assert(msg._caller == _smData.getPowerGenerator() && "The requested PowerGenerator to attach is different from the received");
		assert(msg._caller != Logic::EntityID::UNASSIGNED && "PowerGenerator caller is unknown");

		// Actualizamos el generador al que finalmente nos hemos conectado
		_smData.setPowerGenerator(msg._caller);

		// Y el estado de conexi�n
		_smData.setAttached(msg._attach);

		_received = true;

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAAttachToGenerator::OnStart() {
		// Inicializamos
		_received = false;

		// Localizamos la entidad del generador al que nos queremos conectar
		CEntity *powerGenerator = _entity->getMap()->getEntityByID(_smData.getPowerGenerator());

		// Si no se pudo localizar, fallamos
		if (powerGenerator == nullptr)
			return LAStatus::FAIL;

		// Preparamos la solicitud de conexi�n indicando qui�nes somos y nuestro consumo
		PowerMessage m(this->_entity->getEntityID(), 10); // TODO Leer en el spawn

		// Reintentamos la petici�n de conexi�n hasta que nos la acepten en alg�n tick
		if (m.Dispatch(*powerGenerator)){
			// Si nos aceptaron nos suspendemos hasta que nos respondan
			return LAStatus::SUSPENDED;
		}
		else
			return LAStatus::READY;
	}

	CLatentAction::LAStatus CLAAttachToGenerator::OnRun(unsigned int msecs) {
		assert(_received && "No message received");

		// Chequeamos que recibimos confirmaci�n de conexi�n v�lida
		if (_smData.getAttached() && _smData.getPowerGenerator()!=EntityID::UNASSIGNED)
			return LAStatus::SUCCESS;

		else
			return LAStatus::FAIL;
	}

}