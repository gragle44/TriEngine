#pragma once

// From glfw3.h
#define TRI_MOUSE_BUTTON_1         0
#define TRI_MOUSE_BUTTON_2         1
#define TRI_MOUSE_BUTTON_3         2
#define TRI_MOUSE_BUTTON_4         3
#define TRI_MOUSE_BUTTON_5         4
#define TRI_MOUSE_BUTTON_6         5
#define TRI_MOUSE_BUTTON_7         6
#define TRI_MOUSE_BUTTON_8         7
#define TRI_MOUSE_BUTTON_LAST      TRI_MOUSE_BUTTON_8
#define TRI_MOUSE_BUTTON_LEFT      TRI_MOUSE_BUTTON_1
#define TRI_MOUSE_BUTTON_RIGHT     TRI_MOUSE_BUTTON_2
#define TRI_MOUSE_BUTTON_MIDDLE    TRI_MOUSE_BUTTON_3

enum class MouseButton : int {
    Button1 = 0,
    Button2 = 1,
    Button3 = 2,
    Button4 = 3,
    Button5 = 4,
    Button6 = 5,
    Button7 = 6,
    Button8 = 7,

    Left = 0,
    Right = 1,
    Middle = 2,
    Last = 7
};