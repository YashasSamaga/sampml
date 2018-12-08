#include <vector>
#include <cmath>
#include <optional>
#include <cassert>

#include <dlib/geometry.h>

#include <sampml/feature_vector.hpp>

#include "samp.hpp"

/* sequence in which the information is provided by the data vector */
using input_vector = sampml::feature_vector<double, 54>;
enum {
	bHit,
	
	fShooterPosX,
	fShooterPosY,
	fShooterPosZ,
	fShooterVelocityX,
	fShooterVelocityY,
	fShooterVelocityZ,
	
	fShooterCameraPosX,
	fShooterCameraPosY,
	fShooterCameraPosZ,
	fShooterCameraFVX,
	fShooterCameraFVY,
	fShooterCameraFVZ,
	fShooterCameraZoom,
	
	fShooterFacingAngle,
	
	iShooterState,
	iShooterSpecialAction,
	bShooterInVehicle,
	bShooterSurfingVehicle,
	bShooterSurfingObject,
	iShooterWeaponID,
	iShooterSkinID,
	iShooterID,

	fVictimPosX,
	fVictimPosY,
	fVictimPosZ,
	fVictimVelocityX,
	fVictimVelocityY,
	fVictimVelocityZ,
	
	fVictimFacingAngle,
	
	iVictimState,
	iVictimSpecialAction,
	bVictimInVehicle,
	bVictimSurfingVehicle,
	bVictimSurfingObject,
	iVictimWeaponID,
	iVictimSkinID,
	iVictimID,

    iHitType,
	fOffsetX, // OPWS
	fOffsetY, // OPWS
	fOffsetZ, // OPWS
	fOriginX, // GPLSV
	fOriginY, // GPLSV
	fOriginZ, // GPLSV
	fHitPosX, // GPLSV
	fHitPosY, // GPLSV
	fHitPosZ, // GPLSV

	iShooterPing,
	fShooterPacketLoss,
	iVictimPing,
	fVictimPacketLoss,

	iSecond,
	iTick
};

using float_element_type = float;
using vector_3d = dlib::vector<float_element_type, 3>;
struct translated_input {
    bool hit; /* TRUE = hit, FALSE = miss */
	
    struct {
        bool in_vehicle;
        bool surfing_vehicle;
        bool surfing_object;

        int state;
	    int special_action;
        int weapon;
        int skin;
        int id;

        vector_3d position;
        vector_3d velocity;
        float_element_type facing_angle;

        struct {
            vector_3d position;
            vector_3d FV;
            float_element_type zoom;
        } camera;

        struct {
            int ping;
            float_element_type packet_loss;
        } network;
    } shooter;    

    struct {
        bool in_vehicle;
        bool surfing_vehicle;
        bool surfing_object;

        int state;
	    int special_action;
        int weapon;
        int skin;
        int id;

        vector_3d position;
        vector_3d velocity;
        float_element_type facing_angle;

        struct {
            int ping;
            float_element_type packet_loss;
        } network;
    } victim;

    struct {
        int hit_type;
        vector_3d offset;
        vector_3d origin;
        vector_3d hit_pos;
    } shot;

    int second;
    int tick;
};

void translate(const input_vector& input, translated_input& output) {
    output.hit = input(bHit);
    
    output.shooter.in_vehicle = input(bShooterInVehicle);
    output.shooter.surfing_object =  input(bShooterSurfingObject);
    output.shooter.surfing_vehicle = input(bShooterSurfingVehicle);
    output.shooter.state = input(iShooterState);
    output.shooter.special_action = input(iShooterSpecialAction);
    output.shooter.weapon = input(iShooterWeaponID);
    output.shooter.skin = input(iShooterSkinID);
    output.shooter.id = input(iShooterID);

    output.shooter.position.x() = input(fShooterPosX);
    output.shooter.position.y() = input(fShooterPosY);
    output.shooter.position.z() = input(fShooterPosZ);

    output.shooter.velocity.x() = input(fShooterVelocityX);
    output.shooter.velocity.y() = input(fShooterVelocityY);
    output.shooter.velocity.z() = input(fShooterVelocityZ);

    output.shooter.facing_angle = input(fShooterFacingAngle);

    output.shooter.camera.position.x() = input(fShooterCameraPosX);
    output.shooter.camera.position.y() = input(fShooterCameraPosY);
    output.shooter.camera.position.z() = input(fShooterCameraPosZ);

    output.shooter.camera.FV.x() = input(fShooterCameraFVX);
    output.shooter.camera.FV.y() = input(fShooterCameraFVY);
    output.shooter.camera.FV.z() = input(fShooterCameraFVZ);

    output.shooter.camera.zoom = input(fShooterCameraZoom);

    output.shooter.network.ping = input(iShooterPing);
    output.shooter.network.packet_loss = input(fShooterPacketLoss);

    /* Victim Information */
    output.victim.in_vehicle = input(bVictimInVehicle);
    output.victim.surfing_object = input(bVictimSurfingObject);
    output.victim.surfing_vehicle = input(bVictimSurfingVehicle);
    output.victim.state = input(iVictimState);
    output.victim.special_action = input(iVictimSpecialAction);
    output.victim.weapon = input(iVictimWeaponID);
    output.victim.skin = input(iVictimSkinID);
    output.victim.id = input(iVictimID);

    output.victim.position.x() = input(fVictimPosX);
    output.victim.position.y() = input(fVictimPosY); 
    output.victim.position.z() = input(fVictimPosZ);

    output.victim.velocity.x() = input(fVictimVelocityX);
    output.victim.velocity.y() = input(fVictimVelocityY);
    output.victim.velocity.z() = input(fVictimVelocityZ);

    output.victim.facing_angle = input(fVictimFacingAngle);

    output.victim.network.ping = input(iVictimPing);
    output.victim.network.packet_loss = input(fVictimPacketLoss);

    /* Shot Information */
    output.shot.hit_type = input(iHitType);

    output.shot.offset.x() = input(fOffsetX);
    output.shot.offset.y() = input(fOffsetY);
    output.shot.offset.z() = input(fOffsetZ);

    output.shot.origin.x() = input(fOriginX);
    output.shot.origin.y() = input(fOriginY);
    output.shot.origin.z() = input(fOriginZ);

    output.shot.hit_pos.x() = input(fHitPosX);
    output.shot.hit_pos.y() = input(fHitPosY);
    output.shot.hit_pos.z() = input(fHitPosZ);

    /* Time information */
    output.second = input(iSecond);
    output.tick = input(iTick);
}

/* the information which we wish to use for training */
using output_vector = sampml::feature_vector<double, 15>;
struct features {
	static constexpr long SHOTS_PER_SAMPLE = 10;

    /*
    ** LOS = Line Of Shot
    ** LOC = Line Of player Centers
    */

    /*
    ** Many aimbots automatically aim along the XY plane only. Once the Z of the aim is
    ** set, the player can hit the victim without changing the aim as the XY is set by the aimbot
    ** and Z generally does not change.
    **
    ** For those who do not use aimbot, they need to adjust the XY continously. As they adjust, the
    ** Z also alters slightly.
    **
    */
	double HitXYOffsetStdDev, /* INVESTIGATE USE */
		   HitZOffsetStdDev;

    /*
    ** Aimbots do miss shots occasionally but they are usually not very off from the target but for
    ** non-aimbots, the miss offsets are generally larger.
    */
    double ImmediateMissXYOffsetStdDev;
    double ImmediateMissZOffsetStdDev; /* INVESTIGATE USE */

    /*
    ** Aimbots maintain very good aim at the player and hence the angle deviations are small
    ** even for random movement of players.
    */
    double LOCLOSAngleDeltaMean;
    double LOCLOSAngleDeltaStdDev;

    /*
    ** For players not using aimbot, the HITS tend to come when the velocity of the enemy
    ** is not changing abruptly. This is not true in the case of aimbots.
    **
    ** Hence, high velocity changes and successful hits indicate the possiblity of aimbot being used.
    **
    ** In most cases, the Z component is negligible and does not indicate anything useful for most aimbots
    ** as they don't adjust for Z. There are two cases:
    ** 1. player changes direction:
    **    in this case, it is mostly a transfer of velocity from parallel component to the perpendicular
    **    component and vice versa
    ** 2. player changes magnitude along one specific direction
    **    if the player changes magnitude along the parallel direction, it doesn't affect the difficulty for
    **    the player significantly but changing the magnitude along the perpendicular direction might make
    **    a difference
    **
    ** From the above analysis, we conclude that knowing the statistics of the perpendicular component
    ** gives enough information.
    **
    ** Puzzle: what if the player gets the immediate shot after a change successfuly but misses everything
    ** that comes next
    */
    double HitVictimVelocityPerpDeltasMean;
    double HitVictimVelocityPerpDeltasStdDev;

    double ImmediateMissVictimVelocityPerpDeltasMean;
    double ImmediateMissVictimVelocityPerpDeltasStdDev;

    /*
    ** If the victim is standing still, we don't want the shooter to be suspected for aimbot.
    ** We will use this for filtering.
    */          
	double ShooterHorizontalAngleStdDev, /* INVESTIGATE USE */
           ShooterHorizontalAngleRateStdDev; /* INVESTIGATE USE */
	
    /* 
    ** As in most cases, the shooter need not adjust their aim while using an aimbot, the Z angle
    ** does not change significantly (if at all) as the XY is automatically adjusted by the aimbot
    ** and Z tends to not change very much.
    **
    ** Without an aimbot, the player needs to continously adjust the XY which also causes the Z
    ** to change slightly.
    */
	double ShooterVerticalAngleStdDev,
           ShooterVerticalAngleModeFrequency,
		   ShooterVerticalAngleRateStdDev;

	double HMRatio;  /* UNSUED */  
};

/* online sample generator 
** collects input vectors and produces an output vector when possible
*/
class transformer {
    public:
        std::vector<output_vector> pool;

        template<class Container>
        void submit(const Container& cont) {
            for(const auto& v : cont)
                submit(v);
        }

        template<class Container = input_vector>
        void submit(const input_vector& raw_input) {
            translated_input input;
            translate(raw_input, input);

            if(filter(input))
                return;           

            shots++;

            vector_3d line_of_centers = input.victim.position - input.shooter.position;               

    		/* REFERENCE FRAME
            ** parallel_axis: axis along line joining the two players
            ** perpendicular_axis: axis along the normal of the plane formed by LOS and vertical line of the player
            ** z_axis: axis perpendicular to parallel_axis and perpendicular_axis
            */
            const vector_3d xy_plane_normal(0, 0, 1); //TODO
            vector_3d parallel_axis, perpendicular_axis, z_axis;				
			parallel_axis 	    = dlib::normalize(line_of_centers);
            perpendicular_axis 	= dlib::normalize(parallel_axis.cross(xy_plane_normal));
            z_axis 				= dlib::normalize(parallel_axis.cross(perpendicular_axis));	

            /* TODO check relative perp velocity 
            CASE player and victim moving together in the same direction */
            vector_3d transformed_victim_velocity;
            transformed_victim_velocity.x() = input.victim.velocity.dot(perpendicular_axis); 
            transformed_victim_velocity.y() = input.victim.velocity.dot(parallel_axis);
            transformed_victim_velocity.z() = input.victim.velocity.dot(z_axis);

            if(prev_victim_velocity.has_value()) {
                float change = transformed_victim_velocity.x() - prev_victim_velocity.value().x();
                if(input.hit)
                    HitVictimVelocityPerpDeltas.push_back(change);
                else
                    ImmediateMissVictimVelocityPerpDeltas.push_back(change);
            }
            prev_victim_velocity = transformed_victim_velocity;

            if(input.hit) {
                hits++;
                miss_streak = 0;

                vector_3d transformed_shot_offset;
                transformed_shot_offset.x() = input.shot.offset.dot(perpendicular_axis);
                transformed_shot_offset.y() = input.shot.offset.dot(parallel_axis);
                transformed_shot_offset.z() = input.shot.offset.dot(z_axis);

                HitXYOffsets.push_back(
                std::sqrt(transformed_shot_offset.x()*transformed_shot_offset.x()
                        + transformed_shot_offset.y()*transformed_shot_offset.y()));

                HitZOffsets.push_back(std::abs(transformed_shot_offset.z()));                    
            } else {
                miss_streak++;    
            }

            if(miss_streak == 1) {
                /* immediate miss */
                vector_3d line_of_shot = input.shot.hit_pos - input.shot.origin;
  
                /* check reliablity */  
                if(line_of_shot.length() < 100.0 && input.shot.hit_pos.length() > 1.0) {
                    vector_3d offset = line_of_centers + line_of_shot;				

                    vector_3d transformed_shot_offset;
                    transformed_shot_offset.x() = offset.dot(perpendicular_axis);
                    transformed_shot_offset.y() = offset.dot(parallel_axis);
                    transformed_shot_offset.z() = offset.dot(z_axis);

                    ImmediateMissXYOffsets.push_back(
                        std::sqrt(transformed_shot_offset.x()*transformed_shot_offset.x()
                                + transformed_shot_offset.y()*transformed_shot_offset.y()));
                    ImmediateMissZOffsets.push_back(std::abs(transformed_shot_offset.z()));
                }
            }

            vector_3d camera_axis = dlib::normalize(input.shooter.camera.FV);
            float angle = camera_axis.dot(line_of_centers)/line_of_centers.length();
                LOCLOSAngleDifferences.push_back(std::abs(std::acos(angle)));

            /*
            ** Horizontal Angle: Z angle
            ** Vertical Angle: angle between camera vector and XY plane
            */

            double hAngle = std::atan2(camera_axis(0), camera_axis(1)) * 180/pi();
            ShooterHorizontalAngles.push_back((hAngle < 0.0) ? -hAngle : 360.0 - hAngle);
            ShooterVerticalAngles.push_back(std::asin(camera_axis(2)) * 180/pi());

            if(shots > features::SHOTS_PER_SAMPLE) {
                if(shots >= 20) {
                    reset();
                    return;
                }

                if(hits == shots)
                    return;

                if(shots - hits < 2)
                    return;

                if(hits <= 3)
                    return;
                    
                output_vector output;

                dlib::matrix<float_element_type, 0, 1> HitXYOffsets_dm;
                HitXYOffsets_dm.set_size(HitXYOffsets.size());
                for(int i = 0; i < HitXYOffsets_dm.size(); i++)
                    HitXYOffsets_dm(i) = HitXYOffsets[i];
                output(0) = dlib::stddev(HitXYOffsets_dm);        

                dlib::matrix<float_element_type, 0, 1> HitZOffsets_dm;
                HitZOffsets_dm.set_size(HitZOffsets.size());
                for(int i = 0; i < HitZOffsets_dm.size(); i++)
                    HitZOffsets_dm(i) = HitZOffsets[i];
                output(1) = dlib::stddev(HitZOffsets_dm);

                dlib::matrix<float_element_type, 0, 1> ImmediateMissXYOffsets_dm;
                ImmediateMissXYOffsets_dm.set_size(ImmediateMissXYOffsets.size());
                for(int i = 0; i < ImmediateMissXYOffsets_dm.size(); i++)
                    ImmediateMissXYOffsets_dm(i) = ImmediateMissXYOffsets[i];
                output(2) = dlib::stddev(ImmediateMissXYOffsets_dm);          

                dlib::matrix<float_element_type, 0, 1> ImmediateMissZOffsets_dm;
                ImmediateMissZOffsets_dm.set_size(ImmediateMissZOffsets.size());
                for(int i = 0; i < ImmediateMissZOffsets_dm.size(); i++)
                    ImmediateMissZOffsets_dm(i) = ImmediateMissZOffsets[i];
                output(3) = dlib::stddev(ImmediateMissZOffsets_dm);

                dlib::matrix<float_element_type, 0, 1> LOCLOSAngleDifferences_dm;
                LOCLOSAngleDifferences_dm.set_size(LOCLOSAngleDifferences.size());
                for(int i = 0; i < LOCLOSAngleDifferences_dm.size(); i++)
                    LOCLOSAngleDifferences_dm(i) = LOCLOSAngleDifferences[i];
                output(4) = dlib::mean(LOCLOSAngleDifferences_dm); 
                output(5) = dlib::stddev(LOCLOSAngleDifferences_dm);

                dlib::matrix<float_element_type, 0, 1> HitVictimVelocityPerpDeltas_dm;
                HitVictimVelocityPerpDeltas_dm.set_size(HitVictimVelocityPerpDeltas.size());
                for(int i = 0; i < HitVictimVelocityPerpDeltas_dm.size(); i++)
                    HitVictimVelocityPerpDeltas_dm(i) = HitVictimVelocityPerpDeltas[i];
                output(6) = dlib::mean(HitVictimVelocityPerpDeltas_dm);
                output(7) = dlib::stddev(HitVictimVelocityPerpDeltas_dm);

                dlib::matrix<float_element_type, 0, 1> ImmediateMissVictimVelocityPerpDeltas_dm;
                ImmediateMissVictimVelocityPerpDeltas_dm.set_size(ImmediateMissVictimVelocityPerpDeltas.size());
                for(int i = 0; i < ImmediateMissVictimVelocityPerpDeltas_dm.size(); i++)
                    ImmediateMissVictimVelocityPerpDeltas_dm(i) = ImmediateMissVictimVelocityPerpDeltas[i];
                output(8) = dlib::mean(ImmediateMissVictimVelocityPerpDeltas_dm);
                output(9) = dlib::stddev(ImmediateMissVictimVelocityPerpDeltas_dm);
                
                dlib::matrix<float_element_type, 0, 1> ShooterHorizontalAngles_dm;
                ShooterHorizontalAngles_dm.set_size(ShooterHorizontalAngles.size());
                for(int i = 0; i < ShooterHorizontalAngles_dm.size(); i++)
                    ShooterHorizontalAngles_dm(i) = ShooterHorizontalAngles[i];
                output(10) = dlib::stddev(ShooterHorizontalAngles_dm);

                dlib::matrix<float_element_type, 0, 1> ShooterHorizontalAngleRates_dm;
                ShooterHorizontalAngleRates_dm.set_size(ShooterHorizontalAngles.size() - 1);
                for(int i = 0; i < ShooterHorizontalAngleRates_dm.size(); i++)
                    ShooterHorizontalAngleRates_dm(i) = ShooterHorizontalAngles[i + 1] - ShooterHorizontalAngles[i];
                output(11) = dlib::stddev(ShooterHorizontalAngleRates_dm);

                dlib::matrix<float_element_type, 0, 1> ShooterVerticalAngles_dm;
                ShooterVerticalAngles_dm.set_size(ShooterVerticalAngles.size());
                for(int i = 0; i < ShooterVerticalAngles_dm.size(); i++)
                    ShooterVerticalAngles_dm(i) = ShooterVerticalAngles[i];
                output(12) = dlib::stddev(ShooterVerticalAngles_dm);
                output(13) = 0.0;

                dlib::matrix<float_element_type, 0, 1> ShooterVerticalAngleRates_dm;
                ShooterVerticalAngleRates_dm.set_size(ShooterHorizontalAngles.size() - 1);
                for(int i = 0; i < ShooterVerticalAngleRates_dm.size(); i++)
                    ShooterVerticalAngleRates_dm(i) = ShooterVerticalAngles[i + 1] - ShooterVerticalAngles[i];
                output(14) = dlib::stddev(ShooterVerticalAngleRates_dm);

                pool.push_back(output);
                reset();
            }
        }

        bool filter(translated_input& input) {
            if(input.shooter.weapon != 31
            || input.shooter.state != samp::PLAYER_STATE_ONFOOT
            || input.victim.state != samp::PLAYER_STATE_ONFOOT
            || input.shooter.special_action != samp::SPECIAL_ACTION_NONE
            || input.victim.special_action != samp::SPECIAL_ACTION_NONE
            || input.shooter.in_vehicle
            || input.shooter.surfing_object
            || input.shooter.surfing_vehicle
            || input.victim.in_vehicle
            || input.victim.surfing_object
            || input.victim.surfing_vehicle
            || (input.hit == false && hits == 0)
            ) {
                reset();
                return true;
            }

            if((input.hit && prev_victimid != samp::INVALID_PLAYER_ID && prev_victimid != input.victim.id)
            || (prev_tick != -1 && input.tick - prev_tick > 600)) {
                reset();
            }

			prev_victimid = input.victim.id;
            prev_tick = input.tick;
            return false;
        }

    private:
        std::optional<vector_3d> prev_victim_velocity;

        std::vector<float_element_type> HitXYOffsets;
        std::vector<float_element_type> HitZOffsets;

        std::vector<float_element_type> ImmediateMissXYOffsets;
        std::vector<float_element_type> ImmediateMissZOffsets;

        std::vector<float_element_type> HitVictimVelocityPerpDeltas;
        std::vector<float_element_type> ImmediateMissVictimVelocityPerpDeltas;

        std::vector<float_element_type> LOCLOSAngleDifferences;

        std::vector<float_element_type> ShooterHorizontalAngles;
        std::vector<float_element_type> ShooterVerticalAngles;

        int shots, hits, miss_streak, prev_victimid, prev_tick;

        void reset () {
            shots = hits = miss_streak = 0;
            prev_victimid = samp::INVALID_PLAYER_ID;
            prev_tick = -1;

            prev_victim_velocity.reset();

            HitXYOffsets.clear();
            HitZOffsets.clear();
            ImmediateMissXYOffsets.clear();
            ImmediateMissZOffsets.clear();
            HitVictimVelocityPerpDeltas.clear();
            ImmediateMissVictimVelocityPerpDeltas.clear();
            LOCLOSAngleDifferences.clear();
            ShooterHorizontalAngles.clear();
            ShooterVerticalAngles.clear();
        }

        static constexpr double pi() { return std::atan(1)*4; }
};