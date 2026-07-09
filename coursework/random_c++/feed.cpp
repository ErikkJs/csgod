#include <iostream>
using namespace std;

float feed(float w1, float w2, float x1, float x2, float theta) {
	float big_x = ( (w1 * x1) + (w2 * x2) ) - theta;
	cout << "big_x is : " << big_x << "\t Charge\n";
	return big_x <= 0 ? -1 : 1;
}

int main() {
	//find the output
	//new value of w1 after learning
	//what is the charge(X)? 
	float w1,w2, x1, x2, theta, label;

	cout << "enter w1: ";
	cin >> w1;
	cout << "enter w2: ";
	cin >> w2;

	cout << "enter x1: ";
	cin >> x1;
	cout << "enter x2: ";
	cin >> x2;

	cout << "enter Theta: ";
	cin >> theta;

	cout << "enter label: ";
	cin >> label;
	cout << "\n\nValues have been entered\n";

	float output = feed(w1, w2, x1, x2, theta);
	cout << "the Output is: " << output << endl;


	cout << "Label - output_guess \n";
	cout << label << " - " << output << "\n";
	float e = label - output;
	cout << "e: " << e << endl;

	//alpha must be a small value	
	float alpha = 0.1;
	cout << "before modification\n";
	w1 = w1 + (alpha * x1 * e);
	cout << "this is the new w1: " << w1 << endl;
	
	return 0;
}
