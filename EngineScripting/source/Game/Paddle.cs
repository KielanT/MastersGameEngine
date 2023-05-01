using Engine.Maths;
using Engine;


namespace Game
{
    public class Paddle : Entity
    {

        public void OnBegin()
        {
            Utility.Logger(LogType.Debug, ID.ToString());
        }

        public void OnUpdate(float deltaTime)
        {
            
        }


    }
}