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

forward OnPlayerSuspectedForAimbot(playerid, Float:probabilities[3], time[3]);
public OnPlayerSuspectedForAimbot(playerid, Float:probabilities[3], time[3])
{
    static const colors[] = { 0x0000FFFF, 0xFF0000FF,0x00FF00FF };

	new str[144], name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
 	format(str, sizeof(str), "%s(%d) >> RF: %.3f (%.3fms) SVM: %.3f (%.3fms), DNN: %.3f (%.3fms)",
	 						name, playerid,
	 						probabilities[0], time[0]/1000.0,
	 						probabilities[1], time[1]/1000.0,
	  						probabilities[2], time[2]/1000.0);
	SendClientMessageToAll(colors[playerid%sizeof(colors)], str);
}
