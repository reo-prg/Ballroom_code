#include "ShaderHandleManager.h"
#include <EffekseerForDXLib.h>

ShaderHandleManager* ShaderHandleManager::instance_ = nullptr;

void ShaderHandleManager::Create(void)
{
	if (instance_) { return; }
	instance_ = new ShaderHandleManager();
}

ShaderHandleManager& ShaderHandleManager::GetInstance(void)
{
	return *instance_;
}

void ShaderHandleManager::Terminate(void)
{
	delete instance_;
}

int ShaderHandleManager::LoadVS(const std::string& filepath)
{
	if (handles_.contains(filepath)) { return -1; }

	int h = LoadVertexShader(filepath.c_str());
	handles_.emplace(filepath, h);
	return h;
}

int ShaderHandleManager::LoadVS(const std::string& key, const std::string& filepath)
{
	if (handles_.contains(key)) { return -1; }

	int h = LoadVertexShader(filepath.c_str());
	handles_.emplace(key, h);
	return h;
}

int ShaderHandleManager::LoadPS(const std::string& filepath)
{
	if (handles_.contains(filepath)) { return -1; }

	int h = LoadPixelShader(filepath.c_str());
	handles_.emplace(filepath, h);
	return h;
}

int ShaderHandleManager::LoadPS(const std::string& key, const std::string& filepath)
{
	if (handles_.contains(key)) { return -1; }

	int h = LoadPixelShader(filepath.c_str());
	handles_.emplace(key, h);
	return h;
}

void ShaderHandleManager::Register(const std::string& key, int handles)
{
	if (handles_.contains(key)) { return; }
	handles_.emplace(key, handles);
}

int ShaderHandleManager::Get(const std::string& key)
{
	if (!handles_.contains(key)) { return -1; }
	return handles_.at(key);
}

ShaderHandleManager::ShaderHandleManager()
{
}

ShaderHandleManager::~ShaderHandleManager()
{
	for (auto& hm : handles_)
	{
		DeleteShader(hm.second);
	}
	handles_.clear();
}
