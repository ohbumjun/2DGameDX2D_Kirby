#pragma once

// define ��
// Normal
const std::string g_NormalBearName = "NormalBear";
const std::string g_PurpleBeatlesName = "PurpleBeatles";
const std::string g_YellowBirdName = "YellowBird";
const std::string g_MushRoomName = "MushRoom";
const std::string g_Squid = "Squid";
// Ability
const std::string g_BeamMonsterName = "Beam";
const std::string g_FireMonsterName    = "Fire";
const std::string g_FightMonsterName = "Fight";
// Boss
const std::string g_BossTree = "BossTree";
// Middle Boss
const std::string g_MiddleHammerName = "MiddleHammerGorilla";
// Effect
const std::string g_SceneChangeStarToGreen3 = "SceneChangeToGreen3";
const std::string g_SceneChangeToGreen1 = "SceneChangeToGreen1";
const std::string g_SceneChangeToGreen2 = "SceneChangeToGreen2";
const std::string g_SceneChangeToGreen4 = "SceneChangeToGreen4";
const std::string g_SceneChangeToGreen5 = "SceneChangeToGreen5";
const std::string g_SceneChangeToFloat2_1 = "SceneChangeToFloat2_1";
const std::string g_SceneChangeToFloat2_2 = "SceneChangeToFloat2_2";
const std::string g_SceneChangeToFloat3 = "SceneChangeToFloat3";
const std::string g_SceneChangeToFloat4 = "SceneChangeToFloat4";
const std::string g_WaterFloatIsland1 = "WaterFloatIsland1";
const std::string g_WaterFloatIsland2_2 = "WaterFloatIsland2_2";
// Item
const std::string g_HPYellowItem = "HPYellowItem";
const std::string g_HPRedItem    = "HPRedItem";
const std::string g_HPGreenItem = "HPGreenItem";
const std::string g_Block             = "BlockItem";
const std::string g_Ladder           = "LadderItem";
const std::string g_Awl                = "AwlItem";

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
