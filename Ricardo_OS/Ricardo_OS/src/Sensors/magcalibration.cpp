#include "magcalibration.h"

#include <utility>
#include <tuple>
#include <math.h>
#include <complex>


#include <iostream>


#include <Eigen/Core>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <Eigen/Sparse>
#include <Eigen/Eigenvalues>
#include <unsupported/Eigen/MatrixFunctions>




MagCalibration::MagCalibration(float fieldMagnitude, float inclination, float declination):
_fieldMagnitude(fieldMagnitude),
_inclination(inclination),
_declination(declination) 
{
    
}

MagCalibrationParameters MagCalibration::calibrate(std::vector<float>& mx,std::vector<float>& my,std::vector<float>& mz) 
{
    if (!(mx.size() == my.size() && my.size() == mz.size())){ //check vector sizes match
        return MagCalibrationParameters{{},{},static_cast<uint8_t>(STATUS::ERR_VECT_SIZE_MISMATCH)}; //error data vector sizes do not match
    }
    const size_t size = mx.size();

    //construct mag data matrix from input vectors
    Eigen::Matrix<float,3,Eigen::Dynamic> magData(3,size);
    magData << Eigen::Map<Eigen::VectorXf>(mx.data(),size).transpose(), // no copy
               Eigen::Map<Eigen::VectorXf>(my.data(),size).transpose(),
               Eigen::Map<Eigen::VectorXf>(mz.data(),size).transpose();

    //calculate best fit ellipsoid
    MagCalibration::ellipsoidParameters ellipsoid = ellipsoidFit(magData);

    //check ellipsoid for errors
    if (ellipsoid.error) {
        return MagCalibrationParameters{{},{},static_cast<uint8_t>(STATUS::ERR_EIG)};
    }

    MagCalibrationParameters ret;
    // inverse of M used multiple tiems
    Eigen::Matrix<float,3,3> M_1 = ellipsoid.M.inverse();
    //calculate b 
    ret.b = -(M_1 * ellipsoid.n);
    //caclualte denominator as a complex value to invoke complex squareroot
    std::complex<float> den = (ellipsoid.n.transpose() * M_1 * ellipsoid.n) - ellipsoid.d;
    //extract only real results from this calculation
    ret.A_1 = ((_fieldMagnitude/std::sqrt(den)) * ellipsoid.M.sqrt()).real();
    ret.error = false;

    return ret;  

}




MagCalibration::ellipsoidParameters MagCalibration::ellipsoidFit(Eigen::Matrix<float,3,Eigen::Dynamic>& magData) 
{
    
    Eigen::Matrix<float,10,Eigen::Dynamic> D(10,magData.cols());
    D << magData.row(0).array().pow(2),magData.row(1).array().pow(2),magData.row(2).array().pow(2),
        2*magData.row(1).cwiseProduct(magData.row(2)),2*magData.row(0).cwiseProduct(magData.row(2)),2*magData.row(0).cwiseProduct(magData.row(1)),
        2*magData.row(0),2*magData.row(1),2*magData.row(2),
        Eigen::MatrixXf::Ones(1,magData.cols());  
    
    //eq(11)
    Eigen::Matrix<float,10,10> S(D * D.transpose());
    Eigen::Matrix<float,6,6> S11(S.topLeftCorner<6,6>());
    Eigen::Matrix<float,6,4> S12(S.topRightCorner<6,4>()); // S_21 is simply this transposed
    Eigen::Matrix<float,4,4> S22(S.bottomRightCorner<4,4>());
    Eigen::Matrix<float,4,4> S22_inv = S22.inverse();

    //eq(8) taking k = 4
    Eigen::Matrix<float,6,6> C1{{-1,1,1,0,0,0},
                                 {1,-1,1,0,0,0},
                                 {1,1,-1,0,0,0},
                                 {0,0,0,-4,0,0},
                                 {0,0,0,0,-4,0},
                                 {0,0,0,0,0,-4}};

    //eq(15) evaluate lhs E*v1 = lambda*v1
    Eigen::Matrix<float,6,6> E = C1.inverse()*( S11 - (S12*( S22_inv )*( S12.transpose() ) ));
    //solve for eigenvalues and eignevectors
    Eigen::EigenSolver<Eigen::Matrix<float,6,6> > E_eigen(E,true);

    if (E_eigen.info() != Eigen::Success){
        return ellipsoidParameters{{},{},{},true}; // failed to solve, return error
    }
    
    //assume the eigen values and vectors are real??
    Eigen::VectorXf E_w = E_eigen.eigenvalues().real();

    //eigen vectors returned in format:
    // [v11.real,v11.complex],[v21.real,v21.complex]
    // [v12.real,v12.complex],[v22.real,v21.complex]
    Eigen::MatrixXf E_v = E_eigen.eigenvectors().real();

    //search for most positive eigenvalue as detailed in paper -> i asumme this is the most positive real eigen value
    int maxIdx = 0;
    float Max_E_w = E_w[maxIdx];

    for (int i = 1; i < E_w.size();i++){
        if (E_w[i] > Max_E_w){
            Max_E_w = E_w[i];
            maxIdx = i;
        }
    }

    Eigen::Vector<float,6> v1 = E_v.col(maxIdx);


    //this was in the python implementation but idk why
    if (v1[0] < 0){
        v1 = -v1;
    }
    
    //v_2 eq(13)
    Eigen::Vector<float,4> v2 = -(S22_inv * S12.transpose() * v1);

    std::cout<<"v"<<"\n";
    std::cout<<v1<<"\n";
    std::cout<<v2<<"\n";

    MagCalibration::ellipsoidParameters ret;
    ret.M << v1[0],v1[3],v1[4],
             v1[3],v1[1],v1[5],
             v1[4],v1[5],v1[2];
    
    ret.n << v2[0],
             v2[1],
             v2[2];
    
    ret.d = v2[3];
    ret.error = false;
    return ret;

}


