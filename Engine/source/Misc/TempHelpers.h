#pragma once

#include <glm/glm.hpp>

namespace temp
{
	 // Temporary helper functions 

 
	static  glm::mat4 MatRotZ(float z)
	{
		float sZ = std::sin(z);
		float cZ = std::cos(z);

		 return glm::mat4{ cZ,  sZ,  0,  0,
						  -sZ,  cZ,  0,  0,
							0,   0,  1,  0,
							0,   0,  0,  1 };

		
	}

	static  glm::mat4 MatRotX(float x)
	{
		float sX = std::sin(x);
		float cX = std::cos(x);

		return glm::mat4{ 1,   0,   0,  0,
					 0,  cX,  sX,  0,
					 0, -sX,  cX,  0,
					 0,   0,   0,  1 };


	}
	
	static  glm::mat4 MatRotY(float y)
	{
		float sY = std::sin(y);
		float cY = std::cos(y);

		return glm::mat4{ cY,   0, -sY,  0,
							0,   1,   0,  0,
						   sY,   0,  cY,  0,
							0,   0,   0,  1 }; 

	}

	static glm::mat4 MatTranslation(const glm::vec3& t)
	{
		return glm::mat4{ 1,   0,   0,  0,
						0,   1,   0,  0,
						0,   0,   1,  0,
					  t.x, t.y, t.z,  1 };
 
	}

	static glm::mat4 MatScaling(const glm::vec3& s)
	{
		return glm::mat4{ s.x,   0,   0,  0,
					   0,   s.y,   0,  0,
					   0,     0, s.z,  0,
					   0,     0,   0,  1 };
	}

	static glm::mat4 MatrixMultiplication(glm::mat4 m1, glm::mat4 m2)
	{
		glm::mat4 mOut;

		mOut[0][0] = m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0] + m1[0][2] * m2[2][0] + m1[0][3] * m2[3][0];
		mOut[0][1] = m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1] + m1[0][2] * m2[2][1] + m1[0][3] * m2[3][1];
		mOut[0][2] = m1[0][0] * m2[0][2] + m1[0][1] * m2[1][2] + m1[0][2] * m2[2][2] + m1[0][3] * m2[3][2];
		mOut[0][3] = m1[0][0] * m2[0][3] + m1[0][1] * m2[1][3] + m1[0][2] * m2[2][3] + m1[0][3] * m2[3][3];

		mOut[1][0] = m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0] + m1[1][2] * m2[2][0] + m1[1][3] * m2[3][0];
		mOut[1][1] = m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1] + m1[1][2] * m2[2][1] + m1[1][3] * m2[3][1];
		mOut[1][2] = m1[1][0] * m2[0][2] + m1[1][1] * m2[1][2] + m1[1][2] * m2[2][2] + m1[1][3] * m2[3][2];
		mOut[1][3] = m1[1][0] * m2[0][3] + m1[1][1] * m2[1][3] + m1[1][2] * m2[2][3] + m1[1][3] * m2[3][3];

		mOut[2][0] = m1[2][0] * m2[0][0] + m1[2][1] * m2[1][0] + m1[2][2] * m2[2][0] + m1[2][3] * m2[3][0];
		mOut[2][1] = m1[2][0] * m2[0][1] + m1[2][1] * m2[1][1] + m1[2][2] * m2[2][1] + m1[2][3] * m2[3][1];
		mOut[2][2] = m1[2][0] * m2[0][2] + m1[2][1] * m2[1][2] + m1[2][2] * m2[2][2] + m1[2][3] * m2[3][2];
		mOut[2][3] = m1[2][0] * m2[0][3] + m1[2][1] * m2[1][3] + m1[2][2] * m2[2][3] + m1[2][3] * m2[3][3];

		mOut[3][0] = m1[3][0] * m2[0][0] + m1[3][1] * m2[1][0] + m1[3][2] * m2[2][0] + m1[3][3] * m2[3][0];
		mOut[3][1] = m1[3][0] * m2[0][1] + m1[3][1] * m2[1][1] + m1[3][2] * m2[2][1] + m1[3][3] * m2[3][1];
		mOut[3][2] = m1[3][0] * m2[0][2] + m1[3][1] * m2[1][2] + m1[3][2] * m2[2][2] + m1[3][3] * m2[3][2];
		mOut[3][3] = m1[3][0] * m2[0][3] + m1[3][1] * m2[1][3] + m1[3][2] * m2[2][3] + m1[3][3] * m2[3][3];

		return mOut;
	}

	static void SetColumn(glm::mat4& m, int iCol, const glm::vec3& v)
	{
		m[iCol][0] = v.x;
		m[iCol][1] = v.y;
		m[iCol][2] = v.z;
	}

	static void SetRow (glm::mat4& m, int iRow, const glm::vec3& v)
	{
		m[0][iRow] = v.x;
		m[1][iRow] = v.y;
		m[2][iRow] = v.z;
	}

	
}
