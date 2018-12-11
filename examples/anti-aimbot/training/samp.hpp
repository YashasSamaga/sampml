namespace samp {
	constexpr int INVALID_PLAYER_ID = 65535;

	enum {
		PLAYER_STATE_NONE,
		PLAYER_STATE_ONFOOT,
		PLAYER_STATE_DRIVER,
		PLAYER_STATE_PASSENGER,
		PLAYER_STATE_WASTED,
		PLAYER_STATE_SPAWNED,
		PLAYER_STATE_SPECTATING
	};

	enum {
		SPECIAL_ACTION_NONE = 0,
		SPECIAL_ACTION_DUCK = 1,
		SPECIAL_ACTION_USEJETPACK = 2,
		SPECIAL_ACTION_ENTER_VEHICLE = 3,
		SPECIAL_ACTION_EXIT_VEHICLE = 4,
		SPECIAL_ACTION_DANCE1 = 5,
		SPECIAL_ACTION_DANCE2 = 6,
		SPECIAL_ACTION_DANCE3 = 7,
		SPECIAL_ACTION_DANCE4 = 8,
		SPECIAL_ACTION_HANDSUP = 10,
		SPECIAL_ACTION_USECELLPHONE = 11,
		SPECIAL_ACTION_SITTING = 12,
		SPECIAL_ACTION_STOPUSECELLPHO = 13,
		SPECIAL_ACTION_DRINK_BEER = 20,
		SPECIAL_ACTION_SMOKE_CIGGY = 21,   
		SPECIAL_ACTION_DRINK_WINE = 22,
		SPECIAL_ACTION_DRINK_SPRUNK = 23,
		SPECIAL_ACTION_CUFFED = 24,
		SPECIAL_ACTION_CARRY = 25,    
		SPECIAL_ACTION_PISSING = 68
	};

	enum {
		CAMERA_MODE_TRAIN = 3, /* Train/tram camera */
		CAMERA_FOLLOW_PED = 4, /* Follow ped (normal behind player camera) */
		CAMERA_SNIPER_AIM = 7, /* Sniper aiming */
		CAMERA_ROCKET_LAUNCHER_AIM = 8, /* Rocket Launcher aiming */
		CAMERA_FIXED_CAMERA = 15, /* Fixed camera (non-moving) - used for Pay 'n' Spray, chase camera, tune shops, entering buildings, buying food etc. */
		CAMERA_VEHICLE_FRONT = 16, /* Vehicle front camera, bike side camera */
		CAMERA_VEHICLE_NORMAL = 18, /* Normal car (+skimmer+helicopter+airplane), several variable distances */
		CAMERA_BOAT = 22, /* Normal boat camera */
		CAMERA_CAMERA_AIM = 46, /* Camera weapon aiming */
		CAMERA_HEAT_SEEKER_AIM = 51, /* Heat-seeking Rocket Launcher aiming */
		CAMERA_WEAPON_AIM = 53, /* Aiming any other weapon */
		CAMERA_PASSENGER_DRIVEBY = 55, /* Vehicle passenger drive-by camera */
		CAMERA_CHASE_BIRD_VIEW = 56, /* Chase camera: helicopter/bird view */
		CAMERA_CHASE_GROUND_VIEW = 57, /* Chase camera: ground camera, zooms in very quickly (Similar to 56, but on the ground) */
		CAMERA_CHASE_HORIZONTAL_FLYBY = 58, /* Chase camera: horizontal flyby past vehicle */
		CAMERA_CHASE_AIR1 = 59, /* Chase camera (for air vehicles only): ground camera, looking up to the air vehicle */
		CAMERA_CHASE_AIR2 = 62, /* Chase camera (for air vehicles only): vertical flyby past air vehicle  */
		CAMERA_CHASE_AIR3 = 63, /* Chase camera (for air vehicles only): horizontal flyby past air vehicle (similar to 58 and 62) */
		CAMERA_CHASE_AIR4 = 64 /* Chase camera (for air vehicles only): camera focused on pilot, similar to pressing LOOK_BEHIND key on foot, but in air vehicle */
	};
}