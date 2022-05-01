#include "ETL.h"
#include "Knn.h"
#include <iostream>
#include <string>
#include <Eigen/Dense>
#include <boost/algorithm/string.hpp>
#include <vector>
#include "MULT.h"
#include <algorithm>


#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <tuple>


using std::cout; using std::endl;
using std::vector; using std::tuple;
using std::string;	
int main(int argc, char* argv[]) {


	Multycells g = Multycells(argc, argv);
	//auto data = ;
	//cout << data.leftCols(1)<<endl<<endl;
	//cout << data.rightCols(1);
	auto tup = g.UnitAllData(0.8);
//cout << std::get<0>(tup).cols()<< endl ;
	//cout << std::get<2>(tup).cols()  <<endl;
	//cout << std::get<0>(tup)<<endl<<endl;
	//cout << std::get<1>(tup) << endl << endl;

	//cout << std::get<2>(tup).col(0) << endl << endl;
	//cout << std::get<3>(tup) << endl << endl;
	Knn aaa;
	//cout ;
	auto dist = aaa.distanse(std::get<0>(tup), std::get<2>(tup).col(0),std::get<1>(tup));//—ху€ли при выводе векторов мы получчаем 0
	auto info = aaa.weight_inicialiser(dist);
	cout << info << endl << endl;
	// 
	
	//Eigen::VectorXd dfff= std::get<1>(tup); // –азобратьс€ когда вектора €вл€ютс€ столбцами а когдла р€дамиж
		//cout << dfff;
	aaa.LOF(std::get<0>(tup), std::get<1>(tup),g.ClassCount());
	return 0;
}


