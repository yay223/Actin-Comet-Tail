#ifndef QUATERNION_H
#define QUATERNION_H

struct Quaternion{
    double q1, q2, q3, q4; //q = q4 + iq1 + jq2 + kq3

    Quaternion(){
        q1 = 0;
        q2 = 0;
        q3 = 0;
        q4 = 0;
    }

//----------------------------------------------------------------------------------------------------//
    Quaternion(double a, double b, double c, double d): q1(a), q2(b), q3(c), q4(d)
    {
    }
    /*The function definition you are seeing is a constructor initializer list in C++. It is used to initialize member variables of a class or struct directly. This approach can be more efficient than assigning values within the constructor body because it initializes the members as the object is created, rather than first creating them with default values and then reassigning them. 
    This is equivalent to: Quaternion(double a, double b, double c, double d) {
    q1 = a;
    q2 = b;
    q3 = c;
    q4 = d;
}*/

//----------------------------------------------------------------------------------------------------//
    Quaternion operator+(const Quaternion& q) const{ //operator+ -> "overloaded" addition operator
        return Quaternion(q1 + q.q1, q2 + q.q2, q3 + q.q3, q4 + q.q4);
    }

    Quaternion operator*(const Quaternion& q) const{
        return Quaternion(q4*(q.q1) + q1*(q.q4) + q2*(q.q3) - q3*(q.q2),q4*(q.q2) + q2*(q.q4) - q1*(q.q3) + q3*(q.q1),q4*(q.q3) + q3*(q.q4) +q1*(q.q2)-q2*(q.q1), q4*(q.q4) - q1*(q.q1) - q2*(q.q2) - q3*(q.q3));
    }

    Quaternion operator*(const double& a) const{
        return Quaternion(q1*a, q2*a, q3*a, q4*a);
    }

    Quaternion operator/(const double& a) const{
        return Quaternion(q1/a, q2/a, q3/a, q4/a);
    }

    
};



inline Quaternion operator*(const Quaternion& a, const Quaternion& b){
    return Quaternion(a.q4*(b.q1) + a.q1*(b.q4) + a.q2*(b.q3) - a.q3*(b.q2),a.q4*(b.q2) + a.q2*(b.q4) - a.q1*(b.q3) + a.q3*(b.q1),a.q4*(b.q3) + a.q3*(b.q4) +a.q1*(b.q2)-a.q2*(b.q1), a.q4*(b.q4) - a.q1*(b.q1) - a.q2*(b.q2) - a.q3*(b.q3));
}

inline Quaternion operator*(const double& a, const Quaternion& q){
    return q*a;
}

inline Quaternion operator/(const double& a, const Quaternion& q){
    return q/a;
}
#endif