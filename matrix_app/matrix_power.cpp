//	Copyright 2019 Yashkir Consulting
#include <armadillo>
using namespace std;
//
void tab2space(string file)
{
	ifstream in(file);
	ofstream buffer;
	string buf = "";
	char c;
	while (in.get(c)) {
		if (c == '\t')
			buf = buf + " ";
		else
			buf = buf + c;
	}
	in.close();
	ofstream out(file);
	out << buf;
	out.close();
}
//
vector<string> split_string(string a_string, string delimiter)
{
	vector<string> a;
	size_t pos = 0;
	std::string token;
	while ((pos = a_string.find(delimiter)) != std::string::npos) {
		token = a_string.substr(0, pos);
		if (token.size() != 0)
			a.push_back(token);
		a_string.erase(0, a_string.find(delimiter) + delimiter.length());
	}
	if (a_string.size() != 0)
		a.push_back(a_string);
	return a;
}
//
arma::vec split_string_to_doubles(string s, string delimiter)
{
	int i, n;
	char *end;
	vector<string> a = split_string(s, delimiter);
	n = int(a.size());
	arma::vec doubles(n);
	for (i = 0; i < n; i++)
		doubles(i) = std::strtod(a[i].c_str(), &end);
	return doubles;
}
//
arma::Mat<double> powm(arma::Mat<double> D, int rank, int k)
{
	//	returns power of a matrix D as T = D^k (k is integer > 0)
	int p;
	arma::Mat<double> T(rank, rank);
	T = D;
	for (p = 1; p < k; p++)
	{
		T = T * D;
	}
	return T;
}
//
arma::Mat<double> normalize_matrix(arma::Mat<double> matr, int rank)
{
	//	rescaling each row to have sum of this row equal to 1
	int j, i;
	double row_sum;
	for (j = 0; j < rank; j++)
	{
		for (i = 0; i < rank; i++)
			if(matr(j, i) < 0)
				matr(j, i) = 0;
		row_sum = 0;
		for (i = 0; i < rank; i++) 
			row_sum = row_sum + matr(j, i);
		for (i = 0; i < rank; i++)
			matr(j, i) = matr(j, i) / row_sum;
	}
	return matr;
}
//
arma::Mat<double> generator(arma::Mat<double> matrix, int rank, int expansion_number)
{/*
	matrix Q (generator) is calculated as log of the credit rating transition matrix,
	by Taylor expansion series
*/
	int  j, k, s;
	arma::Mat<double> Q(rank, rank);
	arma::Mat<double> R(rank, rank);
	arma::Mat <double> D(rank, rank);
	D = matrix;
	for (j = 0; j < rank; j++) D(j, j) = D(j, j) - 1.;
	Q = D;

	for (k = 2; k <= expansion_number; k++)
	{
		s = int(pow(-1, k + 1));
		R = powm(D, rank, k);
		Q = Q + R * s / k;
	}
	return Q;
}
//
arma::Mat<double> mpower(arma::Mat<double> Q, int rank, double q, int expansion_number)
{
	//	Returns the matrix B calculated as exp(q * Q) which is matrix^q (by Taylor expansion)
	int  j, k;
	double kfact, v, qk;
	arma::Mat<double> B(rank, rank);
	arma::Mat<double> R(rank, rank);

	B.fill(0);
	for (j = 0; j < rank; j++) B(j, j) = 1.;

	kfact = 1.;
	qk = 1;
	for (k = 1; k <= expansion_number; k++)
	{
		kfact = kfact * k;
		qk = qk * q;
		v = qk / kfact;
		R = powm(Q, rank, k);
		B = B + v * R;
	}
	return B;
}
//
arma::Mat<double> matrix_to_power(arma::Mat<double> matrix, int rank, double q, int expansion_number)
{
	//	raising matrix to arbitrary power
	double dm;
	arma::Mat<double> B;
	arma::Mat<double> Q;
	arma::Mat<double> M;
	int m;
	// splitting q into an integer "m" and a fraction (dm < 1)
	m = int(q);
	dm = q - m;
	if (q >= 1 && dm == 0)		//when the power is an integer:  1,2,3,4,...
	{
		B = powm(matrix, rank, m);
		B = normalize_matrix(B, rank);
		return B;
	}
	else if (q < 1)				// power is less than 1  (0.5  or 0.25 or anything < 1 , but > 0)
	{
		Q = generator(matrix, rank, expansion_number); // Q= log matrix  using "Tailor series"
		B = mpower(Q, rank, q, expansion_number);	   // B=matrix^q as B=exp(q Q) using "Tailor series"	
		B = normalize_matrix(B, rank);
		return B;
	}
	else if (q > 1)				// power value = integer m  + fraction dm
	{
		M = powm(matrix, rank, m);						// matrix^m by multiplication
		M = normalize_matrix(M, rank);
		Q = generator(matrix, rank, expansion_number);	// Q= log (matrix)  using "Tailor series"
		B = mpower(Q, rank, dm, expansion_number);		// matrix^dm = exp(Q) using "Tailor series"
		B = normalize_matrix(B, rank);
		B = M % B;						// matrix^q = (matrix^m) x (matrix^dm)
		B = normalize_matrix(B, rank);
		return(B);
	}
	else {
		cout << "Error: matrix power < 0";
		return matrix;
	}
}
//
void matrix_to_file(vector<string> header, arma::dmat matrix, string file)
{
	int i, j;
	int rank = (int)header.size();
	ofstream body(file);
	for (i = 0; i < rank; i++)
		body << header[i] << " ";
	body << endl;
	for (j = 0; j < rank; j++)
	{
		for (i = 0; i < rank; i++)
			body << matrix(j, i) << " ";
		body << endl;
	}
	body.close();
}

int main(int argc, char **argv)  // argv: must have two numbers: power(double) and expansion(integer)
{
	int j, i;
	int rank;
	string line;
	string delimiter = " ";
	double power = stof(argv[1]);
	int expansion_number = stoi(argv[2]);
	
	string original_matrix_file = argv[3];
	tab2space(original_matrix_file);

	string matrix_in_power_file = argv[4];
	vector<string> ratings;
	
	ifstream mat_file(original_matrix_file);
	getline(mat_file, line);
									
	ratings = split_string(line, delimiter);
	rank = (int)ratings.size();

	arma::dmat matrix(rank,rank);
	arma::dmat power_of_matrix(rank, rank);

	arma::vec matrix_row;
	for (j = 0; j < rank; j++)
	{
		getline(mat_file, line);
		matrix_row = split_string_to_doubles(line, delimiter);
		for (i = 0; i < rank; i++)
			matrix(j, i) = matrix_row(i);
	}
	mat_file.close();

	power_of_matrix = matrix_to_power(matrix,rank, power, expansion_number);

	matrix_to_file(ratings, power_of_matrix, matrix_in_power_file);

	matrix.print("\n Original matrix");
	power_of_matrix.print("\n Matrix^" + to_string(power));

	return 0;
}
//	Copyright 2019 Yashkir Consulting
