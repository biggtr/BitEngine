#pragma once
#include "Bit/Math/Vector.h"
#include "Bit/UI/Widgets.h"
#include <vector>

enum class DRAW_COMMAND_TYPE
{
    RECT,
    CIRCLE,
    LINE,
    TEXTURE,
};
enum class LAYOUT_TYPE
{
    VERTICAL,
    HORIZONTAL
};
struct ElementID
{
    u64 ParentID;
    u64 ID; 
};

struct DrawCommand
{
    ElementID Element;
    DRAW_COMMAND_TYPE Type;
    const char* Label;
    Rect Bounds;
    BMath::Vec4 Color;
};
b8 UIInitialize(u64* memoryRequirement, void* state);
void UIShutdown(void* state);

void UIBeginFrame();
void UIEndFrame();


void UIBeginLayout(LAYOUT_TYPE type, Rect bounds);
void UIEndLayout();

// create new empty window
void UIBegin(const char* label, Rect bounds);
void UIEnd();

