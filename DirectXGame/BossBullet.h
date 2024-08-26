#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class BossBullet{
public:
	
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position,const Vector3& velocity);
	void Update();
	void Draw();

	Vector3 GetWorldPosition();
	
	void OnCollision();
	bool IsDead() { return isDead_; }

private:

	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;

	int32_t deathTimer_ = kLifeTime;

	bool isDead_ = false;
};