#include "tfpch.h"
#include "MeshLoader.h"

namespace Tituf {  

    std::vector<Ref<VertexArray>> MeshLoader::LoadModel(const std::string& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        std::vector<Ref<VertexArray>> meshes;

        if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
            TF_CORE_ERROR("Error loading model: {0}", importer.GetErrorString());
            return meshes;
        }

        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            meshes.push_back(ProcessMesh(scene->mMeshes[i]));
        }

        return meshes;
    }

    Ref<VertexArray> MeshLoader::ProcessMesh(const aiMesh* mesh)
    {
        std::vector<float> vertices;
        std::vector<uint32_t> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            vertices.push_back(mesh->mVertices[i].x);
            vertices.push_back(mesh->mVertices[i].y);
            vertices.push_back(mesh->mVertices[i].z);

            if (mesh->HasNormals()) {
                vertices.push_back(mesh->mNormals[i].x);
                vertices.push_back(mesh->mNormals[i].y);
                vertices.push_back(mesh->mNormals[i].z);
            }
            else {
                vertices.push_back(0); vertices.push_back(0); vertices.push_back(1);
            }

            if (mesh->mTextureCoords[0]) {
                vertices.push_back(mesh->mTextureCoords[0][i].x);
                vertices.push_back(mesh->mTextureCoords[0][i].y);
            }
            else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            const aiFace& face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        auto VA = Ref<VertexArray>(VertexArray::Create());
        auto VB = Ref<VertexBuffer>(VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float)));

        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal" },
            { ShaderDataType::Float2, "a_TexCoord" }
        };
        VB->SetLayout(layout);
        VA->AddVertexBuffer(VB);

        auto IB = Ref<IndexBuffer>(IndexBuffer::Create(indices.data(), indices.size()));
        VA->SetIndexBuffer(IB);

        return VA;
    }

} // namespace Tituf
