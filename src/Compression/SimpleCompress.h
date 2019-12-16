#pragma once
#include "DefineFiles\EricType.h"

namespace EricCore {
    class SimpleCompress
    {
    public:
        SimpleCompress();
        ~SimpleCompress();

        int SimpleCompress::compress(eu32_p src, eu32_p desc, eu32 maxLen);
        int SimpleCompress::find_duplicate_value(eu32_p src, eu32 maxLen);
        int SimpleCompress::decompress(eu32_p src, eu32_p desc, eu32 maxLen);
    };
}