#pragma once
#include <string>
#include <unordered_map>

#define SubmixHandleMng SubmixHandleManager::GetInstance()

class SubmixHandleManager
{
public:
	static SubmixHandleManager& GetInstance(void)
	{
		static SubmixHandleManager ins;
		return ins;
	}

	void Add(int submixHandle, const std::string& key)
	{
		handles_.try_emplace(key, submixHandle);
	}

	int GetHandle(const std::string& key)
	{
		if (handles_.contains(key)) { return handles_.at(key); }
		return 0;
	}
private:
	std::unordered_map<std::string, int> handles_;
};

