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

const FEATURE_VECTOR_SIZE = _:E_FEATURE_VECTOR * 8 + 1;
enum E_NETWORK_STATS {
	ping,
	packet_loss
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
