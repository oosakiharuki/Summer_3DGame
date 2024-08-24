#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

	viewProjection_ = viewProjection;
}

void Ground::Update() {

}

void Ground::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}