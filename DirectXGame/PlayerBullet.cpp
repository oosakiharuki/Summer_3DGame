#include "PlayerBullet.h"
#include "TextureManager.h"
#include <cassert>


void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) { 
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("playerBullet.png");
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void PlayerBullet::OnCollision() {
	isDead_ = true;
}

void PlayerBullet::Rotate(const Vector3& direction) { 
	worldTransform_.rotation_.y = direction.y;
}

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos{};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

//void PlayerBullet::SetParent(const WorldTransform* parent) { 
//	worldTransform_.parent_ = parent;
//}

void PlayerBullet::Update() {
	worldTransform_.UpdateMatrix();

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) { 
	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}