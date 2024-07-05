#include <vector>
#include <Coordinate.h>
#include <Quaternion.h>



class Rotation{
    private:
        int numRotatingParticles;//number of particles to use to make interactions sites on bead surface
        double a, b, c; //principal semi axes of the bead
        
        std::vector <struct Quaternion> q_pos; //position quaternions
        std::vector <struct Quaternion> q_vel; //velocity quaternions
        std::vector <struct Quaternion> q_acc; //acceleration quaternions

        std::vector <struct Quaternion> q_bins; //quaternion bins

    public:
        Rotation();
        Rotation(int numRotatingParticles, double a, double b, double c);
};
