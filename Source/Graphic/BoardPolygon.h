#pragma once
#include <EffekseerForDXLib.h>
#include <array>
#include "../Math/VectorInterface.h"
#include "../Math/Matrix33.h"

class BoardPolygon
{
public:
	BoardPolygon();
	~BoardPolygon();

	/// <summary>
	/// xy���ʏ�ō��W�ݒ�
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPosition(const Vector2<float>& pos);
	
	/// <summary>
	/// ���s��(z���W)�̐ݒ�
	/// </summary>
	/// <param name="zpos">���W</param>
	void SetZ(float zpos);

	/// <summary>
	/// 3D�̍��W�ݒ�
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPosition(const VECTOR& pos);

	/// <summary>
	/// ��]�̐ݒ�
	/// </summary>
	/// <param name="angle">�p�x(���W�A��)</param>
	void SetRotate(float angle);

	/// <summary>
	/// ��]�̉��Z
	/// </summary>
	/// <param name="angle">�p�x(���W�A��)</param>
	void AddRotate(float angle);

	/// <summary>
	/// �g��̐ݒ�
	/// </summary>
	/// <param name="scale">�g�嗦</param>
	void SetScale(const Vector2<float>& scale);

	/// <summary>
	/// �摜�̍��E�𔽓]�����邩
	/// </summary>
	void TextureInvertX(bool flag);

	/// <summary>
	/// �摜�̏㉺�𔽓]�����邩
	/// </summary>
	void TextureInvertY(bool flag);

	/// <summary>
	/// �摜��ݒ肷��
	/// </summary>
	/// <param name="handle">�摜�n���h��</param>
	void SetTexture(int handle);

	void SetVertexShader(int handle);
	void SetPixelShader(int handle);

	void Draw(void);
private:
	void Initialize(void);

	void ApplyPosToVert(void);
	void ApplyUvToVert(void);

	static constexpr int BoardVertNum = 4;
	static constexpr int BoardIndexNum = 6;

	std::array<VERTEX3D, BoardVertNum> vert_;
	std::array<unsigned short, BoardIndexNum> index_;

	VECTOR centerPos_;
	Vector2<float> scale_;

	float angle_;
	Matrix angleMat_;

	bool texinvx_;
	bool texinvy_;

	int vertexShaderHandle_ = -1;
	int pixelShaderHandle_ = -1;
	
	int textureHandle_ = -1;

	MATERIALPARAM matparam_;
};

