#include "HitParticle.h"

void HitParticle::Initialize(ViewProjection* viewProjection, const Vector3& position,uint32_t textureHandele) {

	model_ = Model::CreateFromOBJ("particle", true);
	viewProjection_ = viewProjection;
	
	for (auto& worldTransform : worldTransform_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}

	textureHandle_ = textureHandele;
}


void HitParticle::Update() {
	for (auto& worldTransform : worldTransform_) {
		worldTransform.TransferMatrix();
		worldTransform.UpdateMatrix();
	}

	counter_ += 1.0f / 60.0f;

	if (counter_ >= kDeleteTimer) {
		counter_ = kDeleteTimer;

		isFinish_ = true;
	}

	for (uint32_t i = 0; i < 4; i++) {

		Vector3 velocity = {kParticleSpeed, kParticleSpeed, kParticleSpeed};
		float angle = kAngle * i;

		Matrix4x4 matrixRotation_ = myMath_->MakeRotateY(angle);

		velocity = myMath_->TransformNormal(velocity, matrixRotation_);

		worldTransform_[i].translation_.x += velocity.x;
		worldTransform_[i].translation_.y += velocity.y;
		worldTransform_[i].translation_.z += velocity.z;

		worldTransform_[i].scale_.x -= kParticleScale_;
		worldTransform_[i].scale_.y -= kParticleScale_;
		worldTransform_[i].scale_.z -= kParticleScale_;
	}
	

	if (isFinish_) {
		return;
	}
}

void HitParticle::Draw() {
	for (auto& worldTransform : worldTransform_) {
		model_->Draw(worldTransform, *viewProjection_,textureHandle_);
	}
	if (isFinish_) {
		return;
	}
}