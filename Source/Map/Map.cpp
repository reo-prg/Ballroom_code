#include "Map.h"
#include <EffekseerForDXLib.h>
#include "MapCollider.h"

Map::Map():model_(-1)
{
}

Map::~Map()
{
	MV1TerminateCollInfo(model_);
	MV1DeleteModel(model_);
}

void Map::LoadMap(const std::string& filepath)
{
	model_ = MV1LoadModel(filepath.c_str());
	MV1SetRotationXYZ(model_, { 0.0f, DX_PI_F, 0.0f });
	int c = MV1GetMaterialNum(model_);

	MV1SetupCollInfo(model_);
	for (int i = 0; i < c; i++)
	{
		MV1SetMaterialAmbColor(model_, i, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

void Map::Draw(void)
{
	MV1DrawModel(model_);
}
