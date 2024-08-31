#include "Player.h"
#include <cassert>
#include "TextureManager.h"
#include "RailCamera.h"

Player::~Player() {
	for (PlayerBullet* bullet_ : bullets_) {
		delete bullet_;
	}

	for (uint32_t i = 0; i < 5; i++) {
		delete sprite[i];
	}
	delete spriteS;
}

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

	viewProjection_ = viewProjection;
	input_ = Input::GetInstance();

	modelBullet_ = Model::CreateFromOBJ("bullet", true);
	bulletDirection = {0, 0, 0};

	for (uint32_t i = 0; i < 5; i++) {

		textureHandleHp[i]= TextureManager::Load("hp-Icon.png");

		sprite[i] = Sprite::Create(
		    textureHandleHp[i], {10.0f + (60.0f * i), 10.0f});
	}
	textureHandleSETUMEI = TextureManager::Load("HUD-sousa.png");
	spriteS = Sprite::Create(textureHandleSETUMEI, {1142.0f, 10.0f});

	audio_ = Audio::GetInstance();
	soundDataHandle[0] = audio_->LoadWave("bullet.mp3");
	soundDataHandle[1] = audio_->LoadWave("damage.mp3");
}

void Player::Rotate() {
	// Rotate Speed
	const float kRotSpeed = 0.02f;

	// Bectol henkou
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
		bulletDirection.y -= kRotSpeed;
	} 
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
		bulletDirection.y += kRotSpeed;
	}
}

//bullet
void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {

		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = myMath_->TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(modelBullet_, worldTransform_.translation_, velocity);

		newBullet->Rotate(bulletDirection);
		bullets_.push_back(newBullet);

		audio_->PlayWave(soundDataHandle[0], false, 0.08f);
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos{};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}



void Player::Damage() { 
	Hp--;
	audio_->PlayWave(soundDataHandle[1], false, 0.2f);
	if (Hp == 0) {
		isDead_ = true;
	} 
}

void Player::Update() {
	worldTransform_.TransferMatrix();

	worldTransform_.UpdateMatrix();

	Rotate();
	Attack();


	//delete bullet
	/// bullet_ != nullptr
	for (PlayerBullet* bullet_ : bullets_) {
		bullet_->Update();
	}

	/// Bullet Dead Timer
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {

			delete bullet;
			return true;
		}
		return false;
	});

#ifdef _DEBUG
	ImGui::Begin("ImGui");
	ImGui::DragFloat3("Player", &worldTransform_.translation_.x, 0.1f);
	ImGui::End();
#endif
}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

	for (PlayerBullet* bullet_ : bullets_) {
		bullet_->Draw(*viewProjection_);
	}

}

void Player::DrawSprite() {

	if (Hp > 4) {
		sprite[4]->Draw();
	}
	if (Hp > 3) {
		sprite[3]->Draw();
	}
	if (Hp > 2) {
		sprite[2]->Draw();
	}
	if (Hp > 1) {
		sprite[1]->Draw();
	}
	if (Hp > 0) {
		sprite[0]->Draw();
	}

	spriteS->Draw();
}