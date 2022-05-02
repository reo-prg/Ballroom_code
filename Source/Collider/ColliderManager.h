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
	/// 判定の追加
	/// </summary>
	void AddCollider(Collider* collider);

	/// <summary>
	/// 判定の削除
	/// </summary>
	void RemoveCollider(Collider* collider);

	/// <summary>
	/// 判定をチェックする
	/// </summary>
	void CheckCollision(void);

	/// <summary>
	/// 判定をチェックする
	/// </summary>
	/// <param name="collider">調べたい判定</param>
	/// <returns>当たっている判定のリスト</returns>
	std::vector<Collider*> CheckCollision(Collider* collider);

	/// <summary>
	/// 判定をチェックする
	/// </summary>
	/// <param name="collider">調べたい判定</param>
	/// <param name="tag">調べたい判定のタグ</param>
	/// <returns>当たっている判定のリスト</returns>
	std::vector<Collider*> CheckCollision(Collider* collider, const std::string& tag);

	/// <summary>
	/// すべて削除する
	/// </summary>
	void Clear(void);
private:
	ColliderManager() = default;
	~ColliderManager() = default;

	static ColliderManager* instance_;

	std::vector<Collider*> colliders_;
};

