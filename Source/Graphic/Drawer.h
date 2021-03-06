#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <EffekseerForDXLib.h>
#include "DrawEnum.h"

#define DrawerIns Drawer::GetInstance()

struct PostProcessParameter
{
	float time_;
	float damage_;
};

struct FadeParameter
{
	FLOAT4 maincolor_;
	FLOAT4 subcolor_;
	float param_[4][4];
	float progress_;
};

class Camera;
struct DrawData2D;
class CameraShake;
struct ShakeDescriptor;
class Board2D;
class Drawer
{
public:
	static void Initialize(void);
	static Drawer& GetInstance(void);
	static void Terminate(void);

	void Update(float delta);
	void Draw(void);
	void Draw(DrawType type);
	void StartDraw(int screenHandle = -1);
	void EndDraw(void);

	void DrawShadowStart(void);
	void DrawWithShadow(void);

	void SetCamera(Camera* camera);

	void StartCameraShake(const ShakeDescriptor& desc);

	/// <summary>
	/// 画像を2Dで描画する
	/// </summary>
	/// <param name="grHandle">画像ハンドル</param>
	/// <param name="x">x座標</param>
	/// <param name="y">y座標</param>
	/// <param name="angle">回転</param>
	/// <param name="scale">拡大率</param>
	/// <param name="trans">透過</param>
	/// <param name="zBuffer">描画順(大きいほうが先に描かれる)</param>
	/// <param name="isBG">背景として描画するか</param>
	void Add2DDraw(int grHandle, float x, float y, double angle, double scale, bool trans, 
		int zBuffer, DrawType type = DrawType::Normal, bool rev_x = false, bool rev_y = false);
	
	/// <summary>
	///	描画リストに独自の処理を追加する
	/// 文字出したりシェーダー使ったりなど
	/// </summary>
	/// <param name="func">実行したい関数</param>
	/// <param name="zBuffer">描画順(大きいほうが先に描かれる)</param>
	/// <param name="isBG">背景として描画するか</param>
	void Add2DDraw(const std::function<void(void)>& func, int zBuffer, DrawType type = DrawType::Normal);

	void SetDamageEffect(float strength, bool absolute = false);


	void SetFadeShader(int handle);

	void SetFadeMainColor(const FLOAT4& color);
	void SetFadeSubColor(const FLOAT4& color);
	void SetFadeParameter(const FLOAT4& value, int parameterIndex_);
	void SetFadeParameter(float value, int parameterIndex_, int subIndex_);
	void SetFadeProgress(float progress);
private:
	Drawer();
	~Drawer();

	void Init(void);

	static Drawer* instance_;

	Camera* camera_ = nullptr;
	std::unique_ptr<CameraShake> cameraShake_;

	int currentScreenHandle_;

	std::unordered_map<DrawType, std::vector<std::unique_ptr<DrawData2D>>> drawlist_;

	int shadowMap_;
	VECTOR smAreaMin_;
	VECTOR smAreaMax_;

	int pass1_;
	int fadepass_;

	std::unique_ptr<Board2D> postProcess_;

	int postHandle_;
	PostProcessParameter* postParam_;

	float targetDamageStr_;

	int fadeHandle_;
	FadeParameter* fadeParam_;
	std::unique_ptr<Board2D> fadePoly_;
};

struct DrawData2D
{
	int handle_;
	float posx_, posy_;
	double rotate_;
	double scale_;
	bool trans_;

	bool reverse_x;
	bool reverse_y;

	int zBuffer_;

	std::function<void(void)> func_;

	bool isUseFunc_ = false;
};
