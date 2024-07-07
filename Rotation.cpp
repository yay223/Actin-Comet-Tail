#include <math.h>
#include "Rotation.h"

#define PI 3.14159265358979323846


Coordinate Rotation::returnInteractionSite(Coordinate beadCenter, Coordinate particlePos, Coordinate beadAxes){
    Coordinate centerToParticleVec = beadCenter-particlePos;

    double gammaAngle;
    if (centerToParticleVec.z > beadCenter.z){
        gammaAngle = atan(pow(pow(centerToParticleVec.x,2) + pow(centerToParticleVec.y,2),0.5) / centerToParticleVec.z);
    }
    else if (centerToParticleVec.z < beadCenter.z){
        gammaAngle = PI + atan(pow(pow(centerToParticleVec.x,2) + pow(centerToParticleVec.y,2),0.5) / centerToParticleVec.z);
    }
    else if (centerToParticleVec.z == beadCenter.z){
        gammaAngle = PI/2;
    }
    else if (centerToParticleVec.z == 0 && centerToParticleVec.x == 0 && centerToParticleVec.y == 0){ //ideally this condition should never be true in a real simulation    
        gammaAngle = 0; //undefined
    }

    double lambdaAngle;
    if (centerToParticleVec.x > beadCenter.x){
        lambdaAngle = atan(centerToParticleVec.y/centerToParticleVec.x);
    }
    else if (centerToParticleVec.x < beadCenter.x && centerToParticleVec.y >= beadCenter.y){
        lambdaAngle = atan(centerToParticleVec.y/centerToParticleVec.x) + PI;
    }
    else if (centerToParticleVec.x < beadCenter.x && centerToParticleVec.y < beadCenter.y){
        lambdaAngle = atan(centerToParticleVec.y/centerToParticleVec.x) - PI;
    }
    else if (centerToParticleVec.x == beadCenter.x && centerToParticleVec.y > beadCenter.y){
        lambdaAngle = PI/2;
    }
    else if (centerToParticleVec.x == beadCenter.x && centerToParticleVec.y < beadCenter.y){
        lambdaAngle = -PI/2;
    }
    else if (centerToParticleVec.x == beadCenter.x && centerToParticleVec.y == beadCenter.y){ //ideally, this condition should never be true in a real simulation
        lambdaAngle = 0; //undefined
    }

    #define beadAxes.x a
    #define beadAxes.y b
    #define beadAxes.z c

    double R = (a*b*c) / (pow((c*c * pow(cos(gammaAngle),2) *(b*b* pow(cos(lambdaAngle),2) + a*a* pow(sin(lambdaAngle) + a*a*b*b * pow(sin(gammaAngle),2),2))),0.5)); //distance from the center of the bead to surface for given lambda and gamma
    
    Coordinate interactionSite;
    interactionSite.x = beadCenter.x + R * cos(lambdaAngle) * cos(gammaAngle);
    interactionSite.y = beadCenter.y + R * cos(lambdaAngle) * sin(gammaAngle);
    interactionSite.z = beadCenter.z + R * sin(lambdaAngle);
    return interactionSite;
}