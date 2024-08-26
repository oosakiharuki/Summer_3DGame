#include "BossBullet.h"
#include "TextureManager.h"
#include <cassert>

void BossBullet::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position, const Vector3& velocity) {
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("bullet/bullet.png");
	worldTransform_.Initialize();

	viewProjection_ = viewProjection;

	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void BossBullet::OnCollision() { 
	isDead_ = true; 
}

Vector3 BossBullet::GetWorldPosition() {
	Vector3 worldPos{};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void BossBullet::Update() {
	worldTransform_.UpdateMatrix();

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void BossBullet::Draw() {

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}