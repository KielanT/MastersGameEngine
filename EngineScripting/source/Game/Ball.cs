using Engine.Maths;
using Engine;


namespace Game
{
    public class Ball : Entity
    {
        public float forceAmount = 100.0f;

        public void OnBegin()
        {
            physicsComponent.AddForce(new Vector3(forceAmount, 0, 0));
        }

        public void OnUpdate(float deltaTime)
        {

        }


    }
}