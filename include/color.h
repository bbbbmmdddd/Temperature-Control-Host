#ifndef color_h
#define color_h
#include <TFT_eSPI.h>

#define black   0x0000  // 黑色，RGB(0,0,0)
#define white   0xFFFF  // 白色，RGB(255,255,255)
#define red     0xF800  // 红色，RGB(255,0,0)
#define green   0x07E0  // 绿色，RGB(0,255,0)
#define blue    0x001F  // 蓝色，RGB(0,0,255)
#define cyan    0x07FF  // 青色，RGB(0,255,255)
#define magenta 0xF81F  // 品红色，RGB(255,0,255)
#define yellow  0xFFE0  // 黄色，RGB(255,255,0)
#define orange  0xFC00  // 橙色，RGB(255,165,0)
#define gray    0x8410  // 灰色，RGB(128,128,128)
#define purple  0x780F  // 紫色，RGB(128,0,128)
#define brown   0xA145  // 棕色，RGB(165,42,42)
#define pink    0xFC1F  // 粉红色，RGB(255,182,193)
#define lime    0x07E0  // 酸橙色，RGB(0,255,0)，与绿色相同
#define teal    0x0410  // 蓝绿色，RGB(0,128,128)
#define navy    0x000F  // 海军蓝，RGB(0,0,128)
#define coral   0xFBEA  // 珊瑚色，RGB(255,127,80)
#define khaki   0xEF7D  // 卡其色，RGB(240,230,140)
#define azure   0x07FF  // 天蓝色，RGB(0,255,255)，与青色相同
#define maroon  0x8000  // 栗色，RGB(128,0,0)
#define turquoise 0x07DF  // 绿松石色，RGB(0,255,127)
#define violet  0x780F  // 紫罗兰色，RGB(128,0,128)，与紫色相同
#define indigo  0x4B0F  // 靛蓝色，RGB(75,0,130)
#define gold    0xFEA0  // 金色，RGB(255,215,0)

/*
TL_DATUM：Top Left,顶部左对齐
TC_DATUM：Top Center,顶部中对齐
TR_DATUM：Top Right,顶部右对齐
ML_DATUM：Middle Left,中间左对齐
MC_DATUM：Middle Center,中间中对齐
MR_DATUM：Middle Right,中间右对齐
BL_DATUM：Bottom Left,底部左对齐
BC_DATUM：Bottom Center,底部中对齐
BR_DATUM：Bottom Right,底部右对齐
CC_DATUM:Center Center,中心对中心
*/

#endif