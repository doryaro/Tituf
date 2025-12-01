#pragma once
#include "VertexArray.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
   
namespace Tituf {

    class MeshLoader
    {
    public:
        MeshLoader() = default;
        ~MeshLoader() = default;

        // Load a model from a file
        std::vector<Ref<VertexArray>> LoadModel(const std::string& path);

    private:
        Ref<VertexArray> ProcessMesh(const aiMesh* mesh);
    };

} // namespace Tituf
