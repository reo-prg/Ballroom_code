#pragma once
#include "UIIntegrator.h"
#include <memory>

class Animation;
class HealthUI :
	public UIIntegrator
{
public:
	HealthUI() = delete;
	HealthUI(int maxHealth, int currentHealth);
	~HealthUI() = default;

	void Update(float delta)override;
	void Draw(void)override;

	void SetHealth(int health);
	int GetHealth(void) { return currentHealth_; }
private:
	void Initialize(void);

	int maxHealth_;
	int currentHealth_;

	UIProgressBar* healthBar_;

	std::unique_ptr<Animation> playerAnim_;
};

