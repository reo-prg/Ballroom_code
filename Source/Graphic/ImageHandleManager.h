#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#define ImageMngIns ImageHandleManager::GetInstance()

class ImageHandleManager
{
public:
	static void Create(void);
	static ImageHandleManager& GetInstance(void);
	static void Terminate(void);

	void Load(const std::string& filepath);
	void Load(const std::string& key, const std::string& filepath);
	void Load(const std::string& key, const std::string& filepath, 
		int div_x, int div_y, int size_x, int size_y);

	void Register(const std::string& key, const std::vector<int>& handles);

	std::vector<int> Get(const std::string& key);
private:
	ImageHandleManager();
	~ImageHandleManager();

	static ImageHandleManager* instance_;

	std::unordered_map<std::string, std::vector<int>> handles_;
};

