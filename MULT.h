#include <vector>
#include "ETL.h"
#include <string>
#include <iostream>
#ifndef MULT_h
#define MULT_h
class Multycells:public ETL {
    std::vector<ETL> file_vec;
    int class_count;
public:
    Multycells(int argc, char* args[]);
    Multycells() {

    }
    Eigen::MatrixXd UnitAllData();
    std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd> UnitAllData(double percente);
    Eigen::MatrixXd TrainTest();
    int ClassCount() {
        return class_count;
    }
    //Eigen::MatrixXd ShafleData();

};
#endif 