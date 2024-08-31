#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"

#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Sprite.h"

class Fade;

class TitleScene {
public:

	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	~TitleScene();
	void Initialize();
	void Updata();
	void Draw();
	bool IsFinished() const { return finished_; }

private:
	Model* modelTitle_ = nullptr;
	Model* modelSky_ = nullptr;
	Model* modelGround_ = nullptr;
	Model* modelplayer_ = nullptr;

	WorldTransform worldTransform_;
	WorldTransform worldTransform2_;
	WorldTransform worldTransform3_;

	uint32_t textureHandle_ = 0u;
	Sprite* sprite_ = nullptr;


	ViewProjection viewProjection_;

	bool finished_ = false;

	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;


	float deltaTimer_;
	static inline float kWideStart = 1.0f;
	static inline float kWideEnd = -1.0f;
	static inline float kTime = 4.0f;
	    

};