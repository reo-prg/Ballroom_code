#pragma once
#include <memory>
#include <string>

class MapCollider;
class Map
{
public:
	Map();
	~Map();

	void LoadMap(const std::string& filepath);

	void Draw(void);

	int GetModelID(void) { return model_; }
private:
	int model_;
};

