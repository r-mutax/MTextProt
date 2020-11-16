#include <any>
#pragma once
#include "mtxtprot.h"


// method
MESSAGE_ID MMGetMessage(MESSAGE& msg);
void MMSendMessage(MESSAGE& msg);
void MMPrepareMessageLoop();