#include "MyMath.h"
#include "WorldTransform.h"
void WorldTransform::UpdateMatrix() {

	static MyMath myMath_;
	matWorld_ = myMath_.MakeAffineMatrix(scale_, rotation_, translation_);

	TransferMatrix();
}