#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/SVD>
#include <eigen3/Eigen/LU>
#ifdef _SPARSE
#define EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET
#include <eigen3/Eigen/Sparse>
#include <redsvd/redsvd.hpp>
#endif

#include "iostream"

using namespace Eigen;

#define DBL_MAX (100000)
#define MINVAL (.00001)

//data :input data 
//d; distance matrix of data
//out: output (dimenstion compressed expression of data)
//n: neigher number
void lle_eigen(MatrixXf & dist,MatrixXf & data,MatrixXf & out,
	       int nnum){
#ifdef _SPARSE
SparseMatrix<float>  W(dist.rows(), dist.cols());
  W.setZero();
#else
  MatrixXf  W= MatrixXf::Zero(dist.rows(), dist.cols());
#endif

  VectorXf::Index m;
  VectorXf vd;
  for (int i = 0;i < data.rows();i++){
    // neigherhood matrix
    MatrixXf  nm(nnum,data.cols());

    vd=dist.row(i);
    vd(i) = DBL_MAX;//self=max

      for (int r = 0;r < nnum;++r){
	vd.minCoeff(&m);
	for (int c = 0;c < data.cols();c++)
	  nm(r, c) = data(m, c) - data(i, c);
	vd(m)=DBL_MAX;
      }

      //local covariance
      MatrixXf lc = nm*nm.transpose();
      if(nnum>data.cols())
	lc+=MINVAL*MatrixXf::Identity(lc.rows(),lc.cols());

      VectorXf v1=VectorXf::Constant(lc.rows(),1.0);
      FullPivLU<MatrixXf> lu(lc);
      VectorXf vw=lu.solve(v1);//lc*vw=v1

      double wsum = vw.sum();

      vd = dist.row(i);
      vd(i) = DBL_MAX;//self=max

      for (int r=0;r<vw.size();r++){
	vd.minCoeff(&m);
#ifdef _SPARSE
	W.insert(i, m) = vw(r)/wsum;
#else
	W(i, m) = vw(r)/wsum;
#endif
	vd(m) = DBL_MAX;
      }
  }
  //  std::cout << "W:" << W << std::endl;

#ifdef _SPARSE
  SparseMatrix<float>sm = -W;

  for (int r = 0;r < sm.rows();r++)
    sm.insert(r,r)= sm(r,r)+1;

  sm = sm.transpose()*sm;

  REDSVD::RedSVD redSVD(sm, out.cols()+1);

  // for (int r = 0;r < out.rows();r++)
  //   for (int c = 0;c < out.cols();c++)
  //     out(r, c) = evecs(r,c +1);

#else
  MatrixXf sm = -W;
  sm+= MatrixXf::Identity(sm.rows(),sm.cols());
  sm = sm.transpose()*sm;
  //  std::cout << "sm:" << sm << std::endl;

  SelfAdjointEigenSolver<MatrixXf> eigensolver(sm);
  MatrixXf evecs=eigensolver.eigenvectors();

  for (int r = 0;r < out.rows();r++)
    for (int c = 0;c < out.cols();c++)
      out(r, c) = evecs(r,c +1);
#endif

  return;
}
