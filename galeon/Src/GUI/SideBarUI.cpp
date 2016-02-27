#include "SideBarUI.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/CEGUI.h>

#include "Map/MapParser.h"
#include "Map/MapEntity.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Placeable.h"
#include "Logic/BuildingManager.h"
#include "Logic/Server.h"
#include "Logic/Entity/Components/Tile.h"
#include "Logic/Maps/Managers/TileManager.h"

#include "Graphics/Scene.h"
#include "Graphics/Server.h"
#include "Graphics/Camera.h"

#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneQuery.h>

#include "Physics/Server.h"
#include "Logic\Entity\Message.h"
#include "AI/Server.h"

namespace GUI
{
	SideBarUI::SideBarUI()
	{
	}


	SideBarUI::~SideBarUI()
	{
	}

	void SideBarUI::init()
	{
		_uibuttonsWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIButtonBar.layout");

		_uibuttonsWindow->getChildElement("CreateFurnace")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createFurnaceReleased, this));

		_uibuttonsWindow->getChildElement("CreateRoad")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createRoadReleased, this));

		_uibuttonsWindow->getChildElement("CreateResource1Building")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createResource1BuildingReleased, this));

		_uibuttonsWindow->getChildElement("CreateResource2Building")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createResource2BuildingReleased, this));

		_uibuttonsWindow->getChildElement("CreateSoul")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createSoulReleased, this));

		_uibuttonsWindow->getChildElement("MoveSoul")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::moveSoulReleased, this));

		_uibuttonsWindow->getChildElement("CreateEvilworks")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createEvilworksReleased, this));

		_uibuttonsWindow->getChildElement("CreateRefinery")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createRefineryReleased, this));

		_uibuttonsWindow->getChildElement("RepairBuilding")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::repairBuildingReleased, this));

		_uibuttonsWindow->getChildElement("ClearTerrain")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::clearTerrainReleased, this));
	}

	void SideBarUI::release()
	{
		// Remove all events to avoid memory leaks
		_uibuttonsWindow->getChildElement("CreateFurnace")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateRoad")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateResource1Building")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateResource2Building")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateSoul")->removeAllEvents();
		_uibuttonsWindow->getChildElement("MoveSoul")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateEvilworks")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateRefinery")->removeAllEvents();
		_uibuttonsWindow->getChildElement("RepairBuilding")->removeAllEvents();
		_uibuttonsWindow->getChildElement("ClearTerrain")->removeAllEvents();
		
		_uibuttonsWindow->destroy();

		_placeableEntity = nullptr;
	}

	void SideBarUI::activate()
	{
		// Activamos la interfaz de usuario
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uibuttonsWindow);
		_uibuttonsWindow->setVisible(true);
		_uibuttonsWindow->activate();
	}

	void SideBarUI::deactivate()
	{
		// Desactivamos la ventana de UiBars.
		_uibuttonsWindow->deactivate();
		_uibuttonsWindow->setVisible(false);
	}

	Logic::CEntity* getTileEntityFromRaycast(){

		Graphics::CCamera* mCamera = Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera();

		float width = (float)mCamera->getViewportWidth();
		float height = (float)mCamera->getViewportHeight();

		CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
		CEGUI::Vector2f mousePos =
			context.getMouseCursor().getPosition();

		Ogre::Ray mouseRay =
			mCamera->getCameraToViewportRay(
			mousePos.d_x / width,
			mousePos.d_y / height);

		Logic::CEntity* entity = Physics::CServer::getSingletonPtr()->raycastClosest(mouseRay, 1000, 1);

		return entity;
	}

	void SideBarUI::tick(unsigned int msecs)
	{
		if (_placeableEntity){

			Logic::CEntity* entity = getTileEntityFromRaycast();

			if (entity)
			{
				if (_roadInConstruction == 2)
				{
					Logic::Tile* to = Logic::CTileManager::getSingletonPtr()->getNearestTile(entity->getPosition());
					std::vector<Logic::Tile*>* path= AI::CServer::getSingletonPtr()->getSoulPathAStarRoute(_originRoadTile, to);

					if (path)
					{
						Logic::CBuildingManager::getSingletonPtr()->floatPlaceableTo(_placeableEntity, entity->getPosition());
						if (_placeableRoadSize > 0){
							for (int i = 0; i < _placeableRoadSize; ++i){
								if (_placeableRoad[i])
									Logic::CBuildingManager::getSingletonPtr()->destroyPlaceable(_placeableRoad[i]);
							}
							_placeableRoadSize = 0;
							free(_placeableRoad);
							_placeableEntity = nullptr;
						}

						int j = 0;

						//borrarņapa
						if (path->size() > 1){
							path->erase(path->cbegin());
						}

						//finborrarņapa
						_placeableRoad = new Logic::CEntity*[path->size()];
						_placeableRoadSize = path->size();
						for (auto it = path->cbegin(); it != path->cend(); ++it)
						{
							Logic::Tile* tile = (*it);
							_placeableRoad[j++] = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "SoulPath", tile->getLogicPosition(), false);
						}
						_placeableEntity = _placeableRoad[0];
					}
				}
				else
				{
					Logic::CBuildingManager::getSingletonPtr()->floatPlaceableTo(_placeableEntity, entity->getPosition());
					if (_roadInConstruction == 1)
						_originRoadTile = Logic::CTileManager::getSingletonPtr()->getNearestTile(entity->getPosition());
				}
			}
		}
	}

	bool SideBarUI::createFurnaceReleased(const CEGUI::EventArgs& e)
	{
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "Furnace", Vector3(0, 0, 0), true);
		return (_placeableEntity != nullptr);
	} 

	bool SideBarUI::createRoadReleased(const CEGUI::EventArgs& e)
	{
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "SoulPath", Vector3(0, 0, 0), false);
		if (_placeableEntity){
			_placeableRoad = new Logic::CEntity*[1];
			_placeableRoadSize = 1;
			_placeableRoad[0] = _placeableEntity;
			_roadInConstruction = 1;
		}
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::createResource1BuildingReleased(const CEGUI::EventArgs& e)
	{
		printf("Create Resource1\n");
		return true;
	}

	bool SideBarUI::createResource2BuildingReleased(const CEGUI::EventArgs& e)
	{
		printf("Create Resource2\n");
		return true;
	}


	bool SideBarUI::createSoulReleased(const CEGUI::EventArgs& e)
	{
		Logic::HellQuartersActionMessage m(1);
		m._type = Logic::MessageType::SEND_SOUL_WORK;
		Logic::CPlaceable* hellQuarters = Logic::CBuildingManager::getSingletonPtr()->findBuilding(Logic::BuildingType::HellQuarters);
		
		m.Dispatch(*hellQuarters->getEntity());
		
		return true;
	}


	bool SideBarUI::moveSoulReleased(const CEGUI::EventArgs& e)
	{
		printf("Move Soul\n");
		return true;
	}


	bool SideBarUI::createEvilworksReleased(const CEGUI::EventArgs& e)
	{
		printf("Create Evilworks\n");
		return true;
	}


	bool SideBarUI::createRefineryReleased(const CEGUI::EventArgs& e)
	{
		printf("Create Refinery\n");
		return true;
	}


	bool SideBarUI::repairBuildingReleased(const CEGUI::EventArgs& e)
	{
		printf("Repair Building\n");
		return true;
	}


	bool SideBarUI::clearTerrainReleased(const CEGUI::EventArgs& e)
	{
		printf("Clear Terrain\n");
		return true;
	}


	void SideBarUI::placeBuildingInConstruction()
	{
		if (_placeableEntity)
		{
			Logic::CEntity* entity = getTileEntityFromRaycast();
			if (entity){
				switch (_roadInConstruction)
				{

				case 0:
				{
					if (Logic::CBuildingManager::getSingletonPtr()->placePlaceable(_placeableEntity))
						_placeableEntity = nullptr;		
					break;

				}
				case 1:
				{
					_roadInConstruction = 2;
					break;
				}
				case 2:
				{
					for (int i = 0; i < _placeableRoadSize; ++i)
						Logic::CBuildingManager::getSingletonPtr()->placePlaceable(_placeableRoad[i]);
					free(_placeableRoad);
					_placeableRoad = nullptr;
					_placeableEntity = nullptr;
					_roadInConstruction = 0;
					_placeableRoadSize = 0;
					break;
				}
				}
			}
		}
	}
}