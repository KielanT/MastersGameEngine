#pragma once

#include "glm/gtc/constants.hpp"

namespace Engine
{
	class Camera
	{
	public:
		//-------------------------------------
		// Construction and Usage
		//-------------------------------------

		// Constructor - initialise all settings, sensible defaults provided for everything.
		Camera(glm::vec3 position = { 0,0,0 }, glm::vec3 rotation = { 0,0,0 },
			float fov = glm::pi<float>() / 3, float aspectRatio = 16.0f / 9.0f, float nearClip = 0.1f, float farClip = 10000.0f)
			: mPosition(position), mRotation(rotation), mFOVx(fov), mAspectRatio(aspectRatio), mNearClip(nearClip), mFarClip(farClip)
		{
		}


		// Control the camera's position and rotation using keys provided
		void Control(float frameTime);


		//-------------------------------------
		// Data access
		//-------------------------------------

		// Getters / setters
		glm::vec3 Position() { return mPosition; }
		glm::vec3 Rotation() { return mRotation; }
		void SetPosition(glm::vec3 position) { mPosition = position; }
		void SetRotation(glm::vec3 rotation) { mRotation = rotation; }

		float FOV() { return mFOVx; }
		float NearClip() { return mNearClip; }
		float FarClip() { return mFarClip; }

		void SetFOV(float fov) { mFOVx = fov; }
		void SetNearClip(float nearClip) { mNearClip = nearClip; }
		void SetFarClip(float farClip) { mFarClip = farClip; }

		// Read only access to camera matrices, updated on request from position, rotation and camera settings
		glm::mat4 ViewMatrix() { UpdateMatrices(); return mViewMatrix; }
		glm::mat4 EngineionMatrix() { UpdateMatrices(); return mEngineionMatrix; }
		glm::mat4 ViewEngineionMatrix() { UpdateMatrices(); return mViewEngineionMatrix; }


		//-------------------------------------
		// Private members
		//-------------------------------------
	private:
		// Update the matrices used for the camera in the rendering pipeline
		void UpdateMatrices();

		// Postition and rotations for the camera (rarely scale cameras)
		glm::vec3 mPosition;
		glm::vec3 mRotation;

		// Camera settings: field of view, aspect ratio, near and far clip plane distances.
		// Note that the FOVx angle is measured in radians (radians = degrees * PI/180) from left to right of screen
		float mFOVx;
		float mAspectRatio;
		float mNearClip;
		float mFarClip;

		// Current view, Engineion and combined view-Engineion matrices (DirectX matrix type)
		glm::mat4 mWorldMatrix; // Easiest to treat the camera like a model and give it a "world" matrix...
		glm::mat4 mViewMatrix;  // ...then the view matrix used in the shaders is the inverse of its world matrix

		glm::mat4 mEngineionMatrix;     // Engineion matrix holds the field of view and near/far clip distances
		glm::mat4 mViewEngineionMatrix; // Combine (multiply) the view and Engineion matrices together, which
		// can sometimes save a matrix multiply in the shader (optional)

		const float ROTATION_SPEED = 2.0f;
		const float MOVEMENT_SPEED = 50.0f;
	};

}

