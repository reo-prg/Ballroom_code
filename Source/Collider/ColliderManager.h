#pragma once
#include <memory>
#include <string>
#include <vector>

#define ColliderMngIns ColliderManager::GetInstance()

struct Collider;
class ColliderManager
{
public:
	static void Create(void);
	static ColliderManager& GetInstance(void);
	static void Terminate(void);

	/// <summary>
	/// ����̒ǉ�
	/// </summary>
	void AddCollider(Collider* collider);

	/// <summary>
	/// ����̍폜
	/// </summary>
	void RemoveCollider(Collider* collider);

	/// <summary>
	/// ������`�F�b�N����
	/// </summary>
	void CheckCollision(void);

	/// <summary>
	/// ������`�F�b�N����
	/// </summary>
	/// <param name="collider">���ׂ�������</param>
	/// <returns>�������Ă��锻��̃��X�g</returns>
	std::vector<Collider*> CheckCollision(Collider* collider);

	/// <summary>
	/// ������`�F�b�N����
	/// </summary>
	/// <param name="collider">���ׂ�������</param>
	/// <param name="tag">���ׂ�������̃^�O</param>
	/// <returns>�������Ă��锻��̃��X�g</returns>
	std::vector<Collider*> CheckCollision(Collider* collider, const std::string& tag);

	/// <summary>
	/// ���ׂč폜����
	/// </summary>
	void Clear(void);
private:
	ColliderManager() = default;
	~ColliderManager() = default;

	static ColliderManager* instance_;

	std::vector<Collider*> colliders_;
};

