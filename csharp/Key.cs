#if __linux__
using linux;
using static linux.keyboard;
#elif __APPLE__
using apple;
using static apple.keyboard;
using static apple.keyboard.dependent;
using static apple.keyboard.independent;
#elif _WIN32
using windows;
using static windows.keyboard;
#endif

namespace Utility {
    public class Key {
        public enum Enum : ushort {
            NONE = 0xffff, /* specified Enum is not defined */
        #if _WIN32
            NUL , /* ␀ */
            CTRL_A , /* Ctrl + A */
            CTRL_B , /* Ctrl + B */
            CTRL_C , /* Ctrl + C */
            CTRL_D , /* Ctrl + D */
            CTRL_E , /* Ctrl + E */
            CTRL_F , /* Ctrl + F */
            CTRL_G , /* Ctrl + G */
            BACKSPACE = VK_BACK, /* ← [Ctrl + H] */
            TAB = VK_TAB, /* ⇥ [Ctrl + I] */
            LINEFEED , /* ␊ [Ctrl + J] */
            CTRL_K , /* Ctrl + K */
            CTRL_L , /* Ctrl + L */
            CTRL_M , /* Ctrl + M */
            CTRL_N , /* Ctrl + N */
            CTRL_O , /* Ctrl + O */
            CTRL_P , /* Ctrl + P */
            CTRL_Q , /* Ctrl + Q */
            CTRL_R , /* Ctrl + R */
            CTRL_S , /* Ctrl + S */
            CTRL_T , /* Ctrl + T */
            CTRL_U , /* Ctrl + U */
            CTRL_V , /* Ctrl + V */
            CTRL_W , /* Ctrl + W */
            CTRL_X , /* Ctrl + X */
            CTRL_Y , /* Ctrl + Y */
            CTRL_Z , /* Ctrl + Z */
            ESC = VK_ESCAPE, /* ␛ */
            CTRL_BACKSLASH , /* Ctrl + \ */
            CTRL_BRACKETRIGHT , /* Ctrl + [ */
            CTRL_ASCIICIRCUM , /* Ctrl + ^ */
            CTRL_UNEDERSCORE , /* Ctrl + _ */
            SPACE = VK_SPACE, /* ⎵ */
            ESCLAM , /* ! */
            DBLQUOTE , /* " */
            NUMBERSIGN , /* # */
            DOLLAR , /* $ */
            PERCENT , /* % */
            AMPERSAND , /* & */
            APOSTROPHE = VK_OEM_7, /* ' (on us keybards) */
            PARENLEFT , /* ( */
            PARENrIGHT , /* ) */
            ASTERISK , /* * */
            PLUS = VK_OEM_PLUS, /* + */
            COMMA = VK_OEM_COMMA, /* , */
            MINUS = VK_OEM_MINUS, /* -- */
            PERIOD = VK_OEM_PERIOD, /* . */
            SLASH = VK_OEM_2, /* / (on us keybards) */
            /*$0*/ZERO = '0', /* 0 */
            /*$1*/ONE = '1', /* 1 */
            /*$2*/TWO = '2', /* 2 */
            /*$3*/THREE = '3', /* 3 */
            /*$4*/FOUR = '4', /* 4 */
            /*$5*/FIVE = '5', /* 5 */
            /*$6*/SIX = '6', /* 6 */
            /*$7*/SEVEN = '7', /* 7 */
            /*$8*/EIGHT = '8', /* 8 */
            /*$9*/NINE = '9', /* 9 */
            COLON , /* , */
            SEMICOLON = VK_OEM_1, /* ; (on us keybards) */
            LESS , /* < (on us keybards)(i think) */
            EQUAL , /* , */
            GREATER , /* > */
            QUESTION , /* ? */
            AT , /* @ */
            A , /* A */
            B , /* B */
            C , /* C */
            D , /* D */
            E , /* E */
            F , /* F */
            G , /* G */
            H , /* H */
            I , /* I */
            J , /* J */
            K , /* K */
            L , /* L */
            M , /* M */
            N , /* N */
            O , /* O */
            P , /* P */
            Q , /* Q */
            R , /* R */
            S , /* S */
            T , /* T */
            U , /* U */
            V , /* V */
            W , /* W */
            X , /* X */
            Y , /* Y */
            Z , /* Z */
            BRACKETLEFT = VK_OEM_4, /* [ (on us keybards) */
            BACKSLASH = VK_OEM_5, /* \ (on us keybards) */
            BRACKETRIGHT = VK_OEM_6, /* ] (on us keybards) */
            ASCIICIRCUM , /* ^  */
            UNEDERSCORE , /* _ */
            GRAVE , /* `*/
            a = 'A', /* a */
            b = 'B', /* b */
            c = 'C', /* c */
            d = 'D', /* d */
            e = 'E', /* e */
            f = 'F', /* f */
            g = 'G', /* g */
            h = 'H', /* h */
            i = 'I', /* i */
            j = 'J', /* j */
            k = 'K', /* k */
            l = 'L', /* l */
            m = 'M', /* m */
            n = 'N', /* n */
            o = 'O', /* o */
            p = 'P', /* p */
            q = 'Q', /* q */
            r = 'R', /* r */
            s = 'S', /* s */
            t = 'T', /* t */
            u = 'U', /* u */
            v = 'V', /* v */
            w = 'W', /* w */
            x = 'X', /* x */
            y = 'Y', /* y */
            z = 'Z', /* z */
            BRACELEFT , /* { */
            BAR , /* | */
            BRACERIGHT , /* } */
            ASCIITILDE = VK_OEM_3, /* ~ (on us keybards) */
            DELETE = VK_DELETE, /* ␡ */
            NOBREAKSPACE , /* ⍽ */
            EXCLAMDOWN , /* ¡ */
            CENT , /* ¢ */
            STERLING , /* £ */
            CURRENCY , /* ¤ */
            YEN , /* ¥ */
            BROKENBAR , /* ¦ */
            SECTION , /* § */
            DIAERESIS , /* ¨ */
            COPYRIGHT , /* © */
            ORDFEMININE , /* ª */
            GUILLEMOTLEFT , /* « */
            NOTSIGN , /* ¬ */
            HYPHEN , /* - */
            REGISTERED , /* ® */
            MACRON , /* ¯ */
            DEGREE , /* ° */
            PLUSMINUS , /* ± */
            TWOSUPERIOR , /* ² */
            THREESUPERIOR , /* ³ */
            ACUTE , /* ´ */
            MU , /* μ */
            PARAGRAPH , /* ¶ */
            PERIODCENTERED , /* · */
            CEDILLA , /* ç */
            ONESUPERIOR , /* ¹ */
            MASCULINE , /* ♂ */
            GUILLEMOTRIGHT , /* » */
            ONEQUARTER , /* ¼ */
            ONEHALF , /* ½ */
            THREEQUARTERS , /* ¾ */
            QUESTIONDOWN , /* ¿ */
            A_GRAVE , /* À */
            A_ACUTE , /* Á */
            A_CIRCUMFLEX , /* Â */
            A_TILDE , /* Ã */
            A_DIAERESIS , /* Ä */
            A_RING , /* Å */
            AE , /* Æ */
            C_CEDILLA , /* Ç */
            E_GRAVE , /* È */
            E_ACUTE , /* É */
            E_CIRCUMFLEX , /* Ê */
            E_DIAERESIS , /* Ë */
            I_GRAVE , /* Ì */
            I_ACUTE , /* Í */
            I_CIRCUMFLEX , /* Î */
            I_DIAERESIS , /* Ï */
            ETH , /* Ð */
            N_TILDE , /* Ñ */
            O_GRAVE , /* Ò */
            O_ACUTE , /* Ó */
            O_CIRCUMFLEX , /* Ô */
            O_TILDE , /* Õ */
            O_DIAERESIS , /* Ö */
            MULTIPLY , /* × */
            O_SLASH , /* Ø */
            U_GRAVE , /* Ù */
            U_ACUTE , /* Ú */
            U_CIRCUMFLEX , /* Û */
            U_DIAERESIS , /* Ü */
            Y_ACUTE , /* Ý */
            THORN , /* Þ */
            SHARP_S , /* ß */
            a_grave , /* à */
            a_acute , /* á */
            a_circumflex , /* â */
            a_tilde , /* ã */
            a_diaeresis , /* ä */
            a_ring , /* å */
            ae , /* æ */
            c_cedilla , /* ç */
            e_grave , /* è */
            e_acute , /* é */
            e_circumflex , /* ê */
            e_diaeresis , /* ë */
            i_grave , /* ì */
            i_acute , /* í */
            i_circumflex , /* î */
            i_diaeresis , /* ï */
            eth , /* ð */
            n_tilde , /* ñ */
            o_grave , /* ò */
            o_acute , /* ó */
            o_circumflex , /* ô */
            o_tilde , /* õ */
            o_diaeresis , /* ö */
            DIVISION , /* ÷ */
            o_slash , /* ø */
            u_grave , /* ù */
            u_acute , /* ú */
            u_circumflex , /* û */
            u_diaeresis , /* ü */
            y_acute , /* ý */
            thorn , /* þ */
            y_diaeresis , /* ÿ */

            // KT_FN
            F1 = VK_F1, /* F1 */
            F2 = VK_F2, /* F2 */
            F3 = VK_F3, /* F3 */
            F4 = VK_F4, /* F4 */
            F5 = VK_F5, /* F5 */
            F6 = VK_F6, /* F6 */
            F7 = VK_F7, /* F7 */
            F8 = VK_F8, /* F8 */
            F9 = VK_F9, /* F9 */
            F10 = VK_F10, /* F10 */
            F11 = VK_F11, /* F11 */
            F12 = VK_F12, /* F12 */
            F13 = VK_F13, /* F13 */
            F14 = VK_F14, /* F14 */
            F15 = VK_F15, /* F15 */
            F16 = VK_F16, /* F16 */
            F17 = VK_F17, /* F17 */
            F18 = VK_F18, /* F18 */
            F19 = VK_F19, /* F19 */
            F20 = VK_F20, /* F20 */
            FIND = VK_HOME, /* Find [Home] */
            INSERT = VK_INSERT, /* Insert */
            REMOVE , /* Remove */
            END = VK_END, /* End */
            SELECT = VK_SELECT, /* Select */
            PGUP = VK_PRIOR, /* Prior [PageUp] */
            PGDN = VK_NEXT, /* Next [PageDown] */
            MACRO , /* Macro */
            HELP = VK_HELP, /* Help */
            DO , /* Do */
            PAUSE = VK_PAUSE, /* Pause */
            F21 = VK_F21, /* F21 */
            F22 = VK_F22, /* F22 */
            F23 = VK_F23, /* F23 */
            F24 = VK_F24, /* F24 */
            F25 , /* F25 */
            F26 , /* F26 */
            F27 , /* F27 */
            F28 , /* F28 */
            F29 , /* F29 */
            F30 , /* F30 */
            F31 , /* F31 */
            F32 , /* F32 */
            F33 , /* F33 */
            F34 , /* F34 */
            F35 , /* F35 */
            F36 , /* F36 */
            F37 , /* F37 */
            F38 , /* F38 */
            F39 , /* F39 */
            F40 , /* F40 */
            F41 , /* F41 */
            F42 , /* F42 */
            F43 , /* F43 */
            F44 , /* F44 */
            F45 , /* F45 */
            F46 , /* F46 */
            F47 , /* F47 */
            F48 , /* F48 */
            F49 , /* F49 */
            F50 , /* F50 */
            F51 , /* F51 */
            F52 , /* F52 */
            F53 , /* F53 */
            F54 , /* F54 */
            F55 , /* F55 */
            F56 , /* F56 */
            F57 , /* F57 */
            F58 , /* F58 */
            F59 , /* F59 */
            F60 , /* F60 */
            F61 , /* F61 */
            F62 , /* F62 */
            F63 , /* F63 */
            F64 , /* F64 */
            F65 , /* F65 */
            F66 , /* F66 */
            F67 , /* F67 */
            F68 , /* F68 */
            F69 , /* F69 */
            F70 , /* F70 */
            F71 , /* F71 */
            F72 , /* F72 */
            F73 , /* F73 */
            F74 , /* F74 */
            F75 , /* F75 */
            F76 , /* F76 */
            F77 , /* F77 */
            F78 , /* F78 */
            F79 , /* F79 */
            F80 , /* F80 */
            F81 , /* F81 */
            F82 , /* F82 */
            F83 , /* F83 */
            F84 , /* F84 */
            F85 , /* F85 */
            F86 , /* F86 */
            F87 , /* F87 */
            F88 , /* F88 */
            F89 , /* F89 */
            F90 , /* F90 */
            F91 , /* F91 */
            F92 , /* F92 */
            F93 , /* F93 */
            F94 , /* F94 */
            F95 , /* F95 */
            F96 , /* F96 */
            F97 , /* F97 */
            F98 , /* F98 */
            F99 , /* F99 */
            F100 , /* F100 */
            F101 , /* F101 */
            F102 , /* F102 */
            F103 , /* F103 */
            F104 , /* F104 */
            F105 , /* F105 */
            F106 , /* F106 */
            F107 , /* F107 */
            F108 , /* F108 */
            F109 , /* F109 */
            F110 , /* F110 */
            F111 , /* F111 */
            F112 , /* F112 */
            F113 , /* F113 */
            F114 , /* F114 */
            F115 , /* F115 */
            F116 , /* F116 */
            F117 , /* F117 */
            F118 , /* F118 */
            F119 , /* F119 */
            F120 , /* F120 */
            F121 , /* F121 */
            F122 , /* F122 */
            F123 , /* F123 */
            F124 , /* F124 */
            F125 , /* F125 */
            F126 , /* F126 */
            F127 , /* F127 */
            F128 , /* F128 */
            F129 , /* F129 */
            F130 , /* F130 */
            F131 , /* F131 */
            F132 , /* F132 */
            F133 , /* F133 */
            F134 , /* F134 */
            F135 , /* F135 */
            F136 , /* F136 */
            F137 , /* F137 */
            F138 , /* F138 */
            F139 , /* F139 */
            F140 , /* F140 */
            F141 , /* F141 */
            F142 , /* F142 */
            F143 , /* F143 */
            F144 , /* F144 */
            F145 , /* F145 */
            F146 , /* F146 */
            F147 , /* F147 */
            F148 , /* F148 */
            F149 , /* F149 */
            F150 , /* F150 */
            F151 , /* F151 */
            F152 , /* F152 */
            F153 , /* F153 */
            F154 , /* F154 */
            F155 , /* F155 */
            F156 , /* F156 */
            F157 , /* F157 */
            F158 , /* F158 */
            F159 , /* F159 */
            F160 , /* F160 */
            F161 , /* F161 */
            F162 , /* F162 */
            F163 , /* F163 */
            F164 , /* F164 */
            F165 , /* F165 */
            F166 , /* F166 */
            F167 , /* F167 */
            F168 , /* F168 */
            F169 , /* F169 */
            F170 , /* F170 */
            F171 , /* F171 */
            F172 , /* F172 */
            F173 , /* F173 */
            F174 , /* F174 */
            F175 , /* F175 */
            F176 , /* F176 */
            F177 , /* F177 */
            F178 , /* F178 */
            F179 , /* F179 */
            F180 , /* F180 */
            F181 , /* F181 */
            F182 , /* F182 */
            F183 , /* F183 */
            F184 , /* F184 */
            F185 , /* F185 */
            F186 , /* F186 */
            F187 , /* F187 */
            F188 , /* F188 */
            F189 , /* F189 */
            F190 , /* F190 */
            F191 , /* F191 */
            F192 , /* F192 */
            F193 , /* F193 */
            F194 , /* F194 */
            F195 , /* F195 */
            F196 , /* F196 */
            F197 , /* F197 */
            F198 , /* F198 */
            F199 , /* F199 */
            F200 , /* F200 */
            F201 , /* F201 */
            F202 , /* F202 */
            F203 , /* F203 */
            F204 , /* F204 */
            F205 , /* F205 */
            F206 , /* F206 */
            F207 , /* F207 */
            F208 , /* F208 */
            F209 , /* F209 */
            F210 , /* F210 */
            F211 , /* F211 */
            F212 , /* F212 */
            F213 , /* F213 */
            F214 , /* F214 */
            F215 , /* F215 */
            F216 , /* F216 */
            F217 , /* F217 */
            F218 , /* F218 */
            F219 , /* F219 */
            F220 , /* F220 */
            F221 , /* F221 */
            F222 , /* F222 */
            F223 , /* F223 */
            F224 , /* F224 */
            F225 , /* F225 */
            F226 , /* F226 */
            F227 , /* F227 */
            F228 , /* F228 */
            F229 , /* F229 */
            F230 , /* F230 */
            F231 , /* F231 */
            F232 , /* F232 */
            F233 , /* F233 */
            F234 , /* F234 */
            F235 , /* F235 */
            F236 , /* F236 */
            F237 , /* F237 */
            F238 , /* F238 */
            F239 , /* F239 */
            F240 , /* F240 */
            F241 , /* F241 */
            F242 , /* F242 */
            F243 , /* F243 */
            F244 , /* F244 */
            F245 , /* F245 */
            UNDO , /* Undo */

            // KT_SPEC
            HOLE , /* VoidSymbol */
            ENTER = VK_RETURN, /* Return */
            SH_REGS , /* Show Registers */
            SH_MEM , /* Show Memory */
            SH_STAT , /* Show State */
            BREAK , /* Break */
            CONS , /* Last Console */
            CAPS = VK_CAPITAL, /* Caps Lock */
            NUM = VK_NUMLOCK, /* Num Lock */
            HOLD = VK_SCROLL, /* Scroll Lock */
            SCROLLFORW , /* Scroll Forward */
            SCROLLBACK , /* Scroll Backward */
            BOOT , /* Boot */
            CAPSON , /* Caps On */
            COMPOSE , /* Compose */
            SAK , /* SAK */
            DECRCONSOLE , /* Decrease Console */
            INCRCONSOLE , /* Increase Console */
            SPAWNCONSOLE , /* EnumboardSignal */
            BARENUMLOCK , /* Bare Num Lock */

            // KT_PAD
            KP0 = VK_NUMPAD0, /* Numpad 0 */
            KP1 = VK_NUMPAD1, /* Numpad 1 */
            KP2 = VK_NUMPAD2, /* Numpad 2 */
            KP3 = VK_NUMPAD3, /* Numpad 3 */
            KP4 = VK_NUMPAD4, /* Numpad 4 */
            KP5 = VK_NUMPAD5, /* Numpad 5 */
            KP6 = VK_NUMPAD6, /* Numpad 6 */
            KP7 = VK_NUMPAD7, /* Numpad 7 */
            KP8 = VK_NUMPAD8, /* Numpad 8 */
            KP9 = VK_NUMPAD9, /* Numpad 9 */

            KPPLUS = VK_ADD, /* Numpad + (not sure) */
            KPMINUS = VK_SUBTRACT, /* Numpad - (not sure) */
            KPSTAR = VK_MULTIPLY, /* Numpad * (not sure) */
            KPSLASH = VK_DIVIDE, /* Numpad / (not sure) */
            KPENTER , /* Numpad Enter */
            KPCOMMA , /* Numpad , */
            KPDOT = VK_DECIMAL, /* Numpad . */
            KPPLUSMINUS , /* Numpad ± */
            KPPARENL , /* Numpad ( */
            KPPARENR , /* Numpad ) */

            // KT_DEAD
            DGRAVE , /* [Dead] Grave */
            DACUTE , /* [Dead] Acute */
            DCIRCM , /* [Dead] CircumFlex */
            DTILDE , /* [Dead] Tilde */
            DDIERE , /* [Dead] Diaeresis */
            DCEDIL , /* [Dead] Cedilla */
            DMACRON , /* [Dead] Macron */
            DBREVE , /* [Dead] Breve (an alias for [Dead] Tilde) */
            DABDOT , /* [Dead] Abovedot */
            DABRING , /* [Dead] Abovering */
            DDBACUTE , /* [Dead] DoubleAcute (an alias for [Dead] Tilde) */
            DCARON , /* [Dead] Caron (an alias for [Dead] Circumflex) */
            DOGONEK , /* [Dead] Ogonek (an alias for [Dead] Cedilla) */
            DIOTA , /* [Dead] Iota */
            DVOICED , /* [Dead] Voiced sound */
            DSEMVOICED , /* [Dead] Semivoiced sound */
            DBEDOT , /* [Dead] BelowDot */
            DHOOK , /* [Dead] Hook */
            DHORN , /* [Dead] Horn */
            DSTROKE , /* [Dead] Stroke */
            DABCOMMA , /* [Dead] Abovecomma */
            DABREVCOMMA , /* [Dead] Abovereversedcomma */
            DDBGRAVE , /* [Dead] Doublegrave */
            DINVBREVE , /* [Dead] Inverted breve */
            DBECOMMA , /* [Dead] Belowcomma */
            DCURRENCY , /* [Dead] Currency */
            DGREEK , /* [Dead] Greek */

            // KT_CUR
            ARROW_DOWN = VK_DOWN, /* Down Arrow */
            ARROW_LEFT = VK_LEFT, /* Left Arrow */
            ARROW_RIGHT = VK_RIGHT, /* Right Arrow */
            ARROW_UP = VK_UP, /* Up Arrow */

            // KT_SHIFT
            SHIFT = VK_SHIFT, /* Shift */
            CTRL = VK_CONTROL, /* Control */
            ALT = VK_LMENU, /* Left Alt */
            ALTGR = VK_RMENU, /* Right Alt [AltGramar] */
            SHIFTL = VK_LSHIFT, /* ShiftLeft */
            SHIFTR = VK_RSHIFT, /* ShiftRight */
            CTRLL = VK_LCONTROL, /* CtrlLeft */
            CTRLR = VK_RCONTROL, /* CtrlRight */
            CAPSSHIFT , /* CapsShift */

            // KT_ASCII
            ASC0 , /* Ascii_0 */
            ASC1 , /* Ascii_1 */
            ASC2 , /* Ascii_2 */
            ASC3 , /* Ascii_3 */
            ASC4 , /* Ascii_4 */
            ASC5 , /* Ascii_5 */
            ASC6 , /* Ascii_6 */
            ASC7 , /* Ascii_7 */
            ASC8 , /* Ascii_8 */
            ASC9 , /* Ascii_9 */
            HEX0 , /* Hex_0 */
            HEX1 , /* Hex_1 */
            HEX2 , /* Hex_2 */
            HEX3 , /* Hex_3 */
            HEX4 , /* Hex_4 */
            HEX5 , /* Hex_5 */
            HEX6 , /* Hex_6 */
            HEX7 , /* Hex_7 */
            HEX8 , /* Hex_8 */
            HEX9 , /* Hex_9 */
            HEXa , /* Hex_A */
            HEXb , /* Hex_B */
            HEXc , /* Hex_C */
            HEXd , /* Hex_D */
            HEXe , /* Hex_E */
            HEXf , /* Hex_F */
            SHIFTLOCK , /* Shift Lock */
            CTRLLOCK , /* AltGr Lock */
            ALTLOCK , /* Error: D:\a\c-c-collab\c-c-collab\binarysharp\windows_vkcodes.cs(23,45): error CS1002: ; expected [D:\a\c-c-collab\c-c-collab\binarysharp\cs-factoryrush.csproj]Control Lock */
            ALTGRLOCK , /* Alt Lock */
            SHIFTLLOCK , /* ShiftL Lock */
            SHIFTRLOCK , /* ShiftR Lock */
            CTRLLLOCK , /* CtrlL Lock */
            CTRLRLOCK , /* CtrlR Lock */
            CAPSSHIFTLOCK , /* CapsShift Lock */
            SHIFT_SLOCK , /* Sticky Shift */
            CTRL_SLOCK , /* Sticky AltGr */
            ALT_SLOCK , /* Sticky Control */
            ALTGR_SLOCK , /* Sticky Alt */
            SHIFTL_SLOCK , /* Sticky ShiftLeft */
            SHIFTR_SLOCK , /* Sticky ShiftRight */
            CTRLL_SLOCK , /* Sticky CtrlLeft */
            CTRLR_SLOCK , /* Sticky CtrlRight */
            CAPSSHIFT_SLOCK , /* Sticky CapsShift */
            BRL_BLANK , /* Brl_blank */
            BRL_DOT1 , /* Brl_dot1 */
            BRL_DOT2 , /* Brl_dot2 */
            BRL_DOT3 , /* Brl_dot3 */
            BRL_DOT4 , /* Brl_dot4 */
            BRL_DOT5 , /* Brl_dot5 */
            BRL_DOT6 , /* Brl_dot6 */
            BRL_DOT7 , /* Brl_dot7 */
            BRL_DOT8 , /* Brl_dot8 */
            BRL_DOT9 , /* Brl_dot9 */
            BRL_DOT10 , /* Brl_dot10 */
            CONSOLE1 , /* Console_1 */
            CONSOLE2 , /* Console_2 */
            CONSOLE3 , /* Console_3 */
            CONSOLE4 , /* Console_4 */
            CONSOLE5 , /* Console_5 */
            CONSOLE6 , /* Console_6 */
            CONSOLE7 , /* Console_7 */
            CONSOLE8 , /* Console_8 */
            CONSOLE9 , /* Console_9 */
            CONSOLE10 , /* Console_10 */
            CONSOLE11 , /* Console_11 */
            CONSOLE12 , /* Console_12 */
            CONSOLE13 , /* Console_13 */
            CONSOLE14 , /* Console_14 */
            CONSOLE15 , /* Console_15 */
            CONSOLE16 , /* Console_16 */
            CONSOLE17 , /* Console_17 */
            CONSOLE18 , /* Console_18 */
            CONSOLE19 , /* Console_19 */
            CONSOLE20 , /* Console_20 */
            CONSOLE21 , /* Console_21 */
            CONSOLE22 , /* Console_22 */
            CONSOLE23 , /* Console_23 */
            CONSOLE24 , /* Console_24 */
            CONSOLE25 , /* Console_25 */
            CONSOLE26 , /* Console_26 */
            CONSOLE27 , /* Console_27 */
            CONSOLE28 , /* Console_28 */
            CONSOLE29 , /* Console_29 */
            CONSOLE30 , /* Console_30 */
            CONSOLE31 , /* Console_31 */
            CONSOLE32 , /* Console_32 */
            CONSOLE33 , /* Console_33 */
            CONSOLE34 , /* Console_34 */
            CONSOLE35 , /* Console_35 */
            CONSOLE36 , /* Console_36 */
            CONSOLE37 , /* Console_37 */
            CONSOLE38 , /* Console_38 */
            CONSOLE39 , /* Console_39 */
            CONSOLE40 , /* Console_40 */
            CONSOLE41 , /* Console_41 */
            CONSOLE42 , /* Console_42 */
            CONSOLE43 , /* Console_43 */
            CONSOLE44 , /* Console_44 */
            CONSOLE45 , /* Console_45 */
            CONSOLE46 , /* Console_46 */
            CONSOLE47 , /* Console_47 */
            CONSOLE48 , /* Console_48 */
            CONSOLE49 , /* Console_49 */
            CONSOLE50 , /* Console_50 */
            CONSOLE51 , /* Console_51 */
            CONSOLE52 , /* Console_52 */
            CONSOLE53 , /* Console_53 */
            CONSOLE54 , /* Console_54 */
            CONSOLE55 , /* Console_55 */
            CONSOLE56 , /* Console_56 */
            CONSOLE57 , /* Console_57 */
            CONSOLE58 , /* Console_58 */
            CONSOLE59 , /* Console_59 */
            CONSOLE60 , /* Console_60 */
            CONSOLE61 , /* Console_61 */
            CONSOLE62 , /* Console_62 */
            CONSOLE63 , /* Console_63 */

            CANCEL = VK_CANCEL, /* Cancel */
            CLEAR = VK_CLEAR, /* Clear */
            KANA = VK_KANA, /* IME Kana mode */
            HANGUL = VK_HANGUL, /* IME Hangul mode */
            IME_ON = VK_IME_ON, /* IME On */
            JUNJA = VK_JUNJA, /* IME Junja mode */
            FINAL = VK_FINAL, /* IME Final mode */
            HANJA = VK_HANJA, /* IME Hanja mode */
            KANJI = VK_KANJI, /* IME Kanji mode */
            IME_OFF = VK_IME_OFF, /* IME Off */
            CONVERT = VK_CONVERT, /* IME Convert */
            NONCONVERT = VK_NONCONVERT, /* IME NonConvert */
            ACCEPT = VK_ACCEPT, /* IME Accept */
            MODECHANGE = VK_MODECHANGE, /* IME ModeChange */
            PRINT = VK_PRINT, /* Print Key */
            EXECUTE = VK_EXECUTE, /* Execute Key */
            SNAPSHOT = VK_SNAPSHOT, /* Print Screen Key */
            SUPERL = VK_LWIN, /* Left Super[Windows] key */
            SUPERR = VK_RWIN, /* Right Super[Windows] key */
            APPS = VK_APPS, /* Applications Key */
            SLEEP = VK_SLEEP, /* Computer Sleep Key */
            AALT = VK_MENU, /* Any Alt */
            BROWSER_BACK = VK_BROWSER_BACK, /* Browser Back */
            BROWSER_FORWARD = VK_BROWSER_FORWARD, /* Browser Forward */
            BROWSER_REFRESH = VK_BROWSER_REFRESH, /* Browser Refresh */
            BROWSER_STOP = VK_BROWSER_STOP, /* Browser Stop */
            BROWSER_SEARCH = VK_BROWSER_SEARCH, /* Browser Search */
            BROWSER_FAVORITES = VK_BROWSER_FAVORITES, /* Browser Favorites */
            BROWSER_HOME = VK_BROWSER_HOME, /* Browser Home */
            VOLUME_MUTE = VK_VOLUME_MUTE, /* Volume Mute */
            VOLUME_DOWN = VK_VOLUME_DOWN, /* Volume Down */
            VOLUME_UP = VK_VOLUME_UP, /* Volume Up */
            NEXT_TRACK = VK_MEDIA_NEXT_TRACK, /* Next Track */
            PREV_TRACK = VK_MEDIA_PREV_TRACK, /* Previous Track */
            STOP = VK_MEDIA_STOP, /* Stop */
            PLAY_PAUSE = VK_MEDIA_PLAY_PAUSE, /* Play/Pause */
            LAUNCH_MAIL = VK_LAUNCH_MAIL, /* Launch Mail */
            LAUNCH_MEDIA_SELECT = VK_LAUNCH_MEDIA_SELECT, /* Select Media */
            LAUNCH_APP1 = VK_LAUNCH_APP1, /* Launch Application 1 */
            LAUNCH_APP2 = VK_LAUNCH_APP2, /* Launch Application 2 */
            MISC = VK_OEM_8, /* Unknown Miscellaneous Character [VK_OEM_8] */
            PROCCESS = VK_PROCESSKEY, /* Process Key */
            ATTN = VK_ATTN, /* Attn Key */
            CRSEL = VK_CRSEL, /* CrSel Key */
            EXSEL = VK_EXSEL, /* ExSel Key */
            EREOF = VK_EREOF, /* Erase EOF Key */
            PLAY = VK_PLAY, /* Play key */
            ZOOM = VK_ZOOM, /* Zoom key */
            PA1 = VK_PA1, /* PA1 key */
            CLEAR2 = VK_OEM_CLEAR, /* OEM Clear */

            // Other (Not documented)

            KPEQUAL = VK_OEM_NEC_EQUAL, /* numpad = */
            DICTIONARY = VK_OEM_FJ_JISHO, /* Dictionary key */
            UNREGISTER = VK_OEM_FJ_MASSHOU, /* Unregister word key */
            REGISTERE = VK_OEM_FJ_TOUROKU, /* Register word key */
            OYAYUBIL = VK_OEM_FJ_LOYA, /* Left OYAYUBI key */
            OYAYUBIR = VK_OEM_FJ_ROYA, /* Right OYAYUBI key */

            // Various extended or enhanced keyboards
            AX = VK_OEM_AX, /* Japanese AX key */
            // MISC2 = VK_OEM_102       = 0xE2;  //  "<>" or "\|" on RT 102-key kbd.
            HELP_ICO = VK_ICO_HELP, /* Help key on ICO */
            _00_ICO = VK_ICO_00, /* 00 key on ICO */
            CLEAR_ICO = VK_ICO_CLEAR, /* Clear key on ICO (i think) */

            // Nokia/Ericsson definitions

            RESET = VK_OEM_RESET,
            JUMP = VK_OEM_JUMP,
            PA1_2 = VK_OEM_PA1, // OEM PA1 key
            PA2 = VK_OEM_PA2,
            PA3 = VK_OEM_PA3,
            WSCTRL = VK_OEM_WSCTRL,
            CUSEL = VK_OEM_CUSEL,
            ATTN2 = VK_OEM_ATTN, // OEM Attn key
            FINISH = VK_OEM_FINISH,
            COPY = VK_OEM_COPY,
            AUTO = VK_OEM_AUTO,
            ENLW = VK_OEM_ENLW,
            BACKTAB = VK_OEM_BACKTAB,

            // Gamepad

            GAMEPAD_A = VK_GAMEPAD_A, /* Gamepad A */
            GAMEPAD_B = VK_GAMEPAD_B, /* Gamepad B */
            GAMEPAD_X = VK_GAMEPAD_X, /* Gamepad X */
            GAMEPAD_Y = VK_GAMEPAD_Y, /* Gamepad Y */
            GAMEPAD_RIGHT_SHOULDER = VK_GAMEPAD_RIGHT_SHOULDER, /* Gamepad Right Shoulder */
            GAMEPAD_LEFT_SHOULDER = VK_GAMEPAD_LEFT_SHOULDER, /* Gamepad Left Shoulder */
            GAMEPAD_LEFT_TRIGGER = VK_GAMEPAD_LEFT_TRIGGER, /* Gamepad Left Trigger */
            GAMEPAD_RIGHT_TRIGGER = VK_GAMEPAD_RIGHT_TRIGGER, /* Gamepad Right Trigger */
            GAMEPAD_DPAD_UP = VK_GAMEPAD_DPAD_UP, /* Gamepad Dpad Up */
            GAMEPAD_DPAD_DOWN = VK_GAMEPAD_DPAD_DOWN, /* Gamepad Dpad Down */
            GAMEPAD_DPAD_LEFT = VK_GAMEPAD_DPAD_LEFT, /* Gamepad Dpad Left */
            GAMEPAD_DPAD_RIGHT = VK_GAMEPAD_DPAD_RIGHT, /* Gamepad Dpad Right */
            GAMEPAD_MENU = VK_GAMEPAD_MENU, /* Gamepad Menu */
            GAMEPAD_VIEW = VK_GAMEPAD_VIEW, /* Gamepad View */
            GAMEPAD_LEFT_THUMBSTICK_BUTTON = VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON, /* Gamepad Left Thumbstick Button */
            GAMEPAD_RIGHT_THUMBSTICK_BUTTON = VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON, /* Gamepad Right Thumbstick Button */
            GAMEPAD_LEFT_THUMBSTICK_UP = VK_GAMEPAD_LEFT_THUMBSTICK_UP, /* Gamepad Left Thumbstick Up */
            GAMEPAD_LEFT_THUMBSTICK_DOWN = VK_GAMEPAD_LEFT_THUMBSTICK_DOWN, /* Gamepad Left Thumbstick Down */
            GAMEPAD_LEFT_THUMBSTICK_RIGHT = VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT, /* Gamepad Left Thumbstick Right */
            GAMEPAD_LEFT_THUMBSTICK_LEFT = VK_GAMEPAD_LEFT_THUMBSTICK_LEFT, /* Gamepad Left Thumbstick Left */
            GAMEPAD_RIGHT_THUMBSTICK_UP = VK_GAMEPAD_RIGHT_THUMBSTICK_UP, /* Gamepad Right Thumbstick Up */
            GAMEPAD_RIGHT_THUMBSTICK_DOWN = VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN, /* Gamepad Right Thumbstick Down */
            GAMEPAD_RIGHT_THUMBSTICK_RIGHT = VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT, /* Gamepad Right Thumbstick Right */
            GAMEPAD_RIGHT_THUMBSTICK_LEFT = VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT, /* Gamepad Right Thumbstick Left */

        #elif __linux__
            // KT_LATIN is always 0 so it's just the values
            NUL = 0, /* ␀ */
            CTRL_A = 1, /* Ctrl + A */
            CTRL_B = 2, /* Ctrl + B */
            CTRL_C = 3, /* Ctrl + C */
            CTRL_D = 4, /* Ctrl + D */
            CTRL_E = 5, /* Ctrl + E */
            CTRL_F = 6, /* Ctrl + F */
            CTRL_G = 7, /* Ctrl + G */
            BACKSPACE = 8, /* ← [Ctrl + H] */
            TAB = 9, /* ⇥ [Ctrl + I] */
            LINEFEED = 10, /* ␊ [Ctrl + J] */
            CTRL_K = 11, /* Ctrl + K */
            CTRL_L = 12, /* Ctrl + L */
            CTRL_M = 13, /* Ctrl + M */
            CTRL_N = 14, /* Ctrl + N */
            CTRL_O = 15, /* Ctrl + O */
            CTRL_P = 16, /* Ctrl + P */
            CTRL_Q = 17, /* Ctrl + Q */
            CTRL_R = 18, /* Ctrl + R */
            CTRL_S = 19, /* Ctrl + S */
            CTRL_T = 20, /* Ctrl + T */
            CTRL_U = 21, /* Ctrl + U */
            CTRL_V = 22, /* Ctrl + V */
            CTRL_W = 23, /* Ctrl + W */
            CTRL_X = 24, /* Ctrl + X */
            CTRL_Y = 25, /* Ctrl + Y */
            CTRL_Z = 26, /* Ctrl + Z */
            ESC = 27, /* ␛ */
            CTRL_BACKSLASH = 28, /* Ctrl + \ */
            CTRL_BRACKETRIGHT = 29, /* Ctrl + [ */
            CTRL_ASCIICIRCUM = 30, /* Ctrl + ^ */
            CTRL_UNEDERSCORE = 31, /* Ctrl + _ */
            SPACE = 32, /* ⎵ */
            ESCLAM = 33, /* ! */
            DBLQUOTE = 34, /* " */
            NUMBERSIGN = 35, /* # */
            DOLLAR = 36, /* $ */
            PERCENT = 37, /* % */
            AMPERSAND = 38, /* & */
            APOSTROPHE = 39, /* ' */
            PARENLEFT = 40, /* ( */
            PARENrIGHT = 41, /* ) */
            ASTERISK = 42, /* * */
            PLUS = 43, /* + */
            COMMA = 44, /* , */
            MINUS = 45, /* - */
            PERIOD = 46, /* . */
            SLASH = 47, /* / */
            /*$0*/ZERO = 48, /* 0 */
            /*$1*/ONE = 49, /* 1 */
            /*$2*/TWO = 50, /* 2 */
            /*$3*/THREE = 51, /* 3 */
            /*$4*/FOUR = 52, /* 4 */
            /*$5*/FIVE = 53, /* 5 */
            /*$6*/SIX = 54, /* 6 */
            /*$7*/SEVEN = 55, /* 7 */
            /*$8*/EIGHT = 56, /* 8 */
            /*$9*/NINE = 57, /* 9 */
            COLON = 58, /* , */
            SEMICOLON = 59, /* ; */
            LESS = 60, /* < */
            EQUAL = 61, /* = */
            GREATER = 62, /* > */
            QUESTION = 63, /* ? */
            AT = 64, /* @ */
            A = 65, /* A */
            B = 66, /* B */
            C = 67, /* C */
            D = 68, /* D */
            E = 69, /* E */
            F = 70, /* F */
            G = 71, /* G */
            H = 72, /* H */
            I = 73, /* I */
            J = 74, /* J */
            K = 75, /* K */
            L = 76, /* L */
            M = 77, /* M */
            N = 78, /* N */
            O = 79, /* O */
            P = 80, /* P */
            Q = 81, /* Q */
            R = 82, /* R */
            S = 83, /* S */
            T = 84, /* T */
            U = 85, /* U */
            V = 86, /* V */
            W = 87, /* W */
            X = 88, /* X */
            Y = 89, /* Y */
            Z = 90, /* Z */
            BRACKETLEFT = 91, /* [ */
            BACKSLASH = 92, /* \ */
            BRACKETRIGHT = 93, /* ] */
            ASCIICIRCUM = 94, /* ^ */
            UNEDERSCORE = 95, /* _ */
            GRAVE = 96, /* ` */
            a = 97, /* a */
            b = 98, /* b */
            c = 99, /* c */
            d = 100, /* d */
            e = 101, /* e */
            f = 102, /* f */
            g = 103, /* g */
            h = 104, /* h */
            i = 105, /* i */
            j = 106, /* j */
            k = 107, /* k */
            l = 108, /* l */
            m = 109, /* m */
            n = 110, /* n */
            o = 111, /* o */
            p = 112, /* p */
            q = 113, /* q */
            r = 114, /* r */
            s = 115, /* s */
            t = 116, /* t */
            u = 117, /* u */
            v = 118, /* v */
            w = 119, /* w */
            x = 120, /* x */
            y = 121, /* y */
            z = 122, /* z */
            BRACELEFT = 123, /* { */
            BAR = 124, /* | */
            BRACERIGHT = 125, /* } */
            ASCIITILDE = 126, /* ~ */
            DELETE = 127, /* ␡ */
            NOBREAKSPACE = 128, /* ⍽ */
            EXCLAMDOWN = 129, /* ¡ */
            CENT = 130, /* ¢ */
            STERLING = 131, /* £ */
            CURRENCY = 132, /* ¤ */
            YEN = 133, /* ¥ */
            BROKENBAR = 134, /* ¦ */
            SECTION = 135, /* § */
            DIAERESIS = 136, /* ¨ */
            COPYRIGHT = 137, /* © */
            ORDFEMININE = 138, /* ª */
            GUILLEMOTLEFT = 139, /* « */
            NOTSIGN = 140, /* ¬ */
            HYPHEN = 141, /* - */
            REGISTERED = 142, /* ® */
            MACRON = 143, /* ¯ */
            DEGREE = 144, /* ° */
            PLUSMINUS = 145, /* ± */
            TWOSUPERIOR = 146, /* ² */
            THREESUPERIOR = 147, /* ³ */
            ACUTE = 148, /* ´ */
            MU = 149, /* μ */
            PARAGRAPH = 150, /* ¶ */
            PERIODCENTERED = 151, /* · */
            CEDILLA = 152, /* ç */
            ONESUPERIOR = 153, /* ¹ */
            MASCULINE = 154, /* ♂ */
            GUILLEMOTRIGHT = 155, /* » */
            ONEQUARTER = 156, /* ¼ */
            ONEHALF = 157, /* ½ */
            THREEQUARTERS = 158, /* ¾ */
            QUESTIONDOWN = 159, /* ¿ */
            A_GRAVE = 160, /* À */
            A_ACUTE = 161, /* Á */
            A_CIRCUMFLEX = 162, /* Â */
            A_TILDE = 163, /* Ã */
            A_DIAERESIS = 164, /* Ä */
            A_RING = 165, /* Å */
            AE = 166, /* Æ */
            C_CEDILLA = 167, /* Ç */
            E_GRAVE = 168, /* È */
            E_ACUTE = 169, /* É */
            E_CIRCUMFLEX = 170, /* Ê */
            E_DIAERESIS = 171, /* Ë */
            I_GRAVE = 172, /* Ì */
            I_ACUTE = 173, /* Í */
            I_CIRCUMFLEX = 174, /* Î */
            I_DIAERESIS = 175, /* Ï */
            ETH = 176, /* Ð */
            N_TILDE = 177, /* Ñ */
            O_GRAVE = 178, /* Ò */
            O_ACUTE = 179, /* Ó */
            O_CIRCUMFLEX = 180, /* Ô */
            O_TILDE = 181, /* Õ */
            O_DIAERESIS = 182, /* Ö */
            MULTIPLY = 183, /* × */
            O_SLASH = 184, /* Ø */
            U_GRAVE = 185, /* Ù */
            U_ACUTE = 186, /* Ú */
            U_CIRCUMFLEX = 187, /* Û */
            U_DIAERESIS = 188, /* Ü */
            Y_ACUTE = 189, /* Ý */
            THORN = 190, /* Þ */
            SHARP_S = 191, /* ß */
            a_grave = 192, /* à */
            a_acute = 193, /* á */
            a_circumflex = 194, /* â */
            a_tilde = 195, /* ã */
            a_diaeresis = 196, /* ä */
            a_ring = 197, /* å */
            ae = 198, /* æ */
            c_cedilla = 199, /* ç */
            e_grave = 200, /* è */
            e_acute = 201, /* é */
            e_circumflex = 202, /* ê */
            e_diaeresis = 203, /* ë */
            i_grave = 204, /* ì */
            i_acute = 205, /* í */
            i_circumflex = 206, /* î */
            i_diaeresis = 207, /* ï */
            eth = 208, /* ð */
            n_tilde = 209, /* ñ */
            o_grave = 210, /* ò */
            o_acute = 211, /* ó */
            o_circumflex = 212, /* ô */
            o_tilde = 213, /* õ */
            o_diaeresis = 214, /* ö */
            DIVISION = 215, /* ÷ */
            o_slash = 216, /* ø */
            u_grave = 217, /* ù */
            u_acute = 218, /* ú */
            u_circumflex = 219, /* û */
            u_diaeresis = 220, /* ü */
            y_acute = 221, /* ý */
            thorn = 222, /* þ */
            y_diaeresis = 223, /* ÿ */

            // KT_FN
            F1 = K_F1, /* F1 */
            F2 = K_F2, /* F2 */
            F3 = K_F3, /* F3 */
            F4 = K_F4, /* F4 */
            F5 = K_F5, /* F5 */
            F6 = K_F6, /* F6 */
            F7 = K_F7, /* F7 */
            F8 = K_F8, /* F8 */
            F9 = K_F9, /* F9 */
            F10 = K_F10, /* F10 */
            F11 = K_F11, /* F11 */
            F12 = K_F12, /* F12 */
            F13 = K_F13, /* F13 */
            F14 = K_F14, /* F14 */
            F15 = K_F15, /* F15 */
            F16 = K_F16, /* F16 */
            F17 = K_F17, /* F17 */
            F18 = K_F18, /* F18 */
            F19 = K_F19, /* F19 */
            F20 = K_F20, /* F20 */
            FIND = K_FIND, /* Find [Home] */
            INSERT = K_INSERT, /* Insert */
            REMOVE = K_REMOVE, /* Remove */
            END = K_SELECT, /* Select [End]      ??? */
            //SELECT = K_SELECT, /* Select [End] ??? */
            PGUP = K_PGUP, /* Prior [PageUp] */
            PGDN = K_PGDN, /* Next [PageDown] */
            MACRO = K_MACRO, /* Macro */
            HELP = K_HELP, /* Help */
            DO = K_DO, /* Do */
            PAUSE = K_PAUSE, /* Pause */
            F21 = K_F21, /* F21 */
            F22 = K_F22, /* F22 */
            F23 = K_F23, /* F23 */
            F24 = K_F24, /* F24 */
            F25 = K_F25, /* F25 */
            F26 = K_F26, /* F26 */
            F27 = K_F27, /* F27 */
            F28 = K_F28, /* F28 */
            F29 = K_F29, /* F29 */
            F30 = K_F30, /* F30 */
            F31 = K_F31, /* F31 */
            F32 = K_F32, /* F32 */
            F33 = K_F33, /* F33 */
            F34 = K_F34, /* F34 */
            F35 = K_F35, /* F35 */
            F36 = K_F36, /* F36 */
            F37 = K_F37, /* F37 */
            F38 = K_F38, /* F38 */
            F39 = K_F39, /* F39 */
            F40 = K_F40, /* F40 */
            F41 = K_F41, /* F41 */
            F42 = K_F42, /* F42 */
            F43 = K_F43, /* F43 */
            F44 = K_F44, /* F44 */
            F45 = K_F45, /* F45 */
            F46 = K_F46, /* F46 */
            F47 = K_F47, /* F47 */
            F48 = K_F48, /* F48 */
            F49 = K_F49, /* F49 */
            F50 = K_F50, /* F50 */
            F51 = K_F51, /* F51 */
            F52 = K_F52, /* F52 */
            F53 = K_F53, /* F53 */
            F54 = K_F54, /* F54 */
            F55 = K_F55, /* F55 */
            F56 = K_F56, /* F56 */
            F57 = K_F57, /* F57 */
            F58 = K_F58, /* F58 */
            F59 = K_F59, /* F59 */
            F60 = K_F60, /* F60 */
            F61 = K_F61, /* F61 */
            F62 = K_F62, /* F62 */
            F63 = K_F63, /* F63 */
            F64 = K_F64, /* F64 */
            F65 = K_F65, /* F65 */
            F66 = K_F66, /* F66 */
            F67 = K_F67, /* F67 */
            F68 = K_F68, /* F68 */
            F69 = K_F69, /* F69 */
            F70 = K_F70, /* F70 */
            F71 = K_F71, /* F71 */
            F72 = K_F72, /* F72 */
            F73 = K_F73, /* F73 */
            F74 = K_F74, /* F74 */
            F75 = K_F75, /* F75 */
            F76 = K_F76, /* F76 */
            F77 = K_F77, /* F77 */
            F78 = K_F78, /* F78 */
            F79 = K_F79, /* F79 */
            F80 = K_F80, /* F80 */
            F81 = K_F81, /* F81 */
            F82 = K_F82, /* F82 */
            F83 = K_F83, /* F83 */
            F84 = K_F84, /* F84 */
            F85 = K_F85, /* F85 */
            F86 = K_F86, /* F86 */
            F87 = K_F87, /* F87 */
            F88 = K_F88, /* F88 */
            F89 = K_F89, /* F89 */
            F90 = K_F90, /* F90 */
            F91 = K_F91, /* F91 */
            F92 = K_F92, /* F92 */
            F93 = K_F93, /* F93 */
            F94 = K_F94, /* F94 */
            F95 = K_F95, /* F95 */
            F96 = K_F96, /* F96 */
            F97 = K_F97, /* F97 */
            F98 = K_F98, /* F98 */
            F99 = K_F99, /* F99 */
            F100 = K_F100, /* F100 */
            F101 = K_F101, /* F101 */
            F102 = K_F102, /* F102 */
            F103 = K_F103, /* F103 */
            F104 = K_F104, /* F104 */
            F105 = K_F105, /* F105 */
            F106 = K_F106, /* F106 */
            F107 = K_F107, /* F107 */
            F108 = K_F108, /* F108 */
            F109 = K_F109, /* F109 */
            F110 = K_F110, /* F110 */
            F111 = K_F111, /* F111 */
            F112 = K_F112, /* F112 */
            F113 = K_F113, /* F113 */
            F114 = K_F114, /* F114 */
            F115 = K_F115, /* F115 */
            F116 = K_F116, /* F116 */
            F117 = K_F117, /* F117 */
            F118 = K_F118, /* F118 */
            F119 = K_F119, /* F119 */
            F120 = K_F120, /* F120 */
            F121 = K_F121, /* F121 */
            F122 = K_F122, /* F122 */
            F123 = K_F123, /* F123 */
            F124 = K_F124, /* F124 */
            F125 = K_F125, /* F125 */
            F126 = K_F126, /* F126 */
            F127 = K_F127, /* F127 */
            F128 = K_F128, /* F128 */
            F129 = K_F129, /* F129 */
            F130 = K_F130, /* F130 */
            F131 = K_F131, /* F131 */
            F132 = K_F132, /* F132 */
            F133 = K_F133, /* F133 */
            F134 = K_F134, /* F134 */
            F135 = K_F135, /* F135 */
            F136 = K_F136, /* F136 */
            F137 = K_F137, /* F137 */
            F138 = K_F138, /* F138 */
            F139 = K_F139, /* F139 */
            F140 = K_F140, /* F140 */
            F141 = K_F141, /* F141 */
            F142 = K_F142, /* F142 */
            F143 = K_F143, /* F143 */
            F144 = K_F144, /* F144 */
            F145 = K_F145, /* F145 */
            F146 = K_F146, /* F146 */
            F147 = K_F147, /* F147 */
            F148 = K_F148, /* F148 */
            F149 = K_F149, /* F149 */
            F150 = K_F150, /* F150 */
            F151 = K_F151, /* F151 */
            F152 = K_F152, /* F152 */
            F153 = K_F153, /* F153 */
            F154 = K_F154, /* F154 */
            F155 = K_F155, /* F155 */
            F156 = K_F156, /* F156 */
            F157 = K_F157, /* F157 */
            F158 = K_F158, /* F158 */
            F159 = K_F159, /* F159 */
            F160 = K_F160, /* F160 */
            F161 = K_F161, /* F161 */
            F162 = K_F162, /* F162 */
            F163 = K_F163, /* F163 */
            F164 = K_F164, /* F164 */
            F165 = K_F165, /* F165 */
            F166 = K_F166, /* F166 */
            F167 = K_F167, /* F167 */
            F168 = K_F168, /* F168 */
            F169 = K_F169, /* F169 */
            F170 = K_F170, /* F170 */
            F171 = K_F171, /* F171 */
            F172 = K_F172, /* F172 */
            F173 = K_F173, /* F173 */
            F174 = K_F174, /* F174 */
            F175 = K_F175, /* F175 */
            F176 = K_F176, /* F176 */
            F177 = K_F177, /* F177 */
            F178 = K_F178, /* F178 */
            F179 = K_F179, /* F179 */
            F180 = K_F180, /* F180 */
            F181 = K_F181, /* F181 */
            F182 = K_F182, /* F182 */
            F183 = K_F183, /* F183 */
            F184 = K_F184, /* F184 */
            F185 = K_F185, /* F185 */
            F186 = K_F186, /* F186 */
            F187 = K_F187, /* F187 */
            F188 = K_F188, /* F188 */
            F189 = K_F189, /* F189 */
            F190 = K_F190, /* F190 */
            F191 = K_F191, /* F191 */
            F192 = K_F192, /* F192 */
            F193 = K_F193, /* F193 */
            F194 = K_F194, /* F194 */
            F195 = K_F195, /* F195 */
            F196 = K_F196, /* F196 */
            F197 = K_F197, /* F197 */
            F198 = K_F198, /* F198 */
            F199 = K_F199, /* F199 */
            F200 = K_F200, /* F200 */
            F201 = K_F201, /* F201 */
            F202 = K_F202, /* F202 */
            F203 = K_F203, /* F203 */
            F204 = K_F204, /* F204 */
            F205 = K_F205, /* F205 */
            F206 = K_F206, /* F206 */
            F207 = K_F207, /* F207 */
            F208 = K_F208, /* F208 */
            F209 = K_F209, /* F209 */
            F210 = K_F210, /* F210 */
            F211 = K_F211, /* F211 */
            F212 = K_F212, /* F212 */
            F213 = K_F213, /* F213 */
            F214 = K_F214, /* F214 */
            F215 = K_F215, /* F215 */
            F216 = K_F216, /* F216 */
            F217 = K_F217, /* F217 */
            F218 = K_F218, /* F218 */
            F219 = K_F219, /* F219 */
            F220 = K_F220, /* F220 */
            F221 = K_F221, /* F221 */
            F222 = K_F222, /* F222 */
            F223 = K_F223, /* F223 */
            F224 = K_F224, /* F224 */
            F225 = K_F225, /* F225 */
            F226 = K_F226, /* F226 */
            F227 = K_F227, /* F227 */
            F228 = K_F228, /* F228 */
            F229 = K_F229, /* F229 */
            F230 = K_F230, /* F230 */
            F231 = K_F231, /* F231 */
            F232 = K_F232, /* F232 */
            F233 = K_F233, /* F233 */
            F234 = K_F234, /* F234 */
            F235 = K_F235, /* F235 */
            F236 = K_F236, /* F236 */
            F237 = K_F237, /* F237 */
            F238 = K_F238, /* F238 */
            F239 = K_F239, /* F239 */
            F240 = K_F240, /* F240 */
            F241 = K_F241, /* F241 */
            F242 = K_F242, /* F242 */
            F243 = K_F243, /* F243 */
            F244 = K_F244, /* F244 */
            F245 = K_F245, /* F245 */
            UNDO = K_UNDO, /* Undo */

            // KT_SPEC
            HOLE = K_HOLE, /* VoidSymbol */
            ENTER = K_ENTER, /* Return */
            SH_REGS = K_SH_REGS, /* Show Registers */
            SH_MEM = K_SH_MEM, /* Show Memory */
            SH_STAT = K_SH_STAT, /* Show State */
            BREAK = K_BREAK, /* Break */
            CONS = K_CONS, /* Last Console */
            CAPS = K_CAPS, /* Caps Lock */
            NUM = K_NUM, /* Num Lock */
            HOLD = K_HOLD, /* Scroll Lock */
            SCROLLFORW = K_SCROLLFORW, /* Scroll Forward */
            SCROLLBACK = K_SCROLLBACK, /* Scroll Backward */
            BOOT = K_BOOT, /* Boot */
            CAPSON = K_CAPSON, /* Caps On */
            COMPOSE = K_COMPOSE, /* Compose */
            SAK = K_SAK, /* SAK */
            DECRCONSOLE = K_DECRCONSOLE, /* Decrease Console */
            INCRCONSOLE = K_INCRCONSOLE, /* Increase Console */
            SPAWNCONSOLE = K_SPAWNCONSOLE, /* EnumboardSignal */
            BARENUMLOCK = K_BARENUMLOCK, /* Bare Num Lock */

            // KT_PAD
            KP0 = K_P0, /* Numpad 0 */
            KP1 = K_P1, /* Numpad 1 */
            KP2 = K_P2, /* Numpad 2 */
            KP3 = K_P3, /* Numpad 3 */
            KP4 = K_P4, /* Numpad 4 */
            KP5 = K_P5, /* Numpad 5 */
            KP6 = K_P6, /* Numpad 6 */
            KP7 = K_P7, /* Numpad 7 */
            KP8 = K_P8, /* Numpad 8 */
            KP9 = K_P9, /* Numpad 9 */
            KPPLUS = K_PPLUS, /* Numpad + */
            KPMINUS = K_PMINUS, /* Numpad - */
            KPSTAR = K_PSTAR, /* Numpad * */
            KPSLASH = K_PSLASH, /* Numpad / */
            KPENTER = K_PENTER, /* Numpad Enter */
            KPCOMMA = K_PCOMMA, /* Numpad , */
            KPDOT = K_PDOT, /* Numpad . */
            KPPLUSMINUS = K_PPLUSMINUS, /* Numpad ± */
            KPPARENL = K_PPARENL, /* Numpad ( */
            KPPARENR = K_PPARENR, /* Numpad ) */

            // KT_DEAD
            DGRAVE = K_DGRAVE, /* [Dead] Grave */
            DACUTE = K_DACUTE, /* [Dead] Acute */
            DCIRCM = K_DCIRCM, /* [Dead] CircumFlex */
            DTILDE = K_DTILDE, /* [Dead] Tilde */
            DDIERE = K_DDIERE, /* [Dead] Diaeresis */
            DCEDIL = K_DCEDIL, /* [Dead] Cedilla */
            DMACRON = K_DMACRON, /* [Dead] Macron */
            DBREVE = K_DBREVE, /* [Dead] Breve (an alias for [Dead] Tilde) */
            DABDOT = K_DABDOT, /* [Dead] Abovedot */
            DABRING = K_DABRING, /* [Dead] Abovering */
            DDBACUTE = K_DDBACUTE, /* [Dead] DoubleAcute (an alias for [Dead] Tilde) */
            DCARON = K_DCARON, /* [Dead] Caron (an alias for [Dead] Circumflex) */
            DOGONEK = K_DOGONEK, /* [Dead] Ogonek (an alias for [Dead] Cedilla) */
            DIOTA = K_DIOTA, /* [Dead] Iota */
            DVOICED = K_DVOICED, /* [Dead] Voiced sound */
            DSEMVOICED = K_DSEMVOICED, /* [Dead] Semivoiced sound */
            DBEDOT = K_DBEDOT, /* [Dead] BelowDot */
            DHOOK = K_DHOOK, /* [Dead] Hook */
            DHORN = K_DHORN, /* [Dead] Horn */
            DSTROKE = K_DSTROKE, /* [Dead] Stroke */
            DABCOMMA = K_DABCOMMA, /* [Dead] Abovecomma */
            DABREVCOMMA = K_DABREVCOMMA, /* [Dead] Abovereversedcomma */
            DDBGRAVE = K_DDBGRAVE, /* [Dead] Doublegrave */
            DINVBREVE = K_DINVBREVE, /* [Dead] Inverted breve */
            DBECOMMA = K_DBECOMMA, /* [Dead] Belowcomma */
            DCURRENCY = K_DCURRENCY, /* [Dead] Currency */
            DGREEK = K_DGREEK, /* [Dead] Greek */

            // KT_CUR
            ARROW_DOWN = K_DOWN, /* Down Arrow */
            ARROW_LEFT = K_LEFT, /* Left Arrow */
            ARROW_RIGHT = K_RIGHT, /* Right Arrow */
            ARROW_UP = K_UP, /* Up Arrow */

            // KT_SHIFT
            SHIFT = K_SHIFT, /* Shift */
            CTRL = K_CTRL, /* AltGramar */
            ALT = K_ALT, /* Control */
            ALTGR = K_ALTGR, /* Alt */
            SHIFTL = K_SHIFTL, /* ShiftLeft */
            SHIFTR = K_SHIFTR, /* ShiftRight */
            CTRLL = K_CTRLL, /* CtrlLeft */
            CTRLR = K_CTRLR, /* CtrlRight */
            CAPSSHIFT = K_CAPSSHIFT, /* CapsShift */

            // KT_ASCII
            ASC0 = K_ASC0, /* Ascii_0 */
            ASC1 = K_ASC1, /* Ascii_1 */
            ASC2 = K_ASC2, /* Ascii_2 */
            ASC3 = K_ASC3, /* Ascii_3 */
            ASC4 = K_ASC4, /* Ascii_4 */
            ASC5 = K_ASC5, /* Ascii_5 */
            ASC6 = K_ASC6, /* Ascii_6 */
            ASC7 = K_ASC7, /* Ascii_7 */
            ASC8 = K_ASC8, /* Ascii_8 */
            ASC9 = K_ASC9, /* Ascii_9 */
            HEX0 = K_HEX0, /* Hex_0 */
            HEX1 = K_HEX1, /* Hex_1 */
            HEX2 = K_HEX2, /* Hex_2 */
            HEX3 = K_HEX3, /* Hex_3 */
            HEX4 = K_HEX4, /* Hex_4 */
            HEX5 = K_HEX5, /* Hex_5 */
            HEX6 = K_HEX6, /* Hex_6 */
            HEX7 = K_HEX7, /* Hex_7 */
            HEX8 = K_HEX8, /* Hex_8 */
            HEX9 = K_HEX9, /* Hex_9 */
            HEXa = K_HEXa, /* Hex_A */
            HEXb = K_HEXb, /* Hex_B */
            HEXc = K_HEXc, /* Hex_C */
            HEXd = K_HEXd, /* Hex_D */
            HEXe = K_HEXe, /* Hex_E */
            HEXf = K_HEXf, /* Hex_F */
            SHIFTLOCK = K_SHIFTLOCK, /* Shift Lock */
            CTRLLOCK = K_CTRLLOCK, /* AltGr Lock */
            ALTLOCK = K_ALTLOCK, /* Control Lock */
            ALTGRLOCK = K_ALTGRLOCK, /* Alt Lock */
            SHIFTLLOCK = K_SHIFTLLOCK, /* ShiftL Lock */
            SHIFTRLOCK = K_SHIFTRLOCK, /* ShiftR Lock */
            CTRLLLOCK = K_CTRLLLOCK, /* CtrlL Lock */
            CTRLRLOCK = K_CTRLRLOCK, /* CtrlR Lock */
            CAPSSHIFTLOCK = K_CAPSSHIFTLOCK, /* CapsShift Lock */
            SHIFT_SLOCK = K_SHIFT_SLOCK, /* Sticky Shift */
            CTRL_SLOCK = K_CTRL_SLOCK, /* Sticky AltGr */
            ALT_SLOCK = K_ALT_SLOCK, /* Sticky Control */
            ALTGR_SLOCK = K_ALTGR_SLOCK, /* Sticky Alt */
            SHIFTL_SLOCK = K_SHIFTL_SLOCK, /* Sticky ShiftLeft */
            SHIFTR_SLOCK = K_SHIFTR_SLOCK, /* Sticky ShiftRight */
            CTRLL_SLOCK = K_CTRLL_SLOCK, /* Sticky CtrlLeft */
            CTRLR_SLOCK = K_CTRLR_SLOCK, /* Sticky CtrlRight */
            CAPSSHIFT_SLOCK = K_CAPSSHIFT_SLOCK, /* Sticky CapsShift */
            BRL_BLANK = K_BRL_BLANK, /* Brl_blank */
            BRL_DOT1 = K_BRL_DOT1, /* Brl_dot1 */
            BRL_DOT2 = K_BRL_DOT2, /* Brl_dot2 */
            BRL_DOT3 = K_BRL_DOT3, /* Brl_dot3 */
            BRL_DOT4 = K_BRL_DOT4, /* Brl_dot4 */
            BRL_DOT5 = K_BRL_DOT5, /* Brl_dot5 */
            BRL_DOT6 = K_BRL_DOT6, /* Brl_dot6 */
            BRL_DOT7 = K_BRL_DOT7, /* Brl_dot7 */
            BRL_DOT8 = K_BRL_DOT8, /* Brl_dot8 */
            BRL_DOT9 = K_BRL_DOT9, /* Brl_dot9 */
            BRL_DOT10 = K_BRL_DOT10, /* Brl_dot10 */
            CONSOLE1 = ((((ushort)KT_CONS)<<8)|0), /* Console_1 */
            CONSOLE2 = ((((ushort)KT_CONS)<<8)|1), /* Console_2 */
            CONSOLE3 = ((((ushort)KT_CONS)<<8)|2), /* Console_3 */
            CONSOLE4 = ((((ushort)KT_CONS)<<8)|3), /* Console_4 */
            CONSOLE5 = ((((ushort)KT_CONS)<<8)|4), /* Console_5 */
            CONSOLE6 = ((((ushort)KT_CONS)<<8)|5), /* Console_6 */
            CONSOLE7 = ((((ushort)KT_CONS)<<8)|6), /* Console_7 */
            CONSOLE8 = ((((ushort)KT_CONS)<<8)|7), /* Console_8 */
            CONSOLE9 = ((((ushort)KT_CONS)<<8)|8), /* Console_9 */
            CONSOLE10 = ((((ushort)KT_CONS)<<8)|9), /* Console_10 */
            CONSOLE11 = ((((ushort)KT_CONS)<<8)|10), /* Console_11 */
            CONSOLE12 = ((((ushort)KT_CONS)<<8)|11), /* Console_12 */
            CONSOLE13 = ((((ushort)KT_CONS)<<8)|12), /* Console_13 */
            CONSOLE14 = ((((ushort)KT_CONS)<<8)|13), /* Console_14 */
            CONSOLE15 = ((((ushort)KT_CONS)<<8)|14), /* Console_15 */
            CONSOLE16 = ((((ushort)KT_CONS)<<8)|15), /* Console_16 */
            CONSOLE17 = ((((ushort)KT_CONS)<<8)|16), /* Console_17 */
            CONSOLE18 = ((((ushort)KT_CONS)<<8)|17), /* Console_18 */
            CONSOLE19 = ((((ushort)KT_CONS)<<8)|18), /* Console_19 */
            CONSOLE20 = ((((ushort)KT_CONS)<<8)|19), /* Console_20 */
            CONSOLE21 = ((((ushort)KT_CONS)<<8)|20), /* Console_21 */
            CONSOLE22 = ((((ushort)KT_CONS)<<8)|21), /* Console_22 */
            CONSOLE23 = ((((ushort)KT_CONS)<<8)|22), /* Console_23 */
            CONSOLE24 = ((((ushort)KT_CONS)<<8)|23), /* Console_24 */
            CONSOLE25 = ((((ushort)KT_CONS)<<8)|24), /* Console_25 */
            CONSOLE26 = ((((ushort)KT_CONS)<<8)|25), /* Console_26 */
            CONSOLE27 = ((((ushort)KT_CONS)<<8)|26), /* Console_27 */
            CONSOLE28 = ((((ushort)KT_CONS)<<8)|27), /* Console_28 */
            CONSOLE29 = ((((ushort)KT_CONS)<<8)|28), /* Console_29 */
            CONSOLE30 = ((((ushort)KT_CONS)<<8)|29), /* Console_30 */
            CONSOLE31 = ((((ushort)KT_CONS)<<8)|30), /* Console_31 */
            CONSOLE32 = ((((ushort)KT_CONS)<<8)|31), /* Console_32 */
            CONSOLE33 = ((((ushort)KT_CONS)<<8)|32), /* Console_33 */
            CONSOLE34 = ((((ushort)KT_CONS)<<8)|33), /* Console_34 */
            CONSOLE35 = ((((ushort)KT_CONS)<<8)|34), /* Console_35 */
            CONSOLE36 = ((((ushort)KT_CONS)<<8)|35), /* Console_36 */
            CONSOLE37 = ((((ushort)KT_CONS)<<8)|36), /* Console_37 */
            CONSOLE38 = ((((ushort)KT_CONS)<<8)|37), /* Console_38 */
            CONSOLE39 = ((((ushort)KT_CONS)<<8)|38), /* Console_39 */
            CONSOLE40 = ((((ushort)KT_CONS)<<8)|39), /* Console_40 */
            CONSOLE41 = ((((ushort)KT_CONS)<<8)|40), /* Console_41 */
            CONSOLE42 = ((((ushort)KT_CONS)<<8)|41), /* Console_42 */
            CONSOLE43 = ((((ushort)KT_CONS)<<8)|42), /* Console_43 */
            CONSOLE44 = ((((ushort)KT_CONS)<<8)|43), /* Console_44 */
            CONSOLE45 = ((((ushort)KT_CONS)<<8)|44), /* Console_45 */
            CONSOLE46 = ((((ushort)KT_CONS)<<8)|45), /* Console_46 */
            CONSOLE47 = ((((ushort)KT_CONS)<<8)|46), /* Console_47 */
            CONSOLE48 = ((((ushort)KT_CONS)<<8)|47), /* Console_48 */
            CONSOLE49 = ((((ushort)KT_CONS)<<8)|48), /* Console_49 */
            CONSOLE50 = ((((ushort)KT_CONS)<<8)|49), /* Console_50 */
            CONSOLE51 = ((((ushort)KT_CONS)<<8)|50), /* Console_51 */
            CONSOLE52 = ((((ushort)KT_CONS)<<8)|51), /* Console_52 */
            CONSOLE53 = ((((ushort)KT_CONS)<<8)|52), /* Console_53 */
            CONSOLE54 = ((((ushort)KT_CONS)<<8)|53), /* Console_54 */
            CONSOLE55 = ((((ushort)KT_CONS)<<8)|54), /* Console_55 */
            CONSOLE56 = ((((ushort)KT_CONS)<<8)|55), /* Console_56 */
            CONSOLE57 = ((((ushort)KT_CONS)<<8)|56), /* Console_57 */
            CONSOLE58 = ((((ushort)KT_CONS)<<8)|57), /* Console_58 */
            CONSOLE59 = ((((ushort)KT_CONS)<<8)|58), /* Console_59 */
            CONSOLE60 = ((((ushort)KT_CONS)<<8)|59), /* Console_60 */
            CONSOLE61 = ((((ushort)KT_CONS)<<8)|60), /* Console_61 */
            CONSOLE62 = ((((ushort)KT_CONS)<<8)|61), /* Console_62 */
            CONSOLE63 = ((((ushort)KT_CONS)<<8)|62), /* Console_63 */

            SELECT, /* Select [End] */
            CANCEL , /* Cancel */
            CLEAR , /* Clear */
            KANA , /* IME Kana mode */
            HANGUL , /* IME Hangul mode */
            IME_ON , /* IME On */
            JUNJA , /* IME Junja mode */
            FINAL , /* IME Final mode */
            HANJA , /* IME Hanja mode */
            KANJI , /* IME Kanji mode */
            IME_OFF , /* IME Off */
            CONVERT , /* IME Convert */
            NONCONVERT , /* IME NonConvert */
            ACCEPT , /* IME Accept */
            MODECHANGE , /* IME ModeChange */
            PRINT , /* Print Key */
            EXECUTE , /* Execute Key */
            SNAPSHOT , /* Print Screen Key */
            SUPERL , /* Left Super[Windows] key */
            SUPERR , /* Right Super[Windows] key */
            APPS , /* Applications Key */
            SLEEP , /* Computer Sleep Key */
            AALT , /* Any Alt */
            BROWSER_BACK , /* Browser Back */
            BROWSER_FORWARD , /* Browser Forward */
            BROWSER_REFRESH , /* Browser Refresh */
            BROWSER_STOP , /* Browser Stop */
            BROWSER_SEARCH , /* Browser Search */
            BROWSER_FAVORITES , /* Browser Favorites */
            BROWSER_HOME , /* Browser Home */
            VOLUME_MUTE , /* Volume Mute */
            VOLUME_DOWN , /* Volume Down */
            VOLUME_UP , /* Volume Up */
            NEXT_TRACK , /* Next Track */
            PREV_TRACK , /* Previous Track */
            STOP , /* Stop */
            PLAY_PAUSE , /* Play/Pause */
            LAUNCH_MAIL , /* Launch Mail */
            LAUNCH_MEDIA_SELECT , /* Select Media */
            LAUNCH_APP1 , /* Launch Application 1 */
            LAUNCH_APP2 , /* Launch Application 2 */
            MISC , /* Unknown Miscellaneous Character [VK_OEM_8] */
            PROCCESS , /* Process Key */
            ATTN , /* Attn Key */
            CRSEL , /* CrSel Key */
            EXSEL , /* ExSel Key */
            EREOF , /* Erase EOF Key */
            PLAY , /* Play key */
            ZOOM , /* Zoom key */
            PA1 , /* PA1 key */
            CLEAR2 , /* OEM Clear */

            // Other (Not documented)

            KPEQUAL , /* numpad , */
            DICTIONARY , /* Dictionary key */
            UNREGISTER , /* Unregister word key */
            REGISTERE , /* Register word key */
            OYAYUBIL , /* Left OYAYUBI key */
            OYAYUBIR , /* Right OYAYUBI key */

            // Various extended or enhanced keyboards
            AX , /* Japanese AX key */
            // MISC2 , , //  "<>" or "\|" on RT 102-key kbd.
            HELP_ICO , /* Help key on ICO */
            _00_ICO , /* 00 key on ICO */
            CLEAR_ICO , /* Clear key on ICO (i think) */

            // Nokia/Ericsson definitions

            RESET , 
            JUMP , 
            PA1_2 , // OEM PA1 key
            PA2 , 
            PA3 , 
            WSCTRL , 
            CUSEL , 
            ATTN2 , // OEM Attn key
            FINISH , 
            COPY , 
            AUTO , 
            ENLW , 
            BACKTAB , 

            // Gamepad

            GAMEPAD_A , /* Gamepad A */
            GAMEPAD_B , /* Gamepad B */
            GAMEPAD_X , /* Gamepad X */
            GAMEPAD_Y , /* Gamepad Y */
            GAMEPAD_RIGHT_SHOULDER , /* Gamepad Right Shoulder */
            GAMEPAD_LEFT_SHOULDER , /* Gamepad Left Shoulder */
            GAMEPAD_LEFT_TRIGGER , /* Gamepad Left Trigger */
            GAMEPAD_RIGHT_TRIGGER , /* Gamepad Right Trigger */
            GAMEPAD_DPAD_UP , /* Gamepad Dpad Up */
            GAMEPAD_DPAD_DOWN , /* Gamepad Dpad Down */
            GAMEPAD_DPAD_LEFT , /* Gamepad Dpad Left */
            GAMEPAD_DPAD_RIGHT , /* Gamepad Dpad Right */
            GAMEPAD_MENU , /* Gamepad Menu */
            GAMEPAD_VIEW , /* Gamepad View */
            GAMEPAD_LEFT_THUMBSTICK_BUTTON , /* Gamepad Left Thumbstick Button */
            GAMEPAD_RIGHT_THUMBSTICK_BUTTON , /* Gamepad Right Thumbstick Button */
            GAMEPAD_LEFT_THUMBSTICK_UP , /* Gamepad Left Thumbstick Up */
            GAMEPAD_LEFT_THUMBSTICK_DOWN , /* Gamepad Left Thumbstick Down */
            GAMEPAD_LEFT_THUMBSTICK_RIGHT , /* Gamepad Left Thumbstick Right */
            GAMEPAD_LEFT_THUMBSTICK_LEFT , /* Gamepad Left Thumbstick Left */
            GAMEPAD_RIGHT_THUMBSTICK_UP , /* Gamepad Right Thumbstick Up */
            GAMEPAD_RIGHT_THUMBSTICK_DOWN , /* Gamepad Right Thumbstick Down */
            GAMEPAD_RIGHT_THUMBSTICK_RIGHT , /* Gamepad Right Thumbstick Right */
            GAMEPAD_RIGHT_THUMBSTICK_LEFT /* Gamepad Right Thumbstick Left */

        #elif __APPLE__
            NUL = 0, /* ␀ */
            HOLE = 0xff
        #elif __BSD__
            ...
        #else
            ...
        #endif
        }

        public class Class {
            static bool ContainsValue(ushort v) {
                return Key.Enum.IsDefined(typeof(Key.Enum), v);
            }
            static Key.Enum ToEnum(ushort v) {
                return (Key.Class.ContainsValue(v)) ? (Key.Enum)(v) : Key.Enum.HOLE;
            }
        }
    }

    public struct ToggledKeys {
        public bool CapsLock;
        public bool NumLock;
        public bool ScrollLock;
    };
}