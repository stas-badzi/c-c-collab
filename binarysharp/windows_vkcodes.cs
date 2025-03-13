#if _WIN32
namespace windows {
#pragma warning disable CS8981
    public class keyboard {
        /*
        * Virtual Keys, Standard Set
        */
        public const ushort VK_LBUTTON       = 0x01;
        public const ushort VK_RBUTTON       = 0x02;
        public const ushort VK_CANCEL        = 0x03;
        public const ushort VK_MBUTTON       = 0x04;    /* NOT contiguous with L & RBUTTON */


        public const ushort VK_XBUTTON1      = 0x05;    /* NOT contiguous with L & RBUTTON */
        public const ushort VK_XBUTTON2      = 0x06;   /* NOT contiguous with L & RBUTTON */


        /*
        * 0x07 : reserved
        */


        public const ushort VK_BACK          = 0x08;
        public const ushort VK_TAB           = 0x09;

        /*
        * 0x0A - 0x0B : reserved
        */

        public const ushort VK_CLEAR         = 0x0C;
        public const ushort VK_RETURN        = 0x0D;

        /*
        * 0x0E - 0x0F : unassigned
        */

        public const ushort VK_SHIFT         = 0x10;
        public const ushort VK_CONTROL       = 0x11;
        public const ushort VK_MENU          = 0x12;
        public const ushort VK_PAUSE         = 0x13;
        public const ushort VK_CAPITAL       = 0x14;

        public const ushort VK_KANA          = 0x15;
        public const ushort VK_HANGEUL       = 0x15;  /* old name - should be here for compatibility */
        public const ushort VK_HANGUL        = 0x15;

        public const ushort VK_IME_ON        = 0x16;
        public const ushort VK_JUNJA         = 0x17;
        public const ushort VK_FINAL         = 0x18;
        public const ushort VK_HANJA         = 0x19;
        public const ushort VK_KANJI         = 0x19;

        public const ushort VK_IME_OFF       = 0x1A;
        public const ushort VK_ESCAPE        = 0x1B;

        public const ushort VK_CONVERT       = 0x1C;
        public const ushort VK_NONCONVERT    = 0x1D;
        public const ushort VK_ACCEPT        = 0x1E;
        public const ushort VK_MODECHANGE    = 0x1F;

        public const ushort VK_SPACE         = 0x20;
        public const ushort VK_PRIOR         = 0x21;
        public const ushort VK_NEXT          = 0x22;
        public const ushort VK_END           = 0x23;
        public const ushort VK_HOME          = 0x24;
        public const ushort VK_LEFT          = 0x25;
        public const ushort VK_UP            = 0x26;
        public const ushort VK_RIGHT         = 0x27;
        public const ushort VK_DOWN          = 0x28;
        public const ushort VK_SELECT        = 0x29;
        public const ushort VK_PRINT         = 0x2A;
        public const ushort VK_EXECUTE       = 0x2B;
        public const ushort VK_SNAPSHOT      = 0x2C;
        public const ushort VK_INSERT        = 0x2D;
        public const ushort VK_DELETE        = 0x2E;
        public const ushort VK_HELP          = 0x2F;

        /*
        * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 -= 0x39)
        * 0x3A - 0x40 : unassigned
        * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 -= 0x5A)
        */

        public const ushort VK_LWIN          = 0x5B;
        public const ushort VK_RWIN          = 0x5C;
        public const ushort VK_APPS          = 0x5D;

        /*
        * 0x5E : reserved
        */

        public const ushort VK_SLEEP         = 0x5F;

        public const ushort VK_NUMPAD0       = 0x60;
        public const ushort VK_NUMPAD1       = 0x61;
        public const ushort VK_NUMPAD2       = 0x62;
        public const ushort VK_NUMPAD3       = 0x63;
        public const ushort VK_NUMPAD4       = 0x64;
        public const ushort VK_NUMPAD5       = 0x65;
        public const ushort VK_NUMPAD6       = 0x66;
        public const ushort VK_NUMPAD7       = 0x67;
        public const ushort VK_NUMPAD8       = 0x68;
        public const ushort VK_NUMPAD9       = 0x69;
        public const ushort VK_MULTIPLY      = 0x6A;
        public const ushort VK_ADD           = 0x6B;
        public const ushort VK_SEPARATOR     = 0x6C;
        public const ushort VK_SUBTRACT      = 0x6D;
        public const ushort VK_DECIMAL       = 0x6E;
        public const ushort VK_DIVIDE        = 0x6F;
        public const ushort VK_F1            = 0x70;
        public const ushort VK_F2            = 0x71;
        public const ushort VK_F3            = 0x72;
        public const ushort VK_F4            = 0x73;
        public const ushort VK_F5            = 0x74;
        public const ushort VK_F6            = 0x75;
        public const ushort VK_F7            = 0x76;
        public const ushort VK_F8            = 0x77;
        public const ushort VK_F9            = 0x78;
        public const ushort VK_F10           = 0x79;
        public const ushort VK_F11           = 0x7A;
        public const ushort VK_F12           = 0x7B;
        public const ushort VK_F13           = 0x7C;
        public const ushort VK_F14           = 0x7D;
        public const ushort VK_F15           = 0x7E;
        public const ushort VK_F16           = 0x7F;
        public const ushort VK_F17           = 0x80;
        public const ushort VK_F18           = 0x81;
        public const ushort VK_F19           = 0x82;
        public const ushort VK_F20           = 0x83;
        public const ushort VK_F21           = 0x84;
        public const ushort VK_F22           = 0x85;
        public const ushort VK_F23           = 0x86;
        public const ushort VK_F24           = 0x87;

        /*
        * 0x88 - 0x8F : UI navigation
        */

        public const ushort VK_NAVIGATION_VIEW    = 0x88;
        public const ushort VK_NAVIGATION_MENU    = 0x89;
        public const ushort VK_NAVIGATION_UP      = 0x8A;
        public const ushort VK_NAVIGATION_DOWN    = 0x8B;
        public const ushort VK_NAVIGATION_LEFT    = 0x8C;
        public const ushort VK_NAVIGATION_RIGHT   = 0x8D;
        public const ushort VK_NAVIGATION_ACCEPT  = 0x8E;
        public const ushort VK_NAVIGATION_CANCEL  = 0x8F;


        public const ushort VK_NUMLOCK       = 0x90;
        public const ushort VK_SCROLL        = 0x91;

        /*
        * NEC PC-9800 kbd definitions
        */
        public const ushort VK_OEM_NEC_EQUAL = 0x92;   // '=' key on numpad

        /*
        * Fujitsu/OASYS kbd definitions
        */
        public const ushort VK_OEM_FJ_JISHO  = 0x92;   // 'Dictionary' key
        public const ushort VK_OEM_FJ_MASSHOU= 0x93;   // 'Unregister word' key
        public const ushort VK_OEM_FJ_TOUROKU= 0x94;   // 'Register word' key
        public const ushort VK_OEM_FJ_LOYA   = 0x95;   // 'Left OYAYUBI' key
        public const ushort VK_OEM_FJ_ROYA   = 0x96;   // 'Right OYAYUBI' key

        /*
        * 0x97 - 0x9F : unassigned
        */

        /*
        * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
        * Used only as parameters to GetAsyncKeyState() and GetKeyState().
        * No other API or message will distinguish left and right keys in this way.
        */
        public const ushort VK_LSHIFT        = 0xA0;
        public const ushort VK_RSHIFT        = 0xA1;
        public const ushort VK_LCONTROL      = 0xA2;
        public const ushort VK_RCONTROL      = 0xA3;
        public const ushort VK_LMENU         = 0xA4;
        public const ushort VK_RMENU         = 0xA5;

        public const ushort VK_BROWSER_BACK       = 0xA6;
        public const ushort VK_BROWSER_FORWARD    = 0xA7;
        public const ushort VK_BROWSER_REFRESH    = 0xA8;
        public const ushort VK_BROWSER_STOP       = 0xA9;
        public const ushort VK_BROWSER_SEARCH     = 0xAA;
        public const ushort VK_BROWSER_FAVORITES  = 0xAB;
        public const ushort VK_BROWSER_HOME       = 0xAC;

        public const ushort VK_VOLUME_MUTE        = 0xAD;
        public const ushort VK_VOLUME_DOWN        = 0xAE;
        public const ushort VK_VOLUME_UP          = 0xAF;
        public const ushort VK_MEDIA_NEXT_TRACK   = 0xB0;
        public const ushort VK_MEDIA_PREV_TRACK   = 0xB1;
        public const ushort VK_MEDIA_STOP         = 0xB2;
        public const ushort VK_MEDIA_PLAY_PAUSE   = 0xB3;
        public const ushort VK_LAUNCH_MAIL        = 0xB4;
        public const ushort VK_LAUNCH_MEDIA_SELECT= 0xB5;
        public const ushort VK_LAUNCH_APP1        = 0xB6;
        public const ushort VK_LAUNCH_APP2        = 0xB7;

        /*
        * 0xB8 - 0xB9 : reserved
        */

        public const ushort VK_OEM_1         = 0xBA;   // ';:' for US
        public const ushort VK_OEM_PLUS      = 0xBB;   // '+' any country
        public const ushort VK_OEM_COMMA     = 0xBC;   // ',' any country
        public const ushort VK_OEM_MINUS     = 0xBD;   // '-' any country
        public const ushort VK_OEM_PERIOD    = 0xBE;   // '.' any country
        public const ushort VK_OEM_2         = 0xBF;   // '/?' for US
        public const ushort VK_OEM_3         = 0xC0;   // '`~' for US

        /*
        *= 0xC1 -= 0xC2 : reserved
        */

        /*
        *= 0xC3 -= 0xDA : Gamepad input
        */

        public const ushort VK_GAMEPAD_A                        = 0xC3;
        public const ushort VK_GAMEPAD_B                        = 0xC4;
        public const ushort VK_GAMEPAD_X                        = 0xC5;
        public const ushort VK_GAMEPAD_Y                        = 0xC6;
        public const ushort VK_GAMEPAD_RIGHT_SHOULDER           = 0xC7;
        public const ushort VK_GAMEPAD_LEFT_SHOULDER            = 0xC8;
        public const ushort VK_GAMEPAD_LEFT_TRIGGER             = 0xC9;
        public const ushort VK_GAMEPAD_RIGHT_TRIGGER            = 0xCA;
        public const ushort VK_GAMEPAD_DPAD_UP                  = 0xCB;
        public const ushort VK_GAMEPAD_DPAD_DOWN                = 0xCC;
        public const ushort VK_GAMEPAD_DPAD_LEFT                = 0xCD;
        public const ushort VK_GAMEPAD_DPAD_RIGHT               = 0xCE;
        public const ushort VK_GAMEPAD_MENU                     = 0xCF;
        public const ushort VK_GAMEPAD_VIEW                     = 0xD0;
        public const ushort VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON   = 0xD1;
        public const ushort VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON  = 0xD2;
        public const ushort VK_GAMEPAD_LEFT_THUMBSTICK_UP       = 0xD3;
        public const ushort VK_GAMEPAD_LEFT_THUMBSTICK_DOWN     = 0xD4;
        public const ushort VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT    = 0xD5;
        public const ushort VK_GAMEPAD_LEFT_THUMBSTICK_LEFT     = 0xD6;
        public const ushort VK_GAMEPAD_RIGHT_THUMBSTICK_UP      = 0xD7;
        public const ushort VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN    = 0xD8;
        public const ushort VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT   = 0xD9;
        public const ushort VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT    = 0xDA;


        public const ushort VK_OEM_4         = 0xDB;  //  '[{' for US
        public const ushort VK_OEM_5         = 0xDC;  //  '\|' for US
        public const ushort VK_OEM_6         = 0xDD;  //  ']}' for US
        public const ushort VK_OEM_7         = 0xDE;  //  ''"' for US
        public const ushort VK_OEM_8         = 0xDF;

        /*
        * 0xE0 : reserved
        */

        /*
        * Various extended or enhanced keyboards
        */
        public const ushort VK_OEM_AX        = 0xE1;  //  'AX' key on Japanese AX kbd
        public const ushort VK_OEM_102       = 0xE2;  //  "<>" or "\|" on RT 102-key kbd.
        public const ushort VK_ICO_HELP      = 0xE3;  //  Help key on ICO
        public const ushort VK_ICO_00        = 0xE4;  //  00 key on ICO

        public const ushort VK_PROCESSKEY    = 0xE5;

        public const ushort VK_ICO_CLEAR     = 0xE6;


        public const ushort VK_PACKET        = 0xE7;

        /*
        * 0xE8 : unassigned
        */

        /*
        * Nokia/Ericsson definitions
        */
        public const ushort VK_OEM_RESET     = 0xE9;
        public const ushort VK_OEM_JUMP      = 0xEA;
        public const ushort VK_OEM_PA1       = 0xEB;
        public const ushort VK_OEM_PA2       = 0xEC;
        public const ushort VK_OEM_PA3       = 0xED;
        public const ushort VK_OEM_WSCTRL    = 0xEE;
        public const ushort VK_OEM_CUSEL     = 0xEF;
        public const ushort VK_OEM_ATTN      = 0xF0;
        public const ushort VK_OEM_FINISH    = 0xF1;
        public const ushort VK_OEM_COPY      = 0xF2;
        public const ushort VK_OEM_AUTO      = 0xF3;
        public const ushort VK_OEM_ENLW      = 0xF4;
        public const ushort VK_OEM_BACKTAB   = 0xF5;

        public const ushort VK_ATTN          = 0xF6;
        public const ushort VK_CRSEL         = 0xF7;
        public const ushort VK_EXSEL         = 0xF8;
        public const ushort VK_EREOF         = 0xF9;
        public const ushort VK_PLAY          = 0xFA;
        public const ushort VK_ZOOM          = 0xFB;
        public const ushort VK_NONAME        = 0xFC;
        public const ushort VK_PA1           = 0xFD;
        public const ushort VK_OEM_CLEAR     = 0xFE;

        /*
        * 0xFF : reserved
        */
    }
#pragma warning restore CS8981
}
#endif