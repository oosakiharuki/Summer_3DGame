#include"Model.h"
#include "WorldTransform.h"


class Player {
public:

	void Initialize(Model* model,uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection& viewProjection);

private:

	uint32_t textureHandle_ = 0u;

	Model* model_ = nullptr;

	WorldTransform worldTransform_;


};