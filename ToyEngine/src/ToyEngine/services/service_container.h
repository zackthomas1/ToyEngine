#pragma once
#include "../core.h"
#include <typeindex>

#include "ToyEngine/services/time_step.h"
#include "ToyEngine/services/input_poll.h"

namespace ToyEngine
{
	class ServiceContainer
	{
	public:
		ServiceContainer() {}

		template<typename TInterface, typename TImpl = TInterface, typename... Args>
		void Register(Args&&... args)
		{
			static_assert(std::is_base_of_v<TInterface, TImpl> || std::is_same_v<TInterface, TImpl>, "TImpl must be derived from TInterface");
			services_[std::type_index(typeid(TInterface))] = MakeScope<ServiceHolder<TImpl>>(std::forward<Args>(args)...);
		}

		template<typename TInterface>
		bool Has() const
		{
			return services_.find(std::type_index(typeid(TInterface))) != services_.end();
		}

		template<typename TInterface>
		TInterface& Get()
		{
			auto it = services_.find(std::type_index(typeid(TInterface))); 
			TY_CORE_ASSERT(it != services_.end(), "Requested service not registered")
			return *static_cast<TInterface*>(it->second->GetRaw());
		}
	private:
		struct IServiceHolder
		{
			virtual ~IServiceHolder() {}
			virtual void* GetRaw() = 0;
		};
	
		template<typename TImpl>
		struct ServiceHolder : IServiceHolder
		{
			template<typename... Args>
			ServiceHolder(Args&&... args)
				: instance(std::forward<Args>(args)...)
			{}
			virtual void* GetRaw() override { return &instance; }
			TImpl instance;
		};

		std::unordered_map<std::type_index, Scope<IServiceHolder>> services_;
	};
}
