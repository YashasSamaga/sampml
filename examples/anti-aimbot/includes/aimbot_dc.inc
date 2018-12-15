#if defined INCLUDED_AIMBOT_DC_INC
	#endinput
#endif
#define INCLUDED_AIMBOT_DC_INC

enum E_SHOT_VECTOR {
	bool:bHit, /* hit or miss w.r.t fixed player */

	/* Shooter Information */
	Float:fShooterPosX,
	Float:fShooterPosY,
	Float:fShooterPosZ,
	Float:fShooterVelocityX,
	Float:fShooterVelocityY,
	Float:fShooterVelocityZ,
	
	Float:fShooterCameraPosX,
	Float:fShooterCameraPosY,
	Float:fShooterCameraPosZ,
	Float:fShooterCameraFVX,
	Float:fShooterCameraFVY,
	Float:fShooterCameraFVZ,
	Float:fShooterCameraZoom,
	Float:fShooterCameraAspectRatio,
	iShooterCameraMode,
	
	Float:fShooterFacingAngle,
	
	iShooterState,
	iShooterSpecialAction,
	bool:bShooterInVehicle,
	bool:bShooterSurfingVehicle,
	bool:bShooterSurfingObject,
	iShooterWeaponID,
	iShooterSkinID,
	iShooterID,
	
	/* Victim Information */
	Float:fVictimPosX,
	Float:fVictimPosY,
	Float:fVictimPosZ,
	Float:fVictimVelocityX,
	Float:fVictimVelocityY,
	Float:fVictimVelocityZ,
	
	Float:fVictimFacingAngle,
	
	iVictimState,
	iVictimSpecialAction,
	bool:bVictimInVehicle,
	bool:bVictimSurfingVehicle,
	bool:bVictimSurfingObject,
	iVictimWeaponID,
	iVictimSkinID,
	iVictimID,
	
	/* Shot Information */
	iHitType,
	Float:fOffsetX, // OPWS
	Float:fOffsetY, // OPWS
	Float:fOffsetZ, // OPWS
	Float:fOriginX, // GPLSV
	Float:fOriginY, // GPLSV
	Float:fOriginZ, // GPLSV
	Float:fHitPosX, // GPLSV
	Float:fHitPosY, // GPLSV
	Float:fHitPosZ, // GPLSV
	
	/* Network Information */
	iShooterPing,
	Float:fShooterPacketLoss,
	iVictimPing,
	Float:fVictimPacketLoss,

	/* Time */
	iSecond,
	iTick
}

stock EncodeShotVector(data[E_SHOT_VECTOR], buffer[], buffer_size = sizeof(buffer)) {
    static const title[] = "VECTOR";
	#pragma unused title

	format( buffer, buffer_size,
	        "%i "               /* bHit 					*/\
			"%.6f %.6f %.6f "	/* fShooterPos				*/\
			"%.6f %.6f %.6f "	/* fShooterVelocity			*/\
			"%.6f %.6f %.6f "	/* fShooterCameraPos 		*/\
			"%.6f %.6f %.6f "	/* fShooterCameraFV			*/\
			"%.6f "             /* fShooterCameraZoom 		*/\
			"%.6f "             /* fShooterCameraAspectRatio*/\
			"%i "               /* iShooterCameraMode     	*/\
			"%.6f "             /* fShooterFacingAngle 		*/\
			"%i "              	/* iShooterState 			*/\
			"%i "              	/* iShooterSpecialAction 	*/\
			"%i "              	/* bShooterInVehicle		*/\
			"%i "              	/* bShooterSurfingVehicle	*/\
			"%i "              	/* bShooterSurfingObject 	*/\
			"%i "              	/* iShooterWeaponModelID	*/\
 			"%i "              	/* iShooterSkinID			*/\
			"%i "              	/* iShooterID				*/\
			"%.6f %.6f %.6f "	/* fVictimPos				*/\
			"%.6f %.6f %.6f "	/* fVictimVelocity			*/\
			"%.6f "             /* fVictimFacingAngle 		*/\
			"%i "              	/* iVictimState 			*/\
			"%i "              	/* iVictimSpecialAction 	*/\
			"%i "              	/* bVictimInVehicle			*/\
			"%i "              	/* bVictimSurfingVehicle	*/\
			"%i "              	/* bVictimSurfingObject 	*/\
			"%i "              	/* iVictimWeaponID			*/\
 			"%i "              	/* iVictimSkinID			*/\
			"%i "              	/* iVictimID				*/\
			"%i "              	/* iHitType                 */\
			"%.6f %.6f %.6f "	/* fOffset					*/\
			"%.6f %.6f %.6f "	/* fOrigin					*/\
			"%.6f %.6f %.6f "	/* fHitPos					*/\
			"%i "               /* iShooterPing             */\
			"%.6f "             /* fShooterPacketLoss       */\
			"%i "               /* iVictimPing             	*/\
			"%.6f "             /* fVictimPacketLoss        */\
			"%i "               /* iSecond                  */\
   			"%i ",              /* iTick                    */\
   			data[bHit],
			data[fShooterPosX], data[fShooterPosY], data[fShooterPosZ],
	        data[fShooterVelocityX], data[fShooterVelocityY], data[fShooterVelocityZ],
	        data[fShooterCameraPosX], data[fShooterCameraPosY], data[fShooterCameraPosZ],
	        data[fShooterCameraFVX], data[fShooterCameraFVY], data[fShooterCameraFVZ],
	        data[fShooterCameraZoom],
	        data[fShooterCameraAspectRatio],
	        data[iShooterCameraMode],
	        data[fShooterFacingAngle],
	        data[iShooterState],
	        data[iShooterSpecialAction],
	        data[bShooterInVehicle],
	        data[bShooterSurfingVehicle],
	        data[bShooterSurfingObject],
	        data[iShooterWeaponID],
	        data[iShooterSkinID],
	        data[iShooterID],
	        data[fVictimPosX], data[fVictimPosY], data[fVictimPosZ],
	        data[fVictimVelocityX], data[fVictimVelocityY], data[fVictimVelocityZ],
	        data[fVictimFacingAngle],
	        data[iVictimState],
	        data[iVictimSpecialAction],
	        data[bVictimInVehicle],
	        data[bVictimSurfingVehicle],
	        data[bVictimSurfingObject],
	        data[iVictimWeaponID],
	        data[iVictimSkinID],
	        data[iVictimID],
	        data[iHitType],
	        data[fOffsetX], data[fOffsetY], data[fOffsetZ],
	        data[fOriginX], data[fOriginY], data[fOriginZ],
	        data[fHitPosX], data[fHitPosY], data[fHitPosZ],
			data[iShooterPing],
			data[fShooterPacketLoss],
			data[iVictimPing],
			data[fVictimPacketLoss],
			data[iSecond],
			data[iTick]);
}

stock CollectDataOPWS(data[E_SHOT_VECTOR], playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ) {
	static last_hitid[MAX_PLAYERS] = {INVALID_PLAYER_ID, ...};

	/* Shooter Information */
	GetPlayerPos(playerid, data[fShooterPosX], data[fShooterPosY], data[fShooterPosZ]);
	GetPlayerVelocity(playerid, data[fShooterVelocityX], data[fShooterVelocityY], data[fShooterVelocityZ]);

	GetPlayerCameraPos(playerid, data[fShooterCameraPosX], data[fShooterCameraPosY], data[fShooterCameraPosZ]);
	GetPlayerCameraFrontVector(playerid, data[fShooterCameraFVX], data[fShooterCameraFVY], data[fShooterCameraFVZ]);
	data[fShooterCameraZoom] = GetPlayerCameraZoom(playerid);
	data[fShooterCameraAspectRatio] = GetPlayerCameraAspectRatio(playerid);
	data[iShooterCameraMode] = GetPlayerCameraMode(playerid);

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

	/* Shot Information */
	data[iHitType] = hittype;
	data[fOffsetX] = fX;
	data[fOffsetY] = fY;
	data[fOffsetZ] = fZ;

	GetPlayerLastShotVectors(playerid,	data[fOriginX], data[fOriginY], data[fOriginZ],
										data[fHitPosX], data[fHitPosY], data[fHitPosZ]);

	/*
	** if the player misses a shot, we will send the information about the previous
	** victim for one more shot just in case it's of use
	*/
	new bool:hit_a_player = (hittype == BULLET_HIT_TYPE_PLAYER);
	data[bHit] = hit_a_player;

	new victimid = hit_a_player ? hitid : last_hitid[playerid];
	if(hit_a_player)
 		last_hitid[playerid] = hitid;

	if(IsPlayerConnected(victimid)) {
		/* Victim Information */
		GetPlayerPos(victimid, data[fVictimPosX], data[fVictimPosY], data[fVictimPosZ]);
		GetPlayerVelocity(victimid, data[fVictimVelocityX], data[fVictimVelocityY], data[fVictimVelocityZ]);

		GetPlayerFacingAngle(victimid, data[fVictimFacingAngle]);

		data[iVictimState] = GetPlayerState(victimid);
		data[iVictimSpecialAction] = GetPlayerSpecialAction(victimid);
		data[bVictimInVehicle] = bool:IsPlayerInAnyVehicle(victimid);
		data[bVictimSurfingVehicle] = (GetPlayerSurfingVehicleID(victimid) != INVALID_VEHICLE_ID);
		data[bVictimSurfingObject] = (GetPlayerSurfingObjectID(victimid) != INVALID_OBJECT_ID);
	 	data[iVictimWeaponID] = GetPlayerWeapon(victimid);
	 	data[iVictimSkinID] = GetPlayerSkin(victimid);
	 	data[iVictimID] = victimid;

	 	data[iVictimPing] = GetPlayerPing(victimid);
		data[fVictimPacketLoss] = NetStats_PacketLossPercent(victimid);
	} else last_hitid[playerid] = INVALID_PLAYER_ID;

	/* Time Information */
	data[iSecond] = gettime();
	data[iTick] = GetTickCount();
}
