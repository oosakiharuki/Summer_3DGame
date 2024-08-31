#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMath.h"

#include <array> 

class HitParticle {
public:
	void Initialize(ViewProjection* viewProjection, const Vector3& position, uint32_t textureHandele);
	void Update();
	void Draw();

	bool IsDead() { return isFinish_; }


private:
	Model* model_ = nullptr;

	static inline const uint32_t kNumParticles = 4;

	static inline const float kDeleteTimer = 1.0f;
	static inline const float kParticleSpeed = 0.15f;
	static inline const float kParticleScale_ = 0.015f; 
	static inline const float kAngle = (2.0f * 3.14f) / 4;

	bool isHit = false;
	bool isFinish_ = false;
	float counter_ = 0.0f;

	std::array<WorldTransform, kNumParticles> worldTransform_;

	ViewProjection* viewProjection_;

	uint32_t textureHandle_ = 0u;

	MyMath* myMath_ = nullptr;
};
