#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class GameClearScene{
public:
	void Initialize();
	void Updata();
	void Draw();
	bool IsFinished() const { return finished_; }

private:
	Model* modelMoji_ = nullptr;
	Model* modelSky_ = nullptr;
	Model* modelGround_ = nullptr;

	WorldTransform worldTransform_;

	//uint32_t textureHandle_ = 0u;

	ViewProjection viewProjection_;

	bool finished_ = false;
};