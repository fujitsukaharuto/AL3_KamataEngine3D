#include "WorldTransform.h"
#include "MathCal.h"

void WorldTransform::UpdateMatrix()
{

	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	TransferMatrix();

}