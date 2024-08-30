#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "MyMath.h"
#include "BossBullet.h"
#include "Audio.h"

class Player;

class Boss{
public:

	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);
	void Update();
	void Draw();
	void OnCollision();
	Vector3 GetWorldPosition();

		
	const WorldTransform* GetWorldTransform() const { return &worldTransform_; }
	void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetTransration() { return worldTransform_.translation_; } 

	bool IsDead() { return isDead_; }

private:
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;


	float angle = 0.0f;
	float r = 70.0f;
	Vector3 p;
	Vector3 c;

	uint32_t Hp = 50;
	bool isDead_ = false;

	BossBullet* bossBullet_ = nullptr;
	std::list<BossBullet*> bullets_;
	Model* modelBullet_ = nullptr;


	Player* player_ = nullptr;
	MyMath* myMath_ = nullptr;


	Audio* audio_ = nullptr;
	uint32_t soundDataHandle[2] = {};// 0 hit 1 K.O
};