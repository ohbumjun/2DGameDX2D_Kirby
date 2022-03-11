#pragma once

// define 문
const std::string g_NormalBearName = "NormalBear";
const std::string g_PurpleBeatlesName = "PurpleBeatles";
const std::string g_YellowBirdName = "YellowBird";
const std::string g_MushRoomName = "MushRoom";
const std::string g_BeanMonsterName = "Beam";
const std::string g_FightMonsterName = "FightMonster";

enum class Tile_EditMode {
	Type,
	Frame
};

enum class Line_DrawType {
	RightBottom,
	RightUp,
	LeftBottom,
	LeftUp
};

static std::string GetRandomString()
{
	// 랜덤 문자열 발생기 
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
