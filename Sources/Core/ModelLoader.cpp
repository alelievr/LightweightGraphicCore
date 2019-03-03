#include "ModelLoader.hpp"

#include "Core/Textures/Texture2D.hpp"

#include ASSIMP_IMPORTER_INCLUDE
#include ASSIMP_POSTPROCESS_INCLUDE

using namespace LWGC;

GameObject *		ModelLoader::Load(const std::string & path, bool optimize) noexcept
{
	// Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene *scene = importer.ReadFile(path,
											aiProcess_CalcTangentSpace |		// We need tangents and bitangent for the shading
											aiProcess_GenNormals |				// We also need normals for the shading
											aiProcess_GenUVCoords |				// UVs too
											aiProcess_Triangulate |				// Transform every non-triangle face (quads, etc.) into triangles
											aiProcess_JoinIdenticalVertices |	// optimize identical vertices
											aiProcess_SortByPType |				// Separate meshes that are not directly connected (allow to have multiple gameobjects from one object file)
											(optimize ? aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph : 0)
	);

	// If the import failed, report it
	if (!scene)
	{
		std::cerr << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	// Now we can access the file's contents and convert it to gameobjects
	return SceneToGameObject(scene);
}

GameObject *	ModelLoader::SceneToGameObject(const aiScene * scene)
{
	GameObject * root = new GameObject();

	// Load all textures
	for (unsigned i = 0; i < scene->mNumTextures; i++)
	{
		auto texture = scene->mTextures[i];
	}

	// Load all materials
	for (unsigned i = 0; i < scene->mNumMaterials; i++)
	{
		auto material = scene->mMaterials[i];
	}

	// Load all meshes
	for (unsigned i = 0; i < scene->mNumMeshes; i++)
	{
		auto mesh = scene->mMeshes[i];
	}

	return root;
}