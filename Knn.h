#include <Eigen/Dense>
#include <Eigen/Sparse>
#ifndef Knn_h
#define Knn_h
class Knn {
	 int k;
	 enum  { SIMPLE, SIM_FROM_POS, KERNEL };
	 double max_dist = 0;
	 //Eigen::MatrixXd data;
	//metric
public:
	
	Knn(): k(10) {
	}
	Knn(int k_): k(k_) {
	}
	double Kernel_fun(double);
	//double Mtx_Euclid(Eigen::MatrixXd, Eigen::MatrixXd);
	///double Mtx_Euclid(Eigen::MatrixXd, Eigen::MatrixXd);
	//double Mtx_Euclid(Eigen::MatrixXd, Eigen::MatrixXd);

	Eigen::MatrixXd distanse(Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd);
	Eigen::MatrixXd weight_inicialiser(Eigen::MatrixXd);
	int Classificator(Eigen::MatrixXd, Eigen::MatrixXd,int );
	void LOF(Eigen::MatrixXd, Eigen::MatrixXd, int);
};
#endif 