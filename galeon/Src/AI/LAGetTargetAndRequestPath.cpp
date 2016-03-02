#include "LAGetTargetAndRequestPath.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"

namespace AI {
	bool CLAGetTargetAndRequestPath::HandleMessage(const SoulMessage& msg) {
		// Rechazamos lo que no sean peticiones. No aceptamos m�s de una petici�n simult�nea
		if (msg._type != MessageType::SOUL_REQUEST || _target != nullptr)
			return false;

		_target = msg._task->getTarget();

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAGetTargetAndRequestPath::OnStart() {
		// Inicializamos
		_target = nullptr;

		// Suspendemos la LA hasta que llegue el mensaje con la petici�n
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAGetTargetAndRequestPath::OnRun() {
		// Verificamos que hay objetivo
		if (_target == nullptr)
			return LAStatus::FAIL;

		// Reintentamos hasta que podamos solicitar ruta
		return requestPath() ? LAStatus::SUCCESS : LAStatus::RUNNING;
	}

	bool CLAGetTargetAndRequestPath::requestPath(){
		WalkSoulPathMessage m(_target);
		return m.Dispatch(*_entity);
	}
}