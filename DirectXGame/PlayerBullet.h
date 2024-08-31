#include "Model.h"
#include "WorldTransform.h"

class PlayerBullet {
public:

	void Initialize(Model* model, const Vector3& position ,const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void Rotate(const Vector3& direction);

	void OnCollision();
	Vector3 GetWorldPosition();

	//void SetParent(const WorldTransform* parent);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;

	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
};