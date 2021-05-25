#include "Player.h"
using namespace std;

string  Raw_GetPlayerName(Player* player)
{
    return player->getNameTag();
}

FloatVec4 Raw_GetPlayerPos(Player* player)
{
    auto pos = player->getPos();
    return {pos.x,pos.y,pos.z,WPlayer(*player).getDimID()};
}

string  Raw_GetXuid(Player* player)
{
    return offPlayer::getXUIDString(player);
}

string  Raw_GetRealName(Player* player)
{
    return offPlayer::getRealName(player);
}

string  Raw_GetIP(Player* player)
{
    return liteloader::getIP(*offPlayer::getNetworkIdentifier(player));
}

bool  Raw_RuncmdAs(Player *player, const string &cmd)
{
    return liteloader::runcmdAs(player,cmd);
}

bool Raw_TeleportPlayer(Player* player, const FloatVec4 &pos)
{
    WPlayer(*player).teleport({pos.x,pos.y,pos.z},pos.dim);
    return true;
}

bool Raw_KillPlayer(Player* player)
{
    SymCall("?kill@Mob@@UEAAXXZ", void, void*)((Actor*)player);
    return true;
}

int  Raw_GetPlayerPermLevel(Player* player)
{
    return (int)WPlayer(*player).getPermLvl();
}

bool  Raw_SetPlayerPermLevel(Player* player, int permLevel)
{
    ((ServerPlayer*)player)->setPermissions((CommandPermissionLevel)permLevel);
    return true;
}

bool Raw_KickPlayer(Player* player, const string &msg)
{
    extern Minecraft *mc;
    auto nh = mc->getServerNetworkHandler();
    NetworkIdentifier* a = offPlayer::getNetworkIdentifier(player);
    nh->disconnectClient(*(NetworkIdentifier*)a, msg, 0);
    return true;
}

bool  Raw_Tell(Player* player, const string &text, TextType type)
{
    WPlayer(*player).sendText(text,type);
    return true;
}

ItemStack* Raw_GetHand(Player* player)
{
    return (ItemStack*)&(player->getSelectedItem());
}

vector<ItemStack*> Raw_GetPack(Player* player)
{
    vector<ItemStack*> res;

    auto v9 = *((uintptr_t*)player + 380);
	auto v13 = (*(__int64(__fastcall**)(uintptr_t))(**(uintptr_t**)(v9 + 176) + 112i64))(*(uintptr_t*)(v9 + 176));
	for (int i = 0; i < v13; ++i)
	{
		auto v15 = (*(__int64(__fastcall**)(uintptr_t, uintptr_t))(**(uintptr_t**)(v9 + 176) + 40i64))(
			*(uintptr_t*)(v9 + 176),
			(unsigned int)i);
		res.push_back((ItemStack*)v15);
	}
    return res;
}

bool Raw_RenamePlayer(Player* player, const string &name)
{
    player->setNameTag(name);
    return true;
}

vector<Player*> Raw_GetOnlinePlayers()
{
    return liteloader::getAllPlayers();
}

bool Raw_IsPlayerValid(Player *player)
{
    auto playerList = Raw_GetOnlinePlayers();
    for(auto p : playerList)
        if(p == player)
            return true;
    return false;
}