#pragma once
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include "../../RapidXML/rapidxml.hpp"
#include "../../RapidXML/rapidxml_utils.hpp"

class UIIntegrator;
typedef void (UIIntegrator::*CreateUIFuncType)(rapidxml::xml_node<>*);
typedef void (UIIntegrator::*CreateTiledUIFuncType)(rapidxml::xml_node<>*, float, float, int);

class UIBase;
#include "PrimitiveUITypeEnumerator.h"
class UIIntegrator
{
public:
	UIIntegrator();
	UIIntegrator(const std::string& filepath);
	virtual ~UIIntegrator();

	/// <summary>
	/// UIの取得
	/// </summary>
	/// <param name="objectName">UIの名前</param>
	/// <returns>UIのアドレス</returns>
	UIBase* GetUIRef(const std::string& objectName);

	/// <summary>
	/// UIの追加
	/// </summary>
	/// <param name="objectName">UIの名前</param>
	/// <param name="ui">追加したいUI</param>
	void AddUI(const std::string& objectName, UIBase* ui);

	/// <summary>
	/// UIの更新
	/// </summary>
	/// <param name="delta">経過時間</param>
	virtual void Update(float delta);

	/// <summary>
	/// UIの描画
	/// </summary>
	virtual void Draw(void);

	/// <summary>
	/// 可視性の設定
	/// </summary>
	/// <param name="visibility">描画するかどうか</param>
	void SetVisible(bool visibility);
protected:
	/// <summary>
	/// 特定の名前のUIを元の型に戻して取得
	/// </summary>
	/// <typeparam name="Ty">元のUIの型</typeparam>
	/// <param name="objectName">UIの名前</param>
	/// <returns>UIのアドレス</returns>
	template<typename Ty>
	Ty* GetUIRefWithDownCast(const std::string& objectName);
	
	// データからUIの作成
	void CreateImageUI(rapidxml::xml_node<>* node);
	void CreateTextUI(rapidxml::xml_node<>* node);
	void CreateBarUI(rapidxml::xml_node<>* node);

	// データから並べたUIの作成
	void CreateTiledImageUI(rapidxml::xml_node<>* node, float x, float y, int index);
	void CreateTiledTextUI(rapidxml::xml_node<>* node, float x, float y, int index);
	void CreateTiledBarUI(rapidxml::xml_node<>* node, float x, float y, int index);

	rapidxml::file<>* file_;
	rapidxml::xml_document<>* doc_;

	std::string filepath_;

	/// <summary>
	/// 持っているUIのリスト
	/// </summary>
	std::unordered_map<std::string, std::unique_ptr<UIBase>> uiList_;

	void InitializeFunc(void);

	/// <summary>
	/// UIの配置の読み込み
	/// </summary>
	/// <param name="filepath">ファイルパス</param>
	void LoadUIPlacement(const std::string& filepath);

	/// <summary>
	/// UIの種類のチェック
	/// </summary>
	/// <param name="node">moduleノード</param>
	void CheckUIType(rapidxml::xml_node<>* node);

	/// <summary>
	/// 並んだUIの作成
	/// </summary>
	/// <param name="node">tileノード</param>
	void CreateTiledUI(rapidxml::xml_node<>* node);

	/// <summary>
	/// 並んだUIの作成
	/// </summary>
	/// <param name="tilenode">tileノード</param>
	/// <param name="modulenode">moduleノード</param>
	void CheckTiledUIType(rapidxml::xml_node<>* tilenode, rapidxml::xml_node<>* modulenode);

	// UI作成の関数ポインタとUIの名前の紐づけ
	std::unordered_map<std::string, CreateUIFuncType> createfunc_;
	std::unordered_map<std::string, CreateTiledUIFuncType> createTiledfunc_;
};

template<typename Ty>
inline Ty* UIIntegrator::GetUIRefWithDownCast(const std::string& objectName)
{
	static_assert(std::is_base_of<UIBase, Ty>::value == true);

	if (uiList_.find(objectName) == uiList_.end())
	{
		return nullptr;
	}

	Ty* ret = dynamic_cast<Ty*>(uiList_.at(objectName).get());
	return ret;
}
