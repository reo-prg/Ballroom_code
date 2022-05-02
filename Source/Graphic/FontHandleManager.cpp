#include "FontHandleManager.h"
#include <EffekseerForDXLib.h>

FontHandleManager* FontHandleManager::instance_ = nullptr;

void FontHandleManager::Create(void)
{
	if (instance_) { return; }
	instance_ = new FontHandleManager();
}

FontHandleManager& FontHandleManager::GetInstance(void)
{
	return *instance_;
}

void FontHandleManager::Terminate(void)
{
	delete instance_;
}

void FontHandleManager::Load(const std::string& key, const std::string& fontname, int size, int thickness, int type)
{
	if (handles_.contains(key)) { return; }
	int h = CreateFontToHandle(fontname.c_str(), size, thickness, type);
	handles_.emplace(key, h);
}

void FontHandleManager::Load(const std::string& key, const std::string& fontname, const std::string& filepath, int size, int thickness, int type)
{
	if (handles_.contains(key)) { return; }
	int result = AddFontResourceEx(filepath.c_str(), FR_PRIVATE, NULL);
	if (result == 0) { return; }
	int h = CreateFontToHandle(fontname.c_str(), size, thickness, type);
	handles_.emplace(key, h);
	filetable_.emplace(key, filepath);
}

int FontHandleManager::Get(const std::string& key)
{
	if (!handles_.contains(key))
	{
		return -1;
	}
	return handles_.at(key);
}

void FontHandleManager::DeleteFont(const std::string& key)
{
	if (!handles_.contains(key))
	{
		return;
	}
	DeleteFontToHandle(handles_.at(key));
	handles_.erase(key);
}

FontHandleManager::FontHandleManager()
{
}

FontHandleManager::~FontHandleManager()
{
	for (auto& h : handles_)
	{
		DeleteFontToHandle(h.second);
	}
	handles_.clear();

	for (auto& [key, filepath] : filetable_)
	{
		RemoveFontResourceEx(filepath.c_str(), FR_PRIVATE, NULL);
	}
}
