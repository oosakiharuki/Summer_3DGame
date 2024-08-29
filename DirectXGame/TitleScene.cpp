#include "TitleScene.h"
#include "Fade.h"
#include "TextureManager.h"
#include "DirectXCommon.h"
#include <cassert>

#define _USE_MATH_DEFINES
#include <math.h>

TitleScene::~TitleScene() { 
	delete fade_;
}

void TitleScene::Initialize() { 
	assert(modelTitle_);

	//textureHandle_ = TextureManager::Load("Title/Title.png");
	modelTitle_ = Model::CreateFromOBJ("Title", true);
	modelSky_ = Model::CreateFromOBJ("skydome", true);
	modelGround_ = Model::CreateFromOBJ("ground", true);
	modelplayer_ = Model::CreateFromOBJ("player", true);
	worldTransform_.Initialize();
	worldTransform2_.Initialize();
	worldTransform3_.Initialize();

	worldTransform3_.translation_.x = 5.0f;
	worldTransform3_.translation_.z = -25.0f;

	viewProjection_.Initialize();

	fade_ = new Fade();
	fade_->Intialize();
	fade_->Start(Fade::Status::FadeIn, 1);
}

void TitleScene::Updata() {

	
	deltaTimer_ += 1.0f / 60.0f;

	float wide = 2.0f * float(M_PI);

	float param = std::sin(wide * deltaTimer_ / kTime);
	float radian = kWideStart + kWideEnd * (param + 1.0f) / 2.0f;
	

	worldTransform2_.translation_.y = std::sin(radian);

	worldTransform2_.UpdateMatrix();
	worldTransform3_.UpdateMatrix();


	switch (phase_) {
	case Phase::kFadeIn:
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		fade_->Stop();
		worldTransform_.TransferMatrix();
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, 1);
			phase_ = Phase::kFadeOut;
		}
		break;
	case Phase::kFadeOut:

		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}

	fade_->Update();
}

void TitleScene::Draw() {

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	modelTitle_->Draw(worldTransform2_,viewProjection_);
	modelSky_->Draw(worldTransform_, viewProjection_);
	modelGround_->Draw(worldTransform_, viewProjection_);
	modelplayer_->Draw(worldTransform3_, viewProjection_);

	fade_->Draw(commandList);
}