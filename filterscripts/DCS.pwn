#include <a_samp>
#include <file>

#include <aimbot_dc>

#pragma dynamic 1000000

#define MAX_SHOT_GAP_MS 1000

static File:dataFile[MAX_PLAYERS];

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	static last_shot_tick[MAX_PLAYERS];
	static total_shots[MAX_PLAYERS], total_hits[MAX_PLAYERS];
	static vectors[MAX_PLAYERS][E_DATA_VECTOR];
	
	new old_tick = last_shot_tick[playerid];
	last_shot_tick[playerid] = GetTickCount();
	if(GetTickCount() - old_tick < MAX_SHOT_GAP_MS) {
	    total_shots[playerid]++;
		if(hittype == BULLET_HIT_TYPE_PLAYER) {
			new index = total_hits[playerid];

			GetPlayerPos(playerid, vectors[playerid][fShooterPosX][index], vectors[playerid][fShooterPosY][index], vectors[playerid][fShooterPosZ][index]);
			GetPlayerVelocity(playerid, vectors[playerid][fShooterVelX][index], vectors[playerid][fShooterVelY][index], vectors[playerid][fShooterVelZ][index]);
			GetPlayerCameraPos(playerid, vectors[playerid][fShooterCameraPosX][index], vectors[playerid][fShooterCameraPosY][index], vectors[playerid][fShooterCameraPosZ][index]);
			GetPlayerCameraFrontVector(playerid, vectors[playerid][fShooterCameraFVX][index], vectors[playerid][fShooterCameraFVY][index], vectors[playerid][fShooterCameraFVZ][index]);
			GetPlayerFacingAngle(playerid, vectors[playerid][fShooterFacingAngle][index]);
			vectors[playerid][iPlayerSpecialAction][index] = GetPlayerSpecialAction(playerid);
            vectors[playerid][iWeaponID][index] = weaponid;
            
			GetPlayerPos(hitid, vectors[playerid][fVictimPosX][index], vectors[playerid][fVictimPosY][index], vectors[playerid][fVictimPosZ][index]);
			GetPlayerVelocity(hitid, vectors[playerid][fVictimVelX][index], vectors[playerid][fVictimVelY][index], vectors[playerid][fVictimVelZ][index]);
            GetPlayerFacingAngle(hitid, vectors[playerid][fShooterFacingAngle][index]);

			vectors[playerid][fOffsetX][index] = fX;
			vectors[playerid][fOffsetY][index] = fY;
			vectors[playerid][fOffsetZ][index] = fZ;
			GetPlayerLastShotVectors(playerid, 	vectors[playerid][fOriginX][index], vectors[playerid][fOriginY][index], vectors[playerid][fOriginZ][index],
												vectors[playerid][fHitPosX][index], vectors[playerid][fHitPosY][index], vectors[playerid][fHitPosZ][index]);

			vectors[playerid][iPing][index] = GetPlayerPing(playerid);
			vectors[playerid][fPacketLoss][index] = NetStats_PacketLossPercent(playerid);

	        total_hits[playerid]++;
         	
	        if(total_hits[playerid] == SHOTS_PER_VECTOR) {
	            vectors[playerid][fRatio] = total_hits[playerid]/float(total_shots[playerid]);
	            total_shots[playerid] = total_hits[playerid] = 0;

	            new line [4096];
	            EncodeDataVector(vectors[playerid], line, sizeof(line));
	            fwrite(dataFile[playerid], line);
				fwrite(dataFile[playerid], "\r\n");

				/*static vectors_count;
				vectors_count++;
				format(line, sizeof(line), "Data Vector [%d] saved.", vectors_count);
				SendClientMessage(playerid, 0xFF0000FF, line);*/
	        }
		}
	} else {
		total_shots[playerid] = total_hits[playerid] = 0;
	}
    return 1;
}

public OnPlayerConnect(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	
	new path[256];
	format(path, sizeof(path), "data_collection/%s.dat", name);
	dataFile[playerid] = fopen(path, io_append);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	fclose(dataFile[playerid]);
	return 1;
}
