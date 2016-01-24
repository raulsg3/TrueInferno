//---------------------------------------------------------------------------
// GameOverState.cpp
//---------------------------------------------------------------------------

/**
@file GameOverState.cpp

Contiene la implementaci�n del estado de game over.

@see Application::CApplicationState
@see Application::CGameOverState

@author David Llans�
@date Agosto, 2010
*/

#include "GameOverState.h"

#include "GUI/Server.h"

#ifdef _WIN32
#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>
#else
#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/widgets/PushButton.h>
#endif

namespace Application {

	CGameOverState::~CGameOverState() 
	{
	} // ~CGameOverState

	//--------------------------------------------------------

	bool CGameOverState::init() 
	{
		CApplicationState::init();

        #ifdef _WIN32
		// Cargamos la ventana que muestra el men�
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("GameOver.layout");
		_gameOverWindow = CEGUI::WindowManager::getSingleton().getWindow("GameOver");

		CEGUI::WindowManager::getSingleton().getWindow("GameOver/Exit")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CGameOverState::exitReleased, this));

        #else
        // TODO Posible error Aquí para cuando el personaje muera.
        _gameOverWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("GameOver.layout");

        _gameOverWindow->getChildElement("Exit")->
                subscribeEvent(CEGUI::PushButton::EventClicked,
                               CEGUI::SubscriberSlot(&CGameOverState::exitReleased, this));

        #endif

		return true;

	} // init

	//--------------------------------------------------------

	void CGameOverState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CGameOverState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el men� y activamos el rat�n.
        #ifdef _WIN32
        CEGUI::System::getSingletonPtr()->setGUISheet(_gameOverWindow);
		_gameOverWindow->setVisible(true);
		_gameOverWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();

        #else
        CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_gameOverWindow);
        _gameOverWindow->setVisible(true);
        _gameOverWindow->activate();
        CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();
        #endif
	} // activate

	//--------------------------------------------------------

	void CGameOverState::deactivate() 
	{
        #ifdef _WIN32
		// Desactivamos la ventana GUI con el men� y el rat�n.
		CEGUI::MouseCursor::getSingleton().hide();
		_gameOverWindow->deactivate();
		_gameOverWindow->setVisible(false);
		
		CApplicationState::deactivate();

        #else
        CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();
        _gameOverWindow->deactivate();
        _gameOverWindow->setVisible(false);

        CApplicationState::deactivate();
        #endif
	} // deactivate

	//--------------------------------------------------------

	void CGameOverState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CGameOverState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CGameOverState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			_app->exitRequest();
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CGameOverState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CGameOverState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CGameOverState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------

	bool CGameOverState::exitReleased(const CEGUI::EventArgs& e)
	{
		_app->exitRequest();
		return true;

	} // exitReleased

} // namespace Application
