#pragma once

bool CheckIfStringIsInt(const std::string& String)
{
    for (int i = 0; i < String.length(); i++)
        if (!isdigit(String[i]))
            return false;
    return true;
}
