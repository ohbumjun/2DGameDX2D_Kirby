#pragma once

enum class Tile_EditMode {
	Type,
	Frame
};


static std::string GetRandomString()
{
	// ���� ���ڿ� �߻��� 
    const char* RandomAlphaNum =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    std::string Str;
    for (unsigned int i = 0; i < 3; ++i)
    {
        Str += RandomAlphaNum[rand() % (sizeof(RandomAlphaNum) - 1)];
    }

    return Str;
};
