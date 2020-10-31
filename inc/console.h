#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

typedef struct _MSize{
    int iWidth;
    int iHeight;
} MSize;

void StartConsoleOperation(void);
void EndConsoleOperation(void);
bool GetConsoleSize(MSize* ms);