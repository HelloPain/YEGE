﻿#ifndef Inc_ege_env_h_
#define Inc_ege_env_h_

#include "ege/def.h"

namespace ege
{

enum initmode_flag
{
	INIT_NOBORDER       = 0x1,
	INIT_CHILD          = 0x2,
	INIT_TOPMOST        = 0x4,
	INIT_RENDERMANUAL   = 0x8,
	INIT_NOFORCEEXIT    = 0x10,
	INIT_WITHLOGO       = 0x100,
	INIT_DEFAULT    = 0x0,
	INIT_ANIMATION  = INIT_DEFAULT | INIT_RENDERMANUAL | INIT_NOFORCEEXIT,
};


// 绘图环境相关函数

EGEAPI void
initgraph(size_t width, size_t height, int flag = INIT_DEFAULT); // 初始化图形环境

EGEAPI void
closegraph(); // 关闭图形环境

EGEAPI bool
is_run();   // 判断UI是否退出

} // namespace ege;

#endif

