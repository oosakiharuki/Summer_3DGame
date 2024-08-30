#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "MyMath.h"
#include "Audio.h"

class Player;
class GameScene;

class Enemy {
public:
	//~Enemy();

	enum class Phase {
		move,
		Fight,
	};

	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, Vector3 position,float direction);

	void Update();
	void Draw();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	void OnCollision();
	void OnStartFight();
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	Vector3 GetTransration() { return worldTransform_.translation_; } 

	bool IsDead() const { return isDead_; }
	bool IsDamage() const { return isDamageParticle_; }

private:

	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;


	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr;


	MyMath* myMath_ = nullptr;

	Phase phace_ = Phase::move;


	float deltaTimer_;
	static inline float kWideStart = 1.0f;
	static inline float kWideEnd = -2.0f;
	static inline float kTime = 2.0f;
	float angle;

	float param;
	float radian;


	float coolTimer_ = 2.0f;
	float attackTime = 1.0f;
	bool firstAttack = true;

	const float kBulletSpeed = 0.12f;

	uint32_t Hp = 3;
	bool isDead_ = false;

	Audio* audio_ = nullptr;
	uint32_t soundDataHandle_ = 0;

	bool isDamageParticle_ = false;
};