#include "PathFinder.h"

namespace Logic {
	RTTI_ROOT_IMPL(CPathFinder);
	IMP_FACTORY(CPathFinder);

	bool CPathFinder::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		// Garantizamos que se llama al padre
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		return true;
	} // spawn

	void CPathFinder::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

} // namespace Logic