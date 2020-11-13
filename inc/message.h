#include <any>
#pragma once
#include "mtxtprot.h"


// method
MESSAGE_ID GetMessage(MESSAGE& msg);
void SendMessage(MESSAGE& msg);
void PrepareMessageLoop();