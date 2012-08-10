#ifndef LOG_H
#define LOG_H


//#define __DBG           // 注释后不编译DBG中的代码
//#define __LOG_CONSOLE   // 注释后不输出日志到控制台
//#define __LOG_FILE      // 注释后不输出日志到文件

#ifdef __DBG
#define DBG(x) x
#else
#define DBG(x)
#endif

#include <QDebug>

extern void MyMsgHandler(QtMsgType, const char *);

#endif // LOG_H
