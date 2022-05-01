#include "ETL.h"

#include <vector>
#include <stdlib.h>
#include <cmath>
#include <boost/algorithm/string.hpp>
#include <random>
#include <algorithm>
#include <iterator>

#include <tuple>
std::vector<std::vector<std::string>> ETL::readCSV() {

	std::ifstream file(dataset);
	if (file.is_open())
		std::cout << "it is ok" << std::endl;
	else {
		std::cout << "something went wrong";
		exit(-1);
	}
	std::vector<std::vector<std::string>> dataString;
	bool header_flag = false;
	if (header)
		header_flag = header;

	std::string line = "";
	while (getline(file, line)) {
		if (header_flag) {
			header_flag = false;
			//std::cout << line;
			continue;

		}
		//std::cout << line;

		std::vector<std::string> vec;

		boost::algorithm::split(vec, line, boost::is_any_of(delimiter));
		
		//std::cout << vec.size();

		dataString.push_back(vec);
	}

	//std::cout << std::endl << std::endl << dataString.size() << std::endl;
	file.close();

	return dataString;
}
std::vector<std::vector<std::string>> ETL::readCSV(std::string  Y="") {

    std::ifstream file(dataset);
    if (!file.is_open())
	{
		std::cout << "something went wrong";
		exit(-1);
	}
    std::vector<std::vector<std::string>> dataString;
    bool header_flag = false;
    if (header)
        header_flag = header;

    std::string line = "";
    while (getline(file, line)) {
        if (header_flag) {
            header_flag = false;
            //std::cout << line;
            continue;
            
        }
        std::vector<std::string> vec;
        boost::algorithm::split(vec, line, boost::is_any_of(delimiter));// как
        if (Y!="")
            vec.push_back(Y);
        dataString.push_back (vec);
    }
    file.close();

    return dataString;
}


void ETL::readCSV(std::vector<std::vector<std::string>>&train, std::vector<std::vector<std::string>>&test,double percent=0.8, std::string  Y = "") {

	std::ifstream file(dataset);
	if (!file.is_open())
	{
		std::cout << "something went wrong";
		exit(-1);
	}
	std::vector<std::vector<std::string>> dataString;
	bool header_flag = false;
	if (header)
		header_flag = header;

	std::string line = "";
	while (getline(file, line)) {
		if (header_flag) {
			header_flag = false;
			//std::cout << line;
			continue;

		}
		std::vector<std::string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delimiter));// как
		if (Y != "")
			vec.push_back(Y);
		dataString.push_back(vec);
	}
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(dataString.begin(), dataString.end(), g);
	train.insert(train.begin(), dataString.begin(), dataString.begin() + int(percent * dataString.size()));
	test.insert(test.begin(), dataString.begin() + int(percent * dataString.size()), dataString.end());// надо проверить
	 // эта конструкция нужна для перетасовки, осознал следующую проблемму возможна ли ситуация когда в тесте содержится вся выборка для одной влетки
	
	file.close();

	//return dataString;
}


Eigen::MatrixXd ETL::CSVtoEigen(std::vector<std::vector<std::string>> dataset) {
    /* Возможно придется добавить возможность на каком объекте есть ошибка*/
    std::size_t rows = dataset.size(); //Пришлось изменить тип с int на std::size_t
    std::size_t cols = dataset[0].size();

	std::random_device rd; 
	std::mt19937 g(rd()); // эта конструкция нужна для перетасовки, осознал следующую проблемму возможна ли ситуация когда в тесте содержится вся выборка для одной влетки
    std::shuffle(dataset.begin(), dataset.end(), g);
    Eigen::MatrixXd mat( rows,cols - 1 ); //Нужно учесть будет ли заголовок или нет .учел

    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols - 1; j++) {
            std::size_t common_pos = dataset[i][j + 1].find_first_of(","); // а вдруг точка,?
                
            if (common_pos !=std::string::npos){
                dataset[i][j + 1].replace(common_pos,1,"."); //Длина числа может быть другой, возможна потеря битов
            }

            mat(i,j) = atof(dataset[i][j+1].c_str());

        }
    }
    return mat.transpose();
}

std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd> ETL::TrainTestSplit(Eigen::MatrixXd data, float train_size) {
	size_t cols = data.cols();
    size_t train_cols = round(train_size * cols);
    size_t test_cols = cols - train_cols;
    //std::cout << train_cols<<std::endl;
    //std::cout << test_cols << std::endl;
	Eigen::MatrixXd train = data.leftCols(train_cols);

	Eigen::MatrixXd X_train = train.topRows(data.rows() - 1);
	Eigen::MatrixXd y_train = train.bottomRows(1);

	Eigen::MatrixXd test = data.rightCols(test_cols);

	Eigen::MatrixXd X_test = test.topRows(data.rows() - 1);
	Eigen::MatrixXd y_test = test.bottomRows(1);

	return std::make_tuple(X_train, y_train, X_test, y_test);
}

auto ETL::Mean(Eigen::MatrixXd data) -> decltype(data.colwise().mean()) {
    return data.colwise().mean(); //Нормировка данных необходима, чтобы перейти к общим единицаи измерения
}

auto ETL::Std(Eigen::MatrixXd data) -> decltype(((data.array().square().colwise().sum()) / (data.rows() - 1)).sqrt()) {
    return ((data.array().square().colwise().sum()) / (data.rows() - 1)).sqrt();
}

Eigen::MatrixXd ETL::Normalize(Eigen::MatrixXd data, bool normalizeTarget) {

    Eigen::MatrixXd dataNorm;
    if (normalizeTarget == true) {
        dataNorm = data;
    }
    else {
        dataNorm = data.leftCols(data.cols() - 1);
    }

    auto mean = Mean(dataNorm);
    Eigen::MatrixXd scaled_data = dataNorm.rowwise() - mean;
    auto std = Std(scaled_data);

    Eigen::MatrixXd norm = scaled_data.array().rowwise() / std;

    if (normalizeTarget == false) {
        norm.conservativeResize(norm.rows(), norm.cols() + 1);
        norm.col(norm.cols() - 1) = data.rightCols(1);
    }

    return norm;
}

void ETL::Vectortofile(std::vector<float> vector, std::string filename) {
    std::ofstream output_file(filename);
    std::ostream_iterator<float> output_iterator(output_file, "\n");
    std::copy(vector.begin(), vector.end(), output_iterator);
}

void ETL::EigentoFile(Eigen::MatrixXd data, std::string filename) {
    std::ofstream output_file(filename);
    if (output_file.is_open()) {
        output_file << data << "\n";
    }
}