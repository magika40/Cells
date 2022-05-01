#include "MULT.h"
#include <Eigen/Sparse>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <random>
Multycells::Multycells(int argc, char* args[]) {

    for (int i = 1; i < argc; i += 2) { //Тут учесть неправильный ввод параметров командной строки
        ETL Cell(args[i], args[i + 1]); //Нужно еще учесть что конструктор есть трех переменных
        file_vec.push_back(Cell);


    }
    class_count = file_vec.size();
}

Eigen::MatrixXd Multycells::TrainTest() {



}
// Цель сделать клетки также хранилищыми их типов
Eigen::MatrixXd Multycells::UnitAllData() {
   
        std::vector<std::vector<std::string>> dataset;
        Eigen::MatrixXd mat; //Нужно учесть что размер признаков постоянен
        std::vector<std::vector<std::string>> cell_data;
        int Y = 0;
        for ( ETL cell : file_vec) {
            
            cell_data = cell.readCSV(std::to_string(Y));
            dataset.insert(dataset.cend(), cell_data.cbegin(), cell_data.cend());
            Y++;//Пока не придумал ничего лучше
            //std::size_t  rows = dataset.size();
            
        }
        //std::cout << 124;
        mat=CSVtoEigen(dataset);

        return mat;
    
}
std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd> Multycells::UnitAllData(double percente) {

    //std::vector<std::vector<std::string>> dataset;
    std::vector<std::vector<std::string>> train;
    std::vector<std::vector<std::string>> test;

	Eigen::MatrixXd mat_train; //Нужно учесть что размер признаков постоянен
    Eigen::MatrixXd mat_test;
	//std::vector<std::vector<std::string>> cell_data;
	int Y = 0;
	for (ETL cell : file_vec) {

		cell.readCSV(train,test, percente, std::to_string(Y));
		//dataset.insert(dataset.cend(), cell_data.cbegin(), cell_data.cend());
		Y++;//Пока не придумал ничего лучше
		//std::size_t  rows = dataset.size();

	}
	//std::cout << 124;
	//std::random_device rd;
	//std::mt19937 g(rd());
    //std::shuffle(train.begin(), train.end(), g);
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(train.begin(), train.end(), g);
	std::shuffle(test.begin(), test.end(), g);
    mat_train = CSVtoEigen(train);
    mat_test = CSVtoEigen(test);
	Eigen::MatrixXd X_train = mat_train.topRows(mat_train.rows() - 1);
	Eigen::MatrixXd Y_train = mat_train.bottomRows(1);

	
	Eigen::MatrixXd X_test = mat_test.topRows(mat_test.rows() - 1);
	Eigen::MatrixXd Y_test = mat_test.bottomRows(1);
	return std::make_tuple(X_train, Y_train, X_test, Y_test);
    
}