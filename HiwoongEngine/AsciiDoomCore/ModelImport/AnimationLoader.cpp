#include "AnimationLoader.h"
#include "ModelImportContext.h"
#include "IModelSource.h"

namespace Hiwoong
{
    bool AnimationLoader::Load(
        const ModelImportContext& context,
        const std::vector<SkeletonNode>& modelNodes,
        std::vector<AnimationClip>& clips
    ) const
    {
        clips.clear();

        if (context.source == nullptr || modelNodes.empty())
            return false;

        clips = context.source->GetAnimationClips();

        const std::vector<SkeletonNode> sourceNodes =
            context.source->GetSkeletonNodes();

        std::vector<int> sourceNodeIndices;

        for (const SkeletonNode& modelNode : modelNodes)
        {
            const int sourceIndex =
                FindNodeIndex(sourceNodes, modelNode.name);

            sourceNodeIndices.push_back(sourceIndex);

        }
        for (AnimationClip& clip : clips)
        {
            clip.sourceNodeIndices = sourceNodeIndices;
        }


        return true;
    }
    int AnimationLoader::FindNodeIndex(
        const std::vector<SkeletonNode>& nodes,
        const std::string& name
    ) const
    {
        for (std::size_t i = 0; i < nodes.size(); ++i)
        {
            if (nodes[i].name == name)
                return static_cast<int>(i);
        }

        return -1;
    }
}