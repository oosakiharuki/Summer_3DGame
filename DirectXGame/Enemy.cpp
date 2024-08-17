#include "Enemy.h"
#include <cassert>

#include "Player.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, Vector3 position) {
	
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	viewProjection_ = viewProjection;

}

Vector3 Enemy::GetWorldPosition() { 
	Vector3 worldPos{};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() { 
	isDead_ = true; 
}

void Enemy::Update() {

	switch (phace_) {
	case Phase::move:

		worldTransform_.TransferMatrix();

		//const float kSpeed_ = 0.2f;

		worldTransform_.UpdateMatrix();

		//worldTransform_.translation_.z -= kSpeed_;


		assert(player_);


		Vector3 playerPostion = player_->GetWorldPosition();
		Vector3 enemyPostion = GetWorldPosition();

		Vector3 subtract = myMath_->Subtract(playerPostion, enemyPostion);

		Vector3 normalize = myMath_->Normalize(subtract);

		normalize.x *= kBulletSpeed;
		normalize.y *= kBulletSpeed;
		normalize.z *= kBulletSpeed;

		Vector3 velocity(normalize);

		velocity = myMath_->TransformNormal(velocity, worldTransform_.matWorld_);

		worldTransform_.translation_.x += velocity.x;
		worldTransform_.translation_.y += velocity.y;
		worldTransform_.translation_.z += velocity.z;

		//EnemyBullet* newBullet = new EnemyBullet();
		//newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//bullets_.push_back(newBullet);
		//bulletTimer_ = 1.0f;

		break;
	case Phase::Fight:
		fightTimer_ -= 0.5f;


		break;
	}
}

void Enemy::Draw() { 

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}