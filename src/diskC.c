#include "diskC.h"

__attribute__((section(".hookTable")))
n64ddStruct_80121220 hookTable = 
{
    .unk_00 = (DiskInitFunc)&__Disk_Init_K1,
    .unk_04 = NULL,
    .unk_08 = NULL,
    .unk_0C = NULL,
    .unk_10 = NULL,
    .unk_14 = NULL,
    .unk_18 = NULL,
    .unk_1C = NULL,
    .unk_20 = NULL,
    .unk_24 = NULL,
    .unk_28 = NULL,
    .unk_2C = NULL,
    .unk_30 = NULL,
    .unk_34 = NULL,
    .unk_38 = NULL,
    .unk_3C = NULL,
    .unk_40 = NULL,
    .unk_44 = NULL,
    .unk_48 = NULL,
    .unk_4C = { 0 },
    .unk_54 = NULL,
    .unk_58 = NULL,
    .unk_5C = { 0 },
    .unk_60 = NULL,
    .unk_64 = NULL,
    .unk_68 = Disk_GetNESMessage,
#if OOT_PAL
    .unk_6C_PAL = NULL,
#endif
    .unk_6C = Disk_SceneDraw,
    .unk_70 = NULL,
    .unk_74 = Disk_GameState,
    .unk_78 = NULL,
};

__attribute__((section(".variableRAM")))
variables64DD vars =
{
    .funcTablePtr = (n64ddStruct_800FEE70_pointers*)0xDEADBEEF,
    .spawnStarwing = false,
};

__attribute__((section(".diskInfo")))
struct_801D9C30 diskInfo =
{
    .diskStart = 0,
    .diskEnd = 0xDEADBEEF,          // Filled out
    .vramStart = 0x80400000,
    .vramEnd = 0xBEEFDEAD,          // by sizes.py
    .hookTablePtr = &hookTable,
    .unk_014 = {0}
};

void Disk_Init(n64ddStruct_800FEE70_pointers* funcPointers, struct n64ddStruct_80121220* funcStruct)
{
    vars.funcTablePtr = funcPointers;
}

void DrawRect(Gfx** gfxp, u8 r, u8 g, u8 b, u32 PosX, u32 PosY, u32 Sizex, u32 SizeY)
{
    Gfx* gfx = *gfxp;

    u16 color5551 = GPACK_RGBA5551(r, g, b, 1);

    gDPPipeSync(gfx++);
    gDPSetFillColor(gfx++, (color5551 << 16) | color5551);
    gDPFillRectangle(gfx++, PosX, PosY, PosX + Sizex, PosY + SizeY);

    *gfxp = gfx;
}

void Disk_GameState(struct GameState* state)
{
    //Input* input = state->input;
    //vars.funcTablePtr->unk_88->save.dayTime += 0x120;
}

void Disk_SceneDraw(struct PlayState* play, SceneDrawConfigFunc* func)
{
    #define __gfxCtx (play->state.gfxCtx)

    Input* input = play->state.input;
    func[play->sceneDrawConfig](play);  

    if (vars.spawnStarwing || CHECK_BTN_ALL(input[0].press.button, BTN_L))
    {
        vars.spawnStarwing = false;
        Audio_PlaySfxGeneral(NA_SE_SY_KINSTA_MARK_APPEAR, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
        
        Player* player = GET_PLAYER(play);

        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, player->actor.world.pos.x,
                    player->actor.world.pos.y + 50.0f, player->actor.world.pos.z, 0, 0, 0, 0);    
    }
    
    Gfx* gfxRef = OVERLAY_DISP;

    // Logo animation state
    static int posX = 100;
    static int posY = 100;
    static int velX = 2;
    static int velY = 2;

    // Logo dimensions
    #define BLOCK_SIZE 7
    #define LOGO_WIDTH (21 * BLOCK_SIZE)  // 5+3+5+5 blocks + 3 gaps
    #define LOGO_HEIGHT (5 * BLOCK_SIZE)

    // Update position
    posX += velX;
    posY += velY;

    // Bounce off edges
    if (posX <= 0 || posX + LOGO_WIDTH >= SCREEN_WIDTH) 
        velX = -velX;
    if (posY <= 0 || posY + LOGO_HEIGHT >= SCREEN_HEIGHT) 
        velY = -velY;

    int currentX = posX;

    // 6
    DrawRect(&gfxRef, 0, 255, 0, currentX, posY + BLOCK_SIZE, BLOCK_SIZE, 4*BLOCK_SIZE);                    // Left vertical
    DrawRect(&gfxRef, 0, 255, 0, currentX + BLOCK_SIZE, posY, 3*BLOCK_SIZE, BLOCK_SIZE);       // Top horizontal
    DrawRect(&gfxRef, 0, 255, 0, currentX + BLOCK_SIZE, posY + 2*BLOCK_SIZE, 3*BLOCK_SIZE, BLOCK_SIZE);  // Middle horizontal
    DrawRect(&gfxRef, 0, 255, 0, currentX + BLOCK_SIZE, posY + 4*BLOCK_SIZE, 3*BLOCK_SIZE, BLOCK_SIZE);  // Bottom horizontal
    DrawRect(&gfxRef, 0, 255, 0, currentX + 4*BLOCK_SIZE, posY + 3*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);  // Right connector

    // 4
    currentX += 5 * BLOCK_SIZE;
    DrawRect(&gfxRef, 255, 255, 0, currentX, posY, BLOCK_SIZE, 3*BLOCK_SIZE);                  // Left vertical (top 3)
    DrawRect(&gfxRef, 255, 255, 0, currentX + 3*BLOCK_SIZE, posY, BLOCK_SIZE, 5*BLOCK_SIZE);   // Right vertical (full)
    DrawRect(&gfxRef, 255, 255, 0, currentX + BLOCK_SIZE, posY + 2*BLOCK_SIZE, 2*BLOCK_SIZE, BLOCK_SIZE);  // Middle horizontal

    // D
    currentX += 5 * BLOCK_SIZE;
    DrawRect(&gfxRef, 0, 255, 255, currentX, posY, BLOCK_SIZE, 5*BLOCK_SIZE);                  // Left vertical
    DrawRect(&gfxRef, 0, 255, 255, currentX + BLOCK_SIZE, posY, 3*BLOCK_SIZE, BLOCK_SIZE);     // Top horizontal
    DrawRect(&gfxRef, 0, 255, 255, currentX + BLOCK_SIZE, posY + 4*BLOCK_SIZE, 3*BLOCK_SIZE, BLOCK_SIZE);  // Bottom horizontal
    DrawRect(&gfxRef, 0, 255, 255, currentX + 4*BLOCK_SIZE, posY + BLOCK_SIZE, BLOCK_SIZE, 3*BLOCK_SIZE);  // Right vertical

    // D
    currentX += 5 * BLOCK_SIZE;
    DrawRect(&gfxRef, 255, 0, 255, currentX, posY, BLOCK_SIZE, 5*BLOCK_SIZE);                  // Left vertical
    DrawRect(&gfxRef, 255, 0, 255, currentX + BLOCK_SIZE, posY, 3*BLOCK_SIZE, BLOCK_SIZE);     // Top horizontal
    DrawRect(&gfxRef, 255, 0, 255, currentX + BLOCK_SIZE, posY + 4*BLOCK_SIZE, 3*BLOCK_SIZE, BLOCK_SIZE);  // Bottom horizontal
    DrawRect(&gfxRef, 255, 0, 255, currentX + 4*BLOCK_SIZE, posY + BLOCK_SIZE, BLOCK_SIZE, 3*BLOCK_SIZE);  // Right vertical

    OVERLAY_DISP = gfxRef;
}

char* msg = "STARWING, GO!\x02";
char* msg2 = "Huehuehuehue.\x02";

s32 Disk_GetNESMessage(struct Font* font)
{
    MessageContext* msgC = (MessageContext*)((u8*)font - offsetof(MessageContext, font));

    if (msgC->textId == 0x1002)
    {
        bcopy(msg, font->msgBuf, 200);
        vars.spawnStarwing = true;
    }
    else
    {
        bcopy(msg2, font->msgBuf, 200);    
    }
    
    return 1;
}