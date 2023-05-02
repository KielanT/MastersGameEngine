using Engine.Maths;
using Engine;

namespace Game
{
    public class Paddle : Entity
    {

        public float paddleSpeed = 25.0f;
        public int playerNum = 1;

        // TODO allow keycodes to be set via editor
        private KeyCodes keyUp;
        private KeyCodes keyDown;
        private KeyCodes keyLeft;
        private KeyCodes keyRight;

        public void OnBegin()
        {
            if(playerNum == 1)
            {
                keyUp = KeyCodes.Key_w;
                keyDown = KeyCodes.Key_s;
                keyLeft = KeyCodes.Key_a;
                keyRight = KeyCodes.Key_d;
            }
            else
            {
                keyUp = KeyCodes.Key_UP;
                keyDown = KeyCodes.Key_DOWN;
                keyLeft = KeyCodes.Key_LEFT;
                keyRight = KeyCodes.Key_RIGHT;
            }
        }

        public void OnUpdate(float deltaTime)
        {
           Vector3 position = transformComponent.Position;
           if (Input.KeyHeld(keyUp))
           {
               position.Y += paddleSpeed * deltaTime;
           }
           if (Input.KeyHeld(keyDown))
           {
               position.Y -= paddleSpeed * deltaTime;
           }

            //if (Input.KeyHeld(keyLeft))
            //{
            //    position.X -= paddleSpeed * deltaTime;
            //}
            //if (Input.KeyHeld(keyRight))
            //{
            //    position.X += paddleSpeed * deltaTime;
            //}

            transformComponent.Position = position;

        }


    }
}