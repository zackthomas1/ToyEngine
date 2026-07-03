#pragma once
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include "ToyEngine/core.h"

#include "ToyEngine/model/model.h"
#include "ToyEngine/model/material.h"
#include "ToyEngine/renderer/shader.h"
#include "ToyEngine/renderer/texture_2d.h"

namespace ToyEngine {
  class ResourceManager {
  public:
    ResourceManager() = default;
    ~ResourceManager() = default;

    /// @brief Registers a new resource class with the Resouce Manager.
    /// A resouce class should only be registered once. If has been previously registered it is skipped 
    /// @tparam T: the type of the resouce class that is being registered 
    template<typename T>
    bool Register()
    {
      // Check if the Resouce class is already registered with the resource manager 
      if (IsRegistered<T>()) {
        TY_CORE_WARN("Resource class '{}' already registered with Resource Manager, skipping.", typeid(T).name());
        return false;
      }

      // Add resource class to resource manager
      resource_library_[std::type_index(typeid(T))] = MakeScope<ResourceHolder<T>>();
      TY_CORE_INFO("Registered resource class: '{}'.", typeid(T).name());
      return true;
    }

    /// @brief Checks if a resource class has already been registered with the resource manager.
    /// @tparam T: The class type of the resouce class being checked.
    /// @return True if the resource class has already been registered, False if not.
    template<typename T>
    bool IsRegistered()
    {
      return resource_library_.find(std::type_index(typeid(T))) != resource_library_.end();
    }

    /// @brief Adds a new individual resouce to the manager. For instace, a new 3D model.
    /// @tparam T: The class type of the resouce being added.
    /// @param name: The name of the resource and the key used in the resource library registry.
    /// @param resource: A shared pointer to the resource being added.
    template<typename T>
    bool Add(const std::string& name, Ref<T> resource)
    {
      if (!IsRegistered<T>()) return false;
      auto& registry = GetRegistry<T>();

      // check that resouce does not already exist in resouce manager library
      if (registry.find((name)) != registry.end()) {
        TY_CORE_WARN("Resource '{}' already exist in {} library, replace", name, typeid(T).name());
      }

      registry[name] = resource;
      TY_CORE_INFO("Add '{}' to resource manager library '{}'", name, typeid(T).name());
      return true;
    }

    /// @brief Gets a reference to the requested resource
    /// @tparam T: The class type of the resource being requested.
    /// @param name: The name of the resource being requested. This is the key used to store the resource in the registry.
    /// @return A shared pointer to the requested resource. If the resource does not exist, returns nullptr.
    template<typename T>
    Ref<T> Get(const std::string& name)
    {
      if (!IsRegistered<T>()) {
        TY_CORE_WARN("Resource class '{}' has not been registered with Resource Manager. Get method returned nullptr", typeid(T).name());
        return nullptr;
      }

      auto& registry = GetRegistry<T>();

      auto it = registry.find(name);
      if (it == registry.end()) {
        // verify that resouce exist in resouce library
        TY_CORE_WARN("Resouce '{}' does not exist in {} library.", name, typeid(T).name());
        return nullptr;
      }

      return it->second;
    }

    /// @brief Checks if a reource with a given name exists in the resource manager library. 
    /// @tparam T: The class type of the resource being checked.
    /// @param name: The name of the resource being checked. This is the key used to store the resource in the registry.
    template<typename T>
    bool Exists(const std::string& name)
    {
      if (!IsRegistered<T>()) return false;
      
      auto& registry = GetRegistry<T>();
      return registry.find(name) != registry.end();
    }

    /// @brief Removes a resource with a given name from the resource manager library.
    /// @tparam T: The class type of the resource being checked.
    /// @param name: The name of the resource being checked. This is the key used to store the resource in the registry.
    template<typename T>
    void Remove(const std::string& name)
    {
      if (!IsRegistered<T>()) {
        TY_CORE_WARN("Resource class '{}' has not been registered with Resource Manager. Remove method returned.", typeid(T).name());
        return;
      }

      auto& registry = GetRegistry<T>();

      auto it = registry.find(name);
      if (it == registry.end()) {
        // verify that resouce exist in resouce library
        TY_CORE_WARN("Resource '{}' does not exist in {} library.", name, typeid(T).name());
        return;
      }

      // remove resource from resource library registry
      registry.erase(it);
      TY_CORE_INFO("Removed resource '{}' from resource manager library {} registry.", name, typeid(T).name());
      return;
    }

  private:
    /// @brief An abstract interface used primarily as the value type in the underlying unordered_map 
    /// data struct that contains the resource_library. This is required so that the resource_library
    /// type definition does not require a templated type.
    struct IResourceHolder {
      virtual ~IResourceHolder() = default;
      virtual void* GetRaw() = 0;
    };


    /// @brief The concrete implementation that 
    /// @tparam T: The resource class type that the instantiated ResouceHolder contains in it registry
    template<typename T>
    struct ResourceHolder : IResourceHolder {
    public:
      ResourceHolder() = default;
      ~ResourceHolder() = default;
      virtual void* GetRaw() override { return &registry_; }
    private:
      /* The underlying registry (unordered_map) that stores the resources of type T */
      std::unordered_map<std::string, Ref<T>> registry_;
    };

    /// @brief Gets a reference to underlying registry (unordered_map) data structure that stores the resources.
    /// @tparam T: The resource class of the registry to return.
    /// @return A reference to underyling unordered_map for the request resource class.
    template<typename T>
    std::unordered_map<std::string, Ref<T>>& GetRegistry()
    {
      TY_CORE_ASSERT(IsRegistered<T>(), "Requested Resouce class has not been registered.");

      // Check that class has been registered with resouce manager
      auto it = resource_library_.find(std::type_index(typeid(T)));

      // Cast void* to an unordere_map reference
      std::unordered_map<std::string, Ref<T>>& registry = *static_cast<std::unordered_map<std::string, Ref<T>>*>(it->second->GetRaw());
      return registry;
    }

    /* The underlying resource library that stores the resource holders for each registered resource type. */
    std::unordered_map<std::type_index, Scope<IResourceHolder>> resource_library_;
  };
}