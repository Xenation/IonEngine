#pragma once
class Entity;
class Transform;
class Component {
public:
	Entity*const entity;
	Transform* transform;

	Component() = delete;
	Component(const Component&) = delete;
	Component(Entity* entity);
	Component(Entity* entity, Transform* transform);
	~Component();

	virtual void onStart() {};
	virtual void onEnable() {};
	virtual void onUpdate() {};
	virtual void onDisable() {};
	virtual void onDestroy() {};

	void enable();
	void disable();
	inline bool isEnabled() { return enabled; }
private:
	bool enabled = true;
};

