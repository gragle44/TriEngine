#pragma once

// From glfw3.h
#define TRI_KEY_SPACE              32
#define TRI_KEY_APOSTROPHE         39  /* ' */
#define TRI_KEY_COMMA              44  /* , */
#define TRI_KEY_MINUS              45  /* - */
#define TRI_KEY_PERIOD             46  /* . */
#define TRI_KEY_SLASH              47  /* / */
#define TRI_KEY_0                  48
#define TRI_KEY_1                  49
#define TRI_KEY_2                  50
#define TRI_KEY_3                  51
#define TRI_KEY_4                  52
#define TRI_KEY_5                  53
#define TRI_KEY_6                  54
#define TRI_KEY_7                  55
#define TRI_KEY_8                  56
#define TRI_KEY_9                  57
#define TRI_KEY_SEMICOLON          59  /* ; */
#define TRI_KEY_EQUAL              61  /* = */
#define TRI_KEY_A                  65
#define TRI_KEY_B                  66
#define TRI_KEY_C                  67
#define TRI_KEY_D                  68
#define TRI_KEY_E                  69
#define TRI_KEY_F                  70
#define TRI_KEY_G                  71
#define TRI_KEY_H                  72
#define TRI_KEY_I                  73
#define TRI_KEY_J                  74
#define TRI_KEY_K                  75
#define TRI_KEY_L                  76
#define TRI_KEY_M                  77
#define TRI_KEY_N                  78
#define TRI_KEY_O                  79
#define TRI_KEY_P                  80
#define TRI_KEY_Q                  81
#define TRI_KEY_R                  82
#define TRI_KEY_S                  83
#define TRI_KEY_T                  84
#define TRI_KEY_U                  85
#define TRI_KEY_V                  86
#define TRI_KEY_W                  87
#define TRI_KEY_X                  88
#define TRI_KEY_Y                  89
#define TRI_KEY_Z                  90
#define TRI_KEY_LEFT_BRACKET       91  /* [ */
#define TRI_KEY_BACKSLASH          92  /* \ */
#define TRI_KEY_RIGHT_BRACKET      93  /* ] */
#define TRI_KEY_GRAVE_ACCENT       96  /* ` */
#define TRI_KEY_WORLD_1            161 /* non-US #1 */
#define TRI_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define TRI_KEY_ESCAPE             256
#define TRI_KEY_ENTER              257
#define TRI_KEY_TAB                258
#define TRI_KEY_BACKSPACE          259
#define TRI_KEY_INSERT             260
#define TRI_KEY_DELETE             261
#define TRI_KEY_RIGHT              262
#define TRI_KEY_LEFT               263
#define TRI_KEY_DOWN               264
#define TRI_KEY_UP                 265
#define TRI_KEY_PAGE_UP            266
#define TRI_KEY_PAGE_DOWN          267
#define TRI_KEY_HOME               268
#define TRI_KEY_END                269
#define TRI_KEY_CAPS_LOCK          280
#define TRI_KEY_SCROLL_LOCK        281
#define TRI_KEY_NUM_LOCK           282
#define TRI_KEY_PRINT_SCREEN       283
#define TRI_KEY_PAUSE              284
#define TRI_KEY_F1                 290
#define TRI_KEY_F2                 291
#define TRI_KEY_F3                 292
#define TRI_KEY_F4                 293
#define TRI_KEY_F5                 294
#define TRI_KEY_F6                 295
#define TRI_KEY_F7                 296
#define TRI_KEY_F8                 297
#define TRI_KEY_F9                 298
#define TRI_KEY_F10                299
#define TRI_KEY_F11                300
#define TRI_KEY_F12                301
#define TRI_KEY_F13                302
#define TRI_KEY_F14                303
#define TRI_KEY_F15                304
#define TRI_KEY_F16                305
#define TRI_KEY_F17                306
#define TRI_KEY_F18                307
#define TRI_KEY_F19                308
#define TRI_KEY_F20                309
#define TRI_KEY_F21                310
#define TRI_KEY_F22                311
#define TRI_KEY_F23                312
#define TRI_KEY_F24                313
#define TRI_KEY_F25                314
#define TRI_KEY_KP_0               320
#define TRI_KEY_KP_1               321
#define TRI_KEY_KP_2               322
#define TRI_KEY_KP_3               323
#define TRI_KEY_KP_4               324
#define TRI_KEY_KP_5               325
#define TRI_KEY_KP_6               326
#define TRI_KEY_KP_7               327
#define TRI_KEY_KP_8               328
#define TRI_KEY_KP_9               329
#define TRI_KEY_KP_DECIMAL         330
#define TRI_KEY_KP_DIVIDE          331
#define TRI_KEY_KP_MULTIPLY        332
#define TRI_KEY_KP_SUBTRACT        333
#define TRI_KEY_KP_ADD             334
#define TRI_KEY_KP_ENTER           335
#define TRI_KEY_KP_EQUAL           336
#define TRI_KEY_LEFT_SHIFT         340
#define TRI_KEY_LEFT_CONTROL       341
#define TRI_KEY_LEFT_ALT           342
#define TRI_KEY_LEFT_SUPER         343
#define TRI_KEY_RIGHT_SHIFT        344
#define TRI_KEY_RIGHT_CONTROL      345
#define TRI_KEY_RIGHT_ALT          346
#define TRI_KEY_RIGHT_SUPER        347
#define TRI_KEY_MENU               348

enum class KeyCode : int16_t
{
    Space           = 32,
    Apostrophe      = 39,  // '
    Comma           = 44,  // ,
    Minus           = 45,  // -
    Period          = 46,  // .
    Slash           = 47,  // /
    
    _0               = 48,
    _1               = 49,
    _2               = 50,
    _3               = 51,
    _4               = 52,
    _5               = 53,
    _6               = 54,
    _7               = 55,
    _8               = 56,
    _9               = 57,
    
    Semicolon       = 59,  // ;
    Equal           = 61,  // =
    
    A               = 65,
    B               = 66,
    C               = 67,
    D               = 68,
    E               = 69,
    F               = 70,
    G               = 71,
    H               = 72,
    I               = 73,
    J               = 74,
    K               = 75,
    L               = 76,
    M               = 77,
    N               = 78,
    O               = 79,
    P               = 80,
    Q               = 81,
    R               = 82,
    S               = 83,
    T               = 84,
    U               = 85,
    V               = 86,
    W               = 87,
    X               = 88,
    Y               = 89,
    Z               = 90,
    
    LeftBracket     = 91,  // [
    Backslash       = 92,  // '\'
    RightBracket    = 93,  // ]
    GraveAccent     = 96,  // `
    
    World1          = 161, // non-US #1
    World2          = 162, // non-US #2

    Escape          = 256,
    Enter           = 257,
    Tab             = 258,
    Backspace       = 259,
    Insert          = 260,
    Delete          = 261,
    Right           = 262,
    Left            = 263,
    Down            = 264,
    Up              = 265,
    PageUp          = 266,
    PageDown        = 267,
    Home            = 268,
    End             = 269,

    CapsLock        = 280,
    ScrollLock      = 281,
    NumLock         = 282,
    PrintScreen     = 283,
    Pause           = 284,

    F1              = 290,
    F2              = 291,
    F3              = 292,
    F4              = 293,
    F5              = 294,
    F6              = 295,
    F7              = 296,
    F8              = 297,
    F9              = 298,
    F10             = 299,
    F11             = 300,
    F12             = 301,
    F13             = 302,
    F14             = 303,
    F15             = 304,
    F16             = 305,
    F17             = 306,
    F18             = 307,
    F19             = 308,
    F20             = 309,
    F21             = 310,
    F22             = 311,
    F23             = 312,
    F24             = 313,
    F25             = 314,

    KP_0            = 320,
    KP_1            = 321,
    KP_2            = 322,
    KP_3            = 323,
    KP_4            = 324,
    KP_5            = 325,
    KP_6            = 326,
    KP_7            = 327,
    KP_8            = 328,
    KP_9            = 329,
    KP_Decimal      = 330,
    KP_Divide       = 331,
    KP_Multiply     = 332,
    KP_Subtract     = 333,
    KP_Add          = 334,
    KP_Enter        = 335,
    KP_Equal        = 336,

    LeftShift       = 340,
    LeftControl     = 341,
    LeftAlt         = 342,
    LeftSuper       = 343,
    RightShift      = 344,
    RightControl    = 345,
    RightAlt        = 346,
    RightSuper      = 347,
    Menu            = 348
};

