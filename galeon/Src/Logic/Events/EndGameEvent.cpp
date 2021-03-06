//---------------------------------------------------------------------------
// CEndGameEvent.cpp
//---------------------------------------------------------------------------

/**
@file CEndGameEvent.cpp

Implementaci�n de un evento de tipo INFO consistente �nicamente en mostrar
el panel de fin del juego (con victoria o derrota).

@see Logic::CEvent

@author Ra�l Segura
@date Marzo, 2016
*/

#include "EndGameEvent.h"

#include "BaseSubsystems/ScriptManager.h"
#include <Audio/Server.h>

namespace Logic {

	void CEndGameEvent::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CEndGameEvent, CEvent>("CEndGameEvent")
				.def(luabind::constructor<bool>())
			];
	}

	//--------------------------------------------------------

	std::string CEndGameEvent::getGUIImageName() const {
		return _eventImage;
	}

	std::string CEndGameEvent::getGUITitle() const {
		return _eventTitle;
	}

	std::string CEndGameEvent::getGUIText() const {
		return _eventText;
	}

	std::string CEndGameEvent::getGUIResumeText() const {
		return "";
	}
	
	//--------------------------------------------------------

	void CEndGameEvent::execute()
	{
		if (_victory)
			Audio::CServer::getSingletonPtr()->playSfxSound("victory");
		else
			Audio::CServer::getSingletonPtr()->playSfxSound("error");
	} // execute

	void CEndGameEvent::initGUIConstants()
	{
		if (_victory) {
			_eventImage = "EventVictory";
			_eventTitle = "VICTORY!";
			_eventText = "Yes! YOU DID IT! You ARE the WINNER! Hell is yours now, totally deserved it I should say. But do not relax yet and keep making them all suffer!";
			_eventTextResume = "";
		}

		else{
			_eventImage = "EventDefeat";
			_eventTitle = "ELIMINATED!";
			_eventText = "Sad news: you failed. You�re done. Out. FIRED. And I thought you had potential... I hate being wrong. I really do. Now get the Hell out of my Hell before I throw you into a furnace.";
			_eventTextResume = "";
		}
	}
	
} // namespace Logic
