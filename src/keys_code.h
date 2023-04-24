#ifndef KEYS_CODE_H
#define KEYS_CODE_H

struct KeysCode 
{
    KeysCode() = delete;

    // Letter Keys
    static const long A;
    static const long B;
    static const long C;
    static const long D;
    static const long E;
    static const long F;
    static const long G;
    static const long H;
    static const long I;
    static const long J;
    static const long K;
    static const long L;
    static const long M;
    static const long N;
    static const long O;
    static const long P;
    static const long Q;
    static const long R;
    static const long S;
    static const long T;
    static const long U;
    static const long V;
    static const long W;
    static const long X;
    static const long Y;
    static const long Z;

    // Number Keys
    static const long Zero;
    static const long One;
    static const long Two;
    static const long Three;
    static const long Four;
    static const long Five;
    static const long Six;
    static const long Seven;
    static const long Eight;
    static const long Nine;

    // Punctuation and Command Keys
    static const long Tab;
    static const long Spacebar;
    static const long Hyphen;
    static const long EqualSign;
    static const long OpenBracket;
    static const long CloseBracket;
    static const long Backslash;
    static const long NonUSPound;
    static const long Semicolon;
    static const long Quote;
    static const long GraveAccentAndTilde;
    static const long Comma;
    static const long Period;
    static const long Slash;
    static const long CapsLock;
    static const long DeleteOrBackspace;
    static const long Escape;
    static const long ReturnOrEnter;
    static const long NonUSBackslash;
    static const long Application;
    static const long Power;

    // Function Keys
    static const long F1;
    static const long F2;
    static const long F3;
    static const long F4;
    static const long F5;
    static const long F6;
    static const long F7;
    static const long F8;
    static const long F9;
    static const long F10;
    static const long F11;
    static const long F12;
    static const long F13;
    static const long F14;
    static const long F15;
    static const long F16;
    static const long F17;
    static const long F18;
    static const long F19;
    static const long F20;

    // Directional and Similar Keys
    static const long PrintScreen;
    static const long ScrollLock;
    static const long Pause;
    static const long Insert;
    static const long Home;
    static const long End;
    static const long PageUp;
    static const long PageDown;
    static const long DeleteForward;
    static const long RightArrow;
    static const long LeftArrow;
    static const long DownArrow;
    static const long UpArrow;

    // Keypad Keys
    static const long KeypadNumLock;
    static const long KeypadSlash;
    static const long KeypadAsterisk;
    static const long KeypadHyphen;
    static const long KeypadPlus;
    static const long KeypadEnter;
    static const long Keypad0;
    static const long Keypad1;
    static const long Keypad2;
    static const long Keypad3;
    static const long Keypad4;
    static const long Keypad5;
    static const long Keypad6;
    static const long Keypad7;
    static const long Keypad8;
    static const long Keypad9;
    static const long KeypadPeriod;
    static const long KeypadEqualSign;

    // International Keys
    static const long International1;
    static const long International2;
    static const long International3;
    static const long International4;
    static const long International5;
    static const long International6;
    static const long International7;
    static const long International8;
    static const long International9;

    // Language Keys
    static const long LANG1;
    static const long LANG2;
    static const long LANG3;
    static const long LANG4;
    static const long LANG5;
    static const long LANG6;
    static const long LANG7;
    static const long LANG8;
    static const long LANG9;

    // Left and Right Side Keys
    static const long LeftControl;
    static const long RightControl;
    static const long LeftShift;
    static const long RightShift;
    static const long LeftAlt;
    static const long RightAlt;
    static const long LeftGUI;
    static const long RightGUI;
};

bool checkKeyPress(long key);

#endif
