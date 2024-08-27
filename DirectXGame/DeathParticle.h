#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMath.h"

#include <array> 

class DeathParticle{
public:
	void Initialize(ViewProjection* viewProjection,const Vector3& position);
	void Updata();
	void Draw();

	bool IsDead() { return isFinish_; }

private:
	
	Model* model_ = nullptr;
	
	static inline const uint32_t kNumParticles = 4;

	static inline const float kDeleteTimer = 5.0f;
	static inline const float kParticleSpeed = 0.25f;
	static inline const float kAngle = (2.0f * 3.14f) / 4;

	bool isFinish_ = false;
	float counter_ = 0.0f; 

	std::array<WorldTransform, kNumParticles> worldTransform_;

	ViewProjection* viewProjection_;

	MyMath* myMath_ = nullptr;
};