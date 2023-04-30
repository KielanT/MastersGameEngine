#include "epch.h"
//#include "Camera.h"
//
//#include "Misc/TempHelpers.h"
//
//#include <glm/gtc/matrix_transform.hpp>
//
//#include "Engine/Platform/SDLInput.h"
//
//#include "entt/entt.hpp"
//
//namespace Engine
//{
//	void Camera::Control(float frameTime)
//	{
//		//**** ROTATION ****
//		if (SDLInput::KeyHeld(SDLK_DOWN)/*KeyHeld(Key_Down)*/)
//		{
//			mRotation.x += ROTATION_SPEED * frameTime; // Use of frameTime to ensure same speed on different machines
//		}
//		if (SDLInput::KeyHeld(SDLK_UP)/*KeyHeld(Key_Up)*/)
//		{
//			mRotation.x -= ROTATION_SPEED * frameTime;
//		}
//		if (SDLInput::KeyHeld(SDLK_RIGHT)/*KeyHeld(Key_Right)*/)
//		{
//			mRotation.y += ROTATION_SPEED * frameTime;
//		}
//		if (SDLInput::KeyHeld(SDLK_LEFT)/*KeyHeld(Key_Left)*/)
//		{
//			mRotation.y -= ROTATION_SPEED * frameTime;
//		}
//		
//		//**** LOCAL MOVEMENT ****
//		if (SDLInput::KeyHeld(SDLK_d)/*KeyHeld(Key_D)*/)
//		{
//			mPosition.x += MOVEMENT_SPEED * frameTime * mWorldMatrix[0][0];
//			mPosition.y += MOVEMENT_SPEED * frameTime * mWorldMatrix[0][1];
//			mPosition.z += MOVEMENT_SPEED * frameTime * mWorldMatrix[0][2];
//		
//		}
//		if (SDLInput::KeyHeld(SDLK_a)/*KeyHeld(Key_A)*/)
//		{
//		
//			mPosition.x -= MOVEMENT_SPEED * frameTime * mWorldMatrix[0][0];
//			mPosition.y -= MOVEMENT_SPEED * frameTime * mWorldMatrix[0][1];
//			mPosition.z -= MOVEMENT_SPEED * frameTime * mWorldMatrix[0][2];
//		}
//		if (SDLInput::KeyHeld(SDLK_w)/*KeyHeld(Key_W)*/)
//		{
//			mPosition.x += MOVEMENT_SPEED * frameTime * mWorldMatrix[2][0];
//			mPosition.y += MOVEMENT_SPEED * frameTime * mWorldMatrix[2][1];
//			mPosition.z += MOVEMENT_SPEED * frameTime * mWorldMatrix[2][2];
//		}
//		if (SDLInput::KeyHeld(SDLK_s)/*KeyHeld(Key_S)*/)
//		{
//			mPosition.x -= MOVEMENT_SPEED * frameTime * mWorldMatrix[2][0];
//			mPosition.y -= MOVEMENT_SPEED * frameTime * mWorldMatrix[2][1];
//			mPosition.z -= MOVEMENT_SPEED * frameTime * mWorldMatrix[2][2];
//		}
//	}
//
//
//	// Update the matrices used for the camera in the rendering pipeline
//	void Camera::UpdateMatrices()
//	{
//
//		glm::mat4 rotationZ = temp::MatRotZ(mRotation.z);
//		glm::mat4 rotationX = temp::MatRotX(mRotation.x);
//		glm::mat4 rotationY = temp::MatRotY(mRotation.y);
//		glm::mat4 transMat = temp::MatTranslation(mPosition);
//
//		// Solution 1
//		glm::mat4 ZX = temp::MatrixMultiplication(rotationZ, rotationX);
//		glm::mat4 ZXY = temp::MatrixMultiplication(ZX, rotationY);
//		glm::mat4 ZXYT = temp::MatrixMultiplication(ZXY, transMat);
//		mWorldMatrix = ZXYT;
//
//		// Solution 2
//		//mWorldMatrix = MatrixMultiplication(MatrixMultiplication(rotationZ, rotationX), MatrixMultiplication(rotationY, transMat));
//
//		// View matrix is the usual matrix used for the camera in shaders, it is the inverse of the world matrix (see lectures)
//		mViewMatrix = glm::inverse(mWorldMatrix);// InverseAffine(mat);
//		//mViewMatrix = InverseAffine(mWorldMatrix);
//
//		// Engineion matrix, how to flatten the 3D world onto the screen (needs field of view, near and far clip, aspect ratio)
//		float tanFOVx = std::tan(mFOVx * 0.5f);
//		float scaleX = 1.0f / tanFOVx;
//		float scaleY = mAspectRatio / tanFOVx;
//		float scaleZa = mFarClip / (mFarClip - mNearClip);
//		float scaleZb = -mNearClip * scaleZa;
//
//		mEngineionMatrix = { scaleX,   0.0f,    0.0f,   0.0f,
//								0.0f, scaleY,    0.0f,   0.0f,
//								0.0f,   0.0f, scaleZa,   1.0f,
//								0.0f,   0.0f, scaleZb,   0.0f };
//
//		// The view-Engineion matrix combines the two matrices usually used for the camera into one, which can save a multiply in the shaders (optional)
//
//		mViewEngineionMatrix = mViewMatrix * mEngineionMatrix;
//	}
//
//	
//}