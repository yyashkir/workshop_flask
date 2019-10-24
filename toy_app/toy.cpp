
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
//
int range;
string chart_data;
string chart_file;
string report;
ofstream report_data;
//
void  do_toy(char *argv)
{
	int i;
	int a = 1, b = 1, f = 1;	
	string toy_string;
	ifstream infile(argv); //reading control parameters from file argv
	infile
		>> toy_string >> range
		>> toy_string >> chart_data
		>> toy_string >> chart_file
		>> toy_string >> report;
	infile.close();

	ifstream toy_stream(chart_data.c_str());
	stringstream buffer;
	buffer << toy_stream.rdbuf();
	toy_string = buffer.str();
	toy_stream.close();

	report_data.open(report);
	report_data << chart_data << endl << toy_string << endl << "Fibbonacci numbers:"<<endl;
	for (i = 1; i < range; i++)
	{
		f = a + b;
		a = b;
		b = f;
		report_data << f << " ";
	}
	report_data.close();

	string plot = "set terminal png;set output '" + chart_file + "';set grid;plot '" + chart_data + "' using 1:2  with lines; ";
	ofstream plot_command_file;
	plot_command_file.open("plotcommand");
	plot_command_file << plot;
	plot_command_file.close();
	int ret = system("gnuplot plotcommand");
}
//
int main(int argc, char **argv)
{	
	do_toy(argv[1]);	
	return 0;
}
