#include "LABurnSouls.h"

#include "Logic\ResourcesManager.h"
#include "Logic\Entity\ParticleType.h"

namespace AI
{
	RTTI_IMPL(CLABurnSouls, CLatentAction);

	CLatentAction::LAStatus CLABurnSouls::OnStart()
	{
		_cokeIncreased = _crudeIncreased = false;
		_totalCokeToIncrease = _totalCrudeToIncrease = 0;

		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLABurnSouls::OnRun(unsigned int msecs)
	{
		// Quemamos las almas encoladas actualmente
		int soulCrude = 0;
		int soulCoke = 0;

		while (_smData.getNumSoulsToBurn() > 0)
		{
			SoulsTrialManager::SoulsCategory nextSoulCategory = _smData.getNextSoulToBurn();
			CSoulsTrialManager::getSingletonPtr()->calculateSoulEvil(nextSoulCategory, soulCrude, soulCoke);

			_totalCrudeToIncrease += soulCrude;
			_totalCokeToIncrease += soulCoke;
		}

		// Notificamos el incremento de Coke si no est� hecho ya
		if (!_cokeIncreased)
		{
			ResourceMessage mCoke;
			mCoke.assembleResourcesChange(Logic::ResourceType::COKE, _totalCokeToIncrease);

			// Si falla intentaremos en el siguiente tick
			if (!mCoke.Dispatch(*_entity))
			{
				return LAStatus::RUNNING;
			}
			else
				_cokeIncreased = true;
		}

		// Notificamos el incremento de Crude si no est� hecho ya
		if (!_crudeIncreased)
		{
			ResourceMessage mCrude;
			mCrude.assembleResourcesChange(Logic::ResourceType::CRUDE, _totalCrudeToIncrease);

			// Si falla intentaremos en el siguiente tick
			if (!mCrude.Dispatch(*_entity))
			{
				return LAStatus::RUNNING;
			}
			else
				_crudeIncreased = true;
		}

		// Si ya se han realizado ambos incrementos...

		// Activamos partículas de acción de edificio
		ParticleMessage pm(ParticleType::BUILDING_ACTION, _burnParticlesDuration);
		bool result = pm.Dispatch(*_entity);
		assert(true && "Can't set building action particles");

		return LAStatus::SUCCESS;
	}
}