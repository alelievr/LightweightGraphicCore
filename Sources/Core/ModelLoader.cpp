#include "ModelLoader.hpp"

#include "Core/Textures/Texture2D.hpp"
#include "Utils/Utils.hpp"

#include "Core/Components/MeshRenderer.hpp"

#include ASSIMP_IMPORTER_INCLUDE
#include ASSIMP_POSTPROCESS_INCLUDE
#include ASSIMP_SCENE_INCLUDE

using namespace LWGC;

// Can't put this in the include file because we can't include assimp in the header
static GameObject *	SceneToGameObject(const std::string & name, const aiScene * scene);
static Mesh *		CreateMesh(aiMesh * mesh);
static Texture *	CreateTexture(aiTexture * texture);
static Material *	CreateMaterial(aiMaterial * material); // send textures too ?

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
	return SceneToGameObject(GetFileNameWithoutExtension(path), scene);
}

GameObject *	SceneToGameObject(const std::string & name, const aiScene * scene)
{
	std::vector< Material * > materials;
	GameObject * root = new GameObject();
	root->SetName(name);

	// Load all textures
	for (unsigned i = 0; i < scene->mNumTextures; i++)
	{
		CreateTexture(scene->mTextures[i]);
	}

	// Load all materials
	for (unsigned i = 0; i < scene->mNumMaterials; i++)
	{
		materials.push_back(CreateMaterial(scene->mMaterials[i]));
	}

	// Load all meshes
	for (unsigned i = 0; i < scene->mNumMeshes; i++)
	{
		auto mesh = CreateMesh(scene->mMeshes[i]);

		// add one game objects per meshes
		root->AddChild(new GameObject(new MeshRenderer(mesh, materials[scene->mMeshes[i]->mMaterialIndex])));
	}

	return root;
}

Mesh *		CreateMesh(aiMesh * mesh)
{

}

Texture *	CreateTexture(aiTexture * texture)
{

}

Material *	CreateMaterial(aiMaterial * material)
{
	for (unsigned j = 0; j < material->mNumProperties; j++)
	{
		auto prop = material->mProperties[j];
	}
}
