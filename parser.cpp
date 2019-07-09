#include "parser.hpp"

// for some reason g++ hates me and i have to make 2 variables that have the same purpose (this and listsFolder in main.cpp). If you know how to fix this i'd love it if you tell me
std::string dir;

ini::ini (const std::string& fileName){
    dir = getenv("HOME");
    dir += "/.lists/"; // set up this variable because it is useful
    
    std::ifstream file(dir + fileName + ".list"); // open file
    ini::name = fileName; // set name variable
    if (file.is_open()){
        task tmp;

        for (std::string line; std::getline(file, line); ){
            if (line[0] == '['){
                if (tmp.name != "")
                    ini::tasks.push_back(tmp);
                
                tmp.name = line.substr(1,line.length() - 2);
                tmp.desc = "";
                tmp.state = 'u';
            }
            if (line.substr(0,5) == "state")
                tmp.state = line[line.length() - 1];

            if (line.substr(0,4) == "desc")
                tmp.desc = line.substr(6, line.length()-7);
        }

        //because the last one actually doesn't get pushed in
        ini::tasks.push_back(tmp);
    } 
    else{
        // error if file couldn't open
        std::cout << "ERROR: COULDN'T OPEN " << dir + fileName << "\n";
        exit(3);
    }
    file.close();
}

ini::~ini(){
    //for debug purposes
}

// operator to make a sorting function for outputFile(std::string)
bool operator<(const task& a, const task& b){
    if (a.state < b.state)
        return true;
    else if (a.state == b.state && a.name < b.name)
        return true;
    else {
        return false;
    }
}

void ini::outputFile(std::string fileName){
    //first sort everything from state and name
    std::sort(tasks.begin(), tasks.end());

    std::string fileDir = dir + fileName + ".list";

    //remove file
    std::remove(fileDir.c_str());

    std::ofstream file(fileDir);
    if (file.is_open()){
        //output to file
        for (std::vector<task>::iterator it = ini::tasks.begin(); it != ini::tasks.end(); it++){
            file << "[" << it -> name << "]\n";
            file << "state=" << it -> state << "\n";
            file << "desc=\"" << it -> desc << "\"";

            if (it != ini::tasks.end() - 1) // make it look pretier
                file << "\n\n";
        }

        file << "\0";
        file.close();
    }
    else{
        // error if new list couldn't be made
        std::cout << "ERROR: COULDN'T MAKE NEW FILE\n";
        exit(3);
    }
}

void ini::outputFile(){
    ini::outputFile(ini::name); // just a shortcut so that i don't have to write the name all the time
}

void ini::add(const std::string& name, const std::string& desc, char state){
    // check if task name already exists
    for (std::vector<task>::iterator it = ini::tasks.begin(); it != ini::tasks.end(); it++){
        if (it -> name == name){
            std::cout << "ERROR: TASK NAME ALREADY EXISTS\n";
            return;
        }
    }

    task tmp; // make temp task and set values

    tmp.name = name;
    tmp.desc = desc;
    tmp.state = state;

    ini::tasks.push_back(tmp); // push task into tasks
}

void ini::rem(const std::string& name){
    for (std::vector<task>::iterator it = ini::tasks.begin(); it != ini::tasks.end(); it++){
        if (it -> name == name){ // loop through list until we found the task that want to delete
            ini::tasks.erase(it); // delere and exit
            return;
        }
    }

    // error if task was not found
    std::cout << "ERROR: TASK NOT FOUND\n";
}