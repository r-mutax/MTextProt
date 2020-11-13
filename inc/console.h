#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "mtxtprot.h"

void StartConsoleOperation(void);
void EndConsoleOperation(void);
bool GetConsoleSize(MSize* ms);
bool ChkConsoleSizeChange(MSize& bef, MSize& cur);