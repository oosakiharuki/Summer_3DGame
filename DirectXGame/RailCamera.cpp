#include "RailCamera.h"
#include "Player.h"

void RailCamera::Initialize() {
	//ViewProjection& viewProjection
	// 
	worldTramsform_.Initialize();
	//worldTramsform_.translation_ = viewProjection.translation_;
	//worldTramsform_.rotation_ = viewProjection.rotation_;

	//Vector3 Translation = {0.0f, 15.0f, -30.0f};
	////Vector3 Translation = {0.0f, 0.0f, -0.0f};

	//worldTramsform_.translation_.x = Translation.x;
	//worldTramsform_.translation_.y = Translation.y;
	//worldTramsform_.translation_.z = Translation.z;

	Vector3 Rotate = {0.3f, 0.0f, 0.0f};
	////Vector3 Rotate = {0.0f, 0.0f, 0.0f};

	viewProjection_.rotation_.x = Rotate.x;
	viewProjection_.rotation_.y = Rotate.y;
	viewProjection_.rotation_.z = Rotate.z;

	viewProjection_.Initialize();

	viewProjection_.farZ = 400.0f;

	input_ = Input::GetInstance();
}


Vector3 RailCamera::GetWorldTranslation() {
	
	Vector3 kCameraTranslation = {0.0f, 0.0f, 0.0f};

	 viewProjection_.rotation_.x += kCameraTranslation.x;	
	 viewProjection_.rotation_.y += kCameraTranslation.y;
	 viewProjection_.rotation_.z += kCameraTranslation.z;

	return viewProjection_.translation_;

}

Vector3 RailCamera::GetWorldRotate() {

	Vector3 kCameraRotate = {0.0f, 0.0f, 0.0f};

	viewProjection_.rotation_.x += kCameraRotate.x;
	viewProjection_.rotation_.y += kCameraRotate.y;
	viewProjection_.rotation_.z += kCameraRotate.z;

	return viewProjection_.rotation_;
}

void RailCamera::Update() {

	////GetWorldTranslation();
	////GetWorldRotate();

	
	const float kRotateSpeed = 0.02f;

	if (target_) {

		Vector3 offset = {0.0f, 14.0f, -40.0f};

		Matrix4x4 rotate = target_->matWorld_;

		offset = myMath_->TransformNormal(offset, rotate);

		viewProjection_.translation_.x = target_->translation_.x + offset.x;
		viewProjection_.translation_.y = target_->translation_.y + offset.y;
		viewProjection_.translation_.z = target_->translation_.z + offset.z;
	}

	 if (input_->PushKey(DIK_A)) {
		viewProjection_.rotation_.y -= kRotateSpeed;
	}
	 else if (input_->PushKey(DIK_D)) {
		viewProjection_.rotation_.y += kRotateSpeed;
	}
	


	viewProjection_.UpdateMatrix();
	//worldTramsform_.UpdateMatrix();

	viewProjection_.matView = myMath_->Inverse(worldTramsform_.matWorld_);


	//ImGui::Begin("Camera");
	//ImGui::DragFloat3("translation", &worldTramsform_.translation_.x, 0.1f, -100.0f,100.0f);
	//ImGui::DragFloat3("rotate", &worldTramsform_.rotation_.x, 0.01f, -10.0f, 10.0f);
	//ImGui::End();
}