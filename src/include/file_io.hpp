// This class takes care of any file I/O operations.

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#ifndef FILE_IO
#define FILE_IO

class FileIO
{
private:
	std::fstream file;
public:
	FileIO(std::string, std::string,int);
	~FileIO();
	void fileWrite(double,double,double);
	void fileWrite(const char*,const char*,const char*);
	void fileWrite(double,double);
	void fileWrite(const char*,const char*);
};
#endif
