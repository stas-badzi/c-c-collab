#pragma once

#include <linux/keyboard.h>
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

class Key {
public:
    enum class Enum : unsigned short {
        NONE = 0xffff, /* only in KeyPressed when no key is pressed */

        // KT_LATIN is always 0 so it's just the values
        // most of these values are locale-dependent
        // generally they 0-255 represent '\x00'-'\xFF' and the key is what it prints in the terminal
        // So ex. ToEnum('ą') works in latin-2, but fails with most other charsets because it doesn't exist in them and in UTF-8 it doesn't fit in 1 byte has to be ("ą")
        // the ones here are from the latin-1 charset (whose values 0-127 are the same in ASCII and most other charsets)
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
        CTRL_UNDERSCORE = 31, /* Ctrl + _ */
        SPACE = 32, /* ⎵ */
        ESCLAM = 33, /* ! */
        DBLQUOTE = 34, /* " */
        NUMBERSIGN = 35, /* # */
        DOLLAR = 36, /* $ */
        PERCENT = 37, /* % */
        AMPERSAND = 38, /* & */
        APOSTROPHE = 39, /* ' */
        PARENLEFT = 40, /* ( */
        PARENRIGHT = 41, /* ) */
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
        UNDERSCORE = 95, /* _ */
        GRAVE = 96, /* `  */
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
        // generally from now on the values will differ in most charsets, the one's before only differ one's like cyrillic, greek, etc.
        EMPTY_1, // in ISO-8859-* (latin-*) charsets chars 128-159 are invisable control chars, so they don't represent any key
        EMPTY_2, // we include them here to so that they don't get tured into Key::Enum::HOLE when using Key::ToEnum('') with them
        EMPTY_3,
        EMPTY_4,
        EMPTY_5,
        EMPTY_6,
        EMPTY_7,
        EMPTY_8,
        EMPTY_9,
        EMPTY_10,
        EMPTY_11,
        EMPTY_12,
        EMPTY_13,
        EMPTY_14,
        EMPTY_15,
        EMPTY_16,
        EMPTY_17,
        EMPTY_18,
        EMPTY_19,
        EMPTY_20,
        EMPTY_21,
        EMPTY_22,
        EMPTY_23,
        EMPTY_24,
        EMPTY_25,
        EMPTY_26,
        EMPTY_27,
        EMPTY_28,
        EMPTY_29,
        EMPTY_30,
        EMPTY_31,
        EMPTY_32,
        NOBREAKSPACE = 160, /* ⍽ */
        EXCLAMDOWN = 161, /* ¡ */
        CENT = 162, /* ¢ */
        STERLING = 163, /* £ */
        CURRENCY = 164, /* ¤ */
        YEN = 165, /* ¥ */
        BROKENBAR = 166, /* ¦ */
        SECTION = 167, /* § */
        DIAERESIS = 168, /* ¨ */
        COPYRIGHT = 169, /* © */
        ORDFEMININE = 170, /* ª */
        GUILLEMOTLEFT = 171, /* « */
        NOTSIGN = 172, /* ¬ */
        HYPHEN = 173, /* - */
        REGISTERED = 174, /* ® */
        MACRON = 175, /* ¯ */
        DEGREE = 176, /* ° */
        PLUSMINUS = 177, /* ± */
        TWOSUPERIOR = 178, /* ² */
        THREESUPERIOR = 179, /* ³ */
        ACUTE = 180, /* ´ */
        MU = 181, /* µ */
        PARAGRAPH = 182, /* ¶ */
        PERIODCENTERED = 183, /* · */
        CEDILLA = 184, /* ç */
        ONESUPERIOR = 185, /* ¹ */
        MASCULINE = 186, /* ♂ */
        GUILLEMOTRIGHT = 187, /* » */
        ONEQUARTER = 188, /* ¼ */
        ONEHALF = 189, /* ½ */
        THREEQUARTERS = 190, /* ¾ */
        QUESTIONDOWN = 191, /* ¿ */
        A_GRAVE = 192, /* À */
        A_ACUTE = 193, /* Á */
        A_CIRCUMFLEX = 194, /* Â */
        A_TILDE = 195, /* Ã */
        A_DIAERESIS = 196, /* Ä */
        A_RING = 197, /* Å */
        AE = 198, /* Æ */
        C_CEDILLA = 199, /* Ç */
        E_GRAVE = 200, /* È */
        E_ACUTE = 201, /* É */
        E_CIRCUMFLEX = 202, /* Ê */
        E_DIAERESIS = 203, /* Ë */
        I_GRAVE = 204, /* Ì */
        I_ACUTE = 205, /* Í */
        I_CIRCUMFLEX = 206, /* Î */
        I_DIAERESIS = 207, /* Ï */
        ETH = 208, /* Ð */
        N_TILDE = 209, /* Ñ */
        O_GRAVE = 210, /* Ò */
        O_ACUTE = 211, /* Ó */
        O_CIRCUMFLEX = 212, /* Ô */
        O_TILDE = 213, /* Õ */
        O_DIAERESIS = 214, /* Ö */
        MULTIPLY = 215, /* × */
        O_SLASH = 216, /* Ø */
        U_GRAVE = 217, /* Ù */
        U_ACUTE = 218, /* Ú */
        U_CIRCUMFLEX = 219, /* Û */
        U_DIAERESIS = 220, /* Ü */
        Y_ACUTE = 221, /* Ý */
        THORN = 222, /* Þ */
        SHARP_S = 223, /* ß */
        a_grave = 224, /* à */
        a_acute = 225, /* á */
        a_circumflex = 226, /* â */
        a_tilde = 227, /* ã */
        a_diaeresis = 228, /* ä */
        a_ring = 229, /* å */
        ae = 230, /* æ */
        c_cedilla = 231, /* ç */
        e_grave = 232, /* è */
        e_acute = 233, /* é */
        e_circumflex = 234, /* ê */
        e_diaeresis = 235, /* ë */
        i_grave = 236, /* ì */
        i_acute = 237, /* í */
        i_circumflex = 238, /* î */
        i_diaeresis = 239, /* ï */
        eth = 240, /* ð */
        n_tilde = 241, /* ñ */
        o_grave = 242, /* ò */
        o_acute = 243, /* ó */
        o_circumflex = 244, /* ô */
        o_tilde = 245, /* õ */
        o_diaeresis = 246, /* ö */
        DIVISION = 247, /* ÷ */
        o_slash = 248, /* ø */
        u_grave = 249, /* ù */
        u_acute = 250, /* ú */
        u_circumflex = 251, /* û */
        u_diaeresis = 252, /* ü */
        y_acute = 253, /* ý */
        thorn = 254, /* þ */
        y_diaeresis = 255, /* ÿ */

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
        END = K_SELECT, /* Select [End] */
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
        KP0 = K_P0, /* numpad 0 */
        KP1 = K_P1, /* numpad 1 */
        KP2 = K_P2, /* numpad 2 */
        KP3 = K_P3, /* numpad 3 */
        KP4 = K_P4, /* numpad 4 */
        KP5 = K_P5, /* numpad 5 */
        KP6 = K_P6, /* numpad 6 */
        KP7 = K_P7, /* numpad 7 */
        KP8 = K_P8, /* numpad 8 */
        KP9 = K_P9, /* numpad 9 */
        KPPLUS = K_PPLUS, /* numpad + */
        KPMINUS = K_PMINUS, /* numpad - */
        KPSTAR = K_PSTAR, /* numpad * */
        KPSLASH = K_PSLASH, /* numpad / */
        KPENTER = K_PENTER, /* numpad Enter */
        KPCOMMA = K_PCOMMA, /* numpad , */
        KPDOT = K_PDOT, /* numpad . */
        KPPLUSMINUS = K_PPLUSMINUS, /* numpad ± */
        KPPARENL = K_PPARENL, /* numpad ( */
        KPPARENR = K_PPARENR, /* numpad ) */

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
        CTRL_SLOCK = K_CTRL_SLOCK, /* Sticky Control */
        ALT_SLOCK = K_ALT_SLOCK, /* Sticky Alt */
        ALTGR_SLOCK = K_ALTGR_SLOCK, /* Sticky AltGr */
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

        CONSOLE1 = K(KT_CONS,0), /* Console_1 */
        CONSOLE2 = K(KT_CONS,1), /* Console_2 */
        CONSOLE3 = K(KT_CONS,2), /* Console_3 */
        CONSOLE4 = K(KT_CONS,3), /* Console_4 */
        CONSOLE5 = K(KT_CONS,4), /* Console_5 */
        CONSOLE6 = K(KT_CONS,5), /* Console_6 */
        CONSOLE7 = K(KT_CONS,6), /* Console_7 */
        CONSOLE8 = K(KT_CONS,7), /* Console_8 */
        CONSOLE9 = K(KT_CONS,8), /* Console_9 */
        CONSOLE10 = K(KT_CONS,9), /* Console_10 */
        CONSOLE11 = K(KT_CONS,10), /* Console_11 */
        CONSOLE12 = K(KT_CONS,11), /* Console_12 */
        CONSOLE13 = K(KT_CONS,12), /* Console_13 */
        CONSOLE14 = K(KT_CONS,13), /* Console_14 */
        CONSOLE15 = K(KT_CONS,14), /* Console_15 */
        CONSOLE16 = K(KT_CONS,15), /* Console_16 */
        CONSOLE17 = K(KT_CONS,16), /* Console_17 */
        CONSOLE18 = K(KT_CONS,17), /* Console_18 */
        CONSOLE19 = K(KT_CONS,18), /* Console_19 */
        CONSOLE20 = K(KT_CONS,19), /* Console_20 */
        CONSOLE21 = K(KT_CONS,20), /* Console_21 */
        CONSOLE22 = K(KT_CONS,21), /* Console_22 */
        CONSOLE23 = K(KT_CONS,22), /* Console_23 */
        CONSOLE24 = K(KT_CONS,23), /* Console_24 */
        CONSOLE25 = K(KT_CONS,24), /* Console_25 */
        CONSOLE26 = K(KT_CONS,25), /* Console_26 */
        CONSOLE27 = K(KT_CONS,26), /* Console_27 */
        CONSOLE28 = K(KT_CONS,27), /* Console_28 */
        CONSOLE29 = K(KT_CONS,28), /* Console_29 */
        CONSOLE30 = K(KT_CONS,29), /* Console_30 */
        CONSOLE31 = K(KT_CONS,30), /* Console_31 */
        CONSOLE32 = K(KT_CONS,31), /* Console_32 */
        CONSOLE33 = K(KT_CONS,32), /* Console_33 */
        CONSOLE34 = K(KT_CONS,33), /* Console_34 */
        CONSOLE35 = K(KT_CONS,34), /* Console_35 */
        CONSOLE36 = K(KT_CONS,35), /* Console_36 */
        CONSOLE37 = K(KT_CONS,36), /* Console_37 */
        CONSOLE38 = K(KT_CONS,37), /* Console_38 */
        CONSOLE39 = K(KT_CONS,38), /* Console_39 */
        CONSOLE40 = K(KT_CONS,39), /* Console_40 */
        CONSOLE41 = K(KT_CONS,40), /* Console_41 */
        CONSOLE42 = K(KT_CONS,41), /* Console_42 */
        CONSOLE43 = K(KT_CONS,42), /* Console_43 */
        CONSOLE44 = K(KT_CONS,43), /* Console_44 */
        CONSOLE45 = K(KT_CONS,44), /* Console_45 */
        CONSOLE46 = K(KT_CONS,45), /* Console_46 */
        CONSOLE47 = K(KT_CONS,46), /* Console_47 */
        CONSOLE48 = K(KT_CONS,47), /* Console_48 */
        CONSOLE49 = K(KT_CONS,48), /* Console_49 */
        CONSOLE50 = K(KT_CONS,49), /* Console_50 */
        CONSOLE51 = K(KT_CONS,50), /* Console_51 */
        CONSOLE52 = K(KT_CONS,51), /* Console_52 */
        CONSOLE53 = K(KT_CONS,52), /* Console_53 */
        CONSOLE54 = K(KT_CONS,53), /* Console_54 */
        CONSOLE55 = K(KT_CONS,54), /* Console_55 */
        CONSOLE56 = K(KT_CONS,55), /* Console_56 */
        CONSOLE57 = K(KT_CONS,56), /* Console_57 */
        CONSOLE58 = K(KT_CONS,57), /* Console_58 */
        CONSOLE59 = K(KT_CONS,58), /* Console_59 */
        CONSOLE60 = K(KT_CONS,59), /* Console_60 */
        CONSOLE61 = K(KT_CONS,60), /* Console_61 */
        CONSOLE62 = K(KT_CONS,61), /* Console_62 */
        CONSOLE63 = K(KT_CONS,62), /* Console_63 */

        SELECT = 3595, /* Select [End] */
        CANCEL , /* Cancel */
        KPCLEAR , /* Empty Keypad Key */
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
        SUPERL , /* Left Super[Windows/Command] key */
        SUPERR , /* Right Super[Windows/Command] key */
        MENU , /* Applications Key */
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
        CLEAR , /* OEM Clear */

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
        CLEAR_ICO , /* Empty key on ICO (i think) */

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

        EISU , /* Eisu (on JIS keyboards) */
        FN , /* Function key */
        };
    using Class = EnumCheck<Enum, Enum::HOLE, Enum::NONE, Enum::NUL, Enum::CTRL_A, Enum::CTRL_B, Enum::CTRL_C, Enum::CTRL_D, Enum::CTRL_E, Enum::CTRL_F, Enum::CTRL_G, Enum::BACKSPACE, Enum::TAB, Enum::LINEFEED, Enum::CTRL_K, Enum::CTRL_L, Enum::CTRL_M, Enum::CTRL_N, Enum::CTRL_O, Enum::CTRL_P, Enum::CTRL_Q, Enum::CTRL_R, Enum::CTRL_S, Enum::CTRL_T, Enum::CTRL_U, Enum::CTRL_V, Enum::CTRL_W, Enum::CTRL_X, Enum::CTRL_Y, Enum::CTRL_Z, Enum::ESC, Enum::CTRL_BACKSLASH, Enum::CTRL_BRACKETRIGHT, Enum::CTRL_ASCIICIRCUM, Enum::CTRL_UNDERSCORE, Enum::SPACE, Enum::ESCLAM, Enum::DBLQUOTE, Enum::NUMBERSIGN, Enum::DOLLAR, Enum::PERCENT, Enum::AMPERSAND, Enum::APOSTROPHE, Enum::PARENLEFT, Enum::PARENRIGHT, Enum::ASTERISK, Enum::PLUS, Enum::COMMA, Enum::MINUS, Enum::PERIOD, Enum::SLASH, Enum::/*$0*/ZERO, Enum::/*$1*/ONE, Enum::/*$2*/TWO, Enum::/*$3*/THREE, Enum::/*$4*/FOUR, Enum::/*$5*/FIVE, Enum::/*$6*/SIX, Enum::/*$7*/SEVEN, Enum::/*$8*/EIGHT, Enum::/*$9*/NINE, Enum::COLON, Enum::SEMICOLON, Enum::LESS, Enum::EQUAL, Enum::GREATER, Enum::QUESTION, Enum::AT, Enum::A, Enum::B, Enum::C, Enum::D, Enum::E, Enum::F, Enum::G, Enum::H, Enum::I, Enum::J, Enum::K, Enum::L, Enum::M, Enum::N, Enum::O, Enum::P, Enum::Q, Enum::R, Enum::S, Enum::T, Enum::U, Enum::V, Enum::W, Enum::X, Enum::Y, Enum::Z, Enum::BRACKETLEFT, Enum::BACKSLASH, Enum::BRACKETRIGHT, Enum::ASCIICIRCUM, Enum::UNDERSCORE, Enum::GRAVE, Enum::a, Enum::b, Enum::c, Enum::d, Enum::e, Enum::f, Enum::g, Enum::h, Enum::i, Enum::j, Enum::k, Enum::l, Enum::m, Enum::n, Enum::o, Enum::p, Enum::q, Enum::r, Enum::s, Enum::t, Enum::u, Enum::v, Enum::w, Enum::x, Enum::y, Enum::z, Enum::BRACELEFT, Enum::BAR, Enum::BRACERIGHT, Enum::ASCIITILDE, Enum::DELETE, Enum::EMPTY_1, Enum::EMPTY_2, Enum::EMPTY_3, Enum::EMPTY_4, Enum::EMPTY_5, Enum::EMPTY_6, Enum::EMPTY_7, Enum::EMPTY_8, Enum::EMPTY_9, Enum::EMPTY_10, Enum::EMPTY_11, Enum::EMPTY_12, Enum::EMPTY_13, Enum::EMPTY_14, Enum::EMPTY_15, Enum::EMPTY_16, Enum::EMPTY_17, Enum::EMPTY_18, Enum::EMPTY_19, Enum::EMPTY_20, Enum::EMPTY_21, Enum::EMPTY_22, Enum::EMPTY_23, Enum::EMPTY_24, Enum::EMPTY_25, Enum::EMPTY_26, Enum::EMPTY_27, Enum::EMPTY_28, Enum::EMPTY_29, Enum::EMPTY_30, Enum::EMPTY_31, Enum::EMPTY_32, Enum::NOBREAKSPACE, Enum::EXCLAMDOWN, Enum::CENT, Enum::STERLING, Enum::CURRENCY, Enum::YEN, Enum::BROKENBAR, Enum::SECTION, Enum::DIAERESIS, Enum::COPYRIGHT, Enum::ORDFEMININE, Enum::GUILLEMOTLEFT, Enum::NOTSIGN, Enum::HYPHEN, Enum::REGISTERED, Enum::MACRON, Enum::DEGREE, Enum::PLUSMINUS, Enum::TWOSUPERIOR, Enum::THREESUPERIOR, Enum::ACUTE, Enum::MU, Enum::PARAGRAPH, Enum::PERIODCENTERED, Enum::CEDILLA, Enum::ONESUPERIOR, Enum::MASCULINE, Enum::GUILLEMOTRIGHT, Enum::ONEQUARTER, Enum::ONEHALF, Enum::THREEQUARTERS, Enum::QUESTIONDOWN, Enum::A_GRAVE, Enum::A_ACUTE, Enum::A_CIRCUMFLEX, Enum::A_TILDE, Enum::A_DIAERESIS, Enum::A_RING, Enum::AE, Enum::C_CEDILLA, Enum::E_GRAVE, Enum::E_ACUTE, Enum::E_CIRCUMFLEX, Enum::E_DIAERESIS, Enum::I_GRAVE, Enum::I_ACUTE, Enum::I_CIRCUMFLEX, Enum::I_DIAERESIS, Enum::ETH, Enum::N_TILDE, Enum::O_GRAVE, Enum::O_ACUTE, Enum::O_CIRCUMFLEX, Enum::O_TILDE, Enum::O_DIAERESIS, Enum::MULTIPLY, Enum::O_SLASH, Enum::U_GRAVE, Enum::U_ACUTE, Enum::U_CIRCUMFLEX, Enum::U_DIAERESIS, Enum::Y_ACUTE, Enum::THORN, Enum::SHARP_S, Enum::a_grave, Enum::a_acute, Enum::a_circumflex, Enum::a_tilde, Enum::a_diaeresis, Enum::a_ring, Enum::ae, Enum::c_cedilla, Enum::e_grave, Enum::e_acute, Enum::e_circumflex, Enum::e_diaeresis, Enum::i_grave, Enum::i_acute, Enum::i_circumflex, Enum::i_diaeresis, Enum::eth, Enum::n_tilde, Enum::o_grave, Enum::o_acute, Enum::o_circumflex, Enum::o_tilde, Enum::o_diaeresis, Enum::DIVISION, Enum::o_slash, Enum::u_grave, Enum::u_acute, Enum::u_circumflex, Enum::u_diaeresis, Enum::y_acute, Enum::thorn, Enum::y_diaeresis, Enum::F1, Enum::F2, Enum::F3, Enum::F4, Enum::F5, Enum::F6, Enum::F7, Enum::F8, Enum::F9, Enum::F10, Enum::F11, Enum::F12, Enum::F13, Enum::F14, Enum::F15, Enum::F16, Enum::F17, Enum::F18, Enum::F19, Enum::F20, Enum::FIND, Enum::INSERT, Enum::REMOVE, Enum::END, Enum::SELECT, Enum::PGUP, Enum::PGDN, Enum::MACRO, Enum::HELP, Enum::DO, Enum::PAUSE, Enum::F21, Enum::F22, Enum::F23, Enum::F24, Enum::F25, Enum::F26, Enum::F27, Enum::F28, Enum::F29, Enum::F30, Enum::F31, Enum::F32, Enum::F33, Enum::F34, Enum::F35, Enum::F36, Enum::F37, Enum::F38, Enum::F39, Enum::F40, Enum::F41, Enum::F42, Enum::F43, Enum::F44, Enum::F45, Enum::F46, Enum::F47, Enum::F48, Enum::F49, Enum::F50, Enum::F51, Enum::F52, Enum::F53, Enum::F54, Enum::F55, Enum::F56, Enum::F57, Enum::F58, Enum::F59, Enum::F60, Enum::F61, Enum::F62, Enum::F63, Enum::F64, Enum::F65, Enum::F66, Enum::F67, Enum::F68, Enum::F69, Enum::F70, Enum::F71, Enum::F72, Enum::F73, Enum::F74, Enum::F75, Enum::F76, Enum::F77, Enum::F78, Enum::F79, Enum::F80, Enum::F81, Enum::F82, Enum::F83, Enum::F84, Enum::F85, Enum::F86, Enum::F87, Enum::F88, Enum::F89, Enum::F90, Enum::F91, Enum::F92, Enum::F93, Enum::F94, Enum::F95, Enum::F96, Enum::F97, Enum::F98, Enum::F99, Enum::F100, Enum::F101, Enum::F102, Enum::F103, Enum::F104, Enum::F105, Enum::F106, Enum::F107, Enum::F108, Enum::F109, Enum::F110, Enum::F111, Enum::F112, Enum::F113, Enum::F114, Enum::F115, Enum::F116, Enum::F117, Enum::F118, Enum::F119, Enum::F120, Enum::F121, Enum::F122, Enum::F123, Enum::F124, Enum::F125, Enum::F126, Enum::F127, Enum::F128, Enum::F129, Enum::F130, Enum::F131, Enum::F132, Enum::F133, Enum::F134, Enum::F135, Enum::F136, Enum::F137, Enum::F138, Enum::F139, Enum::F140, Enum::F141, Enum::F142, Enum::F143, Enum::F144, Enum::F145, Enum::F146, Enum::F147, Enum::F148, Enum::F149, Enum::F150, Enum::F151, Enum::F152, Enum::F153, Enum::F154, Enum::F155, Enum::F156, Enum::F157, Enum::F158, Enum::F159, Enum::F160, Enum::F161, Enum::F162, Enum::F163, Enum::F164, Enum::F165, Enum::F166, Enum::F167, Enum::F168, Enum::F169, Enum::F170, Enum::F171, Enum::F172, Enum::F173, Enum::F174, Enum::F175, Enum::F176, Enum::F177, Enum::F178, Enum::F179, Enum::F180, Enum::F181, Enum::F182, Enum::F183, Enum::F184, Enum::F185, Enum::F186, Enum::F187, Enum::F188, Enum::F189, Enum::F190, Enum::F191, Enum::F192, Enum::F193, Enum::F194, Enum::F195, Enum::F196, Enum::F197, Enum::F198, Enum::F199, Enum::F200, Enum::F201, Enum::F202, Enum::F203, Enum::F204, Enum::F205, Enum::F206, Enum::F207, Enum::F208, Enum::F209, Enum::F210, Enum::F211, Enum::F212, Enum::F213, Enum::F214, Enum::F215, Enum::F216, Enum::F217, Enum::F218, Enum::F219, Enum::F220, Enum::F221, Enum::F222, Enum::F223, Enum::F224, Enum::F225, Enum::F226, Enum::F227, Enum::F228, Enum::F229, Enum::F230, Enum::F231, Enum::F232, Enum::F233, Enum::F234, Enum::F235, Enum::F236, Enum::F237, Enum::F238, Enum::F239, Enum::F240, Enum::F241, Enum::F242, Enum::F243, Enum::F244, Enum::F245, Enum::UNDO, Enum::HOLE, Enum::ENTER, Enum::SH_REGS, Enum::SH_MEM, Enum::SH_STAT, Enum::BREAK, Enum::CONS, Enum::CAPS, Enum::NUM, Enum::HOLD, Enum::SCROLLFORW, Enum::SCROLLBACK, Enum::BOOT, Enum::CAPSON, Enum::COMPOSE, Enum::SAK, Enum::DECRCONSOLE, Enum::INCRCONSOLE, Enum::SPAWNCONSOLE, Enum::BARENUMLOCK, Enum::KP0, Enum::KP1, Enum::KP2, Enum::KP3, Enum::KP4, Enum::KP5, Enum::KP6, Enum::KP7, Enum::KP8, Enum::KP9, Enum::KPPLUS, Enum::KPMINUS, Enum::KPSTAR, Enum::KPSLASH, Enum::KPENTER, Enum::KPCOMMA, Enum::KPDOT, Enum::KPPLUSMINUS, Enum::KPPARENL, Enum::KPPARENR, Enum::DGRAVE, Enum::DACUTE, Enum::DCIRCM, Enum::DTILDE, Enum::DDIERE, Enum::DCEDIL, Enum::DMACRON, Enum::DBREVE, Enum::DABDOT, Enum::DABRING, Enum::DDBACUTE, Enum::DCARON, Enum::DOGONEK, Enum::DIOTA, Enum::DVOICED, Enum::DSEMVOICED, Enum::DBEDOT, Enum::DHOOK, Enum::DHORN, Enum::DSTROKE, Enum::DABCOMMA, Enum::DABREVCOMMA, Enum::DDBGRAVE, Enum::DINVBREVE, Enum::DBECOMMA, Enum::DCURRENCY, Enum::DGREEK, Enum::ARROW_DOWN, Enum::ARROW_LEFT, Enum::ARROW_RIGHT, Enum::ARROW_UP, Enum::SHIFT, Enum::CTRL, Enum::ALT, Enum::ALTGR, Enum::SHIFTL, Enum::SHIFTR, Enum::CTRLL, Enum::CTRLR, Enum::CAPSSHIFT, Enum::ASC0, Enum::ASC1, Enum::ASC2, Enum::ASC3, Enum::ASC4, Enum::ASC5, Enum::ASC6, Enum::ASC7, Enum::ASC8, Enum::ASC9, Enum::HEX0, Enum::HEX1, Enum::HEX2, Enum::HEX3, Enum::HEX4, Enum::HEX5, Enum::HEX6, Enum::HEX7, Enum::HEX8, Enum::HEX9, Enum::HEXa, Enum::HEXb, Enum::HEXc, Enum::HEXd, Enum::HEXe, Enum::HEXf, Enum::SHIFTLOCK, Enum::CTRLLOCK, Enum::ALTLOCK, Enum::ALTGRLOCK, Enum::SHIFTLLOCK, Enum::SHIFTRLOCK, Enum::CTRLLLOCK, Enum::CTRLRLOCK, Enum::CAPSSHIFTLOCK, Enum::SHIFT_SLOCK, Enum::CTRL_SLOCK, Enum::ALT_SLOCK, Enum::ALTGR_SLOCK, Enum::SHIFTL_SLOCK, Enum::SHIFTR_SLOCK, Enum::CTRLL_SLOCK, Enum::CTRLR_SLOCK, Enum::CAPSSHIFT_SLOCK, Enum::BRL_BLANK, Enum::BRL_DOT1, Enum::BRL_DOT2, Enum::BRL_DOT3, Enum::BRL_DOT4, Enum::BRL_DOT5, Enum::BRL_DOT6, Enum::BRL_DOT7, Enum::BRL_DOT8, Enum::BRL_DOT9, Enum::BRL_DOT10, Enum::CONSOLE1, Enum::CONSOLE2, Enum::CONSOLE3, Enum::CONSOLE4, Enum::CONSOLE5, Enum::CONSOLE6, Enum::CONSOLE7, Enum::CONSOLE8, Enum::CONSOLE9, Enum::CONSOLE10, Enum::CONSOLE11, Enum::CONSOLE12, Enum::CONSOLE13, Enum::CONSOLE14, Enum::CONSOLE15, Enum::CONSOLE16, Enum::CONSOLE17, Enum::CONSOLE18, Enum::CONSOLE19, Enum::CONSOLE20, Enum::CONSOLE21, Enum::CONSOLE22, Enum::CONSOLE23, Enum::CONSOLE24, Enum::CONSOLE25, Enum::CONSOLE26, Enum::CONSOLE27, Enum::CONSOLE28, Enum::CONSOLE29, Enum::CONSOLE30, Enum::CONSOLE31, Enum::CONSOLE32, Enum::CONSOLE33, Enum::CONSOLE34, Enum::CONSOLE35, Enum::CONSOLE36, Enum::CONSOLE37, Enum::CONSOLE38, Enum::CONSOLE39, Enum::CONSOLE40, Enum::CONSOLE41, Enum::CONSOLE42, Enum::CONSOLE43, Enum::CONSOLE44, Enum::CONSOLE45, Enum::CONSOLE46, Enum::CONSOLE47, Enum::CONSOLE48, Enum::CONSOLE49, Enum::CONSOLE50, Enum::CONSOLE51, Enum::CONSOLE52, Enum::CONSOLE53, Enum::CONSOLE54, Enum::CONSOLE55, Enum::CONSOLE56, Enum::CONSOLE57, Enum::CONSOLE58, Enum::CONSOLE59, Enum::CONSOLE60, Enum::CONSOLE61, Enum::CONSOLE62, Enum::CONSOLE63, Enum::CANCEL, Enum::KPCLEAR, Enum::KANA, Enum::HANGUL, Enum::IME_ON, Enum::JUNJA, Enum::FINAL, Enum::HANJA, Enum::KANJI, Enum::IME_OFF, Enum::CONVERT, Enum::NONCONVERT, Enum::ACCEPT, Enum::MODECHANGE, Enum::PRINT, Enum::EXECUTE, Enum::SNAPSHOT, Enum::SUPERL, Enum::SUPERR, Enum::MENU, Enum::SLEEP, Enum::AALT, Enum::BROWSER_BACK, Enum::BROWSER_FORWARD, Enum::BROWSER_REFRESH, Enum::BROWSER_STOP, Enum::BROWSER_SEARCH, Enum::BROWSER_FAVORITES, Enum::BROWSER_HOME, Enum::VOLUME_MUTE, Enum::VOLUME_DOWN, Enum::VOLUME_UP, Enum::NEXT_TRACK, Enum::PREV_TRACK, Enum::STOP, Enum::PLAY_PAUSE, Enum::LAUNCH_MAIL, Enum::LAUNCH_MEDIA_SELECT, Enum::LAUNCH_APP1, Enum::LAUNCH_APP2, Enum::MISC, Enum::PROCCESS, Enum::ATTN, Enum::CRSEL, Enum::EXSEL, Enum::EREOF, Enum::PLAY, Enum::ZOOM, Enum::PA1, Enum::CLEAR, Enum::KPEQUAL, Enum::DICTIONARY, Enum::UNREGISTER, Enum::REGISTERE, Enum::OYAYUBIL, Enum::OYAYUBIR, Enum::AX, Enum::HELP_ICO, Enum::_00_ICO, Enum::CLEAR_ICO, Enum::RESET, Enum::JUMP, Enum::PA1_2, Enum::PA2, Enum::PA3, Enum::WSCTRL, Enum::CUSEL, Enum::ATTN2, Enum::FINISH, Enum::COPY, Enum::AUTO, Enum::ENLW, Enum::BACKTAB, Enum::EISU, Enum::FN>;
};