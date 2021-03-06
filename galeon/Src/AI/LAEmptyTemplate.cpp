/*
#include "LAEmptyTemplate.h"

#include "Logic\Entity\Message.h"
#include "AI\SMEmptyTemplateData.h"

namespace AI {
	RTTI_IMPL(CLAEmptyTemplate, CLatentAction);
	
	bool CLAEmptyTemplate::HandleMessage(const XXXMessage& msg) {
		// Rechazamos lo que no sean mensajes de petición
		if (msg._type != MessageType::EMPTY_TEMPLATE_REQUEST)
			return false;

		// No se aceptan peticiones simultáneas
		if (_received)
			return false;

		_received = true;

		// Guardamos datos en la memoria compartida de la SM
		_smData.setXXX(msg._xxx);

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAEmptyTemplate::OnStart() {
		// Inicializamos
		_received = false;

		// Suspendemos la LA hasta que llegue un mensaje de petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAEmptyTemplate::OnRun(unsigned int msecs) {
		// ...
		return LAStatus::SUCCESS;
	}

}
*/