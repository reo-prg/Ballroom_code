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

// 内部処理用の定義

// ソースボイスの最大数
constexpr size_t SourceVoiceArrayMaxSize = 1024;

// サブミックスボイスの最大数
constexpr size_t SubmixVoiceArrayMaxSize = 256;

// それぞれのハンドルのマスク
constexpr int SourceHandleMask = 0x0000ffff;
constexpr int SubmixHandleMask = 0x00ff0000;

// ハンドルの種類の識別用
constexpr int IdentifyMask = 0x70000000;
constexpr int SourceIdentifyID = 0x10000000;
constexpr int SubmixIdentifyID = 0x20000000;

// サブミックスのシフトさせるビット数
constexpr int SubmixHandleShift = 16;

constexpr int RootSubmixHandle = SubmixIdentifyID + 0;

constexpr unsigned int RootProcessingStage = 128;


struct SubmixVoice;
struct SourceVoice;

/// <summary>
/// ボイスの状態
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
	/// 音を読み込む
	/// </summary>
	/// <param name="filename">ファイルパス</param>
	/// <param name="key">キー</param>
	void LoadSound(const std::string& filename, const std::string& key);

	/// <summary>
	/// サブミックスボイスを作成する
	/// </summary>
	/// <param name="outputHandles">出力先のサブミックスボイスのハンドル</param>
	/// <returns>作成したサブミックスボイスに対応するハンドル</returns>
	int CreateSubmix(std::initializer_list<int> outputHandles = { RootSubmixHandle });

	/// <summary>
	/// 再生する
	/// </summary>
	/// <param name="key">読み込んだ音のキー</param>
	/// <param name="volume">音量</param>
	/// <param name="deleteIfEnd">最後まで再生した後に自動で破棄するか</param>
	/// <param name="outputHandles">出力先のサブミックス</param>
	/// <returns>再生した音に対応するハンドル</returns>
	int Play(const std::string& key, float volume = 1.0f, bool deleteIfEnd = true, std::initializer_list<int> outputHandles = { RootSubmixHandle });
	
	/// <summary>
	/// 自動で破棄する音を再生する
	/// </summary>
	/// <param name="key">読み込んだ音のキー</param>
	/// <param name="volume">音量</param>
	/// <param name="outputHandles">出力先のサブミックス</param>
	/// <returns>再生した音に対応するハンドル</returns>
	int PlayTemporary(const std::string& key, float volume = 1.0f, std::initializer_list<int> outputHandles = { RootSubmixHandle });
	
	/// <summary>
	/// 音をループ再生する
	/// </summary>
	/// <param name="key">読み込んだ音のキー</param>
	/// <param name="begin">ループの開始位置</param>
	/// <param name="length">ループの長さ(0.0fで最後まで再生)</param>
	/// <param name="loopCount">ループ回数</param>
	/// <param name="volume">音量</param>
	/// <param name="deleteIfEnd">最後まで再生した後に自動で破棄するか</param>
	/// <param name="outputHandles">出力先のサブミックス</param>
	/// <returns>再生した音に対応するハンドル</returns>
	int PlayLoop(const std::string& key, float begin, float length, unsigned int loopCount, 
		float volume = 1.0f, bool deleteIfEnd = true, std::initializer_list<int> outputHandles = { RootSubmixHandle });
	
	/// <summary>
	/// 音を最初から再生する
	/// </summary>
	/// <param name="handle">再生した音のハンドル</param>
	void PlayAgain(int handle);

	/// <summary>
	/// 音を再生位置を決めて再生する
	/// </summary>
	/// <param name="handle">再生した音のハンドル</param>
	/// <param name="begin">開始位置</param>
	/// <param name="length">終了位置(0.0fで最後まで)</param>
	void PlayAgain(int handle, float begin, float length);

	/// <summary>
	/// 再生中の音の進度を取得する
	/// </summary>
	/// <param name="sourceHandle">再生中の音のハンドル</param>
	/// <returns>進度(0.0f - 1.0f)</returns>
	float GetProgress(int sourceHandle);
	
	/// <summary>
	/// 音量を設定する
	/// </summary>
	/// <param name="handle">ハンドル</param>
	/// <param name="volume">音量</param>
	void SetVolume(int handle, float volume);

	/// <summary>
	/// 音量を取得する
	/// </summary>
	/// <param name="handle">ハンドル</param>
	/// <returns>音量</returns>
	float GetVolume(int handle);

	/// <summary>
	/// 停止している音をその位置から再生する
	/// </summary>
	/// <param name="handle">停止中の音のハンドル</param>
	void Continue(int handle);

	/// <summary>
	/// 音を停止させる
	/// </summary>
	/// <param name="handle">再生中の音のハンドル</param>
	void Stop(int handle);

	/// <summary>
	/// 読み込んだ音を削除する
	/// </summary>
	/// <param name="key">読み込んである音のキー</param>
	void Unload(const std::string& key);

	/// <summary>
	/// 停止中の音をすべて再生する
	/// </summary>
	void ContinueAll(void);

	/// <summary>
	/// 再生中の音をすべて停止する
	/// </summary>
	/// <param name="destroy">音を破棄するかどうか</param>
	void StopAll(bool destroy);

	/// <summary>
	/// ハンドルに対応した物を削除する
	/// </summary>
	/// <param name="handle">ハンドル</param>
	void DeleteHandle(int handle);

	void Update(void);

	/// <summary>
	/// 再生した音の出力先を追加する
	/// </summary>
	/// <param name="sourceHandle">再生した音のハンドル</param>
	/// <param name="targetHandle">出力先のハンドル</param>
	void AddSourceOutputTarget(int sourceHandle, int targetHandle);

	/// <summary>
	/// サブミックスボイスの出力先を追加する
	/// </summary>
	/// <param name="submixHandle">サブミックスのハンドル</param>
	/// <param name="targetHandle">出力先のハンドル</param>
	void AddSubmixOutputTarget(int submixHandle, int targetHandle);

	/// <summary>
	/// 再生した音の出力先を除外する
	/// </summary>
	/// <param name="sourceHandle">再生した音のハンドル</param>
	/// <param name="targetHandle">除外するハンドル</param>
	void RemoveSourceOutputTarget(int sourceHandle, int targetHandle);

	/// <summary>
	/// サブミックスボイスの出力先を除外する
	/// </summary>
	/// <param name="sourceHandle">サブミックスのハンドル</param>
	/// <param name="targetHandle">除外するハンドル</param>
	void RemoveSubmixOutputTarget(int submixHandle, int targetHandle);

	/// <summary>
	/// フィルターを設定する
	/// </summary>
	/// <param name="handle">設定したいハンドル</param>
	/// <param name="type">フィルターの種類</param>
	/// <param name="frequency">周波数</param>
	/// <param name="danping">減衰</param>
	void SetFilter(int handle, XAUDIO2_FILTER_TYPE type, float frequency, float danping);

	/// <summary>
	/// エフェクトを追加する
	/// </summary>
	/// <param name="handle">追加したいハンドル</param>
	/// <param name="type">エフェクトの種類</param>
	/// <param name="active">有効にするか</param>
	/// <param name="insertPosition">エフェクトを挿入する位置</param>
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

	// 出力先のサブミックス
	std::vector<XAUDIO2_SEND_DESCRIPTOR> send_;
	std::vector<SubmixVoice*> output_;

	// 再生終了時に自動で破棄するか
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

	// エフェクトの種類
	AudioEffectType type_;
	
	// エフェクトのインスタンス
	IUnknown* pEffect_;

	// パラメーター
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

	// 入力先のサブミックス
	std::vector<SubmixVoice*> input_;

	// 出力先のサブミックス
	std::vector<SubmixVoice*> output_;

	// 入力先のソース
	std::vector<SourceVoice*> sources_;

	// かかっているエフェクト
	std::vector<XAUDIO2_EFFECT_DESCRIPTOR> efkDesc_;
	std::vector<EffectParams> efkParam_;

	int handle_;

	// 処理されるステージ
	unsigned int stage_;
};
