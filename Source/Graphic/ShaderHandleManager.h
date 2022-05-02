#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#define ShaderMngIns ShaderHandleManager::GetInstance()

class ShaderHandleManager
{
public:
	static void Create(void);
	static ShaderHandleManager& GetInstance(void);
	static void Terminate(void);

	int LoadVS(const std::string& filepath);
	int LoadVS(const std::string& key, const std::string& filepath);

	int LoadPS(const std::string& filepath);
	int LoadPS(const std::string& key, const std::string& filepath);

	void Register(const std::string& key, int handles);

	int Get(const std::string& key);
private:
	ShaderHandleManager();
	~ShaderHandleManager();

	static ShaderHandleManager* instance_;

	std::unordered_map<std::string, int> handles_;
};

