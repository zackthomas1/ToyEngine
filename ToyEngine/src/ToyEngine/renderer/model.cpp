#include "pch.h"
#include "model.h"

namespace ToyEngine{
	Model::Model(const char* path, bool flip_vertically, bool gamma) :
		flip_vertically_(flip_vertically), gamma_(gamma), m_model_mat(1.0f)
	{
		loadModel(path);
	}

	void Model::loadModel(std::string path)
	{
		//Declare Importer object from Assimp namespace
		Assimp::Importer importer;

		//aiProcess_Triangulate: tells Assimp that model does not (entirely) consist of triangles. Transform all model primatives into triangles
		//aiProcess_FlipUVs: flips the texture coordinates on the y-axis where necessary during processing
		//aiProcess_GenNormals: creates normal vectors for each vertex if the model doesn't contain normal vectors.
		//aiProcess_SplitLargeMeshes: splits large meshes into smaller sub-meshes which is useful if your rendering has a maximum number of vertices allowed and can only process smaller meshes.
		//aiProcess_OptimizeMeshes: does the reverse by trying to join several meshes into one larger mesh, reducing drawing calls for optimization.
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			TY_CORE_ERROR(importer.GetErrorString());
			return;
		}

		directory_ = path.substr(0, path.find_last_of('/'));
		m_meshes.reserve(scene->mNumMeshes);

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		//Each node (possibly) contains a set of children.
		//First process the node in question, then process all the node's children. 
		//Recursively calls the same function on node children until all nodes have been processed. 

		//proccess all the node's meshes
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			//check each of the node's mesh indices and retrieve 
			//corresponding mesh by indexing the scene's mMeshes array
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.emplace_back(processMesh(mesh, scene));
		}

		//do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}

	Ref<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// process vertices (position, normal, texcoord)
		std::vector<Vertex> vertices;
		vertices.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			if (mesh->mTextureCoords[0]) {
				vertices.emplace_back(Vertex(
					glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),    // position
					glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),       // normal 
					glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)           // texcoord
				));
			}
			else {
				vertices.emplace_back(Vertex(
					glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),    // position
					glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),       // normal 
					glm::vec2(0.0f)                                                                 // texcoord
				));
			}
		}

		//process indices 
		std::vector<unsigned int> indices;
		indices.reserve(mesh->mNumFaces * 3);
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		//process material 
		if (mesh->mMaterialIndex >= 0) {
			std::vector <Ref<Texture2D>> textures;
			aiMaterial * mat  = scene->mMaterials[mesh->mMaterialIndex];
			loadMaterialTextures(mat, aiTextureType_DIFFUSE, textures);
			loadMaterialTextures(mat, aiTextureType_SPECULAR, textures);

			return MakeRef<Mesh>(vertices, indices, MakeRef<Material>(textures));
		}
		else {
			TY_CORE_ERROR("TODO: Implement Mesh constructor no Material parameter"); 
			//return MakeRef<Mesh>(vertices, indices);
		}
	}

	void Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, Vector<Ref<Texture2D>> &textures)
	{
		//iterates over all the texture locations of the given texture type, retrieves the texture's file location, 
		//then loads and generates the texture and stores the information in a Texture2D object.
		textures.reserve(mat->GetTextureCount(type));

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
			// Most scenes re-use several of their textures onto several meshes; 
			// Loading textures is not an expensive operation. Avoid loading and generating the same texture for each mesh.
			// Reuse previously generated textuers, to avoid texture loading bottleneck.
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			for (unsigned int j = 0; j < m_textures.size(); j++)
			{
				// Wherever a load a texture, we first check if it hasn't been loaded already.
				// If so, take texture and skip the entire loading routine, saving a lot of processing power.
				// To be able to compare textures we need to store their path as well
				std::string mat_path = directory_ + "/" + std::string(str.C_Str()).c_str();
				if (std::strcmp(m_textures[j]->m_path.data(), mat_path.c_str()) == 0)
				{
					textures.emplace_back(m_textures[j]);
					skip = true;
					break;
				}
			}
			if (!skip) {
				aiString str;
				mat->GetTexture(type, i, &str);

				// Load texture
				std::string filename = directory_ + "/" + std::string(str.C_Str());
				Ref<Texture2D> texture =  MakeRef<Texture2D>(filename, static_cast<eTextureType>(type), true);
				textures.emplace_back(texture);
				m_textures.push_back(texture);
			}
		}
	}

	Ref<Model> Model::Create(const char* path, bool flip_vertically, bool gamma)
	{
		return MakeRef<Model>(path, flip_vertically, gamma);
	}
}