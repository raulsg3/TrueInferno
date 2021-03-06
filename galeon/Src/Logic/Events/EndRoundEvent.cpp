//---------------------------------------------------------------------------
// CEndRoundEvent.cpp
//---------------------------------------------------------------------------

/**
@file CEndRoundEvent.cpp

Implementaci�n de un evento de tipo INFO que muestra al jugador
el desenlace de la ronda (victoria o derrota).

@see Logic::CEvent

@author �lvaro Valera
@date Mayo, 2016
*/

#include "EndRoundEvent.h"

#include "BaseSubsystems/ScriptManager.h"

#include "GUI/Server.h"
#include "GUI/UIManager.h"
#include "GUI/RankUI.h"

namespace Logic {
	std::string CEndRoundEvent::getGUIImageName() const {
		return _eventImage;
	}

	std::string CEndRoundEvent::getGUITitle() const {
		return _eventTitle;
	}

	std::string CEndRoundEvent::getGUIText() const {
		return _eventText;
	}

	std::string CEndRoundEvent::getGUIResumeText() const {
		return "";
	}

	//--------------------------------------------------------

	void CEndRoundEvent::execute()
	{
	} // execute
	
	void CEndRoundEvent::initGUIConstants()
	{
		if(_victory) {
			_eventImage = "EventVictory";
			_eventTitle = "NEXT ROUND!";
			_eventText = "YES! I KNEW you had potential. HAH! Look at your rivals' faces. Hades Favor increased in 66.666! Now go into the next round and give them Hell!";
			GUI::CServer::getSingletonPtr()->getUIManager()->getRankUI()->round++;
			GUI::CServer::getSingletonPtr()->getUIManager()->getRankUI()->start = true;
		}

		else {
			_eventImage = "EventDefeat";
			_eventTitle = "ELIMINATED!";
			_eventText = "Sad news: you failed. You�re done. Out. FIRED. And I thought you had potential... I hate being wrong. I really do. Now get the Hell out of my Hell before I throw you into a furnace.";
			_eventTextResume = "";
		}
	}

} // namespace Logic
