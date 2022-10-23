//--------------------------------------------------------------------------------------
// Class encapsulating a model
//--------------------------------------------------------------------------------------
// Holds a pointer to a mesh as well as position, rotation and scaling, which are converted to a world matrix when required
// This is more of a convenience class, the Mesh class does most of the difficult work.
#pragma once



#include "Mesh.h"
#include "Misc/TempHelpers.h"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>   

namespace Engine
{
    class  Model
    {
    public:
        //-------------------------------------
        // Construction / Usage
        //-------------------------------------

        Model(Mesh* mesh, glm::vec3 position = { 0,0,0 }, glm::vec3 rotation = { 0,0,0 }, float scale = 1);


        // The render function simply passes this model's matrices over to Mesh:Render.
        // All other per-frame constants must have been set already along with shaders, textures, samplers, states etc.
        void Render();



        //-------------------------------------
        // Data access
        //-------------------------------------

        //********************************
        // All functions now accept a "node" parameter which specifies which node in the hierarchy to use. Defaults to 0, the root.
        // The hierarchy is stored in depth-first order

        // Getters - model only stores matrices. Position, rotation and scale are extracted if requested.
        glm::vec3 Position(int node = 0) 
        { 
			return glm::column(mWorldMatrices[node], 3);
        }         // Position is on bottom row of matrix
        glm::vec3 Rotation(int node = 0) 
        { 
            glm::vec3 out;
            glm::extractEulerAngleXYZ(mWorldMatrices[node], out.x , out.y, out.z);
            return out;
        }  // Getting angles from a matrix is complex - see .cpp file
        glm::vec3 Scale(int node = 0)
        {
            return{ glm::length(glm::column(mWorldMatrices[node], 0)),
                    glm::length(glm::column(mWorldMatrices[node], 1)),
                    glm::length(glm::column(mWorldMatrices[node], 2)) };

        } // Scale is length of rows 0-2 in matrix
        glm::mat4 WorldMatrix(int node = 0) 
        { 
            return mWorldMatrices[node];
        }

        // Setters - model only stores matricies , so if user sets position, rotation or scale, just update those aspects of the matrix
        void SetPosition(glm::vec3 position, int node = 0) 
        { 
			temp::SetColumn(mWorldMatrices[node], 3, position);
        }

        void SetRotation(glm::vec3 rotation, int node = 0)
        {
            glm::mat4 SZ = temp::MatrixMultiplication(temp::MatScaling(Scale(node)), temp::MatRotZ(rotation.z));
            glm::mat4 XY = temp::MatrixMultiplication(temp::MatRotX(rotation.x), temp::MatRotY(rotation.y));
            glm::mat4 SZXY = temp::MatrixMultiplication(SZ, XY);
            mWorldMatrices[node] = temp::MatrixMultiplication(SZXY, temp::MatTranslation(Position(node))); 
        }

        // Two ways to set scale: x,y,z separately, or all to the same value
        // To set scale without affecting rotation, normalise each row, then multiply it by the scale value.
        void SetScale(glm::vec3 scale, int node = 0)
        {
			temp::SetColumn(mWorldMatrices[node], 0, glm::normalize(glm::column(mWorldMatrices[node], 0)) * scale.x);
			temp::SetColumn(mWorldMatrices[node], 1, glm::normalize(glm::column(mWorldMatrices[node], 1)) * scale.y);
			temp::SetColumn(mWorldMatrices[node], 2, glm::normalize(glm::column(mWorldMatrices[node], 2)) * scale.z);
        }
        void SetScale(float scale) { SetScale({ scale, scale, scale }); }

        void SetWorldMatrix(glm::mat4 matrix, int node = 0)
        { 
            mWorldMatrices[node] = matrix;
        }

        //-------------------------------------
        // Private data / members
        //-------------------------------------
    private:
        Mesh* mMesh;

        // World matrices for the model
        // Now that meshes have multiple parts, we need multiple matrices. The root matrix (the first one) is the world matrix
        // for the entire model. The remaining matrices are relative to their parent part. The hierarchy is defined in the mesh (nodes)
        std::vector<glm::mat4> mWorldMatrices;

        const float ROTATION_SPEED = 2.0f;
        const float MOVEMENT_SPEED = 50.0f;

    };
}

