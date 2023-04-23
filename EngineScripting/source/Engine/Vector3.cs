using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace Engine.Maths
{
    public class Vector3
    {
        

        public Vector3(float x, float y, float z)
        {
            X = x; 
            Y = y; 
            Z = z;
        }

        public float X, Y, Z;
    }
}
