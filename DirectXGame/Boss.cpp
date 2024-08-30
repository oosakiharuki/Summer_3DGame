#include "Boss.h"
#include <cassert>
#include <numbers>

#include "Player.h"

void Boss::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	assert(model);

	model_ = model;
	worldTransform_.Initialize();
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;

	worldTransform_.translation_ = {0, 0, 0};
	p = {0, 0, 0};
	c = {0, 0, 0};

	modelBullet_ = Model::CreateFromOBJ("bullet", true);

	audio_ = Audio::GetInstance();
	soundDataHandle[0] = audio_->LoadWave("hit.mp3");
	soundDataHandle[1] = audio_->LoadWave("K.O.mp3");

}

void Boss::OnCollision() {
	Hp--;
	if (Hp <= 0) {
		audio_->PlayWave(soundDataHandle[1], false, 0.8f);
		isDead_ = true;
	} else {
		audio_->PlayWave(soundDataHandle[0],false,0.5f);
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

	switch (action_) {
	case Action::RotateR:
		changeAction -= 1.0f / 60.0f;

		angle += 3.14f / 600.0f;

		p.x = c.x + std::cos(angle) * r;
		p.y = c.y;
		p.z = c.z + std::sin(angle) * r;

		worldTransform_.translation_.x = p.x;
		worldTransform_.translation_.y = p.y;
		worldTransform_.translation_.z = p.z;
	
		worldTransform_.rotation_.y -= 3.14f / 600.0f;

		if (changeAction < 0) {
			action_ = Action::FireLevel;
			changeAction = kMaxTime;
		} 



		break;
	case Action::RotateL:
		changeAction -= 1.0f / 60.0f;

		angle -= 3.14f / 600.0f;

		p.x = c.x + std::cos(angle) * r;
		p.y = c.y;
		p.z = c.z + std::sin(angle) * r;

		worldTransform_.translation_.x = p.x;
		worldTransform_.translation_.y = p.y;
		worldTransform_.translation_.z = p.z;

		worldTransform_.rotation_.y += 3.14f / 600.0f;

		if (changeAction < 0) {
			action_ = Action::FireLevel;
			changeAction = kMaxTime;
		}
		
		break;
	case Action::FireLevel:
		
		if (!isFireStart) {
			worldTransform_.scale_.y += 0.02f;
			worldTransform_.scale_.x -= 0.02f;

			if (worldTransform_.scale_.y > 3.0f) {
				isFireStart = true;
			}
		} else {
			worldTransform_.scale_.y = 1.0f;
			worldTransform_.scale_.x = 1.0f;		
		}

		if (isFireStart) {
			isFirePower = true;
			changeAction -= 1.0f / 60.0f;
		} else {
			isFirePower = false;
		}


		if (changeAction < kMaxTime - 5.0f) {
			if (flont) {
				action_ = Action::RotateR;
				flont = false; 
			} else {
				action_ = Action::RotateL;
				flont = true;
			}
			changeAction = kMaxTime;
			isFireStart = false;	
			isFirePower = false;
		}

		break;
	}

}

void Boss::Draw() {

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}