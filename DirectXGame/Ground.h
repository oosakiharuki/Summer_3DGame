#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Ground{
public:
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);
	void Update();
	void Draw();

private:
	Model* model_ = nullptr;
	uint32_t textureHandle_;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
};