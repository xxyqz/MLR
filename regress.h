#pragma once
#include"C:/std_lib_facilities.h"

struct Matrix {
	int m, n;
	int yn;
	int fold;
	char comma = ',';
	string xname;
	string yname;;
	vector<vector<double>>x;
	vector<vector<double>>y;
	vector<vector<double>>ycal;
	vector<vector<double>>b;
	void read_data();
	void print(vector<vector<double>>&);
	vector<vector<double>> transpose(vector<vector<double>>&);
	vector<vector<double>> times(vector<vector<double>>&, vector<vector<double>>&);
	vector<vector<double>> inverse(vector<vector<double>>&);
	double determinant(vector<vector<double>>);
	vector<vector<double>> regress(vector<vector<double>>&, vector<vector<double>>&);
	void cvleaveoneout(vector<vector<double>>&, vector<vector<double>>&);
	void cvkfold(vector<vector<double>>&, vector<vector<double>>&,int&);
	double value(vector<vector<double>>&, vector<vector<double>>&);
	void r2(vector<vector<double>>&, vector<vector<double>>&);
};

Matrix matrix;

void Matrix::read_data()
{
	cout << "Please enter input x file name: ";
	cin >> xname;
	ifstream xist{ xname };
	while (xist)
	{
		string cell;
		if (!getline(xist, cell)) break;
		istringstream line(cell);
		vector<double>v1;
		while (line) {
			string cell;
			if (!getline(line, cell, ',')) break;
			double temp = strtod(cell.c_str(), NULL);
			v1.push_back(temp);
		}
		x.push_back(v1);
	}
	m = x.size();
	n = x[0].size();
	print(x);

	cout << "Please enter input y file name: ";
	cin >> yname;
	ifstream yist{ yname };
	while (yist)
	{
		string cell;
		if (!getline(yist, cell)) break;
		istringstream line(cell);
		vector<double>v1;
		while (line) {
			string cell;
			if (!getline(line, cell, ',')) break;
			double temp = strtod(cell.c_str(), NULL);
			v1.push_back(temp);
		}
		y.push_back(v1);
	}
	yn = y[0].size();
	print(y);
}
void Matrix::print(vector<vector<double>>&a) {
	for (int i = 0; i < a.size(); i++) {
		for (int j = 0; j <a[0].size(); j++)
		{
			cout << a[i][j] << "\t";
		}
		cout << endl;
	}
}
double Matrix::value(vector<vector<double>>&y, vector<vector<double>>&ycal) {
	double sum1 = 0;
	double sum2 = 0;
	double sum = 0;
	for (int i = 0; i < y.size(); i++) {
		for (int j = 0; j < y[0].size(); j++) {
			sum += y[i][j];
			sum2 += pow((y[i][j] - ycal[i][j]), 2);
		}
	}
	double average = sum / y.size();
	for (int i = 0; i < y.size(); i++) {
		for (int j = 0; j < y[0].size(); j++) {
			sum1 += pow((y[i][j] - average), 2);
		}
	}
	return  1 - sqrt(sum2) / sqrt(sum1);
}
vector<vector<double>> Matrix::transpose(vector<vector<double>>& a) {
	vector<vector<double>>at;
	for (int i = 0; i < a[0].size(); i++){
		vector<double>row;
		for (int j = 0; j < a.size(); j++){
			double temp= a[j][i];
			row.push_back(temp);
		}
		at.push_back(row);
	}
	return at;
}
vector<vector<double>> Matrix::times(vector<vector<double>>& a, vector<vector<double>>& b) {
	vector<vector<double>>ab;
	for (int i = 0; i < a.size(); i++){
		vector<double>row;
		for (int j = 0; j < b[0].size(); j++){
			double sum =0;
			for (int k = 0; k < b.size(); k++){
				sum += a[i][k] * b[k][j];
			}
			row.push_back(sum);
		}
		ab.push_back(row);
	}
	return ab;
}
vector<vector<double>> Matrix::inverse(vector<vector<double>>& a) {
	vector<vector<double>> inverse;
	double deta=determinant(a);
	if (deta == 0){
		cout << "Singular Matrix!!!" << endl;
	}
	for (int i = 0; i < a.size(); i++){
		vector<double>row;
		for (int j = 0; j < a.size(); j++){
			vector<vector<double>> cofactor = a;
			cofactor.erase(cofactor.begin() + i);
			for (int r = 0; r < a.size() - 1; r++){
				cofactor[r].erase(cofactor[r].begin() + j);
			}
					double temp =pow(-1,(i+j+2))* determinant(cofactor)/deta;
					row.push_back(temp);
		}
		inverse.push_back(row);
	}
	return inverse;
}
double Matrix::determinant(vector<vector<double>> a) {
	double determinant = 1;
	int k = 0;
	while (k < a.size() - 1)
	{
		for (int i = k; i < a.size(); i++)
		{
			if (a[k][k] != 0) {
				double c = a[i][k] / a[k][k];
				if (i != k) {
					for (int j = 0; j < a[0].size(); j++) {
						a[i][j] = a[i][j] - a[k][j] * c;
					}
				}
				else {

				}
			}
			else {
				if (i != k) {
					for (int j = 0; j < a[0].size(); j++) {
						a[i][j] = a[i][j] - a[i][k];
					}
				}
				else {

				}
			}
		}
		k++;
	}
	for (int i = 0; i < a.size(); i++) {
		determinant *= a[i][i];
	}

	return determinant;
}
vector<vector<double>> Matrix::regress(vector<vector<double>>&x, vector<vector<double>>&y)
{
	vector<vector<double>>xt;
	vector<vector<double>>xtx;
	vector<vector<double>>invxtx;
	vector<vector<double>>xty;
	vector<vector<double>>b;
	vector<vector<double>>ycal;
	xt = matrix.transpose(x);
	xtx = matrix.times(xt, x);
	invxtx = matrix.inverse(xtx);
	xty = matrix.times(xt, y);
	b = matrix.times(invxtx, xty);
	ycal = matrix.times(x, b);

	return b;
}
void Matrix::cvleaveoneout(vector<vector<double>>&x, vector<vector<double>>&y) {
	vector<vector<double>>ypred;
	for (int i = 0; i < x.size(); i++) {
		vector<vector<double>>xtrain = x;
		vector<vector<double>>ytrain = y;
		vector<vector<double>>xtest;
		xtrain.erase(xtrain.begin() + i);
		ytrain.erase(ytrain.begin() + i);
		vector<double>xtestrow;
		for (int xj = 0; xj < x[0].size(); xj++) {
			double temp = x[i][xj];
			xtestrow.push_back(temp);
		}
		xtest.push_back(xtestrow);
		vector<vector<double>>cvb = matrix.regress(xtrain, ytrain);
		vector<vector<double>>ytestcal2d = matrix.times(xtest, cvb);
		vector<double>ytestcal1d;
		ytestcal1d.push_back(ytestcal2d[0][0]);
		ypred.push_back(ytestcal1d);
	}
	double q2 = value(y, ypred);
	cout << "leave one out q2 is:" << q2 << endl;
}
void Matrix::cvkfold(vector<vector<double>>&x, vector<vector<double>>&y,int& fold) {
	vector<vector<double>>ypred;
	int line = y.size() / fold;
	for (int k = 0; k < fold; k++){
		vector<vector<double>>xtrain = x;
		vector<vector<double>>ytrain = y;
		vector<vector<double>>xtest;
		int up = k*line;
		int down = (k+1)*line;
		xtrain.erase(xtrain.begin() + up, xtrain.begin() + down);
		ytrain.erase(ytrain.begin() + up, ytrain.begin() + down);
		for (int xi = up; xi < down; xi++) {
			vector<double>xtestrow;
			for (int xj = 0; xj < x[0].size(); xj++) {
				double temp = x[xi][xj];
				xtestrow.push_back(temp);
			}	
			xtest.push_back(xtestrow);
		}
		vector<vector<double>>cvb = matrix.regress(xtrain, ytrain);
		vector<vector<double>>ytestcal2d = matrix.times(xtest, cvb);
		for (int ycali = 0; ycali < ytestcal2d.size(); ycali++) {
			vector<double>ytestcal1d;
			ytestcal1d.push_back(ytestcal2d[ycali][0]);
			ypred.push_back(ytestcal1d);
		}
}
	double q2 = value(y, ypred);
	cout <<fold <<" fold q2 is:" << q2 << endl;
}
void Matrix::r2(vector<vector<double>>&x, vector<vector<double>>&y) {
	matrix.b = matrix.regress(matrix.x, matrix.y);
	matrix.ycal = matrix.times(matrix.x, matrix.b);
	cout << "b is:" << endl;
	matrix.print(matrix.b);
	double r2 = matrix.value(matrix.y, matrix.ycal);
	cout << "r2 is:" << r2 << endl;
}