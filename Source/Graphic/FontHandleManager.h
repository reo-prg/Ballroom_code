#pragma once
#include <string>
#include <unordered_map>

#define FontMngIns FontHandleManager::GetInstance()

class FontHandleManager
{
public:
	static void Create(void);
	static FontHandleManager& GetInstance(void);
	static void Terminate(void);

	void Load(const std::string& key, const std::string& fontname, 
		int size, int thickness, int type = -1);
	void Load(const std::string& key, const std::string& fontname, const std::string& filepath,
		int size, int thickness, int type = -1);

	int Get(const std::string& key);

	void DeleteFont(const std::string& key);
private:
	FontHandleManager();
	~FontHandleManager();

	static FontHandleManager* instance_;

	std::unordered_map<std::string, int> handles_;
	std::unordered_map<std::string, std::string> filetable_;
};

