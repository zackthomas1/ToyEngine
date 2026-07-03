#pragma once
#include "../core.h"
#include <typeindex>

#include "ToyEngine/services/window.h"
#include "ToyEngine/services/time_step.h"
#include "ToyEngine/services/input_poll.h"
#include "ToyEngine/services/resource_manager.h"

namespace ToyEngine
{
  class ServiceContainer
  {
  public:
    /// @brief Default constructor for the ServiceContainer class.
    ServiceContainer() = default;

    /// @brief Registers a service implementation for a given interface type.
    /// @tparam TInterface: The interface type of the service to register.
    /// @tparam TImpl: The implementation type of the service. Defaults to TInterface.
    /// @tparam Args: The types of the arguments to pass to the service constructor.
    /// @param args: The arguments to pass to the service constructor.
    /// @return True if the service was registered successfully, false otherwise.
    template<typename TInterface, typename TImpl = TInterface, typename... Args>
    bool Register(Args&&... args)
    {
      // Ensure that TImpl is derived from or the same as TInterface
      static_assert(std::is_base_of_v<TInterface, TImpl> || std::is_same_v<TInterface, TImpl>, "TImpl must be derived from or be the same as TInterface");

      // Check if the service is already registered and log a warning if it is being replaced.
      if (Has<TInterface>()) {
        TY_CORE_WARN("Service {} already registered, replacing", typeid(TInterface).name());
      }

      // Store the service instance in the services_ map using its type index as the key.
      services_[std::type_index(typeid(TInterface))] = MakeScope<ServiceHolder<TImpl>>(std::forward<Args>(args)...);
      TY_CORE_INFO("Registered service: {}", typeid(TInterface).name());
      return true;
    }

    /// @brief Checks if a service of the given interface type is registered.
    /// @tparam TInterface: The interface type of the service to check.
    /// @return True if the service is registered, false otherwise.
    template<typename TInterface>
    bool Has() const
    {
      // Check if the service is registered by looking it up in the services_ map using its type index.
      return services_.find(std::type_index(typeid(TInterface))) != services_.end();
    }

    /// @brief Retrieves a reference to the registered service of the given interface type.
    /// @tparam TInterface: The interface type of the service to retrieve.
    /// @return Reference to the registered service.
    template<typename TInterface>
    TInterface& Get()
    {
      auto it = services_.find(std::type_index(typeid(TInterface))); 
      TY_CORE_ASSERT(it != services_.end(), "Requested service not registered")
      return *static_cast<TInterface*>(it->second->GetRaw());
    }
  private:
    /// @brief Abstract base class for holding service instances.
    /// This class provides a common interface for retrieving the raw pointer to the service instance.
    struct IServiceHolder
    {
      virtual ~IServiceHolder() = default;
      virtual void* GetRaw() = 0;
    };
  
    /// @brief Template class for holding a specific service implementation instance.
    /// @tparam TImpl: The type of the service implementation to hold.
    template<typename TImpl>
    struct ServiceHolder : IServiceHolder
    {
      /// @brief Constructs a ServiceHolder with the given arguments to initialize the service instance.
      /// @tparam Args: The types of the arguments to pass to the service constructor.
      /// @param args: The arguments to pass to the service constructor.
      template<typename... Args>
      ServiceHolder(Args&&... args)
        : instance(std::forward<Args>(args)...) {}

      /// @brief Retrieves a raw pointer to the held service instance.
      /// @return Pointer to the held service instance.
      virtual void* GetRaw() override { return &instance; }

      /* The instance of the service implementation. */
      TImpl instance;
    };

    /* Map storing the registered services, keyed by their type index. */
    std::unordered_map<std::type_index, Scope<IServiceHolder>> services_;
  };
}
