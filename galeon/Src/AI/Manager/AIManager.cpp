//---------------------------------------------------------------------------
// AIManager.cpp
//---------------------------------------------------------------------------

/**
@file AIManager.cpp

Contiene la implementaci�n del gestor de eventos del juego. Los eventos ser�n
creados desde un script de LUA, procesados (en caso de que sea necesario)
y encolados hasta que llegue el momento de su lanzamiento.

@see Logic::CScriptManager

@author �lvaro Valera
@date Abril, 2016
*/

#include <cassert>
#include <iostream>
#include <algorithm>
#include <climits>
#include <cmath>

#include "AIManager.h"

#include "BaseSubsystems/ScriptManager.h"
#include "Logic/Entity/Message.h"
#include "Map/MapEntity.h"
#include "Logic/Server.h"
#include "Logic/TimeManager.h"
#include "Logic\GameRuntimeContext.h"

namespace AI {

	CAIManager* CAIManager::_instance = 0;

	//--------------------------------------------------------

	CAIManager::CAIManager() : 
		_baseScorePerRound(0), _scoreMaxRelativeDeviation(0), 
		_difficulty(0), _baseDifficulty(0), _minBaseDifficulty(0), _maxBaseDifficulty(0), _numRound(1)
	{
		_instance = this;

		// Creamos el player
		_player = new CPlayer();

	} // CAIManager

	//--------------------------------------------------------

	CAIManager::~CAIManager()
	{
		assert(_instance);
		_instance = 0;

		// Borramos todos los dioses
		for (auto it = _gods.begin(); it != _gods.end(); ++it){
			delete(it->second);
			it->second = nullptr;
		}

		// Limpiamos las estructuras
		_gods.clear();
		_ranking.clear();

		// Apuntamos theBoss a null porque su memoria ya deber�a estar liberada tras vaciar _gods
		_theBoss = nullptr;

		// Liberamos el jugador
		if (_player != nullptr) {
			delete(_player);
			_player = nullptr;
		}

	} // ~CAIManager

	//--------------------------------------------------------

	bool CAIManager::Init()
	{
		assert(!_instance && "Segunda inicializaci�n de Logic::CAIManager no permitida!");

		new CAIManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CAIManager::Release()
	{
		assert(_instance && "Logic::CAIManager no est� inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	void CAIManager::spawn(const Map::CEntity *managerInfo)
	{
		_baseScorePerRound = managerInfo->getIntAttribute("baseScorePerRound");
		_scoreMaxRelativeDeviation = managerInfo->getFloatAttribute("scoreMaxRelativeDeviation");
		_minBaseDifficulty = managerInfo->getFloatAttribute("minBaseDifficulty");
		_maxBaseDifficulty = managerInfo->getFloatAttribute("maxBaseDifficulty");

		loadAIScript("AIManager.lua");

		// Asignaci�n de scores objetivo iniciales
		assignGodTargetScores(_baseScorePerRound, (int)(_baseScorePerRound * _scoreMaxRelativeDeviation));

		// Metemos en el ranking al player
		_ranking.push_back(_player);

		// Inicializaci�n de la dificultad base
		_baseDifficulty = _minBaseDifficulty;

		// Se ajusta el incremento por ronda para que la primera ronda empiece con _minBaseDifficulty y la �ltima con _maxBaseDifficulty
		_baseDifficultyIncrement = (_maxBaseDifficulty - _minBaseDifficulty) / (_gods.size() - 1);
	}
	
	//--------------------------------------------------------

	void CAIManager::tick(unsigned int msecs)
	{
		bool pause = Logic::CTimeManager::getSingletonPtr()->getPause();
		
		// Si la partida est� en pausa, evitamos que los dioses incrementen su puntuaci�n
		if (pause)
			return;

		// Tick Lua AIManager
		ScriptManager::CScriptManager::GetPtrSingleton()->executeProcedure("tickAIManager", msecs);

		// Tick gods
		for (auto it = _gods.begin(); it != _gods.end(); ++it){
			(it->second)->tick(msecs);
		}
		
	} // tick

	//--------------------------------------------------------

	bool CAIManager::loadAIScript(const std::string& filename)
	{
		if (!ScriptManager::CScriptManager::GetPtrSingleton()->loadScript(filename.c_str()))
			return false;

		return true;

	} // loadEventsScript

	//--------------------------------------------------------

	bool CAIManager::open() {
		luaRegister();

		return true;

	} // open

	//--------------------------------------------------------

	void CAIManager::close() {

	} // close

	//--------------------------------------------------------

	void CAIManager::luaRegister() {
		// AIManager.
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CAIManager>("CAIManager")
				.def("addGod", &CAIManager::addGod)
				.def("getCurrentDifficulty", &CAIManager::getCurrentDifficulty)
				.def("getGameRuntimeContext", &CAIManager::getGameRuntimeContext)
				.enum_("GameRuntimeContext")
				[
					luabind::value("RUNTIME_DEV", GameRuntimeContext::DEV),
					luabind::value("RUNTIME_GAME", GameRuntimeContext::GAME),
					luabind::value("RUNTIME_SCRIPTED_DEMO", GameRuntimeContext::SCRIPTED_DEMO)
				]
				.scope
				[
					luabind::def("getSingletonPtr", &CAIManager::getSingletonPtr)
				]
			];

	} // luaRegister

	//--------------------------------------------------------

	void CAIManager::addGod(const std::string& name, bool isBoss){
		// Creamos el nuevo dios
		CGod* newGod = new CGod(name, isBoss);
		
		// Lo insertamos en el contenedor de dioses
		_gods[name] = newGod;

		// Si es el jefe lo marcamos aparte
		if (isBoss) {
			_theBoss = newGod;
		}

		// Si no, lo a�adimos al ranking
		else { // isBoss == false
			_ranking.push_back(newGod);
		}
	}

	bool CAIManager::eliminateGod(const std::string& name){
		// Localizamos al dios
		CGod* god = _gods[name];
		
		// Chequeamos que el dios exista
		if (god == nullptr){
			assert(false && "God can't be eliminated because it's not in the game");
			return false;
		}

		// No permitimos que se elimine al jefe
		if (god->isBoss()){
			assert(false && "Can't eliminate boss god");
			return false;
		}
			
		// Notificamos al dios que est� eliminado en C++
		god->eliminate();

		// Notificamos al dios que est� eliminado en Lua
		ScriptManager::CScriptManager::GetPtrSingleton()->executeProcedure("eliminateGodAIManager", name);

		return true;
	}

	/** Funci�n para ordenar dioses en orden de score descendiente */
	struct st_godScoreCompare {
		bool operator() (IRankedCharacter* lhs, IRankedCharacter* rhs)
		{
			return lhs->getScore() > rhs->getScore();
		}
	} godScoreCompare;

	std::vector<IRankedCharacter*> CAIManager::getGodRanking() {
		// Reordenamos los dioses por puntuaci�n
		std::sort(_ranking.begin(), _ranking.end(), godScoreCompare);

		return _ranking;
	}

	CGod* CAIManager::getWorstActiveGod(){
		// Buscaremos el dios activo con peor puntuaci�n
		int worstScore = INT_MAX;
		CGod *worstGod = nullptr;

		for (auto it = _gods.cbegin(); it != _gods.cend(); ++it){
			CGod* god = (it->second);

			// Descartamos dioses no activos y al jefe
			if (god->isBoss() || god->isEliminated())
				continue;

			if (god->getScore() < worstScore){
				worstGod = god;
				worstScore = god->getScore();
			}
		}

		// Si llegamos al final es que no hab�a ning�n dios activo en el ranking
		return worstGod;
	}

	void CAIManager::startNextRound()
	{
		++_numRound;

		// No hace falta eliminar dios porque ya se ha hecho desde el GameManager

		// Asignamos nueva puntuaci�n objetivo a los dioses para las rondas a partir de la 2�
		int base;

		// Tomamos como base la puntuaci�n actual del mejor dios + un incremento creciente por ronda
		switch (Logic::CServer::getSingletonPtr()->getGameRuntimeContext()){
		case GameRuntimeContext::SCRIPTED_DEMO:
		case GameRuntimeContext::GAME:
			// 1� ronda: BASE_SCORE_PER_ROUND
			// 2� ronda: MEJOR_DIOS + 2^1 * BASE_SCORE_PER_ROUND
			// 3� ronda: MEJOR_DIOS + 2^2 * BASE_SCORE_PER_ROUND
			base = getGodRanking().front()->getScore() + pow(2, _numRound - 1) * _baseScorePerRound;
			break;
		default:
			// 1� ronda: BASE_SCORE_PER_ROUND
			// 2� ronda: MEJOR_DIOS + 2 * BASE_SCORE_PER_ROUND
			// 3� ronda: MEJOR_DIOS + 3 * BASE_SCORE_PER_ROUND
			base = getGodRanking().front()->getScore() + _numRound * _baseScorePerRound;
			break;
		}

		// Asignamos la puntuaci�n objetivo con cierta variedad relativa
		assignGodTargetScores(base, (int)(base * _scoreMaxRelativeDeviation));

		// Aumentamos la dificultad base
		_baseDifficulty += _baseDifficultyIncrement;

		//std::cout << "Base difficulty set to: " << _baseDifficulty << std::endl;
	}

	CGod* CAIManager::getGod(const std::string& name){
		return _gods[name];
	}

	bool CAIManager::HandleMessage(const Message& msg)
	{
		assert(!"Est�s enviando un Message sin tipo.");

		return false;
	}

	bool CAIManager::HandleMessage(const TransformMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const PositionMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const RotationMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const DimensionsMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const ColorMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const MaterialMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const AnimationMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const ControlMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const PhysicMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const TouchMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const DamageMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const WorkerMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const WalkSoulPathMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const HellQuartersMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const MovePlaceableMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const SoulSenderMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const SoulMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const CheckValidPositionPlaceableMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const SoulBurnMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const ResourceMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const GetCostPlaceableMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const LogisticsMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const ToggleMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const IconMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	float CAIManager::getCurrentDifficulty(){
		CGod* worstGod = getWorstActiveGod();

		// Caso extremo. La partida deber�a haber acabado antes de quedarnos sin dioses activos
		if (worstGod == nullptr)
			return _baseDifficulty;

		// Comparamos la puntuaci�n actual del jugador y la del peor dios activo
		int current = _player->getScore();
		int target = worstGod->getScore();

		// Caso base. S�lo deber�a pasar al arrancar. Devolvemos la dificultad base
		if (target == 0)
			return _baseDifficulty;

		// Obtenemos la diferencia relativa con respecto al objetivo
		float relativeDiff = (current - target) / (float) target;
			
		// Escalamos a [_baseDifficulty, 1] en funci�n de la desviaci�n relativa que aceptamos
		// Truncamos los extremos
		if (relativeDiff < -_scoreMaxRelativeDeviation)
			return _baseDifficulty;
		
		if (relativeDiff > _scoreMaxRelativeDeviation)
			return 1;

		// Escalamos el resto
		return Math::linearScale(-_scoreMaxRelativeDeviation, _scoreMaxRelativeDeviation, _baseDifficulty, 1, relativeDiff);
	}

	GameRuntimeContext CAIManager::getGameRuntimeContext(){
		return Logic::CServer::getSingletonPtr()->getGameRuntimeContext();
	}

	void CAIManager::assignGodTargetScores(int baseScore, int maxDifference){
		for (auto it = _gods.begin(); it != _gods.end(); ++it){
			int randomDifference = - maxDifference + std::rand() % (2 * maxDifference);
			it->second->setTargetScore(baseScore + randomDifference);
		}
	}

} // namespace Logic
