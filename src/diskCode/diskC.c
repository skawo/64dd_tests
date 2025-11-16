#include "diskC.h"
#include "versionedCode.c"

__attribute__((section(".diskHeader")))
char Header[] = "ZELDA_DD";

__attribute__((section(".diskInfo")))
diskInfo diskInfoData = 
{
    .diskStart    = &__Disk_Start,
    .diskEnd      = &__Disk_End,         
    .vramStart    = &__Disk_VramStart,
    .vramEnd      = &__Disk_VramEnd,  
    .hookTablePtr = &hookTable,
    .unk_014      = { 0 }
};

ddHookTable hookTable = 
{
    .diskInit                   = (DiskInitFunc)&__Disk_Init_K1,
    .diskDestroy                = NULL,
    .loadRoom                   = NULL,
    .sceneInit                  = NULL,
    .playInit                   = NULL,
    .playDestroy                = NULL,
    .mapDataInit                = NULL,
    .mapDataDestroy             = NULL,
    .mapDataSetDungeons         = NULL,
    .mapExpDestroy              = NULL,
    .mapExpTextureLoadDungeons  = NULL,
    .mapMarkInit                = NULL,
    .mapMarkDestroy             = NULL,
    .pauseMapMarkInit           = NULL,
    .pauseMapMarkDestroy        = NULL,
    .kaleidoInit                = NULL,
    .kaleidoDestroy             = NULL,
    .kaleidoLoadDungeonMap      = NULL,
    .getSceneEntry              = NULL,
    .unk_4C                     = { 0 },
    .handleEntranceTriggers     = NULL,
    .setMessageTables           = NULL,
    .unk_5C                     = { 0 },
    .loadCreditsMsg             = NULL,
    .loadJapaneseMsg            = NULL,
    .loadEnglishMsg             = Disk_GetENGMessage,
#if OOT_PAL
    .loadEnglishMsg             = NULL,
    .loadGermanMsg              = NULL,
    .loadFrenchMsg              = NULL,
#endif
    .sceneDraw                  = Disk_SceneDraw,
    .asyncDma                   = NULL,
    .gameStateUpdate            = Disk_GameState,
    .cutsceneSetScript          = NULL,
};


globals64DD vars = 
{
    .funcTablePtr             = (ddFuncPointers*)0xDEADBEEF,
    .hookTablePtr             = (ddHookTable*)0xDEADBEEF,
    .spawnArwing              = false,
    .gameVersion              = -1,
    .defaultSfxPos            = (Vec3f){ 0, 0, 0 },
    .defaultFreqAndVolScale   = 1.0f,
    .defaultReverb            = 0,
}; // This must be < 0x1060 bytes

void Disk_Init(ddFuncPointers* funcTablePtr, ddHookTable* hookTablePtr)
{
    funcTablePtr->osWritebackDCacheAll();
    funcTablePtr->osInvalICache((void*)RAM_START, RAM_LENGTH);
 
    vars.funcTablePtr = funcTablePtr;
    vars.hookTablePtr = hookTablePtr;    

    // Check game version by comparing the address of the funcTablePtr.
    for (int i = NTSC_1_0; i <= NTSC_1_2; i++)
    {
        if (funcTablePtr_Table[i - 1] == (void*)funcTablePtr)
            vars.gameVersion = i;
    }

    // If no valid version detected, show error screen and hang.
    if (vars.gameVersion < NTSC_1_0)
    {
        u32* viReg = (u32*)K0_TO_K1(VI_ORIGIN_REG);
        u8* frameBuffer = (void*)K0_TO_K1(*viReg);
        u8* comprBuf = (u8*)SEGMENT_STATIC_START;
        vars.funcTablePtr->loadFromDisk(comprBuf, (u32)EZLJ_ERROR_VERSION_YAZ0, EZLJ_ERROR_VERSION_YAZ0_LEN);
        ddYaz0_Decompress(comprBuf, frameBuffer, EZLJ_ERROR_VERSION_YAZ0_LEN);
        while (true);
    }
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

    //u32* vi_reg = (u32*)K0_TO_K1(VI_ORIGIN_REG);
    vars.funcTablePtr->faultDrawText(25, 25, "Oh hello we can print to screen! %x", (u32)EZLJ_ERROR_VERSION_YAZ0);

    if (vars.spawnArwing || CHECK_BTN_ALL(input[0].press.button, BTN_L))
    {
        vars.spawnArwing = false;
        Audio_PlaySfxGeneral_Versioned(vars.gameVersion, NA_SE_SY_KINSTA_MARK_APPEAR, &vars.defaultSfxPos, 4, 
                                       &vars.defaultFreqAndVolScale, &vars.defaultFreqAndVolScale, &vars.defaultReverb);
        
        Player* player = GET_PLAYER(play);

        Actor_Spawn_Versioned(vars.gameVersion, &play->actorCtx, play, ACTOR_EN_CLEAR_TAG, player->actor.world.pos.x,
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

char* msg = "ARWING, GO!\x02";
char* msg2 = "Huehuehuehue.\x02";

s32 Disk_GetENGMessage(struct Font* font)
{
    MessageContext* msgC = (MessageContext*)((u8*)font - offsetof(MessageContext, font));

    //vars.funcTablePtr->loadFromDisk(font->msgBuf, font->msgOffset + (u32)STRINGDATA_BIN, font->msgLength);

    if (msgC->textId == 0x1002)
    {
        ddMemcpy(msg, font->msgBuf, 200);
        vars.spawnArwing = true;
    }
    else
    {
        ddMemcpy(msg2, font->msgBuf, 200);    
    }

    return 1;
}

void Disk_SetMessageTables(struct MessageTableEntry** Japanese, struct MessageTableEntry** English, struct MessageTableEntry** Credits)
{

}