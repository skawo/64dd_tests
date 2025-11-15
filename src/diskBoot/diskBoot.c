#include "diskBoot.h"

void ScreenSetup();

#ifndef __FILESYSTEM_START
    #define __FILESYSTEM_START 0
#endif

void Disk_Boot()
{
    u32* frameBuffer = (u32*)0x80380000;
    ScreenSetup(frameBuffer);

    u32* graphic = (u32*)((u32)EZLJ_ERROR_IPL_BIN + BLK_SIZE_ZONE0 + __FILESYSTEM_START + 0x80000400);

    for (int i = 0; i < EZLJ_ERROR_IPL_BIN_LEN / 4; i++)
        frameBuffer[i] = graphic[i];


    while (true);    
}

void ScreenSetup(void* frameBuffer)
{
    #define WRITE_REG(reg, data) *((u32*)K0_TO_K1(reg)) = (u32)data
    WRITE_REG(VI_STATUS_REG, VI_CTRL_TYPE_16 | 
                             VI_CTRL_GAMMA_DITHER_ON | 
                             VI_CTRL_DIVOT_ON | 
                             VI_CTRL_ANTIALIAS_MODE_1 |
                             VI_CTRL_PIXEL_ADV_DEFAULT);
                             
    WRITE_REG(VI_ORIGIN_REG, frameBuffer);
    WRITE_REG(VI_WIDTH_REG, WIDTH(SCREEN_WIDTH));
    WRITE_REG(VI_V_INTR_REG, VINTR(2));
    WRITE_REG(VI_V_CURRENT_LINE_REG, VCURRENT(0));
    WRITE_REG(VI_TIMING_REG, BURST(57, 34, 5, 62));
    WRITE_REG(VI_V_SYNC_REG, VSYNC(525));
    WRITE_REG(VI_H_SYNC_REG, HSYNC(3093, 0));
    WRITE_REG(VI_H_SYNC_LEAP_REG, LEAP(3093, 3093));
    WRITE_REG(VI_H_VIDEO_REG, HSTART(108, 748));
    WRITE_REG(VI_V_VIDEO_REG, VSTART(37, 511));
    WRITE_REG(VI_V_BURST_REG, BURST(4, 2, 14, 0));
    WRITE_REG(VI_X_SCALE_REG, SCALE(2, 0));
    WRITE_REG(VI_Y_SCALE_REG, SCALE(1, 0)); 
}