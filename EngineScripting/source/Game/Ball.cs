using Engine.Maths;
using Engine;


namespace Game
{
    public class Ball : Entity
    {
        public float forceAmount = 100.0f;

        public void OnBegin()
        {
            Utility.Logger(LogType.Info, "start");
            physicsComponent.AddForce(new Vector3(forceAmount, 0, 0));
        }

        public void OnUpdate(float deltaTime)
        {

        }

        public override void OnContact(ulong id)
        {
            Utility.Logger(LogType.Info, "OnContact Child");
            //physicsComponent.AddForce(new Vector3(0, forceAmount * 1, 0));
        }
    }
}