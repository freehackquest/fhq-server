#ifndef EMPLOY_LEAKS_H
#define EMPLOY_LEAKS_H

#include <employees.h>
#include <model_leak.h>

class EmployLeaks : public EmployBase {
	public:
		EmployLeaks();
		static std::string name() { return "EmployLeaks"; }
		virtual bool init();

		bool addLeak(ModelLeak* pModelLeak);
		const ModelLeak* findLeakById(int nId);
		bool removeLeak(int nId);
		bool updateLeak(ModelLeak* pModelLeak);
		nlohmann::json toJson();

	private:
		std::map<int, ModelLeak*> m_vCacheLeaks;
};

#endif // EMPLOY_LEAKS_H
