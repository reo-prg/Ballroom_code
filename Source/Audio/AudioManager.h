#pragma once
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <xapofx.h>
#include <array>
#include <initializer_list>
#include <list>
#include <string>
#include <memory>
#include <unordered_map>
#include "EffectDefines.h"
#include "../Utility/HandleArray.h"


#define AudioIns AudioManager::GetInstance()

// ���������p�̒�`

// �\�[�X�{�C�X�̍ő吔
constexpr size_t SourceVoiceArrayMaxSize = 1024;

// �T�u�~�b�N�X�{�C�X�̍ő吔
constexpr size_t SubmixVoiceArrayMaxSize = 256;

// ���ꂼ��̃n���h���̃}�X�N
constexpr int SourceHandleMask = 0x0000ffff;
constexpr int SubmixHandleMask = 0x00ff0000;

// �n���h���̎�ނ̎��ʗp
constexpr int IdentifyMask = 0x70000000;
constexpr int SourceIdentifyID = 0x10000000;
constexpr int SubmixIdentifyID = 0x20000000;

// �T�u�~�b�N�X�̃V�t�g������r�b�g��
constexpr int SubmixHandleShift = 16;

constexpr int RootSubmixHandle = SubmixIdentifyID + 0;

constexpr unsigned int RootProcessingStage = 128;


struct SubmixVoice;
struct SourceVoice;

/// <summary>
/// �{�C�X�̏��
/// </summary>
enum class VoiceState
{
	Playing,
	Stop,
};


class WAVLoader;
class AudioManager
{
public:
	static void Create();
	static AudioManager& GetInstance(void);
	static void Terminate(void);

	/// <summary>
	/// ����ǂݍ���
	/// </summary>
	/// <param name="filename">�t�@�C���p�X</param>
	/// <param name="key">�L�[</param>
	void LoadSound(const std::string& filename, const std::string& key);

	/// <summary>
	/// �T�u�~�b�N�X�{�C�X���쐬����
	/// </summary>
	/// <param name="outputHandles">�o�͐�̃T�u�~�b�N�X�{�C�X�̃n���h��</param>
	/// <returns>�쐬�����T�u�~�b�N�X�{�C�X�ɑΉ�����n���h��</returns>
	int CreateSubmix(std::initializer_list<int> outputHandles = { RootSubmixHandle });

	/// <summary>
	/// �Đ�����
	/// </summary>
	/// <param name="key">�ǂݍ��񂾉��̃L�[</param>
	/// <param name="volume">����</param>
	/// <param name="deleteIfEnd">�Ō�܂ōĐ�������Ɏ����Ŕj�����邩</param>
	/// <param name="outputHandles">�o�͐�̃T�u�~�b�N�X</param>
	/// <returns>�Đ��������ɑΉ�����n���h��</returns>
	int Play(const std::string& key, float volume = 1.0f, bool deleteIfEnd = true, std::initializer_list<int> outputHandles = { RootSubmixHandle });
	
	/// <summary>
	/// �����Ŕj�����鉹���Đ�����
	/// </summary>
	/// <param name="key">�ǂݍ��񂾉��̃L�[</param>
	/// <param name="volume">����</param>
	/// <param name="outputHandles">�o�͐�̃T�u�~�b�N�X</param>
	/// <returns>�Đ��������ɑΉ�����n���h��</returns>
	int PlayTemporary(const std::string& key, float volume = 1.0f, std::initializer_list<int> outputHandles = { RootSubmixHandle });
	
	/// <summary>
	/// �������[�v�Đ�����
	/// </summary>
	/// <param name="key">�ǂݍ��񂾉��̃L�[</param>
	/// <param name="begin">���[�v�̊J�n�ʒu</param>
	/// <param name="length">���[�v�̒���(0.0f�ōŌ�܂ōĐ�)</param>
	/// <param name="loopCount">���[�v��</param>
	/// <param name="volume">����</param>
	/// <param name="deleteIfEnd">�Ō�܂ōĐ�������Ɏ����Ŕj�����邩</param>
	/// <param name="outputHandles">�o�͐�̃T�u�~�b�N�X</param>
	/// <returns>�Đ��������ɑΉ�����n���h��</returns>
	int PlayLoop(const std::string& key, float begin, float length, unsigned int loopCount, 
		float volume = 1.0f, bool deleteIfEnd = true, std::initializer_list<int> outputHandles = { RootSubmixHandle });
	
	/// <summary>
	/// �����ŏ�����Đ�����
	/// </summary>
	/// <param name="handle">�Đ��������̃n���h��</param>
	void PlayAgain(int handle);

	/// <summary>
	/// �����Đ��ʒu�����߂čĐ�����
	/// </summary>
	/// <param name="handle">�Đ��������̃n���h��</param>
	/// <param name="begin">�J�n�ʒu</param>
	/// <param name="length">�I���ʒu(0.0f�ōŌ�܂�)</param>
	void PlayAgain(int handle, float begin, float length);

	/// <summary>
	/// �Đ����̉��̐i�x���擾����
	/// </summary>
	/// <param name="sourceHandle">�Đ����̉��̃n���h��</param>
	/// <returns>�i�x(0.0f - 1.0f)</returns>
	float GetProgress(int sourceHandle);
	
	/// <summary>
	/// ���ʂ�ݒ肷��
	/// </summary>
	/// <param name="handle">�n���h��</param>
	/// <param name="volume">����</param>
	void SetVolume(int handle, float volume);

	/// <summary>
	/// ���ʂ��擾����
	/// </summary>
	/// <param name="handle">�n���h��</param>
	/// <returns>����</returns>
	float GetVolume(int handle);

	/// <summary>
	/// ��~���Ă��鉹�����̈ʒu����Đ�����
	/// </summary>
	/// <param name="handle">��~���̉��̃n���h��</param>
	void Continue(int handle);

	/// <summary>
	/// �����~������
	/// </summary>
	/// <param name="handle">�Đ����̉��̃n���h��</param>
	void Stop(int handle);

	/// <summary>
	/// �ǂݍ��񂾉����폜����
	/// </summary>
	/// <param name="key">�ǂݍ���ł��鉹�̃L�[</param>
	void Unload(const std::string& key);

	/// <summary>
	/// ��~���̉������ׂčĐ�����
	/// </summary>
	void ContinueAll(void);

	/// <summary>
	/// �Đ����̉������ׂĒ�~����
	/// </summary>
	/// <param name="destroy">����j�����邩�ǂ���</param>
	void StopAll(bool destroy);

	/// <summary>
	/// �n���h���ɑΉ����������폜����
	/// </summary>
	/// <param name="handle">�n���h��</param>
	void DeleteHandle(int handle);

	void Update(void);

	/// <summary>
	/// �Đ��������̏o�͐��ǉ�����
	/// </summary>
	/// <param name="sourceHandle">�Đ��������̃n���h��</param>
	/// <param name="targetHandle">�o�͐�̃n���h��</param>
	void AddSourceOutputTarget(int sourceHandle, int targetHandle);

	/// <summary>
	/// �T�u�~�b�N�X�{�C�X�̏o�͐��ǉ�����
	/// </summary>
	/// <param name="submixHandle">�T�u�~�b�N�X�̃n���h��</param>
	/// <param name="targetHandle">�o�͐�̃n���h��</param>
	void AddSubmixOutputTarget(int submixHandle, int targetHandle);

	/// <summary>
	/// �Đ��������̏o�͐�����O����
	/// </summary>
	/// <param name="sourceHandle">�Đ��������̃n���h��</param>
	/// <param name="targetHandle">���O����n���h��</param>
	void RemoveSourceOutputTarget(int sourceHandle, int targetHandle);

	/// <summary>
	/// �T�u�~�b�N�X�{�C�X�̏o�͐�����O����
	/// </summary>
	/// <param name="sourceHandle">�T�u�~�b�N�X�̃n���h��</param>
	/// <param name="targetHandle">���O����n���h��</param>
	void RemoveSubmixOutputTarget(int submixHandle, int targetHandle);

	/// <summary>
	/// �t�B���^�[��ݒ肷��
	/// </summary>
	/// <param name="handle">�ݒ肵�����n���h��</param>
	/// <param name="type">�t�B���^�[�̎��</param>
	/// <param name="frequency">���g��</param>
	/// <param name="danping">����</param>
	void SetFilter(int handle, XAUDIO2_FILTER_TYPE type, float frequency, float danping);

	/// <summary>
	/// �G�t�F�N�g��ǉ�����
	/// </summary>
	/// <param name="handle">�ǉ��������n���h��</param>
	/// <param name="type">�G�t�F�N�g�̎��</param>
	/// <param name="active">�L���ɂ��邩</param>
	/// <param name="insertPosition">�G�t�F�N�g��}������ʒu</param>
	/// <returns></returns>
	int AddEffect(int handle, AudioEffectType type, bool active, int insertPosition = -1);

	void SetReverbParameter(const XAUDIO2FX_REVERB_I3DL2_PARAMETERS& param, int submixHandle, int effectIndex = -1);
	void SetReverbParameter(const XAUDIO2FX_REVERB_PARAMETERS& param, int submixHandle, int effectIndex = -1);
	void SetEchoParameter(float strength, float delay, float reverb, int submixHandle, int effectIndex = -1);
	void SetEqualizerParameter(const FXEQ_PARAMETERS& param, int submixHandle, int effectIndex = -1);
	void SetMasteringLimiterParameter(UINT32 release, UINT32 loudness, int submixHandle, int effectIndex = -1);
	void SetFXReverbParameter(float diffuse, float roomsize, int submixHandle, int effectIndex = -1);

	XAUDIO2FX_VOLUMEMETER_LEVELS* GetVolumeMeterParameter(int submixHandle, int effectIndex = -1);
private:
	AudioManager();
	AudioManager(const AudioManager&) = delete;
	AudioManager operator=(const AudioManager&) = delete;
	~AudioManager();

	static AudioManager* instance_;

	void Initialize(void);

	bool SourceHandleIsValid(int handle);
	bool SubmixHandleIsValid(int handle);

	int FindEffect(int handle, AudioEffectType type);

	std::unique_ptr<WAVLoader> wavLoader_;

	IXAudio2* xaudioCore_;
	IXAudio2MasteringVoice* masterVoice_;
	XAUDIO2_VOICE_DETAILS masterVoiceDetails_ = {};

	std::unordered_map<std::string, std::string> filenameTable_;

	HandleArray<SourceVoice, SourceVoiceArrayMaxSize> source_;
	HandleArray<SubmixVoice, SubmixVoiceArrayMaxSize> submix_;
};

struct SourceVoice
{
	SourceVoice() = default;
	~SourceVoice()
	{
		if (sourceVoice_ != nullptr)
		{
			sourceVoice_->DestroyVoice();
		}
	}

	WAVEFORMATEX waveFormat_;
	XAUDIO2_BUFFER buffer_;
	IXAudio2SourceVoice* sourceVoice_ = nullptr;
	VoiceState vState_;

	int handle_;

	// �o�͐�̃T�u�~�b�N�X
	std::vector<XAUDIO2_SEND_DESCRIPTOR> send_;
	std::vector<SubmixVoice*> output_;

	// �Đ��I�����Ɏ����Ŕj�����邩
	bool deleteWhenEnd_ = false;
};

struct EffectParams
{
	~EffectParams()
	{
		if (type_ == AudioEffectType::VolumeMeter)
		{
			XAUDIO2FX_VOLUMEMETER_LEVELS* level = reinterpret_cast<XAUDIO2FX_VOLUMEMETER_LEVELS*>(param_);
			delete[] level->pPeakLevels;
			delete[] level->pRMSLevels;
			delete param_;

			pEffect_->Release();
		}
	}

	// �G�t�F�N�g�̎��
	AudioEffectType type_;
	
	// �G�t�F�N�g�̃C���X�^���X
	IUnknown* pEffect_;

	// �p�����[�^�[
	void* param_;
};

struct SubmixVoice
{
	SubmixVoice() = default;
	~SubmixVoice()
	{
		if (submixVoice_ != nullptr)
		{
			submixVoice_->DestroyVoice();
		}
	}

	IXAudio2SubmixVoice* submixVoice_ = nullptr;

	std::vector<XAUDIO2_SEND_DESCRIPTOR> send_;

	// ���͐�̃T�u�~�b�N�X
	std::vector<SubmixVoice*> input_;

	// �o�͐�̃T�u�~�b�N�X
	std::vector<SubmixVoice*> output_;

	// ���͐�̃\�[�X
	std::vector<SourceVoice*> sources_;

	// �������Ă���G�t�F�N�g
	std::vector<XAUDIO2_EFFECT_DESCRIPTOR> efkDesc_;
	std::vector<EffectParams> efkParam_;

	int handle_;

	// ���������X�e�[�W
	unsigned int stage_;
};
