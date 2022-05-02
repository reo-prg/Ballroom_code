#pragma once
#include <memory>
#include <vector>

class ThrowObject;
class ThrowObjManager
{
public:
	ThrowObjManager();
	~ThrowObjManager();

	void Update(float delta);
	void Draw(void);

	void Add(ThrowObject* object);
	void DrawSimpleShadow(int stagemodel);
private:
	std::vector<std::unique_ptr<ThrowObject>> object_;
};

