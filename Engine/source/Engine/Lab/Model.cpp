//--------------------------------------------------------------------------------------
// Class encapsulating a model
//--------------------------------------------------------------------------------------
// Holds a pointer to a mesh as well as position, rotation and scaling, which are converted to a world matrix when required
// This is more of a convenience class, the Mesh class does most of the difficult work.
#include "epch.h"
#include "Model.h"


#include "GraphicsHelpers.h"


namespace Engine
{
	Model::Model(std::shared_ptr<Mesh> mesh, glm::vec3 position /*= { 0,0,0 }*/, glm::vec3 rotation /*= { 0,0,0 }*/, float scale /*= 1*/)
		: mMesh(mesh)
	{
		// Set default matrices from mesh
		mWorldMatrices.resize(mesh->NumberNodes());
		for (int i = 0; i < mWorldMatrices.size(); ++i)
			mWorldMatrices[i] = mesh->GetNodeDefaultMatrix(i);
	}



	// The render function simply passes this model's matrices over to Mesh:Render.
	// All other per-frame constants must have been set already along with shaders, textures, samplers, states etc.
	void Model::Render()
	{
		mMesh->Render(mWorldMatrices);
	}

}