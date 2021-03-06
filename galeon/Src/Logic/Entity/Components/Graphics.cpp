/**
@file Graphics.cpp

Contiene la implementación del componente que controla la representación
gráfica de la entidad.
 
@see Logic::CGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "Graphics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Message.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/ParticleType.h"
#include "Map/MapEntity.h"
#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/StaticEntity.h"

namespace Logic 
{
	RTTI_ROOT_IMPL(CGraphics);
	IMP_FACTORY(CGraphics);
	
	//---------------------------------------------------------

	CGraphics::~CGraphics() 
	{
		if(_graphicsEntity)
		{
			_scene->removeEntity(_graphicsEntity);
			delete _graphicsEntity;
			_graphicsEntity = 0;
		}

		_currentParticles.clear();
	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CGraphics::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		_scene = _entity->getMap()->getScene();

		if (entityInfo->hasAttribute("model"))
			_model = entityInfo->getStringAttribute("model");

		_graphicsEntity = createGraphicsEntity(entityInfo);

		if (!_graphicsEntity)
			return false;

		if (entityInfo->hasAttribute("visible")) {
			bool visible = entityInfo->getBoolAttribute("visible");
			_graphicsEntity->setVisible(visible);
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	Graphics::CEntity* CGraphics::createGraphicsEntity(const Map::CEntity *entityInfo)
	{
		bool isStatic = false;
		if(entityInfo->hasAttribute("static"))
			isStatic = entityInfo->getBoolAttribute("static");

		if(isStatic)
		{
			_graphicsEntity = new Graphics::CStaticEntity(_entity->getName(), _model, _entity->getMeshDimensions());
			if(!_scene->addStaticEntity(static_cast<Graphics::CStaticEntity*>(_graphicsEntity)))
				return nullptr;
		}
		else
		{
			_graphicsEntity = new Graphics::CEntity(_entity->getName(),_model, _entity->getMeshDimensions());
			if(!_scene->addEntity(_graphicsEntity))
				return nullptr;
		}

		_graphicsEntity->setTransform(_entity->getTransform());

		if (entityInfo->hasAttribute("material"))
		{
			std::string materialName = entityInfo->getStringAttribute("material");
			_graphicsEntity->setMaterialName(materialName);
		}

		if (entityInfo->hasAttribute("castShadows"))
			_graphicsEntity->setCastShadows(entityInfo->getBoolAttribute("castShadows"));
		else
			_graphicsEntity->setCastShadows(false);

		return _graphicsEntity;

	} // createGraphicsEntity

	//---------------------------------------------------------
	
	Graphics::CEntity* CGraphics::getGraphicsEntity()
	{
		return _graphicsEntity;

	} // getGraphicsEntity

	//---------------------------------------------------------

	void CGraphics::setMaterialName(const std::string &materialName)
	{
		_graphicsEntity->setMaterialName(materialName);

	} // setMaterialName

	//---------------------------------------------------------

	void CGraphics::setCustomParameter(int index, Vector4 param)
	{
		_graphicsEntity->setCustomParameter(index, param);

	} // setCustomParameter

	//---------------------------------------------------------
	
	bool CGraphics::HandleMessage(const TransformMessage& m)
	{
		_graphicsEntity->setTransform(m._transform);
		return true;

	} // SET_TRANSFORM

	//---------------------------------------------------------
	
	bool CGraphics::HandleMessage(const ColorMessage& m)
	{
		_graphicsEntity->setDiffuseColor(m._rgb);
		return true;

	} // SET_COLOR

	//---------------------------------------------------------

	bool CGraphics::HandleMessage(const MaterialMessage& m)
	{
		_graphicsEntity->setMaterialName(m._name);
		return true;

	} // SET_MATERIAL_NAME

	//---------------------------------------------------------

	bool CGraphics::HandleMessage(const ToggleMessage& m)
	{
		// Sólo atendemos mensajes de información de cambio
		if (m._type != MessageType::TOGGLE_INFO)
			return false;

		// Oscurecemos o aclaramos el color dependiendo de si deshabilitan o habilitan
		//if (m._enabled)
		//	_graphicsEntity->makeClearerColor();
		//else
		//	_graphicsEntity->makeDarkerColor();
		
		return true;
	}

	bool CGraphics::HandleMessage(const ParticleMessage &m)
	{
		switch (m._type)
		{
		case MessageType::PARTICLE_START:
			startParticles(m._particleType, m._duration);
			return true;

		case MessageType::PARTICLE_STOP:
			stopParticles(m._particleType);
			return true;

		default:
			assert(false && "Unimplemented type for ParticleMessage");
			return false;
		}
	}

	void CGraphics::tick(unsigned int msecs)
	{
		// Paramos las partículas cuya duración haya finalizado
		for (auto it = _currentParticles.begin(); it != _currentParticles.end(); ++it)
		{
			// Ignoramos los infinitos (duración=0) o los ya parados (duración<0)
			int duration = it->second;
			if (duration <= 0)
				continue;

			if ((_currentParticles[it->first] -= msecs) <= 0){
				stopParticles(it->first);
			}
		}
	}

	void CGraphics::startParticles(ParticleType particleType, int duration)
	{
		// Añadimos (o rearrancamos) las partículas
		_graphicsEntity->startParticles(particleType);

		// Refrescamos la duración
		_currentParticles[particleType] = duration;
	}

	void CGraphics::stopParticles(ParticleType particleType)
	{
		// Detenemos las partículas
		_graphicsEntity->stopParticles(particleType);

		// Anulamos su duración
		_currentParticles[particleType] = -1;
	}

} // namespace Logic

