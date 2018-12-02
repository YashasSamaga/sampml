#if defined INCLUDED_AIMBOT_DC_INC
	#endinput
#endif
#define INCLUDED_AIMBOT_DC_INC

enum E_FEATURE_VECTOR {
	Float:fShooterPosX,
	Float:fShooterPosY,
	Float:fShooterPosZ,
	Float:fShooterVelX,
	Float:fShooterVelY,
	Float:fShooterVelZ,
	Float:fShooterFacingAngle,
	Float:fVictimPosX,
	Float:fVictimPosY,
	Float:fVictimPosZ,
	Float:fVictimVelX,
	Float:fVictimVelY,
	Float:fVictimVelZ,
	Float:fOffsetX,
	Float:fOffsetY,
	Float:fOffsetZ,
	Float:fOriginX,
	Float:fOriginY,
	Float:fOriginZ,
	Float:fHitPosX,
	Float:fHitPosY,
	Float:fHitPosZ
};

stock EncodeFeatureVector(const data[E_FEATURE_VECTOR], buffer[], buffer_size = sizeof(buffer)) {
	static const title[] = "VECTOR";
	#pragma unused title

	format( buffer, buffer_size,
			"%.2f %.2f %.2f "	/* fShooterPos	*/\
			"%.2f %.2f %.2f "	/* fShooterVel	*/\
			"%.2f "             /* fShooterFacingAngle */\
			"%.2f %.2f %.2f "	/* fVictimPos	*/\
			"%.2f %.2f %.2f "	/* fVictimVel	*/\
			"%.2f %.2f %.2f "	/* fOffset		*/\
			"%.2f %.2f %.2f "	/* fOrigin		*/\
			"%.2f %.2f %.2f ",	/* fHitPos		*/\
			data[fShooterPosX], data[fShooterPosY], data[fShooterPosZ],
	        data[fShooterVelX], data[fShooterVelY], data[fShooterVelZ],
	        data[fShooterFacingAngle],
	        data[fVictimPosX], data[fVictimPosY], data[fVictimPosZ],
	        data[fVictimVelX], data[fVictimVelY], data[fVictimVelZ],
	        data[fOffsetX], data[fOffsetY], data[fOffsetZ],
	        data[fOriginX], data[fOriginY], data[fOriginZ],
	        data[fHitPosX], data[fHitPosY], data[fHitPosZ]);
}

const SHOTS_PER_VECTOR = 10;
enum E_DATA_VECTOR {
	/* Shooter Information */
	Float:fShooterPosX[SHOTS_PER_VECTOR],
	Float:fShooterPosY[SHOTS_PER_VECTOR],
	Float:fShooterPosZ[SHOTS_PER_VECTOR],
	Float:fShooterVelX[SHOTS_PER_VECTOR],
	Float:fShooterVelY[SHOTS_PER_VECTOR],
	Float:fShooterVelZ[SHOTS_PER_VECTOR],
	Float:fShooterCameraPosX[SHOTS_PER_VECTOR],
	Float:fShooterCameraPosY[SHOTS_PER_VECTOR],
	Float:fShooterCameraPosZ[SHOTS_PER_VECTOR],
	Float:fShooterCameraFVX[SHOTS_PER_VECTOR],
	Float:fShooterCameraFVY[SHOTS_PER_VECTOR],
	Float:fShooterCameraFVZ[SHOTS_PER_VECTOR],
	Float:fShooterFacingAngle[SHOTS_PER_VECTOR],
	iPlayerSpecialAction[SHOTS_PER_VECTOR],
	iWeaponID[SHOTS_PER_VECTOR],
	
	/* Victim Information */
	Float:fVictimPosX[SHOTS_PER_VECTOR],
	Float:fVictimPosY[SHOTS_PER_VECTOR],
	Float:fVictimPosZ[SHOTS_PER_VECTOR],
	Float:fVictimVelX[SHOTS_PER_VECTOR],
	Float:fVictimVelY[SHOTS_PER_VECTOR],
	Float:fVictimVelZ[SHOTS_PER_VECTOR],
	Float:fVictimFacingAngle[SHOTS_PER_VECTOR],
	
	/* Shot Information */
	Float:fOffsetX[SHOTS_PER_VECTOR], // OPWS
	Float:fOffsetY[SHOTS_PER_VECTOR], // OPWS
	Float:fOffsetZ[SHOTS_PER_VECTOR], // OPWS
	Float:fOriginX[SHOTS_PER_VECTOR], // GPLSV
	Float:fOriginY[SHOTS_PER_VECTOR], // GPLSV
	Float:fOriginZ[SHOTS_PER_VECTOR], // GPLSV
	Float:fHitPosX[SHOTS_PER_VECTOR], // GPLSV
	Float:fHitPosY[SHOTS_PER_VECTOR], // GPLSV
	Float:fHitPosZ[SHOTS_PER_VECTOR], // GPLSV
	
	/* Network Information */
	iPing[SHOTS_PER_VECTOR],
	Float:fPacketLoss[SHOTS_PER_VECTOR],
	
	/* Other Information */
	Float:fRatio
};

stock EncodeDataVector(const data[E_DATA_VECTOR], buffer[], buffer_size = sizeof(buffer)) {
	static const title[] = "VECTOR";
	#pragma unused title
	
	for(new i = 0; i < SHOTS_PER_VECTOR; i++) {
	    new line[512];
		format( line, sizeof(line),
				"%.2f %.2f %.2f "	/* fShooterPos			*/\
				"%.2f %.2f %.2f "	/* fShooterVel			*/\
				"%.2f %.2f %.2f "   /* fShooterCameraPosX 	*/\
				"%.2f %.2f %.2f "   /* fShooterCameraFV 	*/\
				"%.2f "             /* fShooterFacingAngle 	*/\
				"%i "               /* iPlayerSpecialAction */\
				"%i "               /* iWeaponID            */\
				"%.2f %.2f %.2f "	/* fVictimPos			*/\
				"%.2f %.2f %.2f "	/* fVictimVel			*/\
				"%.2f "             /* fVictimFacingAngle 	*/\
				"%.2f %.2f %.2f "	/* fOffset				*/\
				"%.2f %.2f %.2f "	/* fOrigin				*/\
				"%.2f %.2f %.2f "	/* fHitPos				*/\
				"%i "               /* iPing	            */\
				"%.2f ",            /* fPacketLoss          */\
				data[fShooterPosX][i], data[fShooterPosY][i], data[fShooterPosZ][i],
		        data[fShooterVelX][i], data[fShooterVelY][i], data[fShooterVelZ][i],
		        data[fShooterCameraPosX][i], data[fShooterCameraPosY][i], data[fShooterCameraPosZ][i],
		        data[fShooterCameraFVX][i], data[fShooterCameraFVY][i], data[fShooterCameraFVZ][i],
		        data[fShooterFacingAngle][i],
		        data[iPlayerSpecialAction][i],
		        data[iWeaponID][i],
		        data[fVictimPosX][i], data[fVictimPosY][i], data[fVictimPosZ][i],
		        data[fVictimVelX][i], data[fVictimVelY][i], data[fVictimVelZ][i],
		        data[fVictimFacingAngle][i],
		        data[fOffsetX][i], data[fOffsetY][i], data[fOffsetZ][i],
		        data[fOriginX][i], data[fOriginY][i], data[fOriginZ][i],
		        data[fHitPosX][i], data[fHitPosY][i], data[fHitPosZ][i],
				data[iPing],
				data[fPacketLoss]);
		strcat(buffer, line, buffer_size);
	}
	new ratio[16];
	format(ratio, sizeof(ratio), "%.2f", data[fRatio]);
	strcat(buffer, ratio, buffer_size);
}
