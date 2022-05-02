#include "ImageHandleManager.h"
#include <EffekseerForDXLib.h>

ImageHandleManager* ImageHandleManager::instance_ = nullptr;

void ImageHandleManager::Create(void)
{
	if (instance_) { return; }
	instance_ = new ImageHandleManager();
}

ImageHandleManager& ImageHandleManager::GetInstance(void)
{
	return *instance_;
}

void ImageHandleManager::Terminate(void)
{
	delete instance_;
}

void ImageHandleManager::Load(const std::string& filepath)
{
	if (handles_.contains(filepath)) { return; }

	int h = LoadGraph(filepath.c_str());
	handles_.emplace(filepath, std::initializer_list<int>{ h });
}

void ImageHandleManager::Load(const std::string& key, const std::string& filepath)
{
	if (handles_.contains(key)) { return; }

	int h = LoadGraph(filepath.c_str());
	handles_.emplace(key, std::initializer_list<int>{ h });
}

void ImageHandleManager::Load(const std::string& key, const std::string& filepath, int div_x, int div_y, int size_x, int size_y)
{
	if (handles_.contains(key)) { return; }

	std::vector<int> hd;
	hd.resize(div_x * div_y);
	LoadDivGraph(filepath.c_str(), div_x * div_y, div_x, div_y, size_x, size_y, hd.data(), true);

	handles_.emplace(key, std::move(hd));
}

void ImageHandleManager::Register(const std::string& key, const std::vector<int>& handles)
{
	if (handles_.contains(key)) { return; }
	handles_.emplace(key, handles);
}

std::vector<int> ImageHandleManager::Get(const std::string& key)
{
	if (!handles_.contains(key))
	{
		return std::initializer_list<int>{ -1 };
	}
	return handles_.at(key);
}

ImageHandleManager::ImageHandleManager()
{
}

ImageHandleManager::~ImageHandleManager()
{
	for (auto& hm : handles_)
	{
		for (auto& h : hm.second)
		{
			DeleteGraph(h);
		}
	}
	handles_.clear();
}
