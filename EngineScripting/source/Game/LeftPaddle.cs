using Engine.Maths;
using Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Game
{
    public class LeftPaddle : Entity
    {

        public void OnBegin()
        {
            Utility.Logger(LogType.Debug, ID.ToString());
        }

        public void OnUpdate(float deltaTime)
        {
            Utility.Logger(LogType.Debug, ID.ToString());
        }


    }
}