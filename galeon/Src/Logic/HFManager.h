#ifndef HFMANAGER_H_
#define HFMANAGER_H_

namespace Logic
{
	class HFManager
	{
	protected:
		static HFManager _instance;
		float _hadesFavor;

	private:
		HFManager() : _hadesFavor(0.0f)
		{}

	public:
		static HFManager& getSingleton()
		{ return _instance; }

		static HFManager* getSingletonPtr()
		{ return &_instance; }

		float getHadesFavor() const { return _hadesFavor; }

		/** Modifica el favor de Hades en la cantidad (positiva o negativa) dada */
		void changeHadesFavor(const float hadesFavor);
	};
}

#endif //HFMANAGER_H_