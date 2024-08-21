#include "DirectXCommon.h"
#include "TextureManager.h"
#include "GameClearScene.h"
#include <cassert>

void GameClearScene::Initialize() {

	assert(modelTitle_);

	textureHandle_ = TextureManager::Load("uvChecker.png");
	modelTitle_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();

}

void GameClearScene::Updata() {
	worldTransform_.TransferMatrix();
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void GameClearScene::Draw() {
	
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	modelTitle_->Draw(worldTransform_, viewProjection_,textureHandle_); // koko ga okashii

}