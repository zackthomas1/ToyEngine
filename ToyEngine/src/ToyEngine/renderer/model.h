#pragma once
#include "ToyEngine/renderer/mesh/mesh.h"
#include "ToyEngine/renderer/material.h"
#include "ToyEngine/renderer/camera/camera.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace ToyEngine
{
	class Model
	{
	public:
		Model(const char* path, bool flip_vertically = false, bool gamma = false);

		static Ref<Model> Create(const char* path, bool flip_vertically = false, bool gamma = false);
	public:
		// Model data
		glm::mat4 m_model_mat;
		Vector<Ref<Mesh>> m_meshes;
		Vector<Ref<Texture2D>> m_textures;
	private:
		//
		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
		void loadMaterialTextures(aiMaterial* mat, aiTextureType type, Vector<Ref<Texture2D>> &textures);
	private:
		std::string directory_;
		bool flip_vertically_, gamma_;
	};
}