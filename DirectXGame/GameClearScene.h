#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Sprite.h"

class GameClearScene{
public:
	void Initialize();
	void Updata();
	void Draw();
	bool IsFinished() const { return finished_; }

private:
	Model* modelSky_ = nullptr;
	Model* modelGround_ = nullptr;

	WorldTransform worldTransform_;

	ViewProjection viewProjection_;

	bool finished_ = false;

	uint32_t textureHandle_ = 0u;
	Sprite* sprite_;
};