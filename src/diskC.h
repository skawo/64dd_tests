#ifndef DISKC_H 
#define DISKC_H

#include "n64dd.h"
#include "save.h"
#include "message.h"
#include "gfx.h"
#include "game.h"
#include "play_state.h"
#include "player.h"
#include "include/controller.h"
#include "sfx.h"

typedef void (*DiskInitFunc)(n64ddStruct_800FEE70_pointers*, struct n64ddStruct_80121220*);   

typedef struct struct_801D9C30 
{
    /* 0x000 */ s32 diskStart;     
    /* 0x004 */ s32 diskEnd;       
    /* 0x008 */ uintptr_t vramStart; 
    /* 0x00C */ uintptr_t vramEnd; 
    /* 0x010 */ n64ddStruct_80121220* hookTablePtr;
    /* 0x014 */ char unk_014[0x104];
} struct_801D9C30; // size = 0x118

typedef struct variables64DD
{
    n64ddStruct_800FEE70_pointers* funcTablePtr;
    n64ddStruct_80121220* hookTablePtr;
    bool spawnStarwing;

} variables64DD;

void Disk_Init(n64ddStruct_800FEE70_pointers* funcTablePtr, struct n64ddStruct_80121220* hookTablePtr);
void Disk_SceneDraw(struct PlayState* play, SceneDrawConfigFunc* func);
void Disk_GameState(struct GameState* state);
s32 Disk_GetNESMessage(struct Font*);

extern void* __Disk_Init_K1;

void Audio_PlaySfxGeneral(u16 sfxId, Vec3f* pos, u8 token, f32* freqScale, f32* vol, s8* reverbAdd);
    asm("Audio_PlaySfxGeneral = 0x800C806C");

Gfx* Gfx_Open(Gfx* gfx);
    asm("Gfx_Open = 0x800A1AA0");

Gfx* Gfx_Close(Gfx* gfx, Gfx* dst);
    asm("Gfx_Close = 0x800A1AAC");
    
Actor* Actor_Spawn(ActorContext* actorCtx, PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX,
                   s16 rotY, s16 rotZ, s16 params);
    asm("Actor_Spawn = 0x80025110");                 
    
void bcopy(const void* __src, void* __dest, int __n);
    asm("bcopy = 0x80004DC0");

asm("gSfxDefaultPos = 0x80104394");
asm("gSfxDefaultFreqAndVolScale = 0x801043A0");
asm("gSfxDefaultReverb = 0x801043A8");

#endif // DISKC_H