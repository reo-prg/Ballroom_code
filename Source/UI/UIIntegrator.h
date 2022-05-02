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
	/// UI�̎擾
	/// </summary>
	/// <param name="objectName">UI�̖��O</param>
	/// <returns>UI�̃A�h���X</returns>
	UIBase* GetUIRef(const std::string& objectName);

	/// <summary>
	/// UI�̒ǉ�
	/// </summary>
	/// <param name="objectName">UI�̖��O</param>
	/// <param name="ui">�ǉ�������UI</param>
	void AddUI(const std::string& objectName, UIBase* ui);

	/// <summary>
	/// UI�̍X�V
	/// </summary>
	/// <param name="delta">�o�ߎ���</param>
	virtual void Update(float delta);

	/// <summary>
	/// UI�̕`��
	/// </summary>
	virtual void Draw(void);

	/// <summary>
	/// �����̐ݒ�
	/// </summary>
	/// <param name="visibility">�`�悷�邩�ǂ���</param>
	void SetVisible(bool visibility);
protected:
	/// <summary>
	/// ����̖��O��UI�����̌^�ɖ߂��Ď擾
	/// </summary>
	/// <typeparam name="Ty">����UI�̌^</typeparam>
	/// <param name="objectName">UI�̖��O</param>
	/// <returns>UI�̃A�h���X</returns>
	template<typename Ty>
	Ty* GetUIRefWithDownCast(const std::string& objectName);
	
	// �f�[�^����UI�̍쐬
	void CreateImageUI(rapidxml::xml_node<>* node);
	void CreateTextUI(rapidxml::xml_node<>* node);
	void CreateBarUI(rapidxml::xml_node<>* node);

	// �f�[�^������ׂ�UI�̍쐬
	void CreateTiledImageUI(rapidxml::xml_node<>* node, float x, float y, int index);
	void CreateTiledTextUI(rapidxml::xml_node<>* node, float x, float y, int index);
	void CreateTiledBarUI(rapidxml::xml_node<>* node, float x, float y, int index);

	rapidxml::file<>* file_;
	rapidxml::xml_document<>* doc_;

	std::string filepath_;

	/// <summary>
	/// �����Ă���UI�̃��X�g
	/// </summary>
	std::unordered_map<std::string, std::unique_ptr<UIBase>> uiList_;

	void InitializeFunc(void);

	/// <summary>
	/// UI�̔z�u�̓ǂݍ���
	/// </summary>
	/// <param name="filepath">�t�@�C���p�X</param>
	void LoadUIPlacement(const std::string& filepath);

	/// <summary>
	/// UI�̎�ނ̃`�F�b�N
	/// </summary>
	/// <param name="node">module�m�[�h</param>
	void CheckUIType(rapidxml::xml_node<>* node);

	/// <summary>
	/// ����UI�̍쐬
	/// </summary>
	/// <param name="node">tile�m�[�h</param>
	void CreateTiledUI(rapidxml::xml_node<>* node);

	/// <summary>
	/// ����UI�̍쐬
	/// </summary>
	/// <param name="tilenode">tile�m�[�h</param>
	/// <param name="modulenode">module�m�[�h</param>
	void CheckTiledUIType(rapidxml::xml_node<>* tilenode, rapidxml::xml_node<>* modulenode);

	// UI�쐬�̊֐��|�C���^��UI�̖��O�̕R�Â�
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
