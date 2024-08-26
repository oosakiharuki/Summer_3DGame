#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "Ground.h"
#include "Boss.h"

#include "DebugCamera.h"

#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void EnemyBorn(Vector3 position, float direction);

	void CheckAllCollisions();

	//void AddEnemyBullet(EnemyBullet* enemyBullet);

	enum class Phase {
		kPlay,
		kDeath,
	};

	void Fire();

	void LoadEnemyPopData();
	void UpdateEnemyPopCommands();

	//void SetPerant(const WorldTransform* parent);

	bool IsFinishDead() const { return isFinishDead; }
	bool IsFinishClear() const { return isFinishClear; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	


	//ビューポート
	ViewProjection viewProjection_;

	//プレイヤー
	Player* player_ = nullptr;
	Model* playerModel_ = nullptr;
	uint32_t playerTextureHandle_ = 0u;

	//敵
	//Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;

	Model* enemyModel_ = nullptr;
	uint32_t enemyTextureHandle_ = 0u;
	
	std::stringstream enemyPopCommands;
	Vector3 enemyPosition;
	bool WaitFlag = false;
	int32_t waitTimer;
	bool isBornFinish = false;

	//ボス
	Boss* boss_ = nullptr;
	Model* bossModel_ = nullptr;
	Model* bossBulletModel_ = nullptr;
	uint32_t bossTextureHandle_ = 0u;

	std::list<BossBullet*> bossBullets_;


	float bulletTimer_ = 2.0f;
	static const int kFireTimer = 2;
	float deltaTimer_ = 1.0f / 60.0f;







	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	//スカイドーム
	Skydome* skydome_ = nullptr;
	Model* skydomeModel_ = nullptr; 
	uint32_t textureHandleSkydome_ = 0u;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	//地面
	Model* groundModel_ = nullptr;
	Ground* ground_ = nullptr;
	uint32_t groundTextureHandle_ = 0u;

	//シーン
	Phase phase_;
	bool isFinishDead = false;
	bool isFinishClear = false;

	MyMath* myMath_ = nullptr;
};