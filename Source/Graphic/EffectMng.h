#pragma once
#include <vector>
#include <memory>
#include <map>
#include <string>
#include "../Math/VectorInterface.h"
#include "BoardPolygon.h"

#define EffectIns EffectMng::GetInstance()

struct EffectData;
class BoardPolygon;
class EffectMng
{
public:
	// �G�t�F�N�g�̍Đ�
	// key:�L�[		pos:���W
	// angle:�p�x	exrate:�g�嗦
	// totalTime:���v�̍Đ�����
	void PlayEffect(const char* key, const Vector3<float>& pos, float angle, float exrate, 
		float totalTime = 1.0, bool invert_x = false, bool invert_y = false);

	// �`��(���t���[���Ă�ł�)
	void Draw(float deltaTime, const Vector2<float>& offset);

	// �G�t�F�N�g�̓ǂݍ���
	// key:�L�[			filename:�f�ނ̃p�X
	// cnt_all:�R�}��	cnt_x:���̃R�}��		cnt_y:�c�̃R�}��
	// size_x:1�R�}�̉��T�C�Y	size_y:1�R�}�̏c�T�C�Y
	void LoadEffect(const char* key, const char* directory);

	static void Create(void)
	{
		instance_ = new EffectMng();
	}

	static EffectMng& GetInstance(void)
	{
		return *instance_;
	};

	static void Destroy(void)
	{
		delete instance_;
	}
private:
	EffectMng() = default;
	EffectMng(const EffectMng&) = delete;
	EffectMng operator=(const EffectMng&) = delete;

	std::vector<std::unique_ptr<EffectData>> playingEffect_;
	std::map<std::string, std::pair<std::string, int>> effects_;

	void Update(float deltaTime);

	static EffectMng* instance_;
};

// �G�t�F�N�g�̏��
struct EffectData
{
	std::string handle_;	// �摜�n���h��
	Vector3<float> pos_;		// ���W
	float angle_;				// �p�x
	float exRate_;				// �g�嗦
	unsigned int frame_;		// ���݂̃t���[����
	unsigned int playSpeed_;	// �Đ����x
	size_t count_;

	Vector2<int> size_;

	std::unique_ptr<BoardPolygon> poly_;

	Vector2<bool> inv_;

	float time_ = 0.0;
	float speed_;

	EffectData(const std::string& handle, const Vector3<float>& pos, float angle, float exrate, float playspeed, size_t count)
		:handle_(handle), pos_(pos), angle_(angle), exRate_(exrate), frame_(0), speed_(playspeed), count_(count) {};
};
