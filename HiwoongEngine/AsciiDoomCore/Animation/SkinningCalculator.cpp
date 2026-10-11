#include "SkinningCalculator.h"

namespace Hiwoong
{
	
	std::vector<ModelVertex> SkinningCalculator::Calculate(
		const ModelMesh& mesh,
		const std::vector<ModelMatrix>& nodeMatrices,
		const ModelMatrix& modelAlignment
	)
	{
		return mesh.vertices;
	}
}