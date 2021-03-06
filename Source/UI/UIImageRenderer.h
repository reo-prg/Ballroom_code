#pragma once
#include "UIBase.h"
#include <memory>
#include <string>
#include "../Math/VectorInterface.h"

struct UIImageDesc
{
    // 座標
    Vector2<float> pos_;

	// 大きさ
    Vector2<float> size_;

	// 描画優先度(大きいほうが先)
    int priority_;

	// 画像のハンドル
    int textureHandle_;

	// 角度
	float angle_;
};

class Square2DPolygon;
class UIImageRenderer :
    public UIBase
{
public:
    UIImageRenderer() = delete;
    UIImageRenderer(const Vector2<float>& pos, const Vector2<float>& size,
        float angle, const std::string& texpath, int priority);
    ~UIImageRenderer();

    /// <summary>
    /// 描画の設定
    /// </summary>
    void SetDesc(const UIImageDesc& desc);

    void SetPosition(const Vector2<float>& pos);
    void SetPositionX(float x);
    void SetPositionY(float y);
    void SetDrawPriority(float priority);

    void SetRotateRad(float angle);
    void SetRotateDeg(float angle);
    void SetSize(const Vector2<float>& size);
    void SetScale(float scale);

    void TextureInvertX(bool flag);
    void TextureInvertY(bool flag);

    void SetTexture(int handle);

    void Draw(void)override;
private:
    void Initialize(const std::string& texpath);

    Vector2<float> size_;
    float scale_;

    float priority_;
    int textureHandle_;
    float angle_;

    std::unique_ptr<Square2DPolygon> poly_;
};

