#ifndef _MyStringLibH_
#define _MyStringLibH_
//-------------------------------------------------------------------------------------------------
//
// Includes
//
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include <ctype.h>
#include <math.h>

//-------------------------------------------------------------------------------------------------
//
// Function Prototypes
//
//-------------------------------------------------------------------------------------------------
unsigned long HexStringToUInt(char *Hex);
int IsHexNumString(char *Hex);

#endif
