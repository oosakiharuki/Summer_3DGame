#include "DirectXCommon.h"
#include "TextureManager.h"
#include "GameOverScene.h"
#include <cassert>

void GameOverScene::Initialize() {
	assert(modelTitle_);

	// textureHandle_ = TextureManager::Load("Title/Title.png");
	modelTitle_ = Model::CreateFromOBJ("player", true);
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
	modelTitle_->Draw(worldTransform_, viewProjection_); // koko ga okashii
}