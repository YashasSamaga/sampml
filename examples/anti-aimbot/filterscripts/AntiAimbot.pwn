#include <a_samp>
#include <aimbot_dc>

native test_vector(playerid, data[E_SHOT_VECTOR], Float:prob[], Float:arr[]);
public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	new data[E_SHOT_VECTOR];
    CollectDataOPWS(data, playerid, weaponid, hittype, hitid, fX, fY, fZ);
	
	new Float:prob[2], Float:arr[15];
	if(test_vector(playerid, data, prob, arr)) {
		static const colors[] = { 0x0000FFFF, 0xFF0000FF,0x00FF00FF };
		
	    new str[144];
	    format(str, sizeof(str), "Aimbot [%d] >> SVM: %.3f, DNN: %.3f", playerid, prob[0], prob[1]);
	    SendClientMessageToAll(colors[playerid%sizeof(colors)], str);
	}
 	return 1;
}

