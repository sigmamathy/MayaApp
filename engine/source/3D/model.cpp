#include <Maya/3D/model.hpp>
#include <Maya/math.hpp>
#include <Maya/texture.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>

namespace Maya {

Model3D::Model3D(std::string const& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
#if MAYA_DEBUG
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        MAYA_LOG("Error (Model3D::Model3D): Cannot load file \"" << path << "\"");
        return;
    }
#endif
    directory = std::filesystem::path(path).parent_path().string();
    ProcessNode(scene->mRootNode, scene);
}

Model3D::~Model3D()
{
    for (Mesh& mesh : meshes) delete mesh.vao;
    for (auto texture : textures) delete texture.second;
}

void Model3D::ProcessNode(void* node, void const* scene)
{
    aiNode* ainode = (aiNode*) node;
    aiScene const* aiscene = (aiScene const*) scene;

    for (unsigned int i = 0; i < ainode->mNumMeshes; i++)
    {
        aiMesh* mesh = aiscene->mMeshes[ainode->mMeshes[i]];
        ProcessMesh(mesh, aiscene);
    }
    
    for (unsigned int i = 0; i < ainode->mNumChildren; i++)
        ProcessNode(ainode->mChildren[i], aiscene);
}

void Model3D::ProcessMesh(void* mesh, void const* scene)
{
    aiMesh* aimesh = (aiMesh*) mesh;
    aiScene const* aiscene = (aiScene const*) scene;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    Mesh& result = meshes.emplace_back();

    vertices.reserve(aimesh->mNumVertices * 8);
    vertices.reserve(aimesh->mNumFaces * 3);

    for (unsigned int i = 0; i < aimesh->mNumVertices; i++)
    {
        vertices.push_back(aimesh->mVertices[i].x);
        vertices.push_back(aimesh->mVertices[i].y);
        vertices.push_back(aimesh->mVertices[i].z);

        vertices.push_back(aimesh->mNormals[i].x);
        vertices.push_back(aimesh->mNormals[i].y);
        vertices.push_back(aimesh->mNormals[i].z);

        auto tex = aimesh->mTextureCoords[0];
        vertices.push_back(tex ? tex[i].x : 0);
        vertices.push_back(tex ? tex[i].y : 0);
    }

    for (unsigned int i = 0; i < aimesh->mNumFaces; i++) {
        aiFace face = aimesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    result.vao = new VertexArray;
    result.vao->LinkVBO(vertices, VertexLayout(3, 3, 2));
    result.vao->LinkIBO(indices);

    if (aimesh->mMaterialIndex >= 0)
    {
        aiMaterial* mat = aiscene->mMaterials[aimesh->mMaterialIndex];
        LoadMaterialTextures(result, mat, aiTextureType_DIFFUSE, "u_texture_diffuse");
        LoadMaterialTextures(result, mat, aiTextureType_SPECULAR, "u_texture_specular");
    }
}

void Model3D::LoadMaterialTextures(Mesh& mesh, void* material, int type, std::string const& type_uniform)
{
    aiMaterial* aimat = (aiMaterial*) material;
    aiTextureType textype = (aiTextureType) type;

    for (unsigned int i = 0; i < aimat->GetTextureCount(textype); i++)
    {
        aiString str;
        aimat->GetTexture(textype, i, &str);

        if (!textures.count(str.C_Str()))
            textures[str.C_Str()] = new Texture(directory + str.C_Str());
        Texture* texture = textures.at(str.C_Str());
        mesh.texture_ptrs.push_back(texture);
        mesh.texture_uniform_names.push_back(type_uniform + "_" + std::to_string(i));
    }
}

void Model3D::Draw(Shader& shader) const
{
    for (Mesh const& mesh : meshes) {
        for (int i = 0; i < mesh.texture_ptrs.size(); i++) {
            mesh.texture_ptrs[i]->Bind(i);
            shader.SetUniform(mesh.texture_uniform_names[i], i);
        }
        mesh.vao->Draw();
    }
}

}