#ifndef PARSER
#define PARSER

//includes (yeah there are too many)
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <stdlib.h>
#include <iomanip>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>
#include <dirent.h>
#include <sys/stat.h> 
#include <sys/types.h>

//other stuffs
struct task{
    std::string name;
    std::string desc;
    char state;
};

class ini{
public:
    ini(const std::string&);
    ~ini();

    std::string name;
    std::vector<task> tasks;

    void outputFile(std::string);
    void outputFile();
    void add(const std::string&, const std::string&, char);
    void rem(const std::string&);
};

#endif
