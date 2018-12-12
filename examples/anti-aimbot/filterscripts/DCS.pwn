#include <a_samp>
#include <file>

#include <aimbot_dc>

static File:dataFile[MAX_PLAYERS];

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	new data[E_SHOT_VECTOR];
	CollectDataOPWS(data, playerid, weaponid, hittype, hitid, fX, fY, fZ);

	new buffer[1024];
	EncodeShotVector(data, buffer);

	fwrite(dataFile[playerid], buffer);
	fwrite(dataFile[playerid], "\r\n");
	return 1;
}

public OnPlayerConnect(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	new path[256];
	format(path, sizeof(path), "data_collection/%s_%d.dat", name, gettime());
	dataFile[playerid] = fopen(path);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	fclose(dataFile[playerid]);
	return 1;
}
