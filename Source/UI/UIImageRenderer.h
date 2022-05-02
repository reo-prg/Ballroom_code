#pragma once
#include "UIBase.h"
#include <memory>
#include <string>
#include "../Math/VectorInterface.h"

struct UIImageDesc
{
    // À•W
    Vector2<float> pos_;

	// ‘å‚«‚³
    Vector2<float> size_;

	// •`‰æ—Dæ“x(‘å‚«‚¢‚Ù‚¤‚ªæ)
    int priority_;

	// ‰æ‘œ‚Ìƒnƒ“ƒhƒ‹
    int textureHandle_;

	// Šp“x
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
    /// •`‰æ‚Ìİ’è
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

