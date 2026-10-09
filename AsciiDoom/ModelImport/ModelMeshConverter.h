#pragma once

#include "ModelImport/ModelMesh.h"
#include "Render/Mesh.h"
/// <summary>
/// ModelMesh를 Mesh로 변환
/// </summary>
namespace Hiwoong
{
    Mesh ConvertToRenderMesh(const ModelMesh& modelMesh);
}