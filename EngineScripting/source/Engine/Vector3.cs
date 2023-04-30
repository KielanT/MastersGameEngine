using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace Engine.Maths
{
    public struct Vector3 // If not a struct exception gets thrown
    {
        public float X, Y, Z;

        public Vector3(float x, float y, float z)
        {
            X = x; 
            Y = y; 
            Z = z;
        }

        
    }
}
