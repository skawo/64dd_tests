#include "diskC.h"

void Audio_PlaySfxGeneral_Versioned(u8 gameVer, u16 sfxId, Vec3f* pos, u8 token, f32* freqScale, f32* vol, s8* reverbAdd)
{
    typedef void Audio_PlaySfxGeneralFunc(u16 sfxId, Vec3f* pos, u8 token, f32* freqScale, f32* vol, s8* reverbAdd);
    Audio_PlaySfxGeneralFunc* func;

    switch (gameVer)
    {
        case NTSC_1_0: func = (Audio_PlaySfxGeneralFunc*)0x800C806C; break;
        case NTSC_1_1: func = (Audio_PlaySfxGeneralFunc*)0x800C823C; break;
        case NTSC_1_2: func = (Audio_PlaySfxGeneralFunc*)0x800C88BC; break;
        default: break;
    }

    func(sfxId, pos, token, freqScale, vol, reverbAdd);
}

void Actor_Spawn_Versioned(u8 gameVer, ActorContext* actorCtx, PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params)
{
    typedef void Actor_SpawnFunc(ActorContext* actorCtx, PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params);
    Actor_SpawnFunc* func;

    switch (gameVer)
    {
        case NTSC_1_0:
        case NTSC_1_1: func = (Actor_SpawnFunc*)0x80025110; break;
        case NTSC_1_2: func = (Actor_SpawnFunc*)0x80025750; break;
        default: break;
    }

    func(actorCtx, play, actorId, posX, posY, posZ, rotX, rotY, rotZ, params);
}

void bcopy_Versioned(u8 gameVer, const void* __src, void* __dest, int __n)
{
    typedef void bcopyFunc(const void* __src, void* __dest, int __n);
    bcopyFunc* func;

    switch (gameVer)
    {
        case NTSC_1_0:
        case NTSC_1_1: func = (bcopyFunc*)0x80004DC0; break;
        case NTSC_1_2: func = (bcopyFunc*)0x80004FD0; break;
        default: break;
    }

    func(__src, __dest, __n);   
}