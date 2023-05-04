using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace Engine.Maths
{
    // Vector3 for maths in the c# 
    public struct Vector3 // If not a struct exception gets thrown
    {
        public float X, Y, Z;

        public Vector3(float x, float y, float z)
        {
            X = x; 
            Y = y; 
            Z = z;
        }

        // Helpers
        // Normalise the vector
        public static Vector3 Normalise(Vector3 v)
        {
            Vector3 v3 = new Vector3(v.X, v.Y, v.Z);
            float mag = (float)Math.Sqrt(v3.X * v3.X + v3.Y * v3.Y + v3.Z * v3.Z);
            v3.X /= mag;
            v3.Y /= mag;
            v3.Z /= mag;
            return v3;
        }

        // Tells C# how the data type should be converted to a string
        public override string ToString() 
        {
            string str = "X: " + X.ToString() + " Y: " + Y.ToString() + " Z: " + Z.ToString();
            return str;
        }

        // Allows for vector multiplication
        public static Vector3 operator *(Vector3 v1, Vector3 v2)
        {
            return new Vector3(v1.X *v2.X, v1.Y * v2.Y, v1.Z * v2.Z);
        }

        // Allows vector to be multiplied by a scalar
        public static Vector3 operator *(Vector3 v1, float scalar)
        {
            return new Vector3(v1.X * scalar, v1.Y * scalar, v1.Z * scalar);
        }
    }
}
