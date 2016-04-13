#ifndef LA_BURN_SOULS_
#define LA_BURN_SOULS_

#include "Logic\Entity\Message.h"
#include "AI\LatentAction.h"
#include "AI\SMSoulBurnerData.h"

namespace AI {
	class CLABurnSouls : public CLatentAction {
		RTTI_DECL;

	public:
		CLABurnSouls(CEntity* entity, CSMSoulBurnerData& smData, int cokePerSoul, int crudePerSoul) : 
			CLatentAction(entity), _smData(smData), _cokePerSoul(cokePerSoul), _crudePerSoul(crudePerSoul) {}

		virtual ~CLABurnSouls() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMSoulBurnerData& _smData;

		const int _cokePerSoul;

		const int _crudePerSoul;

		/** Flag para se�alar que el incremento de coke ya est� hecho */
		bool _cokeIncreased;

		/** Flag para se�alar que el incremento de crude ya est� hecho */
		bool _crudeIncreased;
	};
}

#endif