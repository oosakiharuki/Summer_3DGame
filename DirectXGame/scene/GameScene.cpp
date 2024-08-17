#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete playerModel_;
	delete player_;
	delete skydomeModel_;
	delete skydome_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	
	viewProjection_.Initialize();

	//プレイヤー
	playerModel_ = Model::Create();
	textureHandle_ = TextureManager::Load("uvChecker.png");
	player_ = new Player();
	player_->Initialize(playerModel_, textureHandle_, &viewProjection_);

	//スカイドーム
	skydomeModel_ = Model::CreateFromOBJ("skydome", true);
	textureHandleSkydome_ = TextureManager::Load("skydome/skydome.png");
	skydome_ = new Skydome();
	skydome_->Initialize(skydomeModel_, textureHandleSkydome_, &viewProjection_);


	//デバッグカメラ
	debugCamera_ = new DebugCamera(1280, 720);
#ifdef _DEBUG
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
#endif
}

void GameScene::Update() { 
	
	// プレイヤー更新
	player_->Update();

	skydome_->Update();

	if (input_->PushKey(DIK_A)) {
		viewProjection_.rotation_.y -= 0.02f;
	}
	if (input_->PushKey(DIK_D)) {
		viewProjection_.rotation_.y += 0.02f;
	}
	if (input_->PushKey(DIK_W)) {
		viewProjection_.rotation_.x -= 0.02f;
	}
	if (input_->PushKey(DIK_S)) {
		viewProjection_.rotation_.x += 0.02f;
	}

	if (input_->PushKey(DIK_K)) {
		viewProjection_.translation_.y -= 0.2f;
		viewProjection_.translation_.z -= 0.2f;
	}
	if (input_->PushKey(DIK_I)) {
		viewProjection_.translation_.y += 0.2f;
		viewProjection_.translation_.z += 0.2f;
	}

#ifdef _DEBUG
	debugCamera_->Update();

	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = true;
	}
#endif
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	skydome_->Draw(viewProjection_);
	
	
	
	// プレイヤー描画
	player_->Draw();

	if (isDebugCameraActive_ == true) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
