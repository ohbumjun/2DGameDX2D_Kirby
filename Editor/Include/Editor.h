#pragma once

// define 문
// Normal
const std::string g_NormalBearName = "NormalBear";
const std::string g_PurpleBeatlesName = "PurpleBeatles";
const std::string g_YellowBirdName = "YellowBird";
const std::string g_MushRoomName = "MushRoom";
// Ability
const std::string g_BeamMonsterName = "Beam";
const std::string g_FireMonsterName    = "Fire";
const std::string g_FightMonsterName = "Fight";
// Effect
const std::string g_SceneChangeStart = "SceneChangeStart";
const std::string g_SceneChangeToGreen2 = "SceneChangeToGreen2";
const std::string g_SceneChangeToGreen1 = "SceneChangeToGreen1";
// Item
const std::string g_HPYellowItem = "HPYellowItem";
const std::string g_HPRedItem    = "HPRedItem";
const std::string g_HPGreenItem = "HPGreenItem";
const std::string g_Block             = "BlockItem";

enum class Tile_EditMode {
	Type,
	Frame
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
