#pragma once

//#include <winuser.h>
#include <windows.h>
#include <limits>

template<typename EnumType, EnumType UNDEFINED, EnumType... Values>
class EnumCheck;

template<typename EnumType, EnumType UNDEFINED>
class EnumCheck<EnumType, UNDEFINED>
{
public:
    static bool constexpr ContainsValue(std::underlying_type_t<EnumType>) {
    static_assert(std::is_enum_v<EnumType>);
        return false;
    }
    static EnumType constexpr ToEnum(std::underlying_type_t<EnumType>) {
    static_assert(std::is_enum_v<EnumType>);
        return UNDEFINED;
    }
};

template<typename EnumType, EnumType UNDEFINED, EnumType V, EnumType... Next>
class EnumCheck<EnumType, UNDEFINED, V, Next...> : private EnumCheck<EnumType, UNDEFINED, Next...> {
    using super = EnumCheck<EnumType, UNDEFINED, Next...>;
public:
    static bool constexpr ContainsValue(std::underlying_type_t<EnumType> v) {
        static_assert(std::is_enum_v<EnumType>);
        return (v == static_cast<std::underlying_type_t<EnumType>>(V) || super::ContainsValue(v));
    }
    static EnumType constexpr ToEnum(std::underlying_type_t<EnumType> v) {
        static_assert(std::is_enum_v<EnumType>);
        return (v == static_cast<std::underlying_type_t<EnumType>>(V)) ? static_cast<EnumType>(v) : super::ToEnum(v);
    }
};

struct ToggledKeys {
    bool CapsLock, NumLock, ScrollLock;
};

struct Key {
    enum class Enum : unsigned short {
        NONE = 0xffff, /* specified Enum is not defined */
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
        GRAVE = , /* `*/
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
        ALTLOCK , /* Control Lock */
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
        AALT = VK_ALT, /* Any Alt */
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
        MISC = VK_OEM_8 /* Unknown Miscellaneous Character [VK_OEM_8] */
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

        KPEQUAL = VK_OEM_NEC_EQUAL /* numpad = */
        DICTIONARY = VK_OEM_FJ_JISHO, /* Dictionary key */
        UNREGISTER = VK_OEM_FJ_MASSHOU, /* Unregister word key */
        REGISTERE = VK_OEM_FJ_TOUROKU, /* Register word key */
        OYAYUBIL = VK_OEM_FJ_LOYA, /* Left OYAYUBI key */
        OYAYUBIR = VK_OEM_FJ_ROYA, /* Right OYAYUBI key */

        // Various extended or enhanced keyboards
        AX = VK_OEM_AX, /* Japanese AX key */
        // MISC2 = VK_OEM_102       = 0xE2;  //  "<>" or "\|" on RT 102-key kbd.
        HELP_ICO = VK_OEM_ICO, /* Help key on ICO */
        00_ICO = VK_ICO_00 /* 00 key on ICO */
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
        GAMEPAD_RIGHT_THUMBSTICK_LEFT = VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT /* Gamepad Right Thumbstick Left */
    }
    using Class = EnumCheck<Enum, Enum::HOLE, Enum::NONE, Enum::NUL, Enum::CTRL_A, Enum::CTRL_B, Enum::CTRL_C, Enum::CTRL_D, Enum::CTRL_E, Enum::CTRL_F, Enum::CTRL_G, Enum::BACKSPACE, Enum::TAB, Enum::LINEFEED, Enum::CTRL_K, Enum::CTRL_L, Enum::CTRL_M, Enum::CTRL_N, Enum::CTRL_O, Enum::CTRL_P, Enum::CTRL_Q, Enum::CTRL_R, Enum::CTRL_S, Enum::CTRL_T, Enum::CTRL_U, Enum::CTRL_V, Enum::CTRL_W, Enum::CTRL_X, Enum::CTRL_Y, Enum::CTRL_Z, Enum::ESC, Enum::CTRL_BACKSLASH, Enum::CTRL_BRACKETRIGHT, Enum::CTRL_ASCIICIRCUM, Enum::CTRL_UNEDERSCORE, Enum::SPACE, Enum::ESCLAM, Enum::DBLQUOTE, Enum::NUMBERSIGN, Enum::DOLLAR, Enum::PERCENT, Enum::AMPERSAND, Enum::APOSTROPHE, Enum::PARENLEFT, Enum::PARENrIGHT, Enum::ASTERISK, Enum::PLUS, Enum::COMMA, Enum::MINUS, Enum::PERIOD, Enum::SLASH, Enum::/*$0*/ZERO, Enum::/*$1*/ONE, Enum::/*$2*/TWO, Enum::/*$3*/THREE, Enum::/*$4*/FOUR, Enum::/*$5*/FIVE, Enum::/*$6*/SIX, Enum::/*$7*/SEVEN, Enum::/*$8*/EIGHT, Enum::/*$9*/NINE, Enum::COLON, Enum::SEMICOLON, Enum::LESS, Enum::EQUAL, Enum::GREATER, Enum::QUESTION, Enum::AT, Enum::A, Enum::B, Enum::C, Enum::D, Enum::E, Enum::F, Enum::G, Enum::H, Enum::I, Enum::J, Enum::K, Enum::L, Enum::M, Enum::N, Enum::O, Enum::P, Enum::Q, Enum::R, Enum::S, Enum::T, Enum::U, Enum::V, Enum::W, Enum::X, Enum::Y, Enum::Z, Enum::BRACKETLEFT, Enum::BACKSLASH, Enum::BRACKETRIGHT, Enum::ASCIICIRCUM, Enum::UNEDERSCORE, Enum::GRAVE, Enum::a, Enum::b, Enum::c, Enum::d, Enum::e, Enum::f, Enum::g, Enum::h, Enum::i, Enum::j, Enum::k, Enum::l, Enum::m, Enum::n, Enum::o, Enum::p, Enum::q, Enum::r, Enum::s, Enum::t, Enum::u, Enum::v, Enum::w, Enum::x, Enum::y, Enum::z, Enum::BRACELEFT, Enum::BAR, Enum::BRACERIGHT, Enum::ASCIITILDE, Enum::DELETE, Enum::NOBREAKSPACE, Enum::EXCLAMDOWN, Enum::CENT, Enum::STERLING, Enum::CURRENCY, Enum::YEN, Enum::BROKENBAR, Enum::SECTION, Enum::DIAERESIS, Enum::COPYRIGHT, Enum::ORDFEMININE, Enum::GUILLEMOTLEFT, Enum::NOTSIGN, Enum::HYPHEN, Enum::REGISTERED, Enum::MACRON, Enum::DEGREE, Enum::PLUSMINUS, Enum::TWOSUPERIOR, Enum::THREESUPERIOR, Enum::ACUTE, Enum::MU, Enum::PARAGRAPH, Enum::PERIODCENTERED, Enum::CEDILLA, Enum::ONESUPERIOR, Enum::MASCULINE, Enum::GUILLEMOTRIGHT, Enum::ONEQUARTER, Enum::ONEHALF, Enum::THREEQUARTERS, Enum::QUESTIONDOWN, Enum::A_GRAVE, Enum::A_ACUTE, Enum::A_CIRCUMFLEX, Enum::A_TILDE, Enum::A_DIAERESIS, Enum::A_RING, Enum::AE, Enum::C_CEDILLA, Enum::E_GRAVE, Enum::E_ACUTE, Enum::E_CIRCUMFLEX, Enum::E_DIAERESIS, Enum::I_GRAVE, Enum::I_ACUTE, Enum::I_CIRCUMFLEX, Enum::I_DIAERESIS, Enum::ETH, Enum::N_TILDE, Enum::O_GRAVE, Enum::O_ACUTE, Enum::O_CIRCUMFLEX, Enum::O_TILDE, Enum::O_DIAERESIS, Enum::MULTIPLY, Enum::O_SLASH, Enum::U_GRAVE, Enum::U_ACUTE, Enum::U_CIRCUMFLEX, Enum::U_DIAERESIS, Enum::Y_ACUTE, Enum::THORN, Enum::SHARP_S, Enum::a_grave, Enum::a_acute, Enum::a_circumflex, Enum::a_tilde, Enum::a_diaeresis, Enum::a_ring, Enum::ae, Enum::c_cedilla, Enum::e_grave, Enum::e_acute, Enum::e_circumflex, Enum::e_diaeresis, Enum::i_grave, Enum::i_acute, Enum::i_circumflex, Enum::i_diaeresis, Enum::eth, Enum::n_tilde, Enum::o_grave, Enum::o_acute, Enum::o_circumflex, Enum::o_tilde, Enum::o_diaeresis, Enum::DIVISION, Enum::o_slash, Enum::u_grave, Enum::u_acute, Enum::u_circumflex, Enum::u_diaeresis, Enum::y_acute, Enum::thorn, Enum::y_diaeresis, Enum::F1, Enum::F2, Enum::F3, Enum::F4, Enum::F5, Enum::F6, Enum::F7, Enum::F8, Enum::F9, Enum::F10, Enum::F11, Enum::F12, Enum::F13, Enum::F14, Enum::F15, Enum::F16, Enum::F17, Enum::F18, Enum::F19, Enum::F20, Enum::FIND, Enum::INSERT, Enum::REMOVE, Enum::END, Enum::SELECT, Enum::PGUP, Enum::PGDN, Enum::MACRO, Enum::HELP, Enum::DO, Enum::PAUSE, Enum::F21, Enum::F22, Enum::F23, Enum::F24, Enum::F25, Enum::F26, Enum::F27, Enum::F28, Enum::F29, Enum::F30, Enum::F31, Enum::F32, Enum::F33, Enum::F34, Enum::F35, Enum::F36, Enum::F37, Enum::F38, Enum::F39, Enum::F40, Enum::F41, Enum::F42, Enum::F43, Enum::F44, Enum::F45, Enum::F46, Enum::F47, Enum::F48, Enum::F49, Enum::F50, Enum::F51, Enum::F52, Enum::F53, Enum::F54, Enum::F55, Enum::F56, Enum::F57, Enum::F58, Enum::F59, Enum::F60, Enum::F61, Enum::F62, Enum::F63, Enum::F64, Enum::F65, Enum::F66, Enum::F67, Enum::F68, Enum::F69, Enum::F70, Enum::F71, Enum::F72, Enum::F73, Enum::F74, Enum::F75, Enum::F76, Enum::F77, Enum::F78, Enum::F79, Enum::F80, Enum::F81, Enum::F82, Enum::F83, Enum::F84, Enum::F85, Enum::F86, Enum::F87, Enum::F88, Enum::F89, Enum::F90, Enum::F91, Enum::F92, Enum::F93, Enum::F94, Enum::F95, Enum::F96, Enum::F97, Enum::F98, Enum::F99, Enum::F100, Enum::F101, Enum::F102, Enum::F103, Enum::F104, Enum::F105, Enum::F106, Enum::F107, Enum::F108, Enum::F109, Enum::F110, Enum::F111, Enum::F112, Enum::F113, Enum::F114, Enum::F115, Enum::F116, Enum::F117, Enum::F118, Enum::F119, Enum::F120, Enum::F121, Enum::F122, Enum::F123, Enum::F124, Enum::F125, Enum::F126, Enum::F127, Enum::F128, Enum::F129, Enum::F130, Enum::F131, Enum::F132, Enum::F133, Enum::F134, Enum::F135, Enum::F136, Enum::F137, Enum::F138, Enum::F139, Enum::F140, Enum::F141, Enum::F142, Enum::F143, Enum::F144, Enum::F145, Enum::F146, Enum::F147, Enum::F148, Enum::F149, Enum::F150, Enum::F151, Enum::F152, Enum::F153, Enum::F154, Enum::F155, Enum::F156, Enum::F157, Enum::F158, Enum::F159, Enum::F160, Enum::F161, Enum::F162, Enum::F163, Enum::F164, Enum::F165, Enum::F166, Enum::F167, Enum::F168, Enum::F169, Enum::F170, Enum::F171, Enum::F172, Enum::F173, Enum::F174, Enum::F175, Enum::F176, Enum::F177, Enum::F178, Enum::F179, Enum::F180, Enum::F181, Enum::F182, Enum::F183, Enum::F184, Enum::F185, Enum::F186, Enum::F187, Enum::F188, Enum::F189, Enum::F190, Enum::F191, Enum::F192, Enum::F193, Enum::F194, Enum::F195, Enum::F196, Enum::F197, Enum::F198, Enum::F199, Enum::F200, Enum::F201, Enum::F202, Enum::F203, Enum::F204, Enum::F205, Enum::F206, Enum::F207, Enum::F208, Enum::F209, Enum::F210, Enum::F211, Enum::F212, Enum::F213, Enum::F214, Enum::F215, Enum::F216, Enum::F217, Enum::F218, Enum::F219, Enum::F220, Enum::F221, Enum::F222, Enum::F223, Enum::F224, Enum::F225, Enum::F226, Enum::F227, Enum::F228, Enum::F229, Enum::F230, Enum::F231, Enum::F232, Enum::F233, Enum::F234, Enum::F235, Enum::F236, Enum::F237, Enum::F238, Enum::F239, Enum::F240, Enum::F241, Enum::F242, Enum::F243, Enum::F244, Enum::F245, Enum::UNDO, Enum::HOLE, Enum::ENTER, Enum::SH_REGS, Enum::SH_MEM, Enum::SH_STAT, Enum::BREAK, Enum::CONS, Enum::CAPS, Enum::NUM, Enum::HOLD, Enum::SCROLLFORW, Enum::SCROLLBACK, Enum::BOOT, Enum::CAPSON, Enum::COMPOSE, Enum::SAK, Enum::DECRCONSOLE, Enum::INCRCONSOLE, Enum::SPAWNCONSOLE, Enum::BARENUMLOCK, Enum::KP0, Enum::KP1, Enum::KP2, Enum::KP3, Enum::KP4, Enum::KP5, Enum::KP6, Enum::KP7, Enum::KP8, Enum::KP9, Enum:: KP9, Enum::KPPLUS, Enum::KPMINUS, Enum::KPSTAR, Enum::KPSLASH, Enum::KPENTER, Enum::KPCOMMA, Enum::KPDOT, Enum::KPPLUSMINUS, Enum::KPPARENL, Enum::KPPARENR, Enum::DGRAVE, Enum::DACUTE, Enum::DCIRCM, Enum::DTILDE, Enum::DDIERE, Enum::DCEDIL, Enum::DMACRON, Enum::DBREVE, Enum::DABDOT, Enum::DABRING, Enum::DDBACUTE, Enum::DCARON, Enum::DOGONEK, Enum::DIOTA, Enum::DVOICED, Enum::DSEMVOICED, Enum::DBEDOT, Enum::DHOOK, Enum::DHORN, Enum::DSTROKE, Enum::DABCOMMA, Enum::DABREVCOMMA, Enum::DDBGRAVE, Enum::DINVBREVE, Enum::DBECOMMA, Enum::DCURRENCY, Enum::DGREEK, Enum::ARROW_DOWN, Enum::ARROW_LEFT, Enum::ARROW_RIGHT, Enum::ARROW_UP, Enum::SHIFT, Enum::CTRL, Enum::ALT, Enum::ALTGR, Enum::SHIFTL, Enum::SHIFTR, Enum::CTRLL, Enum::CTRLR, Enum::CAPSSHIFT, Enum::ASC0, Enum::ASC1, Enum::ASC2, Enum::ASC3, Enum::ASC4, Enum::ASC5, Enum::ASC6, Enum::ASC7, Enum::ASC8, Enum::ASC9, Enum::HEX0, Enum::HEX1, Enum::HEX2, Enum::HEX3, Enum::HEX4, Enum::HEX5, Enum::HEX6, Enum::HEX7, Enum::HEX8, Enum::HEX9, Enum::HEXa, Enum::HEXb, Enum::HEXc, Enum::HEXd, Enum::HEXe, Enum::HEXf, Enum::SHIFTLOCK, Enum::CTRLLOCK, Enum::ALTLOCK, Enum::ALTGRLOCK, Enum::SHIFTLLOCK, Enum::SHIFTRLOCK, Enum::CTRLLLOCK, Enum::CTRLRLOCK, Enum::CAPSSHIFTLOCK, Enum::SHIFT_SLOCK, Enum::CTRL_SLOCK, Enum::ALT_SLOCK, Enum::ALTGR_SLOCK, Enum::SHIFTL_SLOCK, Enum::SHIFTR_SLOCK, Enum::CTRLL_SLOCK, Enum::CTRLR_SLOCK, Enum::CAPSSHIFT_SLOCK, Enum::BRL_BLANK, Enum::BRL_DOT1, Enum::BRL_DOT2, Enum::BRL_DOT3, Enum::BRL_DOT4, Enum::BRL_DOT5, Enum::BRL_DOT6, Enum::BRL_DOT7, Enum::BRL_DOT8, Enum::BRL_DOT9, Enum::BRL_DOT10, Enum::CONSOLE1, Enum::CONSOLE2, Enum::CONSOLE3, Enum::CONSOLE4, Enum::CONSOLE5, Enum::CONSOLE6, Enum::CONSOLE7, Enum::CONSOLE8, Enum::CONSOLE9, Enum::CONSOLE10, Enum::CONSOLE11, Enum::CONSOLE12, Enum::CONSOLE13, Enum::CONSOLE14, Enum::CONSOLE15, Enum::CONSOLE16, Enum::CONSOLE17, Enum::CONSOLE18, Enum::CONSOLE19, Enum::CONSOLE20, Enum::CONSOLE21, Enum::CONSOLE22, Enum::CONSOLE23, Enum::CONSOLE24, Enum::CONSOLE25, Enum::CONSOLE26, Enum::CONSOLE27, Enum::CONSOLE28, Enum::CONSOLE29, Enum::CONSOLE30, Enum::CONSOLE31, Enum::CONSOLE32, Enum::CONSOLE33, Enum::CONSOLE34, Enum::CONSOLE35, Enum::CONSOLE36, Enum::CONSOLE37, Enum::CONSOLE38, Enum::CONSOLE39, Enum::CONSOLE40, Enum::CONSOLE41, Enum::CONSOLE42, Enum::CONSOLE43, Enum::CONSOLE44, Enum::CONSOLE45, Enum::CONSOLE46, Enum::CONSOLE47, Enum::CONSOLE48, Enum::CONSOLE49, Enum::CONSOLE50, Enum::CONSOLE51, Enum::CONSOLE52, Enum::CONSOLE53, Enum::CONSOLE54, Enum::CONSOLE55, Enum::CONSOLE56, Enum::CONSOLE57, Enum::CONSOLE58, Enum::CONSOLE59, Enum::CONSOLE60, Enum::CONSOLE61, Enum::CONSOLE62, Enum::CONSOLE63, Enum:: CONSOLE63, Enum::CANCEL, Enum::CLEAR, Enum::KANA, Enum::HANGUL, Enum::IME_ON, Enum::JUNJA, Enum::FINAL, Enum::HANJA, Enum::KANJI, Enum::IME_OFF, Enum::CONVERT, Enum::NONCONVERT, Enum::ACCEPT, Enum::MODECHANGE, Enum::PRINT, Enum::EXECUTE, Enum::SNAPSHOT, Enum::SUPERL, Enum::SUPERR, Enum::APPS, Enum::SLEEP, Enum::AALT, Enum::BROWSER_BACK, Enum::BROWSER_FORWARD, Enum::BROWSER_REFRESH, Enum::BROWSER_STOP, Enum::BROWSER_SEARCH, Enum::BROWSER_FAVORITES, Enum::BROWSER_HOME, Enum::VOLUME_MUTE, Enum::VOLUME_DOWN, Enum::VOLUME_UP, Enum::NEXT_TRACK, Enum::PREV_TRACK, Enum::STOP, Enum::PLAY_PAUSE, Enum::LAUNCH_MAIL, Enum::LAUNCH_MEDIA_SELECT, Enum::LAUNCH_APP1, Enum::LAUNCH_APP2, Enum::MISC, Enum::PROCCESS, Enum::ATTN, Enum::CRSEL, Enum::EXSEL, Enum::EREOF, Enum::PLAY, Enum::ZOOM, Enum::PA1, Enum::CLEAR2, Enum::KPEQUAL, Enum::DICTIONARY, Enum::UNREGISTER, Enum::REGISTERE, Enum::OYAYUBIL, Enum::OYAYUBIR, Enum::AX, Enum::HELP_ICO, Enum::00_ICO, Enum::CLEAR_ICO, Enum::RESET, Enum::JUMP, Enum::PA1_2, Enum::PA2, Enum::PA3, Enum::WSCTRL, Enum::CUSEL, Enum::ATTN2, Enum::FINISH, Enum::COPY, Enum::AUTO, Enum::ENLW, Enum::BACKTAB, Enum::GAMEPAD_A, Enum::GAMEPAD_B, Enum::GAMEPAD_X, Enum::GAMEPAD_Y, Enum::GAMEPAD_RIGHT_SHOULDER, Enum::GAMEPAD_LEFT_SHOULDER, Enum::GAMEPAD_LEFT_TRIGGER, Enum::GAMEPAD_RIGHT_TRIGGER, Enum::GAMEPAD_DPAD_UP, Enum::GAMEPAD_DPAD_DOWN, Enum::GAMEPAD_DPAD_LEFT, Enum::GAMEPAD_DPAD_RIGHT, Enum::GAMEPAD_MENU, Enum::GAMEPAD_VIEW, Enum::GAMEPAD_LEFT_THUMBSTICK_BUTTON, Enum::GAMEPAD_RIGHT_THUMBSTICK_BUTTON, Enum::GAMEPAD_LEFT_THUMBSTICK_UP, Enum::GAMEPAD_LEFT_THUMBSTICK_DOWN, Enum::GAMEPAD_LEFT_THUMBSTICK_RIGHT, Enum::GAMEPAD_LEFT_THUMBSTICK_LEFT, Enum::GAMEPAD_RIGHT_THUMBSTICK_UP, Enum::GAMEPAD_RIGHT_THUMBSTICK_DOWN, Enum::GAMEPAD_RIGHT_THUMBSTICK_RIGHT, Enum::GAMEPAD_RIGHT_THUMBSTICK_LEFT>
};