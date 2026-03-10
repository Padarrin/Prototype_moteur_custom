#pragma once
struct MeshRenderer;

class ECS
{
private:
	std::map<int, std::vector<Component*>> m_components;
	std::vector<Entity*> m_entities;
	int _nbEntities = 0;

	std::vector<System*> m_systems;
	std::vector<Collider*> m_colliders;

	int m_score = 0;

public:
	ECS();
	~ECS();
	Entity* createEntity();
	void deleteEntity(Entity* entity);
	void addSystem(System* _system);
	void Update(float deltatime);

	void addCollider(Entity* _entity);
	Collider* GetCollider(Entity* _entity);
	void DeleteCollider(Entity* _entity);
	void CheckCollision();
	void CheckTypeMesh(MeshRenderer* _mesh, Collider* _collider);

	int GetScore() const { return m_score; }

	template <typename ComponentType>
	void addComponent(Entity* instance)
	{
		if (instance == nullptr) return;

		ComponentType* newComponent = new ComponentType;

		m_components[instance->mIndex].push_back(newComponent);
	}

	template <typename T>
	T* GetComponent(Entity& instance)
	{
		if (m_components.find(instance.mIndex) == m_components.end()) 
		{
			return nullptr;
		}

		std::vector<Component*>& entityComponents = m_components[instance.mIndex];

		for (Component* c : entityComponents)
		{
			T* found = dynamic_cast<T*>(c);
			if (found != nullptr) {
				return found; 
			}
		}
		return nullptr; 
	}

	template <typename ComponentType>
	void deleteComponent(Entity* instance)
	{
		if (instance == nullptr) return;

		auto mapIt = m_components.find(instance->mIndex);
		if (mapIt == m_components.end()) return;

		std::vector<Component*>& entityComponents = mapIt->second;

		for (auto compIt = entityComponents.begin(); compIt != entityComponents.end(); ++compIt)
		{
			ComponentType* target = dynamic_cast<ComponentType*>(*compIt);
			if (target != nullptr)
			{
				delete* compIt;
				entityComponents.erase(compIt);
				return;
			}
		}
	}

	template <typename... T>
	std::vector<Entity*> GetEntitiesWith()
	{
		std::vector<Entity*> results;

		for (Entity* entity : m_entities)
		{
			if ((HasComponent<T>(entity) && ...))
			{
				results.push_back(entity);
			}
		}

		return results;
	}

	template <typename T>
	bool HasComponent(Entity* instance)
	{
		if (m_components.find(instance->mIndex) == m_components.end())
			return false;

		std::vector<Component*>& entityComponents = m_components[instance->mIndex];

		for (Component* c : entityComponents)
		{
			if (dynamic_cast<T*>(c) != nullptr) {
				return true;
			}
		}
		return false;
	}
};
