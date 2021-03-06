#include "SideBarUI.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/CEGUI.h>

#include "GUI/Server.h"
#include "UIManager.h"

#include "Map/MapParser.h"
#include "Map/MapEntity.h"

#include "Logic/Events/EventManager.h"
#include "Logic/Events/Event.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Placeable.h"
#include "Logic/Entity/Components/BuildingSelection.h"
#include "Logic/BuildingManager.h"
#include "Logic/Server.h"
#include "Logic/TutorialManager.h"
#include "Logic/Entity/Components/Tile.h"
#include "Logic/Maps/Managers/TileManager.h"
#include "Logic/Maps/Managers/WorkManager.h"

#include "Graphics/Scene.h"
#include "Graphics/Server.h"
#include "Graphics/Camera.h"

#include "Audio/Server.h"

#include <OgreCamera.h>
#include <OgreViewport.h>

#include "Physics/Server.h"
#include "Logic/Entity/Message.h"
#include "AI/Server.h"

#include "Logic\Entity\IconType.h"

using namespace SideBar;

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
		_uibuttonsWindow->setRiseOnClickEnabled(false);

		std::string roadButton = buildingButtonsNamesMap.at(BuildingButton::ROAD);
		_uibuttonsWindow->getChildElement(roadButton)->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createRoadReleased, this));
		_uibuttonsWindow->getChildElement(roadButton)->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameEnter, this));
		_uibuttonsWindow->getChildElement(roadButton)->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameExit, this));

		std::string mineButton = buildingButtonsNamesMap.at(BuildingButton::MINE);
		_uibuttonsWindow->getChildElement(mineButton)->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createMineReleased, this));
		_uibuttonsWindow->getChildElement(mineButton)->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameEnter, this));
		_uibuttonsWindow->getChildElement(mineButton)->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameExit, this));

		std::string gasPlantButton = buildingButtonsNamesMap.at(BuildingButton::GAS_PLANT);
		_uibuttonsWindow->getChildElement(gasPlantButton)->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createGasPlantReleased, this));
		_uibuttonsWindow->getChildElement(gasPlantButton)->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameEnter, this));
		_uibuttonsWindow->getChildElement(gasPlantButton)->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameExit, this));

		std::string furnaceButton = buildingButtonsNamesMap.at(BuildingButton::FURNACE);
		_uibuttonsWindow->getChildElement(furnaceButton)->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createFurnaceReleased, this));
		_uibuttonsWindow->getChildElement(furnaceButton)->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameEnter, this));
		_uibuttonsWindow->getChildElement(furnaceButton)->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameExit, this));

		std::string evilworksButton = buildingButtonsNamesMap.at(BuildingButton::EVILWORKS);
		_uibuttonsWindow->getChildElement(evilworksButton)->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createEvilworksReleased, this));
		_uibuttonsWindow->getChildElement(evilworksButton)->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameEnter, this));
		_uibuttonsWindow->getChildElement(evilworksButton)->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameExit, this));

		std::string refineryButton = buildingButtonsNamesMap.at(BuildingButton::REFINERY);
		_uibuttonsWindow->getChildElement(refineryButton)->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createRefineryReleased, this));
		_uibuttonsWindow->getChildElement(refineryButton)->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameEnter, this));
		_uibuttonsWindow->getChildElement(refineryButton)->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameExit, this));

		std::string powerGeneratorButton = buildingButtonsNamesMap.at(BuildingButton::POWER_GENERATOR);
		_uibuttonsWindow->getChildElement(powerGeneratorButton)->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createPowerGeneratorReleased, this));
		_uibuttonsWindow->getChildElement(powerGeneratorButton)->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameEnter, this));
		_uibuttonsWindow->getChildElement(powerGeneratorButton)->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameExit, this));

		std::string warehouseButton = buildingButtonsNamesMap.at(BuildingButton::WAREHOUSE);
		_uibuttonsWindow->getChildElement(warehouseButton)->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createWarehouseReleased, this));
		_uibuttonsWindow->getChildElement(warehouseButton)->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameEnter, this));
		_uibuttonsWindow->getChildElement(warehouseButton)->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameExit, this));

		std::string clearTerrainButton = buildingButtonsNamesMap.at(BuildingButton::CLEAR_TERRAIN);
		_uibuttonsWindow->getChildElement(clearTerrainButton)->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::clearTerrainReleased, this));
		_uibuttonsWindow->getChildElement(clearTerrainButton)->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameEnter, this));
		_uibuttonsWindow->getChildElement(clearTerrainButton)->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea, CEGUI::SubscriberSlot(&SideBarUI::buttonFrameExit, this));

		//_uibuttonsWindow->getChildElement("RepairBuilding")->subscribeEvent(CEGUI::PushButton::EventClicked,
		//	CEGUI::SubscriberSlot(&SideBarUI::repairBuildingReleased, this));

		//_uibuttonsWindow->getChildElement("CreateResearchLab")->subscribeEvent(CEGUI::PushButton::EventClicked,
		//	CEGUI::SubscriberSlot(&SideBarUI::createResearchLabReleased, this));
	}

	void SideBarUI::release()
	{
		// Remove all events to avoid memory leaks
		_uibuttonsWindow->getChildElement("CreateRoad")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateMine")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateGasPlant")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateFurnace")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateEvilworks")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateRefinery")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreatePowerGenerator")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateWarehouse")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateClearTerrain")->removeAllEvents();
		//_uibuttonsWindow->getChildElement("RepairBuilding")->removeAllEvents();
		//_uibuttonsWindow->getChildElement("CreateResearchLab")->removeAllEvents();

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

	// TODO Usar funci�n id�ntica de Physics::Server
	Logic::CEntity* getEntityFromRaycast()
	{
		Graphics::CCamera* mCamera = Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera();

		float width = (float)mCamera->getViewportWidth();
		float height = (float)mCamera->getViewportHeight();

		CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
		CEGUI::Vector2f mousePos = context.getMouseCursor().getPosition();

		Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.d_x / width, mousePos.d_y / height);
		Logic::CEntity* entity = Physics::CServer::getSingletonPtr()->raycastClosest(mouseRay, 1000);

		return entity;
	}

	// TODO Usar funci�n id�ntica de Physics::Server
	Logic::CEntity* getEntityFromRaycastToGroup(int collisiongroup)
	{
		Graphics::CCamera* mCamera = Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera();

		float width = (float)mCamera->getViewportWidth();
		float height = (float)mCamera->getViewportHeight();

		CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
		CEGUI::Vector2f mousePos = context.getMouseCursor().getPosition();

		Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.d_x / width, mousePos.d_y / height);
		Logic::CEntity* entity = Physics::CServer::getSingletonPtr()->raycastClosest(mouseRay, 1000, collisiongroup);

		return entity;
	}

	void SideBarUI::tick(unsigned int msecs)
	{
		//TODO El resize en CEGUI tiene algun fallo con los botones...
		//Graphics::CCamera* mCamera = Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera();
		//float width = (float)mCamera->getViewportWidth();
		//float height = (float)mCamera->getViewportHeight();
		//printf("width %f height %f\n", width, height);
		//CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(width,height));

		using namespace Logic;

		Logic::CTimeManager& tm = Logic::CTimeManager::getSingleton();

		float total_seconds = tm.getRemainingRoundTime() / 1000.0f;
		unsigned int minutes = static_cast<unsigned int>(trunc(total_seconds / 60.0f));
		unsigned int seconds = static_cast<unsigned int>(((total_seconds / 60.0f) - minutes) * 60.0f);

		_uibuttonsWindow->getChild("TimeLeft")->setText(std::to_string(minutes) + ":" + ((seconds > 9) ? std::to_string(seconds) : "0" + std::to_string(seconds)));
		_redrawUICountLimit -= msecs;

		if (_redrawUICountLimit <= 0)
		{
			_redrawUICountLimit = _redrawUICountResetValue;

			if (_sidebarVisible)
			{
				for (unsigned int i = 0; i < _uibuttonsWindow->getChildCount(); ++i)
					_uibuttonsWindow->getChildAtIdx(i)->moveToFront();
			}
			else
			{
				UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();
				uiManager->getBuildingSelectionUI()->changeButtonLayout();
			}
		}

		// Blinking button
		if (_blinkingButton)
		{
			_blinkingButtonTickCount += msecs;

			if (_blinkingButtonTickCount >= _blinkingButtonTickLimit && _blinkingButtonFrameWindow != nullptr)
			{
				_blinkingButtonTickCount = 0;

				bool blinkingNormalFrame = _blinkingButtonFrameWindow->getProperty("Image") == buttonFrame;
				_blinkingButtonFrameWindow->setProperty("Image", blinkingNormalFrame ? buttonFrameWhite : buttonFrame);
			}
		}

		if (!_deactivateCursorAnimation){

			if (_dropBuilding){
				_tickCountCursorAnimationLimit -= msecs * 10;
				if (_tickCountCursorAnimationLimit <= 0){
					_tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;

					if (_cursorAnimationStatus > 14){
						_cursorAnimationStatus = 1;
						_dropBuilding = false;
						return;
					}

					CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoDropCursors/CursorDrop" + std::to_string(_cursorAnimationStatus));
					CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoDropCursors/CursorDrop" + std::to_string(_cursorAnimationStatus));
					_cursorAnimationStatus++;
				}
				return;
			}

			if (_slap){
				_tickCountCursorAnimationLimit -= msecs * 7;
				if (_tickCountCursorAnimationLimit <= 0){
					_tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;

					if (_cursorAnimationStatus > 15){
						_cursorAnimationStatus = 1;
						_slap = false;
						return;
					}

					CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoSlapCursors/CursorSlap" + std::to_string(_cursorAnimationStatus));
					CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoSlapCursors/CursorSlap" + std::to_string(_cursorAnimationStatus));
					_cursorAnimationStatus++;
				}
				return;
			}

			if (_onGoldFrame && !_clearTerrain){
				_tickCountCursorAnimationLimit -= msecs*6.5;
				if (_tickCountCursorAnimationLimit <= 0){
					_tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;

					if (_cursorAnimationStatus > 16)
						_cursorAnimationStatus = 1;

					CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoGoldCursors/CursorGold" + std::to_string(_cursorAnimationStatus));
					CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoGoldCursors/CursorGold" + std::to_string(_cursorAnimationStatus));
					_cursorAnimationStatus++;
				}
			}

			if (_clearTerrain){
				Logic::CEntity* entity = getEntityFromRaycastToGroup(3);
				if (entity){
					_firstAnimation = true;
					_tickCountCursorAnimationLimit -= msecs*1.5;
					if (_tickCountCursorAnimationLimit <= 0){
						_tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;

						if (_cursorAnimationStatus > 3)
							_cursorAnimationStatus = 1;

						CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoOtherCursors/CursorClear" + std::to_string(_cursorAnimationStatus));
						CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoOtherCursors/CursorClear" + std::to_string(_cursorAnimationStatus));
						_cursorAnimationStatus++;
					}
				}
				else
				{
					if (_firstAnimation){
						_tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;
						_cursorAnimationStatus = 1;
						CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoOtherCursors/CursorClear1");
						CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoOtherCursors/CursorClear1");
						_firstAnimation = false;
					}
				}
			}
			else if (!_placeableEntity && !_onUIScreen && !_onButtonFrame)
			{
				Logic::CEntity* entity = getEntityFromRaycastToGroup(2);
				if (entity)
				{
					_tickCountCursorAnimationLimit -= msecs * 18;
					if (_tickCountCursorAnimationLimit <= 0){
						_tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;

						if (_cursorAnimationStatus > 41)
							_cursorAnimationStatus = 1;

						CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoSelectCursors/CursorSelect" + std::to_string(_cursorAnimationStatus));
						CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoSelectCursors/CursorSelect" + std::to_string(_cursorAnimationStatus));
						_cursorAnimationStatus++;
					}

				}
				else
				{
					_tickCountCursorAnimationLimit -= msecs * 3.5;
					if (_tickCountCursorAnimationLimit <= 0){
						_tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;

						if (_cursorAnimationStatus > 8)
							_cursorAnimationStatus = 1;

						CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoIdleCursors/CursorIdle" + std::to_string(_cursorAnimationStatus));
						CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoIdleCursors/CursorIdle" + std::to_string(_cursorAnimationStatus));
						_cursorAnimationStatus++;
					}
				}
			}

		}
		else if ((_firstAnimation || _dropBuilding) && !_placeableEntity){
			if (_clearTerrain){
				CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoOtherCursors/CursorClear1");
				CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoOtherCursors/CursorClear1");
			}
			else if (!_placeableEntity && !_onUIScreen && !_onButtonFrame){
					CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoIdleCursors/CursorIdle1");
					CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoIdleCursors/CursorIdle1");
			}
			_firstAnimation = false;
			_dropBuilding = false;
		}


		if (_placeableEntity){
			_tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;
			if (_firstAnimation){
				_cursorAnimationStatus = 1;
				CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoOtherCursors/CursorBuild");
				CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoOtherCursors/CursorBuild");
				_firstAnimation = false;
			}

			Logic::CEntity* entity = getEntityFromRaycastToGroup(1);

			if (entity)
			{
				// Obtenemos la Tile sacando el componente directamente
				Logic::Tile* to = entity->getComponent<Logic::Tile>();

				if (_roadInConstruction == 2) // TODO Convendr�a usar un enum para mejorar legiblidad
				{
					_tickCountLimit-=msecs;
					if (_previousOriginRoadTile != _originRoadTile || _previousToRoadTile != to && _tickCountLimit <= 0)
					{
						path = AI::CServer::getSingletonPtr()->getSoulPathAStarRoute(_originRoadTile, to, true);
						_previousOriginRoadTile = _originRoadTile;
						_previousToRoadTile = to;
						_tickCountLimit = _tickCountResetValue;
					}
					if (path)
					{
						Logic::CBuildingManager::getSingletonPtr()->floatPlaceableTo(_placeableEntity, to->getLogicPosition(), false);
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

						//borrar�apa (�VV: en principio las rutas ya nunca van a tener el primer nodo repetido por lo que esto ya no har�a falta)
						/*if (path->size()){
							path->erase(path->cbegin());
						}*/

						//finborrar�apa
						_placeableRoad = new Logic::CEntity*[path->size()];
						_placeableRoadSize = path->size();
						for (auto it = path->cbegin(); it != path->cend(); ++it)
						{
							Logic::Tile* tile = (*it);
							_placeableRoad[j++] = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "SoulPath", tile->getLogicPosition(), true, false);
						}
						_placeableEntity = _placeableRoad[0];

						// Liberamos la ruta
						delete path;
						path = nullptr;
					}
				}
				else{
					Logic::CBuildingManager::getSingletonPtr()->floatPlaceableTo(_placeableEntity, to->getLogicPosition(), true);
				}

			}
		}
	}

	bool SideBarUI::createRoadReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction(true);
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "SoulPath", Vector3(0, 0, 0), true, true);
		if (_placeableEntity){
			_placeableRoad = new Logic::CEntity*[1];
			_placeableRoadSize = 1;
			_placeableRoad[0] = _placeableEntity;
			_roadInConstruction = 1;
		}
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::continueRoadCreation()
	{
		ClearBuildingConstruction(false);
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "SoulPath", Vector3(0, 0, 0), true, true);
		if (_placeableEntity){
			_placeableRoad = new Logic::CEntity*[1];
			_placeableRoadSize = 1;
			_placeableRoad[0] = _placeableEntity;
			_roadInConstruction = 1;
		}
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::createMineReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction(true);
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "Mine", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::createGasPlantReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction(true);
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "GasPlant", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::createFurnaceReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction(true);
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "Furnace", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::createEvilworksReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction(true);
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "EvilWorks", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::createRefineryReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction(true);
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "Refinery", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::createPowerGeneratorReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction(true);
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "PowerGenerator", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::createWarehouseReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction(true);
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "Warehouse", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::clearTerrainReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction(true);
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoOtherCursors/CursorClear1");
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoOtherCursors/CursorClear1");
		// @TODO Cambiar puntero a pala
		_clearTerrain = true;
		return true;
	}

	bool SideBarUI::continueClearTerrain()
	{
		ClearBuildingConstruction(false);
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoOtherCursors/CursorClear1");
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoOtherCursors/CursorClear1");
		// @TODO Cambiar puntero a pala
		_clearTerrain = true;
		return true;
	}

	bool SideBarUI::repairBuildingReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction(true);

		// Icono random
		Logic::CEntity* ent = (*(Logic::CBuildingManager::getSingletonPtr()->getBuildings()[Logic::BuildingType::HellQuarters]->begin()))->getEntity(); // TODO TEST pruebas
		Logic::IconMessage m(Logic::MessageType::ICON_ADD, static_cast<Logic::IconType::IconType>(Math::random(1, 10)));

		const bool result = m.Dispatch(*ent);
		assert(result && "Can't add test icon");

		printf("Repair Building\n");
		return true;
	}

	bool SideBarUI::createResearchLabReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction(true);
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "ResearchLabs", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}

	void SideBarUI::ClearBuildingConstruction(bool clearPopups){
		if (clearPopups)
			GUI::CServer::getSingletonPtr()->getUIManager()->disablePopupWindows();
		
		if (!_onUIScreen && !_onButtonFrame){
			_cursorAnimationStatus = 1;
			_tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;
			CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoIdleCursors/CursorIdle1");
			CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoIdleCursors/CursorIdle1");
		}
		_firstAnimation = true;
		_clearTerrain = false;
		_roadInConstruction = 0;
		path = nullptr;
		_previousOriginRoadTile = nullptr;
		_previousToRoadTile = nullptr;
		_tickCountLimit = _tickCountResetValue;

		if (_placeableRoadSize > 0){
			for (int i = 0; i < _placeableRoadSize; ++i){
				if (_placeableRoad[i])
					Logic::CBuildingManager::getSingletonPtr()->destroyPlaceable(_placeableRoad[i]);
			}
			_placeableRoadSize = 0;
			free(_placeableRoad);
			_placeableEntity = nullptr;
		}
		else if (_placeableEntity){
			Logic::CBuildingManager::getSingletonPtr()->destroyPlaceable(_placeableEntity);
			_placeableEntity = nullptr;
		}
	}

	bool SideBarUI::buttonFrameEnter(const CEGUI::EventArgs& e)
	{
		_onButtonFrame = true;
		_onGoldFrame = true;
		if (!_clearTerrain && !_placeableEntity){
			_cursorAnimationStatus = 1;
			_tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;
			CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoGoldCursors/CursorGold1");
			CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoGoldCursors/CursorGold1");
		}
		return true;
	}

	bool SideBarUI::buttonFrameExit(const CEGUI::EventArgs& e)
	{
		_onButtonFrame = false;
		_onGoldFrame = false;
		_firstAnimation = true;
		if (!_onUIScreen && !_clearTerrain && !_placeableEntity){
			_cursorAnimationStatus = 1;
			_tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;
			CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoIdleCursors/CursorIdle1");
			CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoIdleCursors/CursorIdle1");
		}
		return true;
	}

	//--------------------------------------------------------

	CEGUI::Window* SideBarUI::getButtonWindowFromName(std::string buttonName)
	{
		for (unsigned int i = 0; i < _uibuttonsWindow->getChildCount(); ++i)
			if (_uibuttonsWindow->getChildAtIdx(i)->getName() == buttonName)
				return _uibuttonsWindow->getChildAtIdx(i);

		return nullptr;
	}

	//--------------------------------------------------------

	void SideBarUI::buildingButtonShow(SideBar::BuildingButton button)
	{
		// Nombre del bot�n pulsado
		std::string buttonName = buildingButtonsNamesMap.at(button);

		// Bot�n pulsado
		CEGUI::Window *buttonWindow = getButtonWindowFromName(buttonName);

		if (buttonWindow == nullptr)
			return;

		// Opci�n 1: Bot�n invisible y no interactuable
		buttonWindow->setVisible(true);

		// Opci�n 2: Bot�n inactivo y eventos muteados
		// @TODO En caso de usar esta opci�n, habr�a que hacer alg�n cambio adicional para mostrar que el
		// bot�n no est� activo (se podr�a hacer un fade out y cambiar el color del icono a gris)
		//buttonWindow->setMutedState(false);
		//buttonWindow->setDisabled(false);
		//buttonWindow->setAlwaysOnTop(false);
	}

	void SideBarUI::buildingButtonHide(SideBar::BuildingButton button)
	{
		// Nombre del bot�n pulsado
		std::string buttonName = buildingButtonsNamesMap.at(button);

		// Bot�n pulsado
		CEGUI::Window *buttonWindow = getButtonWindowFromName(buttonName);

		if (buttonWindow == nullptr)
			return;

		// Opci�n 1: Bot�n invisible y no interactuable
		buttonWindow->setVisible(false);

		// Opci�n 2: Bot�n inactivo y eventos muteados
		// @TODO En caso de usar esta opci�n, habr�a que hacer alg�n cambio adicional para mostrar que el
		// bot�n no est� activo (se podr�a hacer un fade out y cambiar el color del icono a gris)
		//buttonWindow->setMutedState(true);
		//buttonWindow->setDisabled(true);
		//buttonWindow->setAlwaysOnTop(true);
	}

	//--------------------------------------------------------

	void SideBarUI::buildingButtonFadeIn(SideBar::BuildingButton button)
	{
		// Nombre del bot�n pulsado
		std::string buttonName = buildingButtonsNamesMap.at(button);

		// Bot�n pulsado
		CEGUI::Window *buttonWindow = getButtonWindowFromName(buttonName);

		if (buttonWindow != nullptr)
			buttonWindow->setAlpha(1.0f);

		// Marco del bot�n pulsado
		CEGUI::Window *buttonFrameWindow = getButtonWindowFromName(buttonName + "Frame");

		if (buttonFrameWindow != nullptr)
			buttonFrameWindow->setAlpha(1.0f);
	}

	void SideBarUI::buildingButtonFadeOut(SideBar::BuildingButton button)
	{
		// Nombre del bot�n pulsado
		std::string buttonName = buildingButtonsNamesMap.at(button);

		// Bot�n pulsado
		CEGUI::Window *buttonWindow = getButtonWindowFromName(buttonName);

		if (buttonWindow != nullptr)
			buttonWindow->setAlpha(0.5f);

		// Marco del bot�n pulsado
		CEGUI::Window *buttonFrameWindow = getButtonWindowFromName(buttonName + "Frame");

		if (buttonFrameWindow != nullptr)
			buttonFrameWindow->setAlpha(0.5f);
	}

	//--------------------------------------------------------

	void SideBarUI::buildingButtonBlinkStart(SideBar::BuildingButton button)
	{
		// Nombre del bot�n pulsado
		std::string buttonName = buildingButtonsNamesMap.at(button);

		// Marco del bot�n pulsado
		CEGUI::Window *buttonFrameWindow = getButtonWindowFromName(buttonName + "Frame");

		// Si no se encuentra...
		if (buttonFrameWindow == nullptr)
			return;

		// Activar parpadeo y guardar referencia al [marco del] bot�n. El cambio de imagen
		// peri�dico se lleva a cabo en el tick
		_blinkingButton = true;
		_blinkingButtonFrameWindow = buttonFrameWindow;
		_blinkingButtonTickCount = _blinkingButtonTickLimit;
	}

	void SideBarUI::buildingButtonBlinkStop(SideBar::BuildingButton button)
	{
		// Comprobamos que haya alg�n bot�n parpadeando
		if (!_blinkingButton)
			return;

		// Nombre del bot�n pulsado
		std::string buttonName = buildingButtonsNamesMap.at(button);

		// Marco del bot�n pulsado
		CEGUI::Window *buttonFrameWindow = getButtonWindowFromName(buttonName + "Frame");

		// Si el pulsado no es el que est� parpadeando
		if (buttonFrameWindow == nullptr || buttonFrameWindow != _blinkingButtonFrameWindow)
			return;

		// Reestablecer la imagen original del marco
		_blinkingButtonFrameWindow->setProperty("Image", buttonFrame);

		// Desactivar parpadeo y limpiar referencia al [marco del] bot�n
		_blinkingButton = false;
		_blinkingButtonFrameWindow = nullptr;
	}

	//--------------------------------------------------------

	bool checkPlaceableConsumeCost(Logic::CEntity* placeableEntity)
	{
		if (!placeableEntity){
			std::cout << "Can't check consume costs on empty placeable" << std::endl;
			return false;
		}

		Logic::GetCostPlaceableMessage m(Logic::MessageType::PLACEABLE_CONSUME_COST, placeableEntity);

		return m.Dispatch(*placeableEntity);
	}

	void SideBarUI::playerInteractionWithRightClick()
	{
		ClearBuildingConstruction(false);
	}

	void SideBarUI::playerInteractionWithLeftClick()
	{
		if (_placeableEntity)
		{
			Logic::CEntity* entity = getEntityFromRaycastToGroup(1);
			// Yo dir�a que entity != nullptr quiere decir que se est� construyendo algo, ya sea
			// edificio o carretera...
			if (entity)
			{
				switch (_roadInConstruction)
				{
					// Se supone que este caso (_roadInConstruction == 0) corresponde a
					// la construcci�n de un edificio?
					case 0:
					{
						_originRoadTile = Logic::CTileManager::getSingletonPtr()->getNearestTile(entity->getPosition());
						if (Logic::CBuildingManager::getSingletonPtr()->checkValidPlaceablePosition(_placeableEntity, _originRoadTile->getLogicPosition()))
						{
							if (checkPlaceableConsumeCost(_placeableEntity))
							{
								Logic::CBuildingManager::getSingletonPtr()->placePlaceable(_placeableEntity);
								Logic::CWorkManager::getSingletonPtr()->reassignWorkers();
								_dropBuilding = true;
								_cursorAnimationStatus = 1;
								_tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;
							}
							else
							{
								Logic::CBuildingManager::getSingletonPtr()->destroyPlaceable(_placeableEntity);
								Audio::CServer::getSingletonPtr()->playSfxSound("error");
							}

							_placeableEntity = nullptr;
							break;
						}
						else
						{
							Audio::CServer::getSingletonPtr()->playSfxSound("error");
						}
					}
					// Y estos dos �ltimos son para construcci�n de carreteras?
					case 1:
					{
						_originRoadTile = Logic::CTileManager::getSingletonPtr()->getNearestTile(entity->getPosition());
						if (Logic::CBuildingManager::getSingletonPtr()->checkValidPlaceablePosition(_placeableEntity, _originRoadTile->getLogicPosition()))
							_roadInConstruction = 2;
						else
							_originRoadTile = nullptr;
						break;
					}
					case 2:
					{
						for (int i = 0; i < _placeableRoadSize; ++i)
						{
							if (checkPlaceableConsumeCost(_placeableRoad[i]))
								Logic::CBuildingManager::getSingletonPtr()->placePlaceable(_placeableRoad[i]);
							else
								Logic::CBuildingManager::getSingletonPtr()->destroyPlaceable(_placeableRoad[i]);
						}
						free(_placeableRoad);
						_placeableRoad = nullptr;
						_placeableEntity = nullptr;
						_roadInConstruction = 0;
						_placeableRoadSize = 0;
						_originRoadTile = nullptr;
						path = nullptr;
						_previousOriginRoadTile = nullptr;
						_previousToRoadTile = nullptr;
						_tickCountLimit = _tickCountResetValue;
						_dropBuilding = true;
						_cursorAnimationStatus = 1;
						_tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;
						continueRoadCreation();
						break;
					}
				}
			}
		}
		// ...luego, por descarte, entrar� aqu� en caso de que se haya hecho click con el
		// bot�n izquierdo y no se estuviera construyendo nada
		else{
			if (_clearTerrain)
			{
				// Modo de limpieza del terreno
				Logic::CEntity* entity = getEntityFromRaycastToGroup(3);
				if (entity)
				{
					// Limpieza de obst�culo
					Logic::CBuildingManager::getSingletonPtr()->destroyPlaceable(entity);
					continueClearTerrain();
					Logic::CTutorialManager::getSingletonPtr()->obstacleRemoved();
				}
				else
					_firstAnimation = true;
			}
			else
			{
				Logic::CEntity* entity = getEntityFromRaycastToGroup(2);
				if (entity)
				{
					// Conclusi�n final: entra aqu� cuando, efectivamente, se hace click en
					// alg�n edificio construido
					GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();
					// Cambia el panel lateral y dem�s cosas relacionadas con la GUI
					uiManager->getBuildingSelectionUI()->setEventWindowVisible(true, entity);
					_uibuttonsWindow->setVisible(false);
					_sidebarVisible = false;
					_redrawUICountLimit = _redrawUICountResetValue;
					// Avisa al TutorialManager de que se ha seleccionado un edificio
					std::string buildingName = entity->getComponent<Logic::CBuildingSelection>()->getBuildingName();
					Logic::CTutorialManager::getSingletonPtr()->buildingSelected(buildingName);
				}
				else
				{
					if (!_slap){
						entity = getEntityFromRaycastToGroup(4);
						if (entity)
						{
							_slap = true;
							_cursorAnimationStatus = 1;
							_tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;
						}
					}
				}
			}		
		}
	}

	void SideBarUI::setEventWindowVisible(bool visible)
	{
		_onUIScreen = false;
		if (!_onButtonFrame && !_slap && !_placeableEntity){
			_cursorAnimationStatus = 1;
			_tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;
			CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoIdleCursors/CursorIdle1");
			CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoIdleCursors/CursorIdle1");
		}
		_uibuttonsWindow->setVisible(visible);
		_sidebarVisible = true;
	}

}