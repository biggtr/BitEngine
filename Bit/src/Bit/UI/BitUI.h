#pragma once
#include "Bit/Core/Strings.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/UI/Widgets.h"
#include <vector>

#define MAX_WIDGETS_ON_SCREEN 4096

enum class BitUISizeKind
{
    SizeKind_Null,
    SizeKind_Pixels,
    SizeKind_TextContent,
    SizeKind_ParentPct,
    SizeKind_ChildrenSum,
};
struct BitUISize
{
    BitUISizeKind kind;
    f32 value;
    f32 strictness;
};
enum BitUIAxis2
{
    BitUI_Axis2_X,
    BitUI_Axis2_Y,
    BitUI_Axis2_COUNT,
};

struct BitUIKey
{
    u64 key;
};

enum BitUIWidgetFlags
{
    BitWidgetFlag_Clickable       = (1<<0),
    BitWidgetFlag_ViewScroll      = (1<<1),
    BitWidgetFlag_DrawText        = (1<<2),
    BitWidgetFlag_DrawBorder      = (1<<3),
    BitWidgetFlag_DrawBackground  = (1<<4),
    BitWidgetFlag_DrawDropShadow  = (1<<5),
    BitWidgetFlag_Clip            = (1<<6),
    BitWidgetFlag_HotAnimation    = (1<<7),
    BitWidgetFlag_ActiveAnimation = (1<<8),
};

struct BitUIWidget
{
    
    // tree links
    BitUIWidget *first;
    BitUIWidget *last;
    BitUIWidget *next;
    BitUIWidget *prev;
    BitUIWidget *parent;

    // hash links
    BitUIWidget *hash_next;
    BitUIWidget *hash_prev;

    // key+generation info
    BitUIKey key;
    u64 last_frame_touched_index;

    // per-frame info provided by builders
    BitUIWidgetFlags flags;
    BitString string;
    BitUISize semantic_size[BitUI_Axis2_COUNT];

    // computed every frame
    f32 computed_rel_position[BitUI_Axis2_COUNT];
    f32 computed_size[BitUI_Axis2_COUNT];
    f32 rect;

    // persistent data
    f32 hot_t;
    f32 active_t;
};

struct DrawCommand
{
    const char* Label;
    Rect Bounds;
    BMath::Vec4 Color;
    BitEngine::Texture* Texture;
    f32 UVs[8];
};

struct BitUIState
{
    BitUIWidget* Root;
    BitUIKey HotKey;
    BitUIKey ActiveKey;
    
    f32 MouseX, MouseY;
    b8 MouseDown;
};

b8 UIInitialize(u64* memoryRequirement, void* state, BitEngine::Renderer2D* renderer);
void UIShutdown(void* state);


BitUIWidget* BitUIMakeWidget(BitUIWidgetFlags flags, BitString string);

void BitUIBeginRow(BitString string);
void BitUIEndRow();
void BitUIBeginCol(BitString string);
void BitUIEndCol();

void BitUIPushParent(BitUIWidget* widget);
BitUIWidget* BitUIPop();
BitUIWidget* BitUITopParent();

void UIBeginFrame();
void UIEndFrame();

