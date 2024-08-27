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

	for (BossBullet* bullet : bossBullets_) {
		delete bullet;
	}

	delete enemyModel_;

	delete skydomeModel_;
	delete skydome_;

	delete railCamera_;

	delete deathparticle_;

	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	
	soundDateHandle_ = audio_->LoadWave("BGM.mp3");

	viewProjection_.Initialize();

	// レールカメラ
	railCamera_ = new RailCamera();
	railCamera_->Initialize();


	//プレイヤー
	playerModel_ = Model::CreateFromOBJ("player",true);
	playerTextureHandle_ = TextureManager::Load("player/castle.png");
	
	player_ = new Player();
	//player_->SetParent(&railCamera_->GetWorldTransform());

	player_->Initialize(playerModel_, playerTextureHandle_, &viewProjection_);


	railCamera_->SetTarget(&player_->GetWorldTransfrom());

	//パーテイクル
	deathparticle_ = new DeathParticle();
	deathparticle_->Initialize(&viewProjection_, player_->GetWorldPosition());


	//敵
	enemyModel_ = Model::CreateFromOBJ("enemy",true);
	enemyTextureHandle_ = TextureManager::Load("enemy/enemy.png");

	//ボス
	boss_ = new Boss();
	bossModel_ = Model::CreateFromOBJ("enemy", true);
	bossBulletModel_ = Model::CreateFromOBJ("bullet", true);
	bossTextureHandle_ = TextureManager::Load("enemy/enemy.png");
	boss_->Initialize(bossModel_,bossTextureHandle_,&viewProjection_);
	boss_->SetPlayer(player_);


	//スカイドーム
	skydomeModel_ = Model::CreateFromOBJ("skydome", true);
	textureHandleSkydome_ = TextureManager::Load("skydome/skydome.png");
	skydome_ = new Skydome();
	skydome_->Initialize(skydomeModel_, textureHandleSkydome_, &viewProjection_);

	//地面
	groundModel_ = Model::CreateFromOBJ("ground", true);
	groundTextureHandle_ = TextureManager::Load("ground/ground.png");
	ground_ = new Ground();
	ground_->Initialize(groundModel_, groundTextureHandle_, &viewProjection_);
	
	
	phase_ = Phase::kFadeIn;
	fade_ = new Fade();
	fade_->Intialize();
	fade_->Start(Fade::Status::FadeIn, 1);
	isFinishDead = false;
	isFinishClear = false;
	isBornFinish = false;


	//デバッグカメラ
	debugCamera_ = new DebugCamera(1280, 720);
#ifdef _DEBUG
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
#endif
}

void GameScene::EnemyBorn(Vector3 position, float direction) {
	// 敵生成
	Enemy* enemy_ = new Enemy();
	enemy_->Initialize(enemyModel_, enemyTextureHandle_, &viewProjection_, position, direction);
	enemy_->SetGameScene(this);

	enemy_->SetPlayer(player_);
	enemies_.push_back(enemy_);
}


void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	const std::list<BossBullet*>& bossBullets = bossBullets_;

	// case radius
	const float plaeyrRadius = 1.0f;

	const float EnemyRadius = 1.0f;

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

		L = ((plaeyrRadius + 10.0f) + EnemyRadius) * ((plaeyrRadius + 10.0f) + EnemyRadius);

		if (distance.x + distance.y + distance.z <= L) {

			player_->OnCollision();
			enemy->OnStartFight(); // ここ　phaceで攻撃に変える
		}
	}
	
	if (isBornFinish && enemies_.empty()) {

		for (PlayerBullet* bullet : playerBullets) {

			posA = boss_->GetWorldPosition();
			posB = bullet->GetWorldPosition();

			Vector3 distance{};

			distance.x = (posB.x - posA.x) * (posB.x - posA.x);
			distance.y = (posB.y - posA.y) * (posB.y - posA.y);
			distance.z = (posB.z - posA.z) * (posB.z - posA.z);

			float L;

			L = (plaeyrRadius + EnemyRadius * 3) * (plaeyrRadius + EnemyRadius * 3);

			if (distance.x + distance.y + distance.z <= L) {

				boss_->OnCollision();
				bullet->OnCollision();
			}
		}
	}

	for (BossBullet* bossbullet : bossBullets) {

		posA = player_->GetWorldPosition();
		posB = bossbullet->GetWorldPosition();

		Vector3 distance{};

		distance.x = (posB.x - posA.x) * (posB.x - posA.x);
		distance.y = (posB.y - posA.y) * (posB.y - posA.y);
		distance.z = (posB.z - posA.z) * (posB.z - posA.z);

		float L;

		L = (plaeyrRadius + EnemyRadius * 3) * (plaeyrRadius + EnemyRadius * 3);

		if (distance.x + distance.y + distance.z <= L) {

			player_->Damage();
			bossbullet->OnCollision();
		}
	}

	for (BossBullet* bossbullet : bossBullets) {
		for (PlayerBullet* playerbullet : playerBullets) {
			
			posA = bossbullet->GetWorldPosition();
			posB = playerbullet->GetWorldPosition();

			Vector3 distance{};

			distance.x = (posB.x - posA.x) * (posB.x - posA.x);
			distance.y = (posB.y - posA.y) * (posB.y - posA.y);
			distance.z = (posB.z - posA.z) * (posB.z - posA.z);

			float L;

			L = (plaeyrRadius + EnemyRadius) * (plaeyrRadius + EnemyRadius);

			if (distance.x + distance.y + distance.z <= L) {

				playerbullet->OnCollision();
				bossbullet->OnCollision();
			}
		}
	}

}

//void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { enemyBullets_.push_back(enemyBullet); }

void GameScene::LoadEnemyPopData() {

	std::ifstream file;
	file.open("Resources/EnemyPop4.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	if (!isBornFinish) {

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

				getline(line_stream, word, ',');
				float direction = (float)std::atof(word.c_str()); // 向き

				EnemyBorn(Vector3(x, y, z), direction);

			} else if (word.find("WAIT") == 0) {
				getline(line_stream, word, ',');

				int32_t waitTime = atoi(word.c_str());

				WaitFlag = true;
				waitTimer = waitTime;

				break; // 待機時間にif Aを使うため一度while文から抜ける
			} else if (word.find("FINISH") == 0) {
				isBornFinish = true; // 敵を生みだすのを停止する
				break;
			}
		}
	}
}

//void GameScene::SetPerant(const WorldTransform* parent) {
//
//
//
//}

void GameScene::Fire() {

	bulletTimer_ -= deltaTimer_;

	if (bulletTimer_ <= 0) {

		assert(player_);

		const float kBulletSpeed = 0.5f;

		Vector3 playerPostion = player_->GetWorldPosition();
		Vector3 bossPostion = boss_->GetWorldPosition();

		Vector3 subtract = myMath_->Subtract(playerPostion, bossPostion);

		Vector3 normalize = myMath_->Normalize(subtract);

		normalize.x *= kBulletSpeed;
		normalize.y *= kBulletSpeed;
		normalize.z *= kBulletSpeed;

		Vector3 velocity(normalize);

		velocity = myMath_->TransformNormal(velocity, boss_->GetWorldTransform()->matWorld_);

		BossBullet* newBullet = new BossBullet();
		newBullet->Initialize(bossBulletModel_,&viewProjection_, bossPostion, velocity);

		bossBullets_.push_back(newBullet);
		bulletTimer_ = kFireTimer;
	}
}

void GameScene::ChangeScene() {
	switch (phase_) {
	case Phase::kFadeIn:
			phase_ = Phase::kPlay;

		break;
	case Phase::kPlay:
		if (player_->IsDead()) {
			phase_ = Phase::kDeath; // プレイヤーが倒されたとき	
		}
		if (boss_->IsDead()) {
			phase_ = Phase::kFadeOutC; // ボスを倒したとき
			fade_->Start(Fade::Status::FadeOut, 1);
		}
		break;
	case Phase::kDeath:
		if (deathparticle_->IsDead()) {
			phase_ = Phase::kFadeOutD;
			fade_->Start(Fade::Status::FadeOut, 1);
		} else {
			audio_->StopWave(voiceHandle_);
		}

		break;
	case Phase::kFadeOutD:

		if (fade_->IsFinished()) {
			isFinishDead = true;
		}
		break;
	case Phase::kFadeOutC:

		if (fade_->IsFinished()) {
			isFinishClear = true;
		} else {
			audio_->StopWave(voiceHandle_);
		}
		break;
	}
	
}


void GameScene::Update() {

	ChangeScene();

	switch (phase_) {
	case Phase::kFadeIn:
		break;
	case Phase::kPlay:

		// プレイヤー更新
		player_->Update();

		LoadEnemyPopData();
		UpdateEnemyPopCommands();

		for (Enemy* enemy_ : enemies_) {
			enemy_->Update();
		}
		skydome_->Update();

		ground_->Update();
		railCamera_->Update();

		viewProjection_.translation_ = railCamera_->GetWorldTranslation();
		viewProjection_.rotation_ = railCamera_->GetWorldRotate();

		// Bullet Dead Timer
		enemies_.remove_if([](Enemy* enemy) {
			if (enemy->IsDead()) {

				delete enemy;
				return true;
			}
			return false;
		});

		/// bullet_ != nullptr
		for (BossBullet* bullet_ : bossBullets_) {
			bullet_->Update();
		}

		// delete bullet
		/// Bullet Dead Timer
		bossBullets_.remove_if([](BossBullet* bullet) {
			if (bullet->IsDead()) {

				delete bullet;
				return true;
			}
			return false;
		});

		// 当たり判定
		CheckAllCollisions();

#ifdef _DEBUG
		debugCamera_->Update();

		if (input_->TriggerKey(DIK_F5)) {
			isDebugCameraActive_ = true;
		}
#endif



		// if (isBornStop && enemies_.empty()) {
		//	isBornStop = true; // ウェーブ中の全て倒した時
		// }

		if (isBornFinish && enemies_.empty()) { // 全て倒した時
			boss_->Update();
			Fire();
		}



		if (audio_->IsPlaying(voiceHandle_) == 0 || voiceHandle_ == -1) {
			voiceHandle_ = audio_->PlayWave(soundDateHandle_, false, 0.05f);
		}

		break;
	case Phase::kDeath:

		
		deathparticle_->Updata();


		// skydome_->Update();

		// for (Enemy* enemy_ : enemies_) {
		//	enemy_->Update();
		// }

		//// Enemy Dead Timer
		// enemies_.remove_if([](Enemy* enemy) {
		//	if (enemy->IsDead()) {

		//		delete enemy;
		//		enemy = nullptr;
		//		return true;
		//	}
		//	return false;
		//});
		//// 当たり判定
		// CheckAllCollisions();
		break;
	case Phase::kFadeOutD:
		break;
	case Phase::kFadeOutC:
		break;
	}
fade_->Update();
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

	switch (phase_) {
	case Phase::kFadeIn:
		break;
	case Phase::kPlay:

		skydome_->Draw(viewProjection_);

		for (Enemy* enemy_ : enemies_) {
			enemy_->Draw();
		}

		if (isBornFinish && enemies_.empty()) {
			boss_->Draw();
		}


		for (BossBullet* bullet_ : bossBullets_) {
			bullet_->Draw();
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

		ground_->Draw();

		break;
	case Phase::kDeath:

		deathparticle_->Draw();
		skydome_->Draw(viewProjection_);

		// プレイヤー描画
		//player_->Draw();

		for (Enemy* enemy_ : enemies_) {
			enemy_->Draw();
		}
		if (isBornFinish && enemies_.empty()) {
			boss_->Draw();
		}

		viewProjection_.UpdateMatrix();

		ground_->Draw();

		break;
	case Phase::kFadeOutD:
		
	case Phase::kFadeOutC:
		deathparticle_->Draw();
		skydome_->Draw(viewProjection_);

		// プレイヤー描画
		// player_->Draw();

		for (Enemy* enemy_ : enemies_) {
			enemy_->Draw();
		}
		if (isBornFinish && enemies_.empty()) {
			boss_->Draw();
		}

		viewProjection_.UpdateMatrix();

		ground_->Draw();

	}
	fade_->Draw(commandList);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player_->DrawSprite();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
