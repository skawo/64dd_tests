#include "diskC.h"

__attribute__((section(".hookTable")))
ddHookTable hookTable = 
{
    .diskInit = (DiskInitFunc)&__Disk_Init_K1,
    .diskDestroy = NULL,
    .loadRoom = NULL,
    .sceneInit = NULL,
    .playInit = NULL,
    .playDestroy = NULL,
    .mapDataInit = NULL,
    .mapDataDestroy = NULL,
    .mapDataSetDungeons = NULL,
    .mapExpDestroy = NULL,
    .mapExpTextureLoad = NULL,
    .mapMarkInit = NULL,
    .mapMarkDestroy = NULL,
    .pauseMapMarkInit = NULL,
    .pauseMapMarkDestroy = NULL,
    .kaleidoInit = NULL,
    .kaleidoDestroy = NULL,
    .kaleidoLoadDungeonMap = NULL,
    .getSceneEntry = NULL,
    .unk_4C = { 0 },
    .handleEntranceTriggers = NULL,
    .setMessageTables = NULL,
    .unk_5C = { 0 },
    .loadCreditsMsg = NULL,
    .loadJapaneseMsg = NULL,
    .loadEnglishMsg = Disk_GetNESMessage,
#if OOT_PAL
    .loadEnglishMsg = NULL,
    .loadGermanMsg = NULL,
    .loadFrenchMsg = NULL,
#endif
    .sceneDraw = Disk_SceneDraw,
    .asyncDma = NULL,
    .gameStateUpdate = Disk_GameState,
    .cutsceneSetScript = NULL,
};

__attribute__((section(".variableRAM")))
variables64DD vars =
{
    .funcTablePtr = (ddFuncPointers*)0xDEADBEEF,
    .hookTablePtr = (ddHookTable*)0xDEADBEEF,
    .spawnStarwing = false,
};

__attribute__((section(".diskInfo")))
struct_801D9C30 diskInfo =
{
    .diskStart = 0,
    .diskEnd = 0xDEADBEEF,          // Filled out
    .vramStart = 0x80400000,
    .vramEnd = 0xDEADBEEF,          // by sizes.py
    .hookTablePtr = &hookTable,
    .unk_014 = {0}
};

void Disk_Init(ddFuncPointers* funcTablePtr, ddHookTable* hookTablePtr)
{
    vars.funcTablePtr = funcTablePtr;
    vars.hookTablePtr = hookTablePtr;
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
    //vars.funcTablePtr->saveContext->save.dayTime += 0x120;
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

    vars.funcTablePtr->loadFromDisk(font->msgBuf, 0x1060, 200);

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