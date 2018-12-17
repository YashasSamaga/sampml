#include <a_samp>
#include <aimbot_dc>

native submit_vector(playerid, data[E_SHOT_VECTOR]);
public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	new data[E_SHOT_VECTOR];
    CollectDataOPWS(data, playerid, weaponid, hittype, hitid, fX, fY, fZ);
	submit_vector(playerid, data);
 	return 1;
}

forward OnPlayerSuspectedForAimbot(playerid, Float:probablities[2]);
public OnPlayerSuspectedForAimbot(playerid, Float:probablities[2])
{
    static const colors[] = { 0x0000FFFF, 0xFF0000FF,0x00FF00FF };

	new str[144];
 	format(str, sizeof(str), "Aimbot [%d] >> SVM: %.3f, DNN: %.3f", playerid, probablities[0], probablities[1]);
	SendClientMessageToAll(colors[playerid%sizeof(colors)], str);
}
