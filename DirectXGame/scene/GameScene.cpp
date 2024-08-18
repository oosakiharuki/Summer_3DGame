#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete playerModel_;
	delete player_;
	
	for (Enemy* enemy_ : enemies_) {
		delete enemy_;
	}
	delete enemyModel_;

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
	playerTextureHandle_ = TextureManager::Load("uvChecker.png");
	
	player_ = new Player();
	player_->Initialize(playerModel_, playerTextureHandle_, &viewProjection_);


	//敵
	enemyModel_ = Model::Create();
	enemyTextureHandle_ = TextureManager::Load("uvChecker.png");

	//for (uint32_t i; i < 1; i++) {
	//	enemy_ = new Enemy();
	//	enemy_->Initialize(playerModel_, enemyTextureHandle_, &viewProjection_);
	//	enemy_->SetPlayer(player_);
	//}

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

void GameScene::EnemyBorn(Vector3 position) {
	// 敵生成
	//for (int32_t i = 0; i < 1; i++) {
		Enemy* enemy_ = new Enemy();
		enemy_->Initialize(enemyModel_, enemyTextureHandle_, &viewProjection_, position);
		enemy_->SetGameScene(this);

		enemy_->SetPlayer(player_);
		enemies_.push_back(enemy_);
	//}
}


void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	//const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;

	// case radius
	const float plaeyrRadius = 1.0f;

	const float EnemyRadius = 1.0f;

	//for (EnemyBullet* bullet : enemyBullets) {

	//	posA = player_->GetWorldPosition();
	//	posB = bullet->GetWorldPosition();

	//	Vector3 distance{};

	//	distance.x = (posB.x - posA.x) * (posB.x - posA.x);
	//	distance.y = (posB.y - posA.y) * (posB.y - posA.y);
	//	distance.z = (posB.z - posA.z) * (posB.z - posA.z);

	//	float L;

	//	L = (plaeyrRadius + EnemyRadius) * (plaeyrRadius + EnemyRadius);

	//	if (distance.x + distance.y + distance.z <= L) {

	//		player_->OnCollision();
	//		bullet->OnCollision();
	//	}
	//}

	for (Enemy* enemy_ : enemies_) {
		for (PlayerBullet* bullet : playerBullets) {

			posA = enemy_->GetWorldPosition();
			posB = bullet->GetWorldPosition();

			Vector3 distance{};

			distance.x = (posB.x - posA.x) * (posB.x - posA.x);
			distance.y = (posB.y - posA.y) * (posB.y - posA.y);
			distance.z = (posB.z - posA.z) * (posB.z - posA.z);

			float L;

			L = (plaeyrRadius + EnemyRadius) * (plaeyrRadius + EnemyRadius);

			if (distance.x + distance.y + distance.z <= L) {

				enemy_->OnCollision();
				bullet->OnCollision();
			}
		}
	}


	for (Enemy* enemy : enemies_) {

		posA = player_->GetWorldPosition();
		posB = enemy->GetWorldPosition();

		Vector3 distance{};

		distance.x = (posB.x - posA.x) * (posB.x - posA.x);
		distance.y = (posB.y - posA.y) * (posB.y - posA.y);
		distance.z = (posB.z - posA.z) * (posB.z - posA.z);

		float L;

		L = (plaeyrRadius + EnemyRadius) * (plaeyrRadius + EnemyRadius);

		if (distance.x + distance.y + distance.z <= L) {

			player_->OnCollision();
			enemy->OnCollision(); // ここ　phaceで攻撃に変える
		}
	}
	
}

//void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { enemyBullets_.push_back(enemyBullet); }

void GameScene::LoadEnemyPopData() {

	std::ifstream file;
	file.open("Resources/EnemyPop1.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands() {

	// if A
	if (WaitFlag) {
		waitTimer--;
		if (waitTimer <= 0) {
			WaitFlag = false;
		}
		return;
	}

	std::string line;

	while (getline(enemyPopCommands, line)) {

		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			EnemyBorn(Vector3(x, y, z));

		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			WaitFlag = true;
			waitTimer = waitTime;

			break; // 待機時間にif Aを使うため一度while文から抜ける
		}
	}
}



void GameScene::Update() { 
	
	// プレイヤー更新
	player_->Update();

	LoadEnemyPopData();
	UpdateEnemyPopCommands();

	for (Enemy* enemy_ : enemies_) {
		enemy_->Update();
	}
	skydome_->Update();


	// Bullet Dead Timer
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {

			delete enemy;
			return true;
		}
		return false;
	});

	//当たり判定
	CheckAllCollisions();



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

	for (Enemy* enemy_ : enemies_) {
		enemy_->Draw();
	}

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
