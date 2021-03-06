#ifndef PATHFOLLOWER_H_
#define PATHFOLLOWER_H_

#include <queue>

#include "BaseSubsystems/Math.h"
#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"

namespace Logic
{
	class Tile;
}

namespace Logic
{
	class PathFollower : public IComponent
	{
		RTTI_DECL;
		DEC_FACTORY(PathFollower);

	public:
		PathFollower() : IComponent(), _dir(0.0f, 0.0f, 0.0f),
			_moving(false), _targetReached(false), _targetReachedNotified(false),
			_startPosition(0.0f, 0.0f, 0.0f), _targetPosition(0.0f, 0.0f, 0.0f)
		{}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool HandleMessage(const WalkSoulPathMessage& msg);
		void tick(unsigned int msecs);
		void addToQueue(const std::vector<Vector3>& path);
	
	protected:
		bool _moving;
		
		/** Flag a true si la entidad ha llegado al destino */
		bool _targetReached;
				
		/** Flag a true si la entidad ha podido notificar que ha llegado al destino */
		bool _targetReachedNotified;
		
		Vector3 _startPosition;
		Vector3 _targetPosition;

		std::queue<Vector3> _path;

		const float ZERO_DISTANCE = 0.01f;

	private:
		/** Direcci�n actual */
		Vector3 _dir;
	};

	REG_FACTORY(PathFollower);
}

#endif // PATHFOLLOWER_H_
