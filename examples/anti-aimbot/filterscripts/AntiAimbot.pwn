#include <a_samp>
#include <aimbot_dc>

stock CollectDataOPWS(data[E_SHOT_VECTOR], playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ) {
	/* Shooter Information */
	GetPlayerPos(playerid, data[fShooterPosX], data[fShooterPosY], data[fShooterPosZ]);
	GetPlayerVelocity(playerid, data[fShooterVelocityX], data[fShooterVelocityY], data[fShooterVelocityZ]);

	GetPlayerCameraPos(playerid, data[fShooterCameraPosX], data[fShooterCameraPosY], data[fShooterCameraPosZ]);
	GetPlayerCameraFrontVector(playerid, data[fShooterCameraFVX], data[fShooterCameraFVY], data[fShooterCameraFVZ]);
    data[fShooterCameraZoom] = GetPlayerCameraZoom(playerid);

	GetPlayerFacingAngle(playerid, data[fShooterFacingAngle]);

	data[iShooterState] = GetPlayerState(playerid);
	data[iShooterSpecialAction] = GetPlayerSpecialAction(playerid);
	data[bShooterInVehicle] = bool:IsPlayerInAnyVehicle(playerid);
	data[bShooterSurfingVehicle] = (GetPlayerSurfingVehicleID(playerid) != INVALID_VEHICLE_ID);
	data[bShooterSurfingObject] = (GetPlayerSurfingObjectID(playerid) != INVALID_OBJECT_ID);
 	data[iShooterWeaponID] = weaponid;
 	data[iShooterSkinID] = GetPlayerSkin(playerid);
 	data[iShooterID] = playerid;

 	data[iShooterPing] = GetPlayerPing(playerid);
	data[fShooterPacketLoss] = NetStats_PacketLossPercent(playerid);

	data[bHit] = (hittype == BULLET_HIT_TYPE_PLAYER);
	if(hittype == BULLET_HIT_TYPE_PLAYER) {
	    /* Victim Information */
        GetPlayerPos(hitid, data[fVictimPosX], data[fVictimPosY], data[fVictimPosZ]);
		GetPlayerVelocity(hitid, data[fVictimVelocityX], data[fVictimVelocityY], data[fVictimVelocityZ]);

	    GetPlayerFacingAngle(hitid, data[fVictimFacingAngle]);

		data[iVictimState] = GetPlayerState(hitid);
		data[iVictimSpecialAction] = GetPlayerSpecialAction(hitid);
		data[bVictimInVehicle] = bool:IsPlayerInAnyVehicle(hitid);
		data[bVictimSurfingVehicle] = (GetPlayerSurfingVehicleID(hitid) != INVALID_VEHICLE_ID);
		data[bVictimSurfingObject] = (GetPlayerSurfingObjectID(hitid) != INVALID_OBJECT_ID);
	 	data[iVictimWeaponID] = GetPlayerWeapon(hitid);
	 	data[iVictimSkinID] = GetPlayerSkin(hitid);
	 	data[iVictimID] = hitid;

		data[iVictimPing] = GetPlayerPing(hitid);
		data[fVictimPacketLoss] = NetStats_PacketLossPercent(hitid);

	 	/* Shot Information */
	 	data[fOffsetX] = fX;
		data[fOffsetY] = fY;
		data[fOffsetZ] = fZ;
		GetPlayerLastShotVectors(playerid, data[fOriginX], data[fOriginY], data[fOriginZ],
										   data[fHitPosX], data[fHitPosY], data[fHitPosZ]);
	}

	/* Time Information */
	data[iSecond] = gettime();
	data[iTick] = GetTickCount();
}

native test_vector(playerid, data[E_SHOT_VECTOR], &Float:prob);
public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	new data[E_SHOT_VECTOR];
    CollectDataOPWS(data, playerid, weaponid, hittype, hitid, fX, fY, fZ);
    
	new Float:prob;
	if(test_vector(playerid, data, prob)) {
	    new str[128];
	    format(str, sizeof(str), "Aimbot probability: %.4f", prob);
	    SendClientMessageToAll(0xFF0000FF, str);
	}
    return 1;
}
