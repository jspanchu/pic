#include "include/file_io.hpp"
FileIO::FileIO(std::string parameter,std::string index,int digits)
{
	std::stringstream filename;
	//Open file for writing and place header row.
	filename << "./output/" << parameter << "." << std::setw(digits) 
	 << std::setfill<char>('0') << index << ".csv";
	this->file.open(filename.str(), std::ios::trunc | std::ios::out);
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
