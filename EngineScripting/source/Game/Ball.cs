using Engine.Maths;
using Engine;
using System;

namespace Game
{
    public class Ball : Entity
    {
        // Ball settings
        public float forceAmount = 25.0f;
        bool bAddforce = false;

        private float timer = 0;
        private float delay = 5;

        public void OnBegin()
        {
            // Adds a random force for at the start
            float x  = GetRandomFloat();
            float y  = GetRandomFloat();
            physicsComponent.AddForce(new Vector3(x * forceAmount, y * forceAmount, 0), ForceType.eVELOCITY_CHANGE);
        }

        public void OnUpdate(float deltaTime)
        {
            timer += deltaTime;
           
            // On Contact change the velocity direction in the oppisite of its movement 
            if (bAddforce)
            {
                Vector3 position = transformComponent.Position;
                Vector3 MovementDirection = Vector3.Normalise(position);
                physicsComponent.AddForce(MovementDirection * -forceAmount, ForceType.eVELOCITY_CHANGE);
                
                bAddforce = false;
            }

            // add a random velocity change
            // Used to stop the ball from getting stuck (ball can still get stuck
            if (timer > delay)
            {
                Utility.Logger(LogType.Info, "Force Change");
                AddRandomChange();
                timer = 0;
            }
        }


        public override void OnContact(ulong id)
        {
            bAddforce = true;
            // PhysX does not like add force being called in call back
            // Appears the issues is because there isn't any multiple threading
        }

        private float GetRandomFloat()
        {
            Random rnd = new Random();

            int posOrNeg = rnd.Next(0, 10);

            if (posOrNeg < 5)
            {
                return -(float)rnd.NextDouble();
            }
            else
            {
                return (float)rnd.NextDouble();
            }
        }

        private void AddRandomChange()
        {

            float x = GetRandomFloat();
            float y = GetRandomFloat();
            physicsComponent.AddForce(new Vector3(x * forceAmount * 10.0f, y * forceAmount * 10.0f, 0), ForceType.eFORCE);
        }

    }
}