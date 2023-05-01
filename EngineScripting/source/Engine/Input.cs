using System;
using System.Collections.Generic;
using System.Diagnostics.Eventing.Reader;
using System.Linq;
using System.Text;
using System.Threading.Tasks;



namespace Engine
{


    public class Input
    {
        public static bool KeyHeld(KeyCodes keycode)
        {
            return InternalCalls.Input_KeyHeld((int)keycode);
        }

       //public static bool KeyHit(KeyCodes keycode)
       //{
       //    return InternalCalls.Input_KeyHit((int)keycode);
       //}
    }

    //*********************************
    // Inputs created from the SDL inputs
    //********************************

    public enum KeyCodes
    {
        Key_a = 'a',
        Key_b = 'b',
        Key_c = 'c',
        Key_d = 'd',
        Key_e = 'e',
        Key_f = 'f',
        Key_g = 'g',
        Key_h = 'h',
        Key_i = 'i',
        Key_j = 'j',
        Key_k = 'k',
        Key_l = 'l',
        Key_m = 'm',
        Key_n = 'n',
        Key_o = 'o',
        Key_p = 'p',
        Key_q = 'q',
        Key_r = 'r',
        Key_s = 's',
        Key_t = 't',
        Key_u = 'u',
        Key_v = 'v',
        Key_w = 'w',
        Key_x = 'x',
        Key_y = 'y',
        Key_z = 'z',

        Key_0 = '0',
        Key_1 = '1',
        Key_2 = '2',
        Key_3 = '3',
        Key_4 = '4',
        Key_5 = '5',
        Key_6 = '6',
        Key_7 = '7',
        Key_8 = '8',
        Key_9 = '9',

        Key_RIGHT =  79 | 1 << 30,
        Key_LEFT  =  80 | 1 << 30,
        Key_DOWN  =  81 | 1 << 30,
        Key_UP    =  82 | 1 << 30,
    }

}