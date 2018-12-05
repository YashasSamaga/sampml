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
			"%.6f "             /* fShooterFacingAngle 		*/\
			"%i  "              /* iShooterState 			*/\
			"%i  "              /* iShooterSpecialAction 	*/\
			"%i  "              /* bShooterInVehicle		*/\
			"%i  "              /* bShooterSurfingVehicle	*/\
			"%i  "              /* bShooterSurfingObject 	*/\
			"%i  "              /* iShooterWeaponModelID	*/\
 			"%i  "              /* iShooterSkinID			*/\
			"%i  "              /* iShooterID				*/\
			"%.6f %.6f %.6f "	/* fVictimPos				*/\
			"%.6f %.6f %.6f "	/* fVictimVelocity			*/\
			"%.6f "             /* fVictimFacingAngle 		*/\
			"%i  "              /* iVictimState 			*/\
			"%i  "              /* iVictimSpecialAction 	*/\
			"%i  "              /* bVictimInVehicle			*/\
			"%i  "              /* bVictimSurfingVehicle	*/\
			"%i  "              /* bVictimSurfingObject 	*/\
			"%i  "              /* iVictimWeaponID			*/\
 			"%i  "              /* iVictimSkinID			*/\
			"%i  "              /* iVictimID				*/\
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

