#pragma once
#include "EricBitMarco.h"

#define BYTE_PER_SECTOR                 (512)
#define SECTOR_TO_BYTE(SEC)             (SHIFT_MUL((SEC), 512))