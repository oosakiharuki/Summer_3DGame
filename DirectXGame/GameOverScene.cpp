#include "DirectXCommon.h"
#include "TextureManager.h"
#include "GameOverScene.h"
#include <cassert>

void GameOverScene::Initialize() {
	assert(modelTitle_);

	textureHandle_ = TextureManager::Load("GameOverTexture.png");
	sprite_ = Sprite::Create(textureHandle_, {0, 0});

	modelSky_ = Model::CreateFromOBJ("skydome", true);
	modelGround_ = Model::CreateFromOBJ("ground", true);
	worldTransform_.Initialize();
	viewProjection_.Initialize();
}

void GameOverScene::Updata() {
	worldTransform_.TransferMatrix();
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void GameOverScene::Draw() {

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	modelSky_->Draw(worldTransform_, viewProjection_);
	modelGround_->Draw(worldTransform_, viewProjection_);

	Sprite::PreDraw(commandList);

	sprite_->Draw();

	Sprite::PostDraw();
}