#ifndef PERFORM_TRANSPORT_SOUL_TASK_H_
#define PERFORM_TRANSPORT_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic\Entity\Message.h"
#include "Logic\ResourcesManager.h"
#include "Logic\Maps\Map.h"
#include "Logic\Entity\Entity.h"
#include "Logic\Entity\BuildingType.h"
#include "Logic\Entity\IconType.h"
#include "Logic/Entity/Components/Billboard.h"

using namespace Logic;

namespace AI{

	class CPerformTransportSoulTask : public CSoulTask {

	public:
		CPerformTransportSoulTask(CMap *map, const TEntityID& toId, ResourceType resourceType, int resourceQuantity,
			Logic::SoulsTrialManager::SoulsCategory category = Logic::SoulsTrialManager::SoulsCategory::UNKNOWN) :
			CSoulTask(map, toId, category), // El objetivo es el punto de final del transporte donde entregaremos los recursos
			_resourceType(resourceType),
			_resourceQuantity(resourceQuantity) {};

		virtual ~CPerformTransportSoulTask() {};

		virtual CSoulTask* clone(){
			return new CPerformTransportSoulTask(_map, _target, _resourceType, _resourceQuantity, _category);
		}

		bool start(){
			CSoulTask::start();

			// Obtenemos la entidad que va a ejecutar la tarea
			CEntity* executor = _map->getEntityByID(_executorId);

			// Si existe, establecemos sus iconos
			if (executor != nullptr){
				// Icono de recurso transportado actualmente por la entidad
				IconMessage m(MessageType::ICON_ADD, Billboard::getResourceIcon(_resourceType));
				bool result = m.Dispatch(*executor);
				assert(result && "Can't set resource icon");
				
				// Icono por edificio de destino
				result = addDestinationBuildingIcon();
				assert(result && "Can't set building icon");

				// Cambio de material
				std::string materialName = "";
				switch (_resourceType) {
				case MINERAL: 
					materialName = "SoulMineral"; 
					break;

				case GAS: 
					materialName = "SoulGas"; 
					break;

				case CRUDE: 
					materialName = "SoulCrude"; 
					break;

				case PURE_EVIL: 
					materialName = "SoulPure"; 
					break;

				case REFINED: 
					materialName = "SoulRefined"; 
					break;

				case COKE: 
					materialName = "SoulCoke"; 
					break;

				default:
					materialName = "";
				}

				if (!materialName.empty()) {
					MaterialMessage m(materialName);
					bool result = m.Dispatch(*executor);
					if (!result){
						assert(false && "Can't change material");
					}
				}
				else {
					assert(false && "No material defined for this resource type");
				}
			}

			else{
				assert(false && "There is no executor starting the task");
				// Dejamos que siga sin icono
			}

			return true;
		}

		bool execute() {
			CSoulTask::execute();

			// Chequeamos que el objetivo al que acabamos de llegar siga existiendo
			CEntity* targetEntity = _map->getEntityByID(_target);

			// Si lo est�
			if (targetEntity != nullptr){
				// Entrengamos los recursos en el objetivo
				ResourceMessage m;
				m.assembleResourcesChange(_resourceType, _resourceQuantity);
				bool ret = m.Dispatch(*targetEntity);

				// En caso correcto los recursos han sido entregados por lo que podemos limpiar
				if (ret){
					_resourceType = ResourceType::NONE;
					_resourceQuantity = 0;
				}

				return ret;
			}

			// Si no
			else{
				std::cout << "Soul's target for PerformTransportSoulTask has disappeared" << std::endl;

				// No perdemos los recursos. Si el alma desaparece, sus recursos se ir�n con ella y no hace falta notificarlo al ResourcesManager
				// Tambi�n podremos querer buscarle otro objetivo

				return true;
			}
		};

	private:
		// Tipo de recurso que porta el alma
		ResourceType _resourceType;

		// Cantidad de recursos que porta el alma
		int _resourceQuantity;

	}; // class CPerformTransportSoulTask

} // namespace AI

#endif // PERFORM_TRANSPORT_SOUL_TASK_H_