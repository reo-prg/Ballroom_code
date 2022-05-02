#pragma once
#include <memory>
#include <string>
#include <vector>

class Camera;
class BackGround
{
public:
	BackGround();
	~BackGround();
	
	void Load(const std::string& filepath);

	void Update(float delta);
	void Draw(void);

	void SetCamera(Camera* camera);
private:
	void Initialize(void);

	struct BGElem
	{
		int gHandle_;

		float speed_;
		float offset_;
		float width_;

		float scale_;
	};

	int screenWidth_;
	int screenHeight_;
	std::vector<BGElem> elem_;

	Camera* camera_ = nullptr;
};

