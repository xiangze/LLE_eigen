#include <eigen3/Eigen/Dense>
// #ifdef _MDS_WITH_REDSVD
// #include <redsvd/redsvd.hpp>
// #else
#include <eigen3/Eigen/SVD>
#include <eigen3/Eigen/LU>
// #endif
using namespace Eigen;

#define DBL_MAX (100000)
#define MINVAL (.00001)

//d; distance matrix
//out:
//n: neigher num
void lle_eigen(MatrixXf & dist,MatrixXf & data,MatrixXf & out,
	       int dim,int nnum){

  MatrixXf  W= MatrixXf::Zero(data.rows(), dist.cols());//square matrix
  VectorXf::Index m;
  VectorXf vd;
  for (int i = 0;i < data.rows();i++){
    // 隣接行列
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

      VectorXf v1(lc.rows(),1.0);
      FullPivLU<MatrixXf> lu(lc);
      VectorXf vw=lu.solve(v1);//lc*vw=v1

      double wsum = vw.sum();

      vd = dist.row(i);
      vd(i) = DBL_MAX;//self=max

      for (int r=0;r<vw.size();r++){
	vd.minCoeff(&m);
	W(i, m) = vw(r)/wsum;
	vd(m) = DBL_MAX;
      }
  }

  MatrixXf sm = -W;
  sm+= MatrixXf::Identity(sm.rows(),sm.cols());

  sm = sm.transpose()*sm;

  SelfAdjointEigenSolver<MatrixXf> eigensolver(sm);

  MatrixXf evecs=eigensolver.eigenvectors();
  for (int r = 0;r < out.rows();r++)
    for (int c = 0;c < out.cols();c++){
      //     out(r, c) = evecs(r,c);
     out(r, c) = evecs(r, evecs.cols() - c - 2);
    }

  //  EigenSolver<MatrixXf> eigensolver(sm);
  //sorted (maybe)
  //  MatrixXcf evecs=eigensolver.eigenvectors();
  //  for (int r = 0;r < out.rows();r++)
  //    for (int c = 0;c < out.cols();c++)
  //      out(r, c) = evecs(r, evecs.cols() - c - 2).real();

  return;
}
