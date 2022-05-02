#include "ThrowObjManager.h"
#include "ThrowObject.h"

ThrowObjManager::ThrowObjManager()
{
}

ThrowObjManager::~ThrowObjManager()
{
}

void ThrowObjManager::Update(float delta)
{
	for (auto& obj : object_)
	{
		obj->Update(delta);
	}
}

void ThrowObjManager::Draw(void)
{
	for (auto& obj : object_)
	{
		obj->Draw();
	}
}

void ThrowObjManager::Add(ThrowObject* object)
{
	if (!object) { return; }
	object_.push_back(std::move(std::unique_ptr<ThrowObject>(object)));
}

void ThrowObjManager::DrawSimpleShadow(int stagemodel)
{
	for (auto& obj : object_)
	{
		obj->DrawSimpleShadow(stagemodel);
	}
}
