
//#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include "Knn.h"
#include <algorithm>
#include <iterator>
#include <cmath>

Eigen::MatrixXd  Knn::distanse(Eigen::MatrixXd data,Eigen::MatrixXd object, Eigen::MatrixXd classes) {
	//Eigen::VectorXd dist(object.rows(),1 );
	//std::cout << classes;
	std::vector <double> vector_of_distance;
	std::vector <double> vector_of_classes;
	Eigen::MatrixXd mat_dist_class(k,2);
	for (int i = 0;i<data.cols();i++) {
		//std::cout << i;
		double dist = sqrt((data.col(i) - object).array().pow(2).sum());
		if (dist > max_dist)//Для парзеновского окна
			max_dist = dist;
		//std::cout << dist<<std::endl;
		if (vector_of_distance.empty() and dist!=0){
			vector_of_distance.push_back(dist);
			vector_of_classes.push_back(classes(i));
		}
		if (vector_of_distance.size() == 1 and dist != 0) {
			if (vector_of_distance[0] > dist) {
				vector_of_distance.insert(vector_of_distance.begin(), dist);
				vector_of_classes.insert(vector_of_classes.begin(), classes(i));
			}
			else {
				vector_of_distance.push_back(dist);
				vector_of_classes.push_back(classes(i));
			}
		}
		else if ( dist!=0){
			
			std::vector <double> ::iterator begin = vector_of_distance.begin();
			std::vector <double> ::iterator end = vector_of_distance.end()-1;
			std::vector <double> ::iterator middle_pos;


			std::vector<double>::iterator begin_cl = vector_of_classes.begin();
			std::vector<double>::iterator begin_cls = vector_of_distance.begin();


			if (*end < dist){
				vector_of_distance.push_back(dist);
				vector_of_classes.push_back(classes(i));
			}
			else if (*begin > dist){
				vector_of_distance.insert(begin, dist);
				vector_of_classes.insert(vector_of_classes.begin(),classes(i));
			}
			else{
			while (end - begin != 1){
				middle_pos = begin + (end - begin) / 2;
				if (*middle_pos > dist)
					end = middle_pos;
				else
					begin = middle_pos;

			}
			vector_of_distance.insert(end, dist); //Нихуя не понятно
			begin_cl += end - begin;
			vector_of_classes.insert(begin_cl , classes(i));
			}
			
			if(vector_of_distance.size()>k ){
				//std::cout << i;
				vector_of_distance.pop_back();
			vector_of_classes.pop_back();
			}

			}


		}



	//std::cout << vector_of_distance.size()<<std::endl;
	for (int i = 0; i < vector_of_distance.size(); i++){
		mat_dist_class(i,0) = vector_of_distance[i];
		mat_dist_class(i, 1) = vector_of_classes[i];
		//std::cout << std::fixed << 8;
		//std::cout << vector_of_classes[i] << std::endl;
	}
	//std::cout  << std::endl;
	return  mat_dist_class;
}
double Knn::Kernel_fun(double x) {
	
	x = x / max_dist;
	//std::cout << (x) << std::endl;
	//return exp(-2 * x*x); //Гаусово ядро
	//assert(x > 1); //
	//return abs(x) <= 1;//Прямоугольное превращается в набор весов равных единице
	//return (1 - abs(x)) * (abs(x) <= 1); // Треугольное
	//return (1 - x * x)  *(abs(x) <= 1); //Квадратичное
	return (1 - x * x) *(1 - x * x)  *(abs(x) <= 1); //Квартичное
}
Eigen::MatrixXd Knn::weight_inicialiser(Eigen::MatrixXd mat_dist_class) {
	int weights = 2; //тут заглушка, нужна перегрузка
	switch(weights){
	case SIMPLE:
		//std::cout << mat_dist_class.rows();
	for (int i = 0; i < mat_dist_class.rows();i++) {
		mat_dist_class(i, 0) = 1;
	}
	return mat_dist_class;
	case KERNEL:
		for (int i = 0; i < mat_dist_class.rows(); i++) {
			mat_dist_class(i, 0) = Kernel_fun(mat_dist_class(i, 0));
		}
		//std::cout << mat_dist_class<<std::endl<<std::endl;
	return mat_dist_class;
	//	return;
	//case KERNEL:
	//	return;
	}
	//return mat_dist_class;
}
int  Knn::Classificator(Eigen::MatrixXd weights, Eigen::MatrixXd classes, int count_classes) {
	//std::cout << k;
	double max_sum = 0;
	double sum = 0;
	int max_arg=0;
	
	for (int i = 0; i < count_classes; i++) {
		for (int j = 0; j < k; j++)
			sum += weights(j) * (classes(j) == i);
		if (sum > max_sum) {
			max_sum = sum;
			sum = 0;
			max_arg = i;
		}

	}
	//std::cout << max_arg << std::endl;
	return max_arg;

}

void Knn::LOF(Eigen::MatrixXd data, Eigen::MatrixXd classes, int count_classes) {

	int best_k = 10;
	int mistake_count=0;//хмхм
	int min_mist_count = data.cols();
	
	//std::cout << classes;
	for (int i = 5; i < 100; i++) {
		k = i;
		for (int j = 0; j < data.cols(); j++) {
			Eigen::MatrixXd dist = distanse(data, data.col(j), classes);
			Eigen::MatrixXd weight_matrix = weight_inicialiser(dist);
			//std::cout << weight_matrix << std::endl << std::endl;
			int arg = Classificator(weight_matrix.col(0), weight_matrix.col(1), count_classes);
			
			if (arg != classes(j)){
				//std::cout << classes(j) << "   " << arg << std::endl;
				
				mistake_count++;
			}
		}
		if (mistake_count < min_mist_count)
		{
			
				min_mist_count = mistake_count;
				
			best_k = i;
		}
		std::cout << k << "   " << (double) mistake_count/ data.cols()*100<<std::endl;
		mistake_count = 0;
	}
	k = best_k;
	std::cout << best_k<<std::endl<< min_mist_count;
}