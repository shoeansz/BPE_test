#include <windows.h>
#include <stdio.h>
#include <string.h>

#pragma once

#define SAFE_DELETE_ARRAY(x) if((x)!=NULL){delete [](x);(x)=NULL;}