#include "include/file_io.hpp"
#include <iostream>
FileIO::FileIO()
{
	//Open file for writing and place header row.
	file.open("./fvx.out", std::ios::trunc | std::ios::out);
}
FileIO::~FileIO()
{
	file.close();
}
void FileIO::fileWrite(const char *a, const char *b, const char *c)
{
	//write headers
	file << a << "," << b << "," << c << std::endl;
}
void FileIO::fileWrite(double f, double v, double x)
{
	//write data
	file << f << "," << v << "," << x << std::endl;
}
