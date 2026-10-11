#include "SkinWeightLoader.h"
#include "ModelImportContext.h"
#include "IModelSource.h"
#include <algorithm>
#include <cmath>
namespace Hiwoong
{
	bool SkinWeightLoader::Load(const ModelImportContext& context, std::vector<ModelMesh>& meshes) const
	{
        //원본이 없거나, 메시 갯수와 대응표가 다르면 중단
        if (context.source == nullptr ||
            context.meshMappings.size() != meshes.size())
        {
            return false;
        }

        //메쉬들과 대응표를 꺼내서 메시 번호와 정점갯수를 확인.
        for (std::size_t meshIdx = 0; meshIdx < meshes.size(); ++meshIdx)
        {
            ModelMesh& mesh = meshes[meshIdx];
            const MeshSourceMapping& mapping = context.meshMappings[meshIdx];

            if (mapping.sourceMeshIndex >= context.source->GetMeshCount() ||
                mapping.sourceVertexIndices.size() != mesh.vertices.size())
            {
                return false;
            }
            // 원본 메시의 관절 바인딩을 현재 메시에 보관한다.
            mesh.skinBindings =
                context.source->GetSkinBindings(mapping.sourceMeshIndex);

            const auto sourceWeights =
                context.source->GetSkinWeights(mapping.sourceMeshIndex);

            mesh.skinWeights.clear();

            //대응표에 적힌 원본 번호를 출력 정점 순서대로 읽는다.
            //그리고 SkinWeightLoader에서 각 정점의 가중치 합을 1로 맞춘다
            //예를들어 1,3 이라면 0.25 0.75로 비율로 변환한다.
            for (std::size_t sourceVertexIdx : mapping.sourceVertexIndices)
            {
                if (sourceVertexIdx >= sourceWeights.size())
                    return false;

                auto vertexWeights = sourceWeights[sourceVertexIdx];
                //유효 숫자 확인
                for (const SkinWeight& weight : vertexWeights)
                {
                    if (!std::isfinite(weight.weight) || weight.weight < 0.0f)
                    {
                        return false;
                    }
                }


                // 가중치가 큰 순서로 정렬한다.
                // 이걸 왜 하냐면 애니메이션이 돌 때 모든 가중치가 있는 관절을 움직인다 하면
                // 프레임 드랍이 심함. 그래서 별로 티가 안나는 계산을 하는 가중치는 무시하고
                // 가중치가 제일 높은 상위 4개만 뽑아서 관절영향을 주게 만듦.
                // 즉 최적화,
                // 순서는 정렬 -> 유효가 큰 가중치 4개 남기기 -> 합계산 -> 나누기.
                std::sort(
                    vertexWeights.begin(),
                    vertexWeights.end(),
                    [](const SkinWeight& a, const SkinWeight& b)
                    {
                        return a.weight > b.weight;
                    }
                );

                // 영향력이 큰 네 개만 남긴다.
                if (vertexWeights.size() > 4)
                {
                    vertexWeights.resize(4);
                }

                float totalWeight = 0.0f;

                for (const SkinWeight& weight : vertexWeights)
                {
                    totalWeight += weight.weight;
                }

                if (totalWeight > 0.0f)
                {
                    for (SkinWeight& weight : vertexWeights)
                    {
                        weight.weight /= totalWeight;
                    }
                }

                mesh.skinWeights.push_back(vertexWeights);
            }
        }

        return true;
	}

}
