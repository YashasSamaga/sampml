#include <vector>
#include <cmath>
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
    //fShooterCameraAspectRatio,
    //iShooterCameraMode,
	
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
            float_element_type aspect_ratio;
            int mode;
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
    output.shooter.camera.mode = samp::CAMERA_WEAPON_AIM; //input(iShooterCameraMode);
    output.shooter.camera.aspect_ratio = 1.6; //input(fShooterCameraAspectRatio);

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
using output_vector = sampml::feature_vector<double, 16>;
struct features {
	static constexpr long SHOTS_PER_SAMPLE = 8;

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
    ** Z variations decrease as the distance between the players increases and hence becomes an
    ** ineffective gauge of aimbot usage
    */

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

    /*
    ** Aimbots do miss shots occasionally but they are usually not very off from the target but for
    ** non-aimbots, the miss offsets are generally larger.
    */

    /*
    ** Aimbots maintain very good aim at the player and hence the angle deviations are small
    ** even for random movement of players.
    */

    /*
    ** As in most cases, the shooter need not adjust their aim while using an aimbot, the Z angle
    ** does not change significantly (if at all) as the XY is automatically adjusted by the aimbot
    ** and Z tends to not change very much.
    **
    ** Without an aimbot, the player needs to continously adjust the XY which also causes the Z
    ** to change slightly.
    */

    double LOCLOSAngleDeltaMean;
    double LOCLOSAngleDeltaStdDev;

	double ShooterVerticalAngleStdDev,
		   ShooterVerticalAngleRateMean;
    int ConstantVerticalAngleStreak;

    double HitVerticalAngleStdDev;
    double HitLOCLOSAngleDeltaMean,
           HitLOCLOSAngleDeltaStdDev;
    double HitVictimVelocityPerpDeltasMean,
           HitVictimVelocityPerpDeltasStdDev;

    double ImmediateMissLOCLOSAngleDeltaMean,
           ImmediateMIssLOCLOSAngleDeltaStdDev;
    double ImmediateMissVictimVelocityPerpDeltasMean,
           ImmediateMissVictimVelocityPerpDeltasStdDev;

    double AllMissLOCLOSAngleDeltaMean,
           AllMissLOCLOSAngleDeltaStdDev;
};

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

            bool try_process = preprocess(input);

            if(try_process)
                process();
        }       

    private:
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
            || input.victim.velocity.length() < 0.02
            || input.shooter.camera.mode != samp::CAMERA_WEAPON_AIM
            || (input.shooter.position - input.victim.position).length() < 10.0
            || (input.shooter.position - input.victim.position).length() > 40.0
            || (input.hit == false && shot_status.size() == 0)
            ) {
                reset();
                return true;
            }

            if((input.hit && prev_victimid != samp::INVALID_PLAYER_ID && prev_victimid != input.victim.id)
            || (prev_tick != -1 && input.tick - prev_tick > 500)) {
                reset();
                if (input.hit == false)
                    return true;
            }

			prev_victimid = input.victim.id;
            prev_tick = input.tick;
            return false;
        }

        bool preprocess(const translated_input& input) {
            const vector_3d xy_plane_normal(0, 0, 1);
            vector_3d line_of_centers = input.victim.position - input.shooter.position;

            vector_3d parallel_axis, perpendicular_axis, z_axis;				
			parallel_axis 	    = dlib::normalize(line_of_centers);
            perpendicular_axis 	= dlib::normalize(parallel_axis.cross(xy_plane_normal));
            z_axis 				= dlib::normalize(parallel_axis.cross(perpendicular_axis));

            vector_3d transformed_shooter_velocity;
            transformed_shooter_velocity.x() = input.shooter.velocity.dot(perpendicular_axis); 
            transformed_shooter_velocity.y() = input.shooter.velocity.dot(parallel_axis);
            transformed_shooter_velocity.z() = input.shooter.velocity.dot(z_axis);

            vector_3d transformed_victim_velocity;
            transformed_victim_velocity.x() = input.victim.velocity.dot(perpendicular_axis); 
            transformed_victim_velocity.y() = input.victim.velocity.dot(parallel_axis);
            transformed_victim_velocity.z() = input.victim.velocity.dot(z_axis);

            const vector_3d camera_axis = input.shooter.camera.FV;
            vector_3d right_vector = dlib::normalize(camera_axis.cross(xy_plane_normal));
            vector_3d up_vector = dlib::normalize(camera_axis.cross(right_vector));

            vector_3d transformed_camera_axis;
            transformed_camera_axis.x() = camera_axis.dot(perpendicular_axis); 
            transformed_camera_axis.y() = camera_axis.dot(parallel_axis);
            transformed_camera_axis.z() = camera_axis.dot(z_axis);         

            const double scaleY = (input.shooter.camera.aspect_ratio > 1.375) ? 0.4 : 0.525;
            const double pX = ((340.0/640.0) - 0.5) * 2.0 * 0.7,
                         pZ = ((225.0/480.0) - 0.5) * 2.0 * scaleY;

            vector_3d reticle_axis;
            reticle_axis = dlib::normalize(camera_axis + right_vector * pX + up_vector * pZ);
            
            vector_3d transformed_reticle_axis;
            transformed_reticle_axis.x() = reticle_axis.dot(perpendicular_axis);
            transformed_reticle_axis.y() = reticle_axis.dot(parallel_axis);
            transformed_reticle_axis.z() = reticle_axis.dot(z_axis);

            shot_status.push_back(input.hit);
            shot_timestamp.push_back(input.tick);
            shooter_facing_angles.push_back(input.shooter.facing_angle);
            transformed_shooter_velocities.push_back(transformed_shooter_velocity);
            transformed_victim_velocities.push_back(transformed_victim_velocity);
            transformed_line_of_centers.push_back(vector_3d(0.0, line_of_centers.length(), 0.0));
            original_camera_axes.push_back(camera_axis);
            transformed_camera_axes.push_back(transformed_camera_axis);
            transformed_reticle_axes.push_back(transformed_reticle_axis);

            return (shot_status.size() >= 12);
        }

        void process() {
            if(shot_status.size() >= 20) {
                reset();
                return;
            }

            if(shot_status.back() == true)
                return;

            int shots = shot_status.size();
            for(int i = shot_status.size() - 1; shot_status[i] == false && shot_status[i - 1] == false; i--) {
                assert(i > 0);
                shots = i;
            }

            int hits = std::count(shot_status.begin(), shot_status.begin() + shots, true),
                misses = shots - hits,
                imisses = 0;
            for(int i = 1; i < shots; i++)
                if(shot_status[i] == false && shot_status[i-1] == true)
                    imisses++;   

            if(misses < 2)
                return;

            if(hits < 3)
                return;

            /* fix series mix TODO ex: ____------ vAngles for aimbot (use running_stats?) */

            output_vector output;
            dlib::matrix<float_element_type, 0, 1> LOCLOSDifferences(shots);
            for(int i = 0; i < shots; i++) {
                const auto& line_of_center = transformed_line_of_centers[i];
                const auto& reticle_axis = transformed_reticle_axes[i];
                LOCLOSDifferences(i) = std::acos(reticle_axis.dot(line_of_center)/line_of_center.length());
            }
            output(0) = dlib::mean(LOCLOSDifferences); /* LOCLOSAngleDeltaMean */ /* absolute? TODO */
            output(1) = dlib::stddev(LOCLOSDifferences); /* LOCLOSAngleDeltaStdDev */

            dlib::matrix<float_element_type, 0, 1> VerticalAngles(shots);
            for(int i = 0; i < shots; i++) {
                const auto& camera_axis = original_camera_axes[i];
                VerticalAngles(i) = std::asin(camera_axis(2));
            }
            output(2) = dlib::stddev(VerticalAngles); /* ShooterVerticalAngleStdDev */

            dlib::matrix<float_element_type, 0, 1> VerticalAngleRates(shots - 1);
            for(int i = 1; i < shots; i++) {
                auto prev = VerticalAngles(i - 1);
                auto cur = VerticalAngles(i);
                VerticalAngleRates(i - 1) = std::abs(cur - prev);
            }
            output(3) = dlib::mean(VerticalAngleRates); /* ShooterVerticalAngleRateMean */

            int max_vangle_streak = 0;
            for(int i = 1, current_streak = 0; i < shots; i++) {
                auto prev = VerticalAngles(i - 1);
                auto cur = VerticalAngles(i);
                if(cur == prev && (shooter_facing_angles[i] != shooter_facing_angles[i - 1]))
                    current_streak++;
                else {
                    max_vangle_streak = std::max(max_vangle_streak, current_streak);
                    current_streak = 0;
                }
            }
            output(4) = max_vangle_streak; /* ConstantVerticalAngleStreak */
            if(max_vangle_streak > 4) {
                std::cout << "BEGIN: \n";
                for(int i = 0; i < shots; i++)
                {
                    std::cout << VerticalAngles(i) << " " << shooter_facing_angles[i] << ' ' << transformed_victim_velocities[i] << " " << shot_timestamp[i] << std::endl;
                }
            }

            dlib::matrix<float_element_type, 0, 1> HitVerticalAngles(hits);
            dlib::matrix<float_element_type, 0, 1> HitLOCLOSDifferences(hits);
            for(int i = 0, hit_idx = 0; i < shots; i++) {
                if(shot_status[i] == true) {
                    HitVerticalAngles(hit_idx) = VerticalAngles(i);
                    HitLOCLOSDifferences(hit_idx) = LOCLOSDifferences(i);
                    hit_idx++; 
                }                
            }
            output(5) = dlib::stddev(HitVerticalAngles); /* HitVerticalAngleStdDev */
            output(6) = dlib::mean(HitLOCLOSDifferences); /* HitLOCLOSAngleDeltaMean */
            output(7) = dlib::stddev(HitLOCLOSDifferences); /* HitLOCLOSAngleDeltaStdDev */
            
            dlib::matrix<float_element_type, 0, 1> HitVictimPerpVelocityDeltas(hits - 1);
            for(int i = 1, hit_idx = 0; i < shots; i++) {
                if(shot_status[i] == true) {
                    const auto& v1 = transformed_victim_velocities[i-1];
                    const auto& v2 = transformed_victim_velocities[i];
                    HitVictimPerpVelocityDeltas(hit_idx) = std::abs(v2.x() - v1.x());
                    hit_idx++;
                }
            }
            output(8) = dlib::mean(HitVictimPerpVelocityDeltas); /* HitVictimVelocityPerpDeltasMean */
            output(9) = dlib::stddev(HitVictimPerpVelocityDeltas); /* HitVictimVelocityPerpDeltasStdDev */        

            dlib::matrix<float_element_type, 0, 1> IMissLOCLOSDifferences(imisses);
            for(int i = 1, imiss_idx = 0; i < shots; i++) {
                if(shot_status[i] == false && shot_status[i-1] == true) {
                    IMissLOCLOSDifferences(imiss_idx) = LOCLOSDifferences(i);
                    imiss_idx++; 
                }                
            }
            output(10) = dlib::mean(IMissLOCLOSDifferences); /* ImmediateMissLOCLOSAngleDeltaMean */
            output(11) = dlib::stddev(IMissLOCLOSDifferences); /* ImmediateMIssLOCLOSAngleDeltaStdDev */
    
            dlib::matrix<float_element_type, 0, 1> IMissVictimPerpVelocityDeltas(imisses);
            for(int i = 1, imiss_idx = 0; i < shots; i++) {
                if(shot_status[i] == false && shot_status[i-1] == true) {
                    const auto& v1 = transformed_victim_velocities[i-1];
                    const auto& v2 = transformed_victim_velocities[i];
                    IMissVictimPerpVelocityDeltas(imiss_idx) = std::abs(v2.x() - v1.x());
                    imiss_idx++;
                }
            }
            output(12) = dlib::mean(IMissVictimPerpVelocityDeltas); /* ImmediateMissVictimVelocityPerpDeltasMean */
            output(13) = dlib::stddev(IMissVictimPerpVelocityDeltas); /* ImmediateMissVictimVelocityPerpDeltasStdDev */

            dlib::matrix<float_element_type, 0, 1> AllMissLOCLOSDifferences(misses);
            for(int i = 1, miss_idx = 0; i < shots; i++) {
                if(shot_status[i] == false) {
                    AllMissLOCLOSDifferences(miss_idx) = LOCLOSDifferences(i);
                    miss_idx++; 
                }                
            }
            output(14) = dlib::mean(AllMissLOCLOSDifferences); /* AllMissLOCLOSAngleDeltaMean */
            output(15) = dlib::stddev(AllMissLOCLOSDifferences); /* AllMissLOCLOSAngleDeltaStdDev */

            pool.push_back(output);
            reset();
        }

        /* processing data */
        std::vector<unsigned int> shot_timestamp;
        std::vector<bool> shot_status;
        std::vector<float_element_type> shooter_facing_angles;
        std::vector<vector_3d> transformed_shooter_velocities;
        std::vector<vector_3d> transformed_victim_velocities;
        std::vector<vector_3d> transformed_line_of_centers;
        std::vector<vector_3d> original_camera_axes;
        std::vector<vector_3d> transformed_camera_axes;
        std::vector<vector_3d> transformed_reticle_axes;

        /* filtering data */
        int prev_victimid, prev_tick;

        void reset () {
            prev_victimid = samp::INVALID_PLAYER_ID;
            prev_tick = -1;

            shot_timestamp.clear();
            shot_status.clear();
            shooter_facing_angles.clear();
            transformed_shooter_velocities.clear();
            transformed_victim_velocities.clear();
            transformed_line_of_centers.clear();
            original_camera_axes.clear();
            transformed_camera_axes.clear();
            transformed_reticle_axes.clear();
        }

        static constexpr double pi() { return std::atan(1)*4; }
};