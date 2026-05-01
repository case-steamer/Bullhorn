#include "FileSysOp.h"

bool FileSysOp::isValid(const std::string& input)
{
    bool result         = false;
    bool fExists        = false;
    bool suffixValid    = false;
    if (fs::exists(fs::path(input))) {fExists = true;}
    if (fExists)
    {for (const std::string& suffix : supportedTypes)
        {if (fs::path(input).extension() == suffix)
            {
                suffixValid = true;
                break;
            }
        }
    }
    if (fExists && suffixValid) 
    {
        result = true;
        return result;
    }
    return result;
}
