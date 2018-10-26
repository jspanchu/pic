#include "include/file_io.hpp"
#include <iostream>
#include <string>
FileIO::FileIO(std::string filename,std::string index)
{
	//Open file for writing and place header row.
	filename.append(index);
	this->file.open(filename, std::ios::trunc | std::ios::out);
}
FileIO::~FileIO()
{
	this->file.close();
}
void FileIO::fileWrite(const char *a, const char *b, const char *c)
{
	//write headers
	this->file << a << "," << b << "," << c << std::endl;
}
void FileIO::fileWrite(double f, double v, double x)
{
	//write data
	this->file << f << "," << v << "," << x << std::endl;
}
void FileIO::fileWrite(const char *a, const char *b)
{
	//write headers
	this->file << a << "," << b << std::endl;
}
void FileIO::fileWrite(double f, double v)
{
	//write data
	this->file << f << "," << v << std::endl;
}
