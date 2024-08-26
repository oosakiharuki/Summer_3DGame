#include "Boss.h"
#include <cassert>

#include "Player.h"

void Boss::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	assert(model);

	model_ = model;
	worldTransform_.Initialize();
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;

	worldTransform_.translation_ = {0, 100, 0};
	p = {0, 0, 0};
	c = {0, 0, 0};

	modelBullet_ = Model::CreateFromOBJ("bullet", true);
}

void Boss::OnCollision() {
	Hp--;
	if (Hp <= 0) {
		isDead_ = true;
	}
}

Vector3 Boss::GetWorldPosition() {
	Vector3 worldPos{};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}



void Boss::Update() {
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();

	worldTransform_.scale_.x = 3.0f;
	worldTransform_.scale_.y = 3.0f;
	worldTransform_.scale_.z = 3.0f;

	angle += 3.14f / 600.0f;

	p.x = c.x + std::cos(angle) * r;
	p.y = c.y;
	p.z = c.z + std::sin(angle) * r;

	worldTransform_.translation_.x = p.x;
	worldTransform_.translation_.y = p.y;
	worldTransform_.translation_.z = p.z;

}

void Boss::Draw() {

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}