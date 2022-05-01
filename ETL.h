#ifndef ETL_h
#define ETL_h

#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <vector>

class ETL
{
    std::string dataset;
    std::string delimiter;
    bool header = true;
public:
    ETL()
    {}
    ETL(std::string data, std::string separator) : dataset(data), delimiter(separator)
    {}
	ETL(std::string data, std::string separator,bool head) : dataset(data), delimiter(separator),header(head)

	{}
    std::vector<std::vector<std::string>> readCSV();
    std::vector<std::vector<std::string>> readCSV(std::string  Y);
    void readCSV(std::vector<std::vector<std::string>>&, std::vector<std::vector<std::string>>&, double,std::string );





    Eigen::MatrixXd CSVtoEigen(std::vector<std::vector<std::string>> dataset);
    Eigen::MatrixXd Normalize(Eigen::MatrixXd data, bool normalizeTarget);
    auto Mean(Eigen::MatrixXd data) -> decltype(data.colwise().mean()); //еще не понимаю
    auto Std(Eigen::MatrixXd data) -> decltype(((data.array().square().colwise().sum()) / (data.rows() - 1)).sqrt());
    std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd> TrainTestSplit(Eigen::MatrixXd data, float train_size);
    
    void Vectortofile(std::vector<float> vector, std::string filename);
    void EigentoFile(Eigen::MatrixXd data, std::string filename);
};

#endif