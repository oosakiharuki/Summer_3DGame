#include "Fade.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include <algorithm>
#include <cassert>

void Fade::Intialize() {
	textureHandle_ = TextureManager::Load("white1x1.png");
	sprite_ = Sprite::Create(textureHandle_, Vector2(0, 0));
	sprite_->SetSize(Vector2(1280, 720));
	sprite_->SetColor(Vector4(0, 0, 0, 1));
}

void Fade::Start(Status status, float duration) {
	status_ = status;
	duration_ = duration;
	countTimer_ = 0.0f;
}

void Fade::Stop() { status_ = Status::None; }

bool Fade::IsFinished() const {
	switch (status_) {
	case Status::None:
		break;
	case Status::FadeIn:
	case Status::FadeOut:
		if (countTimer_ >= duration_) {
			return true;
		} else {
			return false;
		}
	}

	return true;
}

void Fade::Update() {
	switch (status_) {
	case Status::None:
		break;
	case Status::FadeIn:
		countTimer_ += 1.0f / 60.0f;

		if (countTimer_ >= duration_) {
			countTimer_ = duration_;
		}

		sprite_->SetColor(Vector4(1, 1, 1, std::clamp(1 - countTimer_ / duration_, 0.0f, 1.0f)));

		break;
	case Status::FadeOut:
		countTimer_ += 1.0f / 60.0f;

		if (countTimer_ >= duration_) {
			countTimer_ = duration_;
		}

		sprite_->SetColor(Vector4(1, 1, 1, std::clamp(countTimer_ / duration_, 0.0f, 1.0f)));
		break;
	}
}

void Fade::Draw(ID3D12GraphicsCommandList* commandList) {

	if (status_ == Status::None) {
		return;
	}

	Sprite::PreDraw(commandList);
	sprite_->Draw();
	Sprite::PostDraw();
}