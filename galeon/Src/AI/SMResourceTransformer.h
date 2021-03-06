#ifndef SM_RESOURCE_TRANSFORMER_
#define SM_RESOURCE_TRANSFORMER_

#include "Logic\Entity\Message.h"
#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\SMResourceTransformerData.h"
#include "AI\LARecountResources.h"
#include "AI\LAAskAndWaitResources.h"
#include "AI\LATransformResources.h"
#include "AI\LatentAction.h"
#include "AI\Condition.h"
#include "Map\MapEntity.h"

namespace AI {
	/**
	Esta FSM controla la lógica de transformación cíclica de recursos de entrada en recursos de salida.

	- Cuenta los recursos de partida del recurso de entrada y de salida
	- En base a la cantidad actual y límite de los recursos de entrada, solicita la máxima cantidad posible que no desborde dicha capacidad
	- Cuenta los recursos conseguidos
	- Paga los costes
	- Transforma los recursos de entrada en salida
	*/ 
	class CSMResourceTransformer : public CStateMachine<CLatentAction, CSMResourceTransformerData> {
	public:
		CSMResourceTransformer(CEntity* entity) : CStateMachine<CLatentAction, CSMResourceTransformerData>(entity) {}

		virtual ~CSMResourceTransformer() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			// Lectura de datos
			if (entityInfo->hasAttribute("transformsFrom"))
				_resourceFrom = Logic::ResourcesManager::parseResourceType(entityInfo->getStringAttribute("transformsFrom"));
			else{
				assert(false && "transformsFrom not defined");
				return false;
			}

			if(entityInfo->hasAttribute("transformsInto"))
				_resourceInto = Logic::ResourcesManager::parseResourceType(entityInfo->getStringAttribute("transformsInto"));
			else{
				assert(false && "transformsInto not defined");
				return false;
			}
			
			if(entityInfo->hasAttribute("transformRatio"))
				_transformRatio = entityInfo->getFloatAttribute("transformRatio");
			else{
				assert(false && "transformRatio not defined");
				return false;
			}

			if(entityInfo->hasAttribute("transformPeriodSecs"))
				_period = 1000 * entityInfo->getIntAttribute("transformPeriodSecs");
			else{
				assert(false && "transformPeriodSecs not defined");
				return false;
			}

			if(entityInfo->hasAttribute("transformParticlesDurationMsecs"))
				_transformParticlesDuration = entityInfo->getIntAttribute("transformParticlesDurationMsecs");
			else{
				assert(false && "transformParticlesDurationMsecs not defined");
				return false;
			}

			// El recurso de coste de la transformación es opcional
			if (entityInfo->hasAttribute("transformCostResource"))
				_costResource = Logic::ResourcesManager::parseResourceType(entityInfo->getStringAttribute("transformCostResource"));
			else
				_costResource = ResourceType::NONE;

			// El ratio de coste de la transformación es opcional
			if (entityInfo->hasAttribute("transformCostRatio"))
				_costRatio = entityInfo->getFloatAttribute("transformCostRatio");
			else
				_costRatio = 0;

			// Creación de SM en base a los datos
			int recountResourcesBeforeAsking = this->addNode(new CLARecountResources<CSMResourceTransformerData>(_entity, _data, _resourceFrom));
			int gatherResources = this->addNode(new CLAAskAndWaitResources(_entity, _data, _resourceFrom, _period));
			int recountResourcesBeforeTransforming = this->addNode(new CLARecountResources<CSMResourceTransformerData>(_entity, _data, _resourceFrom));
			int transformResources = this->addNode(new CLATransformResources(_entity, _data, _resourceFrom, _resourceInto, _transformRatio, _costResource, _costRatio, _transformParticlesDuration));

			this->addEdge(recountResourcesBeforeAsking, gatherResources, new CConditionFinished());
			this->addEdge(gatherResources, recountResourcesBeforeTransforming, new CConditionFinished());
			this->addEdge(recountResourcesBeforeTransforming, transformResources, new CConditionFinished());
			this->addEdge(transformResources, recountResourcesBeforeAsking, new CConditionFinished());

			this->setInitialNode(recountResourcesBeforeAsking);
			this->resetExecution();

			return true;
		}

		SM_HANDLE_MESSAGE(ResourceMessage);

	private:
		/** Recurso de entrada */
		ResourceType _resourceFrom;

		/** Recurso de salida */
		ResourceType _resourceInto;

		/** Ratio de conversión del recurso de entrada en el recurso de salida. 
		* Ejemplo: Si el ratio es 0.8, cada recurso de entrada se transforma en 0.8 del de salida */
		float _transformRatio;

		/** Periodo (ms) con que se hace la transformación */
		int _period;

		/** Tipo de recurso para los costes */
		ResourceType _costResource;

		/** Ratio de coste del recurso de costes sobre los recursos de entrada. 
		* Ejemplo: Si el ratio es 2, cada recurso de entrada que se quiera transformar cuesta 2 del de costes */
		float _costRatio;

		/** Duración (ms) de las partículas de transformación de recursos */
		int _transformParticlesDuration;
	};
}

#endif // SM_RESOURCE_TRANSFORMER_