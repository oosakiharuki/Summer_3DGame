#include"Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "MyMath.h"
#include "PlayerBullet.h"

#include "Sprite.h"
#ifdef _DEBUG
#include "imgui.h"
#endif

#include <list>
#include "Audio.h"

class RailCamera;

class Player {
public:
	
	~Player();
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);
	
	void Update();
	
	void Draw();
	void DrawSprite();

	void Rotate();	
	void Attack();

	Vector3 GetWorldPosition();
	
	void OnCollision();
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	//void SetParent(const WorldTransform* parent);

	const WorldTransform& GetWorldTransfrom() const { return worldTransform_; }

	void Damage();
	bool IsDead() const { return isDead_; }

private:
	
	uint32_t textureHandle_ = 0u;

	Model* model_ = nullptr;

	WorldTransform worldTransform_;

	const ViewProjection* viewProjection_ = nullptr;

	Input* input_ = nullptr;

	MyMath* myMath_ = nullptr;

	 //PlayerBullet* bullet_ = nullptr;
	Model* modelBullet_ = nullptr;

	std::list<PlayerBullet*> bullets_;
	Vector3 bulletDirection;

	RailCamera* railCamera_ = nullptr;

	uint32_t Hp = 5;
	uint32_t textureHandleHp[5] = {0u,0u,0u,0u,0u};
	Sprite* sprite[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
	bool isDead_ = false;

	
	Audio* audio_ = nullptr;
	uint32_t soundDataHandle[2] = {};
};