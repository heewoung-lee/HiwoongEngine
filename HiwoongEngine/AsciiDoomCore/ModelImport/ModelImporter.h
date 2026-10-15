#pragma once

#include "IModelImporter.h"
#include <memory>

namespace Hiwoong
{
    class IModelSourceReader;
    class IMeshLoader;
    class ISkeletonLoader;
    class ISkinWeightLoader;
    class IAnimationLoader;

    class ModelImporter final : public IModelImporter
    {
    public:
        ModelImporter(
            std::shared_ptr<const IModelSourceReader> sourceReader,
            std::shared_ptr<const IMeshLoader> meshLoader,
            std::shared_ptr<const ISkeletonLoader> skeletonLoader,
            std::shared_ptr<const ISkinWeightLoader> skinWeightLoader,
            std::shared_ptr<const IAnimationLoader> animationLoader
        );

        ModelImportResult Import(
            const std::string& filePath,
            float frontRotationY = 0.0f
        ) const override;

        bool ImportAnimation(
            ModelData& model,
            const std::string& filePath,
            const std::string& name
        ) const override;

    private:
        //모델외형의 방향을 내 게임엔진에 맞춘다.
        void AlignModel(
            ModelData& model,
            float frontRotationY
        ) const;

    private:
        std::shared_ptr<const IModelSourceReader> sourceReader;
        std::shared_ptr<const IMeshLoader> meshLoader;
        std::shared_ptr<const ISkeletonLoader> skeletonLoader;
        std::shared_ptr<const ISkinWeightLoader> skinWeightLoader;
        std::shared_ptr<const IAnimationLoader> animationLoader;
    };


}