#include "Enemy.h"
#include <cassert>
#include <numbers>

#include "Player.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, Vector3 position, float direction) {
	
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	
	if (direction != 0.0f) {
		worldTransform_.rotation_.y += std::numbers::pi_v<float> / direction;
	}

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
	Hp--;
	if (Hp <= 0) {
		isDead_ = true;
	}
}

void Enemy::OnStartFight() {
	phace_ = Phase::Fight;
}

void Enemy::Update() {
	
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
	
	Vector3 playerPostion = player_->GetWorldPosition();
	Vector3 enemyPostion = GetWorldPosition();

	Vector3 subtract = myMath_->Subtract(playerPostion, enemyPostion);

	Vector3 normalize = myMath_->Normalize(subtract);

	normalize.x *= kBulletSpeed;
	normalize.y *= kBulletSpeed;
	normalize.z *= kBulletSpeed;

	Vector3 velocity(normalize);

	switch (phace_) {
	case Phase::move:

		assert(player_);

		//velocity = myMath_->TransformNormal(velocity, worldTransform_.matWorld_);

		worldTransform_.translation_.x += velocity.x;
		worldTransform_.translation_.y += velocity.y;
		worldTransform_.translation_.z += velocity.z;

		//EnemyBullet* newBullet = new EnemyBullet();
		//newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//bullets_.push_back(newBullet);
		//bulletTimer_ = 1.0f;

		break;
	case Phase::Fight:

		if (coolTimer_ <= 0) {
		
			attackTime -= 1.0f / 60.0f;

			Vector3 AttackVlocity = {0.2f,0.0f,0.0f};


			worldTransform_.translation_.x += velocity.x;
			worldTransform_.translation_.y += velocity.y;
			worldTransform_.translation_.z += velocity.z;

			worldTransform_.rotation_.x -= AttackVlocity.x;

			if (attackTime <= 0.0f)
			{
				coolTimer_ = 2.0f;
				attackTime = 1.0f;
				firstAttack = false;
				player_->Damage();
				worldTransform_.rotation_.x = 0.0f;
			}

		} else {

			coolTimer_ -= 1.0f / 60.0f;

			worldTransform_.rotation_.x += 0.01f;
			if (!firstAttack) {

				worldTransform_.translation_.x -= velocity.x / 2;
				worldTransform_.translation_.y -= velocity.y / 2;
				worldTransform_.translation_.z -= velocity.z / 2;
			}
		}

		break;
	}
}

void Enemy::Draw() { 

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}