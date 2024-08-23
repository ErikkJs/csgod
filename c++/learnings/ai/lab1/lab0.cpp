#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
 
int main(int argc, char *argv[]) {
	/* 
	* Code to load a CSV into a 2-D vector. No need to modify
	* the following code.
	*/
	std::vector< std::vector<double> > data;	// Vector containing data and labels
	std::ifstream file( argv[1] );			// File to load data
	int tp = 0;					// True positives
	int p  = 0;					// Number of positives
	int tN=0;					// true negative
	int n =0;					// negative
	int fp = 0;					//false positive
	/* 
	* Part 0: Code to load in from a CSV, no need to edit this
	*/
	std::string line;
	while(std::getline(file, line)) {
	    std::vector<double> row;
	    std::stringstream iss(line);

	    std::string val;
	    while (std::getline(iss, val, ',')) {
		try {
		    double temp = std::stod(val);
		    row.push_back(temp);
		}
		catch (...) {
		    std::cout << "Bad input: " << val << std::endl;
		}
	    }
	    data.push_back(row);
	}

	/* 
	* Part 1: Reflex agent
	*/
	// Iterate over the array and make a decision based on some value.
	for (const auto &row : data) {
		// Each row represents a data sample.
		// The columns represent a specific feature.
		// For the Iris dataset:
		//	0 - Sepal length
		//	1 - Sepal width
		//	2 - Petal length
		//	3 - Petal width
		// The final column (4) is the label
		double prediction = -1;
		double label = row.back();
		std::cout << "Data values: ";
		for (const auto &col : row )
			std::cout << col << '\t';
		std::cout << std::endl;

		std::cout << "Current sample is labeled " << label;

		/* 
		* Part 1.1: Reflex rule
		*/
		// If the sepal width (column 2) is geq to 3.0, predict 1 (versicolor)
		std::cout << " and reflex agent predicts ";
		if (row[3] >= 1.0) 
			prediction = 1;
		std::cout << prediction << "." << std::endl;
		/* 
		* Part 1.2: Reflex rule
		*/
		// Increment the number of true positives
		if ( label == -1 && prediction == -1 ){
			tN++; }


		// Count the number of positives
		if ( label == -1 ){
			p++;
		}else 
		    n++;

		// false positives 
		if ( label == 1 && prediction == -1 ){
		fp++;
		}

		// true negative 
		if ( label == 1 && prediction == 1)
		{
		    tp++;
		}
		



	}

	/* 
	* Part 2: Performance metrics
	*/
	std::cout << "True positives: " << tp << std::endl;
	std::cout << "Num. positives: " << p << std::endl;
	std::cout << "True positive rate: " << (double) tp / p * 100 << '%' <<  std::endl;
	std::cout << "Sensitivity: " << ((double)tp/p)* 100<<"%"<< std:: endl;
	std::cout << "SPecificity: " << ((double)tN/n)* 100<< "%" << std::endl;
	std:: cout << "Metric precision: " << (double)tp/(tp+fp)* 100 << "%"  << std::endl;
	std:: cout << "Accuracy: " <<((double)tp+tN)/((double)(data.size()))* 100 << "%"  << std::endl;
}
