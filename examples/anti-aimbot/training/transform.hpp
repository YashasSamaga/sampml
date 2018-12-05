#include <vector>
#include <cmath>

#include <dlib/geometry.h>

#include <sampml/feature_vector.hpp>

#include "samp.hpp"

/* sequence in which the information is provided by the data vector */
using input_vector = sampml::feature_vector<double, 53>;
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

/* the information which we wish to use for training */
using output_vector = sampml::feature_vector<double, 77>;
struct features {
	static constexpr long SHOTS_PER_VECTOR = 10;
	struct {
		double VictimShooterDistance;
	
		double ShooterVelocityPerpendicular,
               ShooterVelocityParallel,
		       ShooterVelocityZ;
	
		double VictimVelocityPerpendicular,
           	   VictimVelocityParallel,
		   	   VictimVelocityZ;
	} shots[SHOTS_PER_VECTOR];

	double XYOffsetStdDev,
		   ZOffsetStdDev;

	double ShooterHorizontalAngleStdDev,
		   ShooterVerticalAngleStdDev;
		
	double ShooterHorizontalAngleRateStdDev,
		   ShooterVerticalAngleRateStdDev;

	double Ratio;       
};

/* online sample generator 
** collects input vectors and produces an output vector when possible
*/
class transformer {
    public:
        std::vector<output_vector> pool;
        
        void submit(const input_vector& input) {
            using vector_3d = dlib::vector<element_type, 3>;

            /* FILTERING
            ** We are interested in specific kind of shots to reduce the complexity
            ** and large data requirements.
            */
            if(input(iShooterWeaponID) != 31
           // || input(iShooterState) != samp::PLAYER_STATE_ONFOOT
           // || input(iVictimState) != samp::PLAYER_STATE_ONFOOT
           // || input(iShooterSpecialAction) != samp::SPECIAL_ACTION_NONE
           // || input(iVictimSpecialAction) != samp::SPECIAL_ACTION_NONE
            || input(bShooterSurfingVehicle)
            || input(bShooterSurfingObject)
            || input(bVictimSurfingVehicle)
            || input(bVictimSurfingObject)) { /* not M4 */
                reset();
                return;
            }

            if ((victimid != -1 && input(bHit) && victimid != input(iVictimID))
            || (last_tick != -1 && input(iTick) - last_tick > 2000)) {
                reset();
            }
            if (input(bHit))
				victimid = input(iVictimID);
            last_tick = input(iTick);
            
            /* FEATURE SELECTION/EXTRACTION
            ** This makes it easier for the model to learn and the features can
            ** also can be directly interpretated by humans.
            */
            shots++;

            if(input(bHit)) {
                int index = hits;
                hits++;

                vector_3d sPos, sVel, vPos, vVel;
                sPos.x() = input(fShooterPosX);
                sPos.y() = input(fShooterPosY);
                sPos.z() = input(fShooterPosZ);
                sVel.x() = input(fShooterVelocityX);
                sVel.y() = input(fShooterVelocityY);
                sVel.z() = input(fShooterVelocityZ);

                vPos.x() = input(fVictimPosX);
                vPos.y() = input(fVictimPosY);
                vPos.z() = input(fVictimPosZ);
                vVel.x() = input(fVictimVelocityX);
                vVel.y() = input(fVictimVelocityY);
                vVel.z() = input(fVictimVelocityZ);

                running_sample.shots[index].VictimShooterDistance = dlib::length(vPos - sPos);

                /* REFERENCE FRAME
                ** parallel_axis: axis along line joining the two players
                ** perpendicular_axis: axis along the normal of the plane formed by LOS and vertical line of the player
                ** z_axis: axis perpendicular to parallel_axis and perpendicular_axis
                */
                vector_3d xy_plane_normal(0, 0, 1);
                vector_3d parallel_axis 	= dlib::normalize(vPos - sPos),
                        perpendicular_axis 	= dlib::normalize(parallel_axis.cross(xy_plane_normal)),
                        z_axis 				= dlib::normalize(parallel_axis.cross(perpendicular_axis));

                running_sample.shots[index].ShooterVelocityPerpendicular = sVel.dot(perpendicular_axis); 
                running_sample.shots[index].ShooterVelocityParallel = sVel.dot(parallel_axis);
                running_sample.shots[index].ShooterVelocityZ = sVel.dot(z_axis);

                running_sample.shots[index].VictimVelocityPerpendicular = vVel.dot(perpendicular_axis); 
                running_sample.shots[index].VictimVelocityParallel = vVel.dot(parallel_axis);
                running_sample.shots[index].VictimVelocityZ = vVel.dot(z_axis);

                vector_3d bOffset;
                bOffset.x() = input(fOffsetX),
                bOffset.y() = input(fOffsetY),
                bOffset.z() = input(fOffsetZ);
                
                vector_3d bOffsetTransformed;
                bOffsetTransformed.x() = bOffset.dot(perpendicular_axis);
                bOffsetTransformed.y() = bOffset.dot(parallel_axis);
                bOffsetTransformed.z() = bOffset.dot(z_axis);

                XYOffsets(index) = std::sqrt(bOffsetTransformed.x()*bOffsetTransformed.x()
                                            + bOffsetTransformed.y()*bOffsetTransformed.y());
                ZOffsets(index) = std::abs(bOffsetTransformed.z());
            }

            /*
            ** Horizontal Angle: Z angle
            ** Vertical Angle: angle between camera vector and XY plane
            */
            vector_3d cFV;
            cFV.x() = input(fShooterCameraFVX),
            cFV.y() = input(fShooterCameraFVY),
            cFV.z() = input(fShooterCameraFVZ);
            vector_3d camera_axis = dlib::normalize(cFV); /* front vector is already normalized */

            double hAngle = std::atan2(camera_axis(0), camera_axis(1));
            sHorzAngle.push_back((hAngle < 0.0) ? -hAngle : 360.0 - hAngle);
            sVertAngle.push_back(std::asin(camera_axis(2)));

            if(hits == features::SHOTS_PER_VECTOR) {
                /* write an output vector */
                output_vector output;
                for(int i = 0; i < features::SHOTS_PER_VECTOR; i++) {
                    output(i*7 + 0) = running_sample.shots[i].VictimShooterDistance;
                    output(i*7 + 1) = running_sample.shots[i].ShooterVelocityPerpendicular;
                    output(i*7 + 2) = running_sample.shots[i].ShooterVelocityParallel;
                    output(i*7 + 3) = running_sample.shots[i].ShooterVelocityZ;
                    output(i*7 + 4) = running_sample.shots[i].VictimVelocityPerpendicular;
                    output(i*7 + 5) = running_sample.shots[i].VictimVelocityParallel;
                    output(i*7 + 6) = running_sample.shots[i].VictimVelocityZ;
                }

                output(70 + 0) = dlib::stddev(XYOffsets);
                output(70 + 1) = dlib::stddev(ZOffsets);

                assert(sHorzAngle.size() == sVertAngle.size());
                assert(sHorzAngle.size() == static_cast<std::size_t>(shots));
                dlib::matrix<double, 0, 1>  sHorzAngleStdDev(sHorzAngle.size());
                dlib::matrix<double, 0, 1>  sVertAngleStdDev(sVertAngle.size());
                dlib::matrix<double, 0, 1>  sHorzAngleChangeStdDev(sHorzAngle.size() - 1);
                dlib::matrix<double, 0, 1>  sVertAngleChangeStdDev(sVertAngle.size() - 1);

                for(int i = 0; i < sHorzAngleStdDev.size(); i++) {
                    sHorzAngleStdDev(i) = sHorzAngle[i];
                    sVertAngleStdDev(i) = sVertAngle[i];
                    if(i != 0) {
                        sHorzAngleChangeStdDev(i - 1) = sHorzAngle[i] - sHorzAngle[i - 1];
                        sVertAngleChangeStdDev(i - 1) = sVertAngle[i] - sVertAngle[i - 1];
                    }
                }
                output(70 + 2) = dlib::stddev(sHorzAngleStdDev);
                output(70 + 3) = dlib::stddev(sVertAngleStdDev);
                output(70 + 4) = dlib::stddev(sHorzAngleChangeStdDev);
                output(70 + 5) = dlib::stddev(sVertAngleChangeStdDev);
                output(70 + 6) = hits/static_cast<double>(shots);
                pool.push_back(output);
                reset();
            }	
        }

    private:
        using element_type = double;

        features running_sample;

        int shots, hits, victimid, last_tick;
        dlib::matrix<element_type, features::SHOTS_PER_VECTOR, 1> XYOffsets;
        dlib::matrix<element_type, features::SHOTS_PER_VECTOR, 1> ZOffsets;
        std::vector<element_type> sHorzAngle;
        std::vector<element_type> sVertAngle;

        void reset () {
            shots = hits = 0;
            victimid = -1;
            last_tick = -1;
            sHorzAngle.clear();
            sVertAngle.clear();
        }
};