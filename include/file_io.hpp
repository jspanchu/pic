// This class takes care of any file I/O operations.

#include <fstream>

#ifndef FILE_IO
#define FILE_IO

class FileIO
{
private:
	std::fstream file;
public:
	FileIO();
	~FileIO();
	void fileWrite(double,double,double);
	void fileWrite(const char*,const char*,const char*);
};
#endif
