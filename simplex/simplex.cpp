#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#define CURRMAX -1000000000
#define CURRMIN 1000000000

using namespace std;

class Matrix
{
private:
	double** matrix;
	int numstr;
	int numcol;	
	int* NotBasis;
public:

	Matrix(double** matrix, int rows, int cols)
	{
		this->numstr = rows;
		this->numcol = cols;
		this->matrix = new double* [rows];

		for (int i = 0; i < rows; ++i)
		{
			this->matrix[i] = new double[cols];
			for (int j = 0; j < cols; ++j)
			{
				this->matrix[i][j] = matrix[i][j];
			}
		}

		this->NotBasis = new int[cols-rows];
		
		for (int i = 0; i < cols - rows; ++i)
		{
			this->NotBasis[i] = -1;
		}
	}

	Matrix(const Matrix& other)
	{
		this->numstr = other.numstr;
		this->numcol = other.numcol;

		this->matrix = new double* [this->numstr];
		for (int i = 0;i < this->numstr; ++i)
		{
			this->matrix[i] = new double[this->numcol];
			for (int j = 0; j < this->numcol; ++i)
			{
				this->matrix[i][j] = other.matrix[i][j];
			}
		}

		this->NotBasis = new int[other.numcol - other.numstr];
		for (int i = 0; i < other.numcol - other.numstr; ++i)
		{
			this->NotBasis[i] = other.NotBasis[i];
		}

	}

	~Matrix()
	{
		for (int i = 0; i < this->numstr; ++i)
		{
			delete[] this->matrix[i];
		}
		delete[] this->matrix;
	}

	double GetValueByIndex(int i, int j);

	int* GetNotBasis();

	void PrintMatrix();

	int FindColMax();

	int FindColMin();

	int FindStr(int col);

	void Step(int str, int col, double leading);

	void PrintAnswer();

};

double Matrix::GetValueByIndex(int i, int j)
{
	return (this->matrix[i][j]);
}

int* Matrix::GetNotBasis()
{
	return (this->NotBasis);
}

void Matrix::PrintMatrix()
{
	for (int i = 0; i < this->numstr; ++i)
	{
		for (int j = 0; j < this->numcol; ++j)
		{
			cout << this->matrix[i][j] << " ";
		}
		cout << endl;
	}
}

int Matrix::FindColMax()
{
	int result = -1;
	double currMin = CURRMIN;
	for (int i = 0; i < this->numstr; ++i)
	{
		if (this->matrix[this->numstr - 1][i] < 0 && this->matrix[this->numstr - 1][i] < currMin)
		{
			result = i;
			currMin = this->matrix[this->numstr - 1][i];
		}
	}
	return result;
}

int Matrix::FindColMin()
{
	int result = -1;
	double currMax = CURRMAX;
	for (int i = 0; i < this->numstr-1; ++i)
	{
		if (this->matrix[this->numstr - 1][i] > 0 && this->matrix[this->numstr - 1][i] > currMax)
		{
			result = i;
			currMax = this->matrix[this->numstr - 1][i];
		}
	}
	return result;
}

int Matrix::FindStr(int col)
{
	int result = -1;
	if (col == -1)
	{
		return result;
	}
	double currMin = CURRMIN;
	for (int i = 0; i < this->numstr - 1; ++i)
	{
		if ((this->matrix[i][this->numcol-1] / this->matrix[i][col]) > 0 && (this->matrix[i][this->numcol - 1] / this->matrix[i][col]) < currMin)
		{
			result = i;
			currMin = this->matrix[i][this->numcol - 1] / this->matrix[i][col];
		}
	}
	if (result != -1)
	{
		for (int i = 0; i < this->numcol - numstr; ++i)
		{
			if ((result + 1) == this->NotBasis[i])
			{
				this->NotBasis[i] = -1;
				break;
			}
		}
		this->NotBasis[col] = result + 1;
	}
	return result;
}

void Matrix::Step(int str, int col, double leading)
{
	for (int i = 0; i < this->numcol; ++i)
	{
		this->matrix[str][i] /= leading;
	}

	for (int i = 0; i < this->numstr; ++i)
	{
		if (i != str)
		{
			double temp = this->matrix[i][col];
			for (int j = 0; j < this->numcol; ++j)
			{
				this->matrix[i][j] = this->matrix[i][j] - this->matrix[str][j] * temp;
			}
		}
	}
}

void Matrix::PrintAnswer()
{
	double func = 0.0;
	for (int i = 0; i < this->numcol - numstr; ++i)
	{
		if (this->NotBasis[i] != -1)
		{
			cout << "x_" << i + 1 << " = " << this->matrix[this->NotBasis[i] - 1][this->numcol-1] << endl;
			func += this->matrix[this->NotBasis[i] - 1][this->numcol - 1];
		}
		else
		{
			cout << "x_" << i + 1 << " = 0" << endl;
		}
	}

	cout << "F = " << func << endl;
}

int main()
{
	double** matrix;
	int rows;
	int cols;
	bool menu;

	cout << "Enter 0 for min\t\t1 for max: ";
	cin >> menu;

	cout << "\nEnter #rows in matrix: ";
	cin >> rows;
	cout << "Enter #col in matrix: ";
	cin >> cols;
	cout << endl;

	matrix = new double* [rows];

	for (int i = 0; i < rows; ++i)
	{
		matrix[i] = new double[cols];
	}

	ifstream inputFile("matrix.txt");

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			inputFile >> matrix[i][j];
		}
	}

	inputFile.close();

	Matrix a(matrix, rows, cols);
	a.PrintMatrix();

	cout << "======================" << endl;

	if (menu == 0)
	{
		int col = a.FindColMin();
		int str = a.FindStr(col);

		while (col != -1 && str != -1)
		{

			a.Step(str, col, a.GetValueByIndex(str, col));
			col = a.FindColMin();
			str = a.FindStr(col);

		}
	}
	else
	{
		int col = a.FindColMax();
		int str = a.FindStr(col);
		while (col != -1 && str != -1)
		{
			a.Step(str, col, a.GetValueByIndex(str, col));
			col = a.FindColMax();
			str = a.FindStr(col);
			 
		}
	}

	a.PrintAnswer();

	system("pause");

	return 0;
}