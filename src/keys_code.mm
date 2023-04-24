#import <GameController/GameController.h>
#import "keys_code.h"
#import "game_controller.h"


bool checkKeyPress(long key)
{
    if ([[InputController controller].keysPressed containsObject:@(key)])
        return true;

    return false;
}



// Letter Keys
const long KeysCode::A = GCKeyCodeKeyA;
const long KeysCode::B = GCKeyCodeKeyB;
const long KeysCode::C = GCKeyCodeKeyC;
const long KeysCode::D = GCKeyCodeKeyD;
const long KeysCode::E = GCKeyCodeKeyE;
const long KeysCode::F = GCKeyCodeKeyF;
const long KeysCode::G = GCKeyCodeKeyG;
const long KeysCode::H = GCKeyCodeKeyH;
const long KeysCode::I = GCKeyCodeKeyI;
const long KeysCode::J = GCKeyCodeKeyJ;
const long KeysCode::K = GCKeyCodeKeyK;
const long KeysCode::L = GCKeyCodeKeyL;
const long KeysCode::M = GCKeyCodeKeyM;
const long KeysCode::N = GCKeyCodeKeyN;
const long KeysCode::O = GCKeyCodeKeyO;
const long KeysCode::P = GCKeyCodeKeyP;
const long KeysCode::Q = GCKeyCodeKeyQ;
const long KeysCode::R = GCKeyCodeKeyR;
const long KeysCode::S = GCKeyCodeKeyS;
const long KeysCode::T = GCKeyCodeKeyT;
const long KeysCode::U = GCKeyCodeKeyU;
const long KeysCode::V = GCKeyCodeKeyV;
const long KeysCode::W = GCKeyCodeKeyW;
const long KeysCode::X = GCKeyCodeKeyX;
const long KeysCode::Y = GCKeyCodeKeyY;
const long KeysCode::Z = GCKeyCodeKeyZ;

// Number Keys
const long KeysCode::Zero = GCKeyCodeZero;
const long KeysCode::One = GCKeyCodeOne;
const long KeysCode::Two = GCKeyCodeTwo;
const long KeysCode::Three = GCKeyCodeThree;
const long KeysCode::Four = GCKeyCodeFour;
const long KeysCode::Five = GCKeyCodeFive;
const long KeysCode::Six = GCKeyCodeSix;
const long KeysCode::Seven = GCKeyCodeSeven;
const long KeysCode::Eight = GCKeyCodeEight;
const long KeysCode::Nine = GCKeyCodeNine;

// Punctuation and Command Keys
const long KeysCode::Tab = GCKeyCodeTab;
const long KeysCode::Spacebar = GCKeyCodeSpacebar;
const long KeysCode::Hyphen = GCKeyCodeHyphen;
const long KeysCode::EqualSign = GCKeyCodeEqualSign;
const long KeysCode::OpenBracket = GCKeyCodeOpenBracket;
const long KeysCode::CloseBracket = GCKeyCodeCloseBracket;
const long KeysCode::Backslash = GCKeyCodeBackslash;
const long KeysCode::NonUSPound = GCKeyCodeNonUSPound;
const long KeysCode::Semicolon = GCKeyCodeSemicolon;
const long KeysCode::Quote = GCKeyCodeQuote;
const long KeysCode::GraveAccentAndTilde = GCKeyCodeGraveAccentAndTilde;
const long KeysCode::Comma = GCKeyCodeComma;
const long KeysCode::Period = GCKeyCodePeriod;
const long KeysCode::Slash = GCKeyCodeSlash;
const long KeysCode::CapsLock = GCKeyCodeCapsLock;
const long KeysCode::DeleteOrBackspace = GCKeyCodeDeleteOrBackspace;
const long KeysCode::Escape = GCKeyCodeEscape;
const long KeysCode::ReturnOrEnter = GCKeyCodeReturnOrEnter;
const long KeysCode::NonUSBackslash = GCKeyCodeNonUSBackslash;
const long KeysCode::Application = GCKeyCodeApplication;
const long KeysCode::Power = GCKeyCodePower;

// Function Keys
const long KeysCode::F1 = GCKeyCodeF1;
const long KeysCode::F2 = GCKeyCodeF2;
const long KeysCode::F3 = GCKeyCodeF3;
const long KeysCode::F4 = GCKeyCodeF4;
const long KeysCode::F5 = GCKeyCodeF5;
const long KeysCode::F6 = GCKeyCodeF6;
const long KeysCode::F7 = GCKeyCodeF7;
const long KeysCode::F8 = GCKeyCodeF8;
const long KeysCode::F9 = GCKeyCodeF9;
const long KeysCode::F10 = GCKeyCodeF10;
const long KeysCode::F11 = GCKeyCodeF11;
const long KeysCode::F12 = GCKeyCodeF12;
const long KeysCode::F13 = GCKeyCodeF13;
const long KeysCode::F14 = GCKeyCodeF14;
const long KeysCode::F15 = GCKeyCodeF15;
const long KeysCode::F16 = GCKeyCodeF16;
const long KeysCode::F17 = GCKeyCodeF17;
const long KeysCode::F18 = GCKeyCodeF18;
const long KeysCode::F19 = GCKeyCodeF19;
const long KeysCode::F20 = GCKeyCodeF20;

// Directional and Similar Keys
const long KeysCode::PrintScreen = GCKeyCodePrintScreen;
const long KeysCode::ScrollLock = GCKeyCodeScrollLock;
const long KeysCode::Pause = GCKeyCodePause;
const long KeysCode::Insert = GCKeyCodeInsert;
const long KeysCode::Home = GCKeyCodeHome;
const long KeysCode::End = GCKeyCodeEnd;
const long KeysCode::PageUp = GCKeyCodePageUp;
const long KeysCode::PageDown = GCKeyCodePageDown;
const long KeysCode::DeleteForward = GCKeyCodeDeleteForward;
const long KeysCode::RightArrow = GCKeyCodeRightArrow;
const long KeysCode::LeftArrow = GCKeyCodeLeftArrow;
const long KeysCode::DownArrow = GCKeyCodeDownArrow;
const long KeysCode::UpArrow = GCKeyCodeUpArrow;

// Keypad Keys
const long KeysCode::KeypadNumLock = GCKeyCodeKeypadNumLock;
const long KeysCode::KeypadSlash = GCKeyCodeKeypadSlash;
const long KeysCode::KeypadAsterisk = GCKeyCodeKeypadAsterisk;
const long KeysCode::KeypadHyphen = GCKeyCodeKeypadHyphen;
const long KeysCode::KeypadPlus = GCKeyCodeKeypadPlus;
const long KeysCode::KeypadEnter = GCKeyCodeKeypadEnter;
const long KeysCode::Keypad0 = GCKeyCodeKeypad0;
const long KeysCode::Keypad1 = GCKeyCodeKeypad1;
const long KeysCode::Keypad2 = GCKeyCodeKeypad2;
const long KeysCode::Keypad3 = GCKeyCodeKeypad3;
const long KeysCode::Keypad4 = GCKeyCodeKeypad4;
const long KeysCode::Keypad5 = GCKeyCodeKeypad5;
const long KeysCode::Keypad6 = GCKeyCodeKeypad6;
const long KeysCode::Keypad7 = GCKeyCodeKeypad7;
const long KeysCode::Keypad8 = GCKeyCodeKeypad8;
const long KeysCode::Keypad9 = GCKeyCodeKeypad9;
const long KeysCode::KeypadPeriod = GCKeyCodeKeypadPeriod;
const long KeysCode::KeypadEqualSign = GCKeyCodeKeypadEqualSign;


// International Keys
const long KeysCode::International1 = GCKeyCodeInternational1;
const long KeysCode::International2 = GCKeyCodeInternational2;
const long KeysCode::International3 = GCKeyCodeInternational3;
const long KeysCode::International4 = GCKeyCodeInternational4;
const long KeysCode::International5 = GCKeyCodeInternational5;
const long KeysCode::International6 = GCKeyCodeInternational6;
const long KeysCode::International7 = GCKeyCodeInternational7;
const long KeysCode::International8 = GCKeyCodeInternational8;
const long KeysCode::International9 = GCKeyCodeInternational9;


// Language Keys
const long KeysCode::LANG1 = GCKeyCodeLANG1;
const long KeysCode::LANG2 = GCKeyCodeLANG2;
const long KeysCode::LANG3 = GCKeyCodeLANG3;
const long KeysCode::LANG4 = GCKeyCodeLANG4;
const long KeysCode::LANG5 = GCKeyCodeLANG5;
const long KeysCode::LANG6 = GCKeyCodeLANG6;
const long KeysCode::LANG7 = GCKeyCodeLANG7;
const long KeysCode::LANG8 = GCKeyCodeLANG8;
const long KeysCode::LANG9 = GCKeyCodeLANG9;

// Left and Right Side Keys
const long KeysCode::LeftControl = GCKeyCodeLeftControl;
const long KeysCode::RightControl = GCKeyCodeRightControl;
const long KeysCode::LeftShift = GCKeyCodeLeftShift;
const long KeysCode::RightShift = GCKeyCodeRightShift;
const long KeysCode::LeftAlt = GCKeyCodeLeftAlt;
const long KeysCode::RightAlt = GCKeyCodeRightAlt;
const long KeysCode::LeftGUI = GCKeyCodeLeftGUI;
const long KeysCode::RightGUI = GCKeyCodeRightGUI;
