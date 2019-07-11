/*
*   PLEASE NOTE THAT THIS CODE LOOKS UGLIER THAN ME RIGHT NOW.
*   I'LL BE SLOWLY FIXING IT TO LOOK AND FUNCTION BETTER.
*/

#include "parser.hpp"

std::string listsFolder;

// outputs how many digits a number has
int getDig(int n){
    int ret = 0;

    while (n != 0){
        n /= 10;
        ++ret;
    }

    return ret;
}

// truncates a string (specifically description)
std::string trunc(std::string input){
    long unsigned int trunc = 30;

    if (input.length() > trunc){
        return input.substr(0,trunc - 3) + "...";
    }
    else{
        return input;
    }
}

// standard error message
void error(){
    std::cout << "Please type \"todo help\" to see how todo is used\n";
    exit(1);
}

// gets info which is seperated by keywords, clauseCount is used to know how many strings the array needs to return
std::string* getInfo(int argc, char** argv, int clauseCount, int startingIterator){ 
    std::string* ret = new std::string[clauseCount]; 
    int i = startingIterator;

    for (int k = 0; k != clauseCount; k++){
        for (; i != argc; i++){
            if (clauseCount == 2 && (!strcmp(argv[i], "to") || !strcmp(argv[i], "from"))){
                break;
            }
            
            if (clauseCount == 3 && ((!strcmp(argv[i], "from") && k == 0) || (!strcmp(argv[i], "to") && k == 1))){
                break;
            }

            if (argv[i][0] == '/')
                memmove(&argv[i][0], &argv[i][1], strlen(argv[i]));

            ret[k] += argv[i];

            if (i != argc - 1 && (strcmp(argv[i + 1], "from") && strcmp(argv[i + 1], "to"))){
                ret[k] += ' ';
            }
        }
        i++; // move i right after the keyword
    }

    if (clauseCount == 2 && ret[1].length() == 0)
        error();
        
    else if (clauseCount == 3){ // i had to make this look like this because if clauseCount != 3 then ret[2] will give segmentation fault
        if (ret[1].length() == 0 || ret[2].length() == 0)
            error();
    }

    if (i != argc + 1) // if i != argc + 1 then it's probably the user's error where they didn't add '/' before a keyword
        error();

    return ret;
}

void argParser(const int argc, char** argv){ // this function makes main look better
    if (strcmp(argv[1], "lists") == 0 && argc == 2){ // check if user wrote "todo lists"
        int i = 1;

        for (auto& dirEntry: std::filesystem::recursive_directory_iterator(listsFolder)){
            if (dirEntry.is_regular_file()) {
                std::filesystem::path file = dirEntry.path();

                if (file.extension() == ".list"){ // if file has extention ".list"
                    std::string listName = file.filename().string(); // this gives "filename.list"
                    listName = listName.substr(0, listName.length() - 5); // this changes "filename.list" to "filename"

                    std::cout << "\033[1;36m" << i++ << ". " << listName << "\033[0m\n";
                }
            }
        }
        exit(0);
    }
    else if (strcmp(argv[1], "tasks") == 0){ // check if user wrote "todo tasks ..."

        if (argc == 2){ // if only "todo tasks"
            int i = 1, j = 1;
            bool isFirst = true;

            std::filesystem::path file;
            for (auto& dirEntry: std::filesystem::recursive_directory_iterator(listsFolder)){ // check all files in $HOME/.lists/
                if (dirEntry.is_regular_file()) {
                    file = dirEntry.path();

                    if(file.extension() == ".list"){ // if file has extention ".list"
                        if (!isFirst) // if it isn't the first file the program saw it would print 2 new lines
                            std::cout << "\n\n";
                        else {
                            isFirst = false;
                        }

                        i = 1;
                        
                        std::string listName = file.filename().string(); // this gives "filename.list"
                        listName = listName.substr(0, listName.length() - 5); // this changes "filename.list" to "filename"
                        ini list(listName);

                        std::cout << "\033[1;36m" << j++ << ". " << list.name << "\n"; // print list name with cyan color

                        for (std::vector<task>::iterator it = list.tasks.begin(); it != list.tasks.end(); it++){ // this will print task name followed by state, each state will make the output a different color
                            if (it -> state == 'c') std::cout << "\t\033[1;32m" <<  std::left << i << ". " << it -> name << std::setw(49 - it -> name.length() - getDig(i++)) << std::right << "\tcompleted\t\t";
                            if (it -> state == 'i') std::cout << "\t\033[1;33m" <<  std::left << i << ". " << it -> name << std::setw(50 - it -> name.length() - getDig(i++)) << std::right << "\tin progress\t\t";
                            if (it -> state == 'n') std::cout << "\t\033[1;31m" <<  std::left << i << ". " << it -> name << std::setw(50 - it -> name.length() - getDig(i++)) << std::right << "\tnot completed\t\t";
                            if (it -> state == 'u') std::cout << "\t\033[1;37m" <<  std::left << i << ". " << it -> name << std::setw(50 - it -> name.length() - getDig(i++) - 1) << std::right << "\tunknown\t\t\t";

                            if (it -> desc.length() != 0) // prints description if the task has one
                                std::cout << trunc(it -> desc);
                            
                            std::cout << "\033[0m\n"; // puts color back to normal (to not mess up terminal color)
                        }
                    }
                }
            }
        }
        else {
            std::string* listName = getInfo(argc, argv, 1, 2); // gets list name that the user wants to check
            ini list(listName[0]); // opens list
            
            int i = 1;

            for (std::vector<task>::iterator it = list.tasks.begin(); it != list.tasks.end(); it++){ //outputs all tasks (read code above if you need explanation)
                if (it -> state == 'c') std::cout << "\033[1;32m" <<  std::left << i << ". " << it -> name << std::setw(49 - it -> name.length() - getDig(i++)) << std::right << "\tcompleted\t\t";
                if (it -> state == 'i') std::cout << "\033[1;33m" <<  std::left << i << ". " << it -> name << std::setw(50 - it -> name.length() - getDig(i++)) << std::right << "\tin progress\t\t";
                if (it -> state == 'n') std::cout << "\033[1;31m" <<  std::left << i << ". " << it -> name << std::setw(50 - it -> name.length() - getDig(i++)) << std::right << "\tnot completed\t\t";
                if (it -> state == 'u') std::cout << "\033[1;37m" <<  std::left << i << ". " << it -> name << std::setw(50 - it -> name.length() - getDig(i++) - 1) << std::right << "\t\tunknown\t\t";

                if (it -> desc.length() != 0)
                    std::cout << "\"" << trunc(it -> desc) << "\"";
                            
                std::cout << "\033[0m\n";
            }
        }
        exit(0);
    }
    else if (strcmp(argv[1], "add") == 0 && argc > 3){
        if (strcmp(argv[2], "list") == 0){ // if user wrote "todo add list (list name)"
            std::string* listName = getInfo(argc, argv, 1, 3); // get list name

            std::ofstream file(listsFolder + listName[0] + ".list"); // open file with name user inputed + .list
            if (file.is_open()){ // if it worked close and exit
                file.close();
                exit(0);
            }
            
            //if not then error
            std::cout << "ERROR: COULDN'T MAKE NEW FILE\n";
            exit(3);
        }
        else if (strcmp(argv[2], "task") == 0 && argc > 5){
            // if user wrote "todo add task n x (task name)" where x is list number
            if (strcmp(argv[3], "n") == 0 || strcmp(argv[3], "num") == 0 || strcmp(argv[3], "number") == 0){ 
                int listNum = atoi(argv[4]); // list number is taken from either "todo lists" or "todo tasks"
                int i = 1;
                std::string* taskName = getInfo(argc, argv, 1, 5); // get task name

                if (taskName[0].length() > 39){
                    std::cout << "ERROR: TASKS CAN ONLY GO UP TO 40 CHARACTERS\n";
                    exit(2);
                }

                std::filesystem::path file;
                for (auto& dirEntry: std::filesystem::recursive_directory_iterator(listsFolder)){
                    if (dirEntry.is_regular_file()) {
                        file = dirEntry.path();

                        if (file.extension() == ".list"){
                            if (i == listNum){ // count until listNum == i
                                std::string listName = file.filename().string();
                                listName = listName.substr(0, listName.length() - 5); // get listName (process was explained before)

                                ini list(listName);
                                list.add(taskName[0], "", 'n'); // add task, default values are "" for description and "not complete" for state

                                list.outputFile(); //output file and exit
                                exit(0);
                            }
                            i++;
                        }
                    }
                }

                // if exited for loop then listNum doesn't exist
                std::cout << "ERROR: LIST NUMBER \"" << listNum << "\" NOT FOUND\n";
                exit(1);
            }
            // if user wrote "todo add task (task name) to (list name)"
            else{
                std::string* info = getInfo(argc, argv, 2, 3); // info[0] = task name, info[1] = list name

                if (info[0].length() > 39){
                    std::cout << "ERROR: TASKS CAN ONLY GO UP TO 40 CHARACTERS\n";
                    exit(2); 
                }

                ini list(info[1]);
                list.add(info[0], "", 'n');

                list.outputFile();
                exit(0);
            }
        }
        else {
            error();
        }
    }
    else if (strcmp(argv[1], "change") == 0 && argc > 6){
        if (strcmp(argv[2], "status") == 0){
            // if user wrote "todo change status x y to s" where x is list number, y is task number and s is new state
            if (strcmp(argv[3], "n") == 0 || strcmp(argv[3], "num") == 0 || strcmp(argv[3], "number") == 0){
                int taskNum = atoi(argv[5]);
                int listNum = atoi(argv[4]);

                if (strcmp(argv[6], "to")) // if to doesn't exist then error
                    error();

                char newState = argv[7][0];

                // if new state is not c, i or n then put it as unknown
                if (newState != 'c' && newState != 'i' && newState != 'n')
                    newState = 'u'; 

                int i = 1;

                for (auto& dirEntry: std::filesystem::recursive_directory_iterator(listsFolder)){
                    if (dirEntry.is_regular_file()) {
                        std::filesystem::path file = dirEntry.path();

                        if(file.extension() == ".list"){
                            if (i == listNum){ // iterate through .list files till i == listNum
                                i = 1;

                                std::string listName = file.filename().string();
                                listName = listName.substr(0, listName.length() - 5);
                                ini list(listName); // open list and iterate through tasks till i == taskNum

                                for (std::vector<task>::iterator it = list.tasks.begin(); it != list.tasks.end(); it++){
                                    if (i == taskNum){
                                        it -> state = newState; // change state and exit
                                        list.outputFile(listName);

                                        exit(0);
                                    }
                                    i++;
                                }
                                //error when taskNum doesn't exist
                                std::cout << "ERROR: TASK NUMBER \"" << taskNum << "\" WAS NOT FOUND\n";
                                exit(1);
                            }

                            i++;
                        }
                    }
                }
                //error when listNum doesn't exist
                std::cout << "ERROR: LIST NUMBER \"" << listNum << "\" WAS NOT FOUND\n";
                exit(1);
            }
            // if user wrote "todo change status of (task name) from (list name) to (new state)"
            else{
                if (strcmp(argv[3], "of"))
                    error();

                std::string* info = getInfo(argc, argv, 3, 4); // info[0] = task name, info[1] = list name, info[2] is new state char
                char newState = info[2][0];

                if (newState != 'c' && newState != 'i' && newState != 'n')
                    newState = 'u';

                ini list(info[1]);

                if (info[0] == "*"){ // if user wrote "\*" for the task part then select all tasks
                    char input;

                    do {
                        std::cout << "Are you sure you want to change all states from " << info[1] << "?[y/n] ";
                        input = getchar();
                    } while (input != 'y' && input != 'n'); // make sure input is either y or n

                    if (input == 'n') // if user doesn't want to select all just exit program
                        exit(0);
                    
                    for (std::vector<task>::iterator it = list.tasks.begin(); it != list.tasks.end(); it++){
                        it -> state = newState;
                    }

                    list.outputFile(); // once everything is edited then exit
                    exit(0);
                }

                for (std::vector<task>::iterator it = list.tasks.begin(); it != list.tasks.end(); it++){
                    if (it -> name == info[0]){ // check if it is pointing at correct task
                        it -> state = newState; // change state and exit
                        
                        list.outputFile();
                        exit(0);
                    }
                }

                std::cout << "ERROR: TASK \"" << info[0] << "\" WAS NOT FOUND\n"; // error when task was not found
                exit(1);
            }
        }
        else if (strcmp(argv[3], "name") == 0 && argc > 6){
            if (strcmp(argv[2], "list") == 0){
                // if user wrote "todo change list name n x to (new list name)" where x is the list number from "todo lists"
                if (strcmp(argv[4], "n") == 0 || strcmp(argv[4], "num") == 0 || strcmp(argv[4], "number") == 0){
                    int listNum = atoi(argv[5]);

                    if (strcmp(argv[6], "to"))
                        error();

                    std::string* newListName = getInfo(argc, argv, 1, 7); // get new list name
                    int i = 1;
                    
                    // iterate through all available lists until new list found
                    for (auto& dirEntry: std::filesystem::recursive_directory_iterator(listsFolder)){
                        if (dirEntry.is_regular_file()) {
                            std::filesystem::path file = dirEntry.path();

                            if(file.extension() == ".list"){
                                if (i == listNum){

                                    std::string listName = file.filename().string(); // get list name
                                    listName = listName.substr(0, listName.length() - 5);
                                    std::string dirToList = listsFolder + listName + ".list"; // get dir to list

                                    ini list(listName);
                                    list.outputFile(newListName[0]); // output list to a new list with the new list name
                                    std::remove(dirToList.c_str()); // delete old list and exit

                                    exit(0);
                                }
                                i++;
                            }
                        }
                    }

                    // error if list number was not found
                    std::cout << "ERROR: COULDN'T FIND LIST NUMBER \"" << listNum << "\"\n";
                    exit(1);
                }
                // if user wrote "todo change list name (list name) to (new list name)"
                else{
                    std::string* info = getInfo(argc, argv, 2, 4); // gets old list name and new one
                    std::string dirToList = listsFolder + info[0] + ".list"; // dir to old list
                    ini list(info[0]); // open new list

                    list.outputFile(info[1]); // output to new list with new name
                    std::remove(dirToList.c_str()); // remove old list and exit

                    exit(0);
                }
            }
            else if (strcmp(argv[2], "task") == 0 && argc > 7){
                // if user writes "todo change task name n x y to (new task name)" where x is list number, y is task number
                if (strcmp(argv[4], "n") == 0 || strcmp(argv[4], "n") == 0 || strcmp(argv[4], "n") == 0){
                    int listNum = atoi(argv[5]);
                    int taskNum = atoi(argv[6]);
                    std::string* newTaskName = getInfo(argc, argv, 1, 8); // gets new list number

                    if (newTaskName[0].length() > 39){
                        std::cout << "ERROR: TASKS CAN ONLY GO UP TO 40 CHARACTERS\n";
                        exit(2);
                    }

                    int i = 1;

                    // iterates until it finds list number
                    for (auto& dirEntry: std::filesystem::recursive_directory_iterator(listsFolder)){
                        if (dirEntry.is_regular_file()) {
                            std::filesystem::path file = dirEntry.path();

                            if(file.extension() == ".list"){
                                if (i == listNum){
                                    i = 1;

                                    std::string listName = file.filename().string();
                                    listName = listName.substr(0, listName.length() - 5);
                                    ini list(listName);

                                    // iterates until it finds task number
                                    for (std::vector<task>::iterator it = list.tasks.begin(); it != list.tasks.end(); it++){
                                        if (i == taskNum){
                                            it -> name = newTaskName[0];

                                            list.outputFile();

                                            exit(0);
                                        }
                                        i++;
                                    }

                                    // error if task number was not found
                                    std::cout << "ERROR: TASK NUMBER \"" << taskNum << "\" WAS NOT FOUND\n";
                                    exit(1);
                                }
                                i++;
                            }
                        }
                    }

                    // error if list number was not found
                    std::cout << "ERROR: LIST NUMBER \"" << listNum << "\" WAS NOT FOUND\n";
                    exit(1);
                }
                // if user wrote "todo change task name (task name) from (list name) to (new task name)"
                else{
                    std::string* info = getInfo(argc, argv, 3, 4); // info[0] = old task name, info[1] = list name, info[2] = new task name

                    if (info[2].length() > 39){
                        std::cout << "ERROR: TASKS CAN ONLY GO UP TO 40 CHARACTERS\n";
                        exit(2);
                    }

                    ini list(info[1]);

                    // iterate through tasks until task is found
                    for (std::vector<task>::iterator it = list.tasks.begin(); it != list.tasks.end(); it++){
                        if (it -> name == info[0]){
                            it -> name = info[2]; // change task name, output and exit
                            list.outputFile();
                            
                            exit(0);
                        }
                    }

                    // error if task was not found
                    std::cout << "ERROR: TASK \"" << info[0] << "\" WAS NOT FOUND\n";
                    exit(1);
                }
            }
            else{
                error();
            }
        }
        else if ((strcmp(argv[2], "description") == 0 || strcmp(argv[2], "desc") == 0) && argc > 7){
            // if user wrote "todo change desc/description n x y to (new description)" where x is list number and y is task number
            if (strcmp(argv[3], "n") == 0 || strcmp(argv[3], "num") == 0 || strcmp(argv[3], "number") == 0){
                int listNum = atoi(argv[4]);
                int taskNum = atoi(argv[5]);
                
                if (strcmp(argv[6], "to") != 0)
                    error();

                std::string* newDesc = getInfo(argc, argv, 1, 7);

                int i = 1;

                // iterate until i == listNum
                std::filesystem::path file;
                for (auto& dirEntry: std::filesystem::recursive_directory_iterator(listsFolder)){
                    if (dirEntry.is_regular_file()) {
                        file = dirEntry.path();

                        if (file.extension() == ".list"){
                            if (i == listNum){
                                i = 1;

                                std::string listName = file.filename().string();
                                listName = listName.substr(0, listName.length() - 5);
                                ini list(listName);

                                // iterate until i == taskNum
                                for (std::vector<task>::iterator it = list.tasks.begin(); it != list.tasks.end(); it++){
                                    if (i == taskNum){
                                        it -> desc = newDesc[0];
                                        list.outputFile(list.name);

                                        exit(0);
                                    }
                                    i++;
                                }

                                // error if task was not found
                                std::cout << "ERROR: TASK NUMBER \"" << taskNum << "\" WAS NOT FOUND\n";
                                exit(1);
                            }
                            i++;
                        }
                    }
                }

                // error if list was not found
                std::cout << "ERROR: LIST NUMBER \"" << listNum << "\" WAS NOT FOUND\n";
                exit(1);
            }
            // if user wrote "todo change desc/description of (task name) from (list name) to (new desc)"
            else{
                if (strcmp(argv[3], "of")) // makes everything sound more natural
                    error();
                
                std::string* info = getInfo(argc, argv, 3, 4); // info[0] = task name, info[1] = list name, info[2] = new description
                ini list(info[1]);

                if (info[0] == "*"){ // if user selected all tasks
                    char c;

                    do {
                        std::cout << "Are you sure you want to change all of the descriptions from " << info[1] << "?[y/n]";
                        c = getchar();
                    } while (c != 'y' && c != 'n'); // get either a y or n

                    if (c == 'n') // n = no, so stop program
                        exit(0);
                    
                    for (std::vector<task>::iterator it = list.tasks.begin(); it != list.tasks.end(); it++){
                        it -> desc = info[2]; // change desc of each task
                    }

                    list.outputFile(); //output and exit
                    exit(0);
                }

                // iterate until task is found
                for (std::vector<task>::iterator it = list.tasks.begin(); it != list.tasks.end(); it++){ 
                    if (it -> name == info[0]){
                        it -> desc = info[2]; // change desc, output and exit
                        list.outputFile();

                        exit(0);
                    }
                }

                // error if task was not found
                std::cout << "ERROR: TASK NAME \"" << info[0] << "\" WAS NOT FOUND\n";
                exit(1);
            }
        }
        else {
            error();
        }
    }
    else if ((strcmp(argv[1], "delete") == 0 || strcmp(argv[1], "remove") == 0) && argc > 3){
        // if user wrote "todo delete/remove list ..."
        if (strcmp(argv[2], "list") == 0){
            // if user wrote "todo delete/remove list n x", where x is list number (from "todo lists")
            if (strcmp(argv[3], "n") == 0 || strcmp(argv[3], "num") == 0 || strcmp(argv[3], "number") == 0){
                int listNum = atoi(argv[4]);
                int i = 1;

                std::filesystem::path file;
                for (auto& dirEntry: std::filesystem::recursive_directory_iterator(listsFolder)){
                    if (dirEntry.is_regular_file()) {
                        file = dirEntry.path();

                        if (file.extension() == ".list"){
                            if (i == listNum){ // iterate until i == list num
                                std::remove(file.string().c_str()); // delete list and exit
    
                                exit(0);
                            }
                            i++;
                        }
                    }
                }

                // error if list was not found
                std::cout << "ERROR: COULDN'T FIND LIST NUMBER \"" << listNum << "\"\n";
                exit(1);
            }
            // if user wrote "todo delete/remove list (list name)"
            else{
                std::string* listName = getInfo(argc, argv, 1, 3);

                if (listName[0] == "*"){ // if user wants to delete all lists
                    char c;
                    
                    do{
                        std::cout << "Are you sure you want to delete all lists?[y/n] ";
                        c = getchar();
                    } while (c != 'y' && c != 'n'); // get y or n answer

                    if (c == 'n') // if n exit because user doesn't want to delete all lists
                        exit(0);

                    std::filesystem::path file;
                    for (auto& dirEntry: std::filesystem::recursive_directory_iterator(listsFolder)){
                        if (dirEntry.is_regular_file()) {
                            file = dirEntry.path();

                            if (file.extension() == ".list"){
                                std::remove(file.string().c_str()); // delete all lists
                            }
                        }
                    }
                    
                    exit(0);
                }

                // if user doesn't want to delete all lists
                std::string listDir = listsFolder + listName[0] + ".list";
                std::remove(listDir.c_str());

                exit(0);
            }
        }
        // if user wrote "todo delete/remove task ..."
        else if (strcmp(argv[2], "task") == 0){
            // if user wrote "todo delete/remove task n x y" where x is list number and y is task number
            if (strcmp(argv[3], "n") == 0 || strcmp(argv[3], "num") == 0 || strcmp(argv[3], "number") == 0){
                int listNum = atoi(argv[4]);
                int taskNum = atoi(argv[5]);
                int i = 1;

                std::filesystem::path file;
                for (auto& dirEntry: std::filesystem::recursive_directory_iterator(listsFolder)){
                    if (dirEntry.is_regular_file()) {
                        file = dirEntry.path();

                        if (file.extension() == ".list"){
                            if (i == listNum){ // iterate until i == with listNum
                                std::string listName = file.filename().string();
                                listName = listName.substr(0, listName.length() - 5);
                                ini list(listName);

                                // remove task that is in index taskNum - 1 because taskNum is 1 above the actual index
                                list.tasks.erase(list.tasks.begin() + taskNum - 1); 
                                list.outputFile();

                                exit(0);
                            }
                            i++;
                        }
                    }
                }

                // error if list was not found
                std::cout << "ERROR: LIST NUMBER \"" << listNum << "\" WAS NOT FOUND\n";
                exit(1);
            }
            // if user wrote "todo delete/remove task (task name) from (list name)"
            else {
                std::string* info = getInfo(argc, argv, 2, 3);
                ini list(info[1]);
                
                if (info[0] == "*"){
                    char c;

                    do{
                        std::cout << "Are you sure you want to delete all tasks from " << info[1] << "?[y/n] ";
                        c = getchar();
                    }while (c != 'y' && c != 'n');

                    if (c == 'n')
                        exit(0);

                    while (!list.tasks.empty())
                        list.tasks.pop_back();

                    list.outputFile();
                    exit(0);
                }

                list.rem(info[0]);
                exit(0);
            }
        }
        else if ((strcmp(argv[2], "desc") == 0 || strcmp(argv[2], "description") == 0) && argc > 5){
            // if user wrote "todo delete/remove description/desc n x y to (new desc)" where x is list number and y is task number
            if (strcmp(argv[3], "n") == 0 || strcmp(argv[3], "num") == 0 || strcmp(argv[3], "number") == 0){
                int listNum = atoi(argv[4]);
                int taskNum = atoi(argv[5]);
                int i = 1;

                std::filesystem::path file;
                for (auto& dirEntry: std::filesystem::recursive_directory_iterator(listsFolder)){
                    if (dirEntry.is_regular_file()) {
                        file = dirEntry.path();

                        if(file.extension() == ".list"){
                            if (i == listNum){ // iterate until list is found
                                std::string listName = file.filename().string();
                                listName = listName.substr(0, listName.length() - 5); // get list name
                                
                                ini list(listName);
                                i = 1;

                                for (std::vector<task>::iterator it = list.tasks.begin(); it != list.tasks.end(); it++){ // iterate until task is found
                                    if (i == taskNum){
                                        it -> desc = ""; // make desc equal to nothing
                                        list.outputFile(); // output file and exit
                                        exit(0);
                                    }
                                    i++;
                                }

                                // error if task was not found
                                std::cout << "ERROR: TASK NUMBER \"" << taskNum << "\" WAS NOT FOUND\n";
                                exit(1);
                            }
                            i++;
                        }
                    }
                }

                // error if list was not found
                std::cout << "ERROR: LIST NUMBER \"" << listNum << "\" WAS NOT FOUND\n";
                exit(1);
            }
            // if user wrote "todo delete/remove description/desc (task name) from (list name) to (new desc)"
            else{
                std::string* info = getInfo(argc, argv, 2, 3);
                ini list(info[1]);

                if (info[0] == "*"){ // if user selects all tasks
                    char c;

                    do {
                        std::cout << "Are you sure you want to delete the descriptions from " << info[1] << "?[y/n] ";
                        c = getchar();
                    } while (c != 'y' && c != 'n'); // get y or n answer

                    if (c == 'n') // if n then exit as user doesn't want to delete all task descs
                        exit(0);
                    
                    for (std::vector<task>::iterator it = list.tasks.begin(); it != list.tasks.end(); it++){
                        it -> desc = ""; // make desc equal to nothing
                    }
                    
                    list.outputFile(); // output list and exit
                    exit(0);
                }

                for (std::vector<task>::iterator it = list.tasks.begin(); it != list.tasks.end(); it++){
                    if (it -> name == info[0]){ // find task user specified about
                        it -> desc = ""; // make desc equal to nothing
                        
                        list.outputFile(); //output list and exit
                        exit(0);
                    }
                }

                // error if task was not found
                std::cout << "ERROR: TASK \"" << info[0] << "\" WAS NOT FOUND\n";
                exit(1);
            }
        }
        else {
            error();
        }
    }
    else if (strcmp(argv[1], "help") == 0 && argc == 2){ // check if user wrote "todo help", no further explanation needed so frick off :(((
        std::cout << "\033[1;35mCommands:\n";
        std::cout << "  \033[1;31madd:\n";
        std::cout << "    \033[1;33mlist [name],\033[1;37m creates a new todo list.\n";
        std::cout << "      \033[1;32mEg: todo add list haha this is a test\n\n";
        std::cout << "    \033[1;33mtask [task] to [list],\033[1;37m adds a new task to an already existing todo list.\n";
        std::cout << "      \033[1;32mEg: todo add task testing to test\n\n";
        std::cout << "  \033[1;31mchange:\n";
        std::cout << "    \033[1;33mlist name [old list name] to [new list name],\033[1;37m changes a list name.\n";
        std::cout << "      \033[1;32mEg: todo change list name testing to test\n\n";
        std::cout << "    \033[1;33mtask name [old task name] from [list] to [new task name],\033[1;37m changes a task name.\n";
        std::cout << "      \033[1;32mEg: todo change task name test from testing to tested\n\n";
        std::cout << "    \033[1;33mstatus of [task] from [list] to [new state],\033[1;37m changes a task's state. \n";
        std::cout << "      \033[1;32mEg: todo change status of test from testing to c\n";
        std::cout << "      \033[1;37mStates can be c for complete, i for in progress n for not complete and u for unavailable. Default state is n\n\n";
        std::cout << "    \033[1;33mdescription/desc of [task] from [list] to [new desc],\033[1;37m used for changing the description of a task.\n";
        std::cout << "      \033[1;32mEg: todo change desc of testing from test to foo\n\n";
        std::cout << "  \033[1;31mdelete/remove:\n";
        std::cout << "    \033[1;33mlist [name],\033[1;37m deletes an already existing list.\n";
        std::cout << "      \033[1;32mEg: todo delete list foo\n\n";
        std::cout << "    \033[1;33mtask [task] from [list],\033[1;37m deletes a task from a list.\n";
        std::cout << "      \033[1;32mEg: todo remove task foo from testing\n\n";
        std::cout << "    \033[1;33mdescription/desc [task] from [list],\033[1;37m removes description from a task.\n";
        std::cout << "      \033[1;32mEg: todo remove desc test from testing\n\n";
        std::cout << "  \033[1;31mtasks:\n";
        std::cout << "    \033[1;32m\"todo tasks\"\033[1;37m would print all tasks from all lists.\n";
        std::cout << "    \033[1;32m\"todo tasks [list name]\"\033[1;37m prints all tasks from a list chosen by the user.\n\n";
        std::cout << "  \033[1;31mlists:\n";
        std::cout << "    \033[1;37mThis command prints all lists, if any other arguments are inputed program will exit with error.\n\n";
        std::cout << "\n\n";
        std::cout << "\033[1;35mKeywords: \033[1;31mto, from\033[1;37m. When these words want to be used for something else then add '/' behind them.\n";
        std::cout << "\n\n";
        std::cout << "\033[1;37mIf \"\\*\" is used then all tasks/lists will be changed with the command that you entered.\n";
        std::cout << "\033[1;32mEg: \"todo delete tasks \\* from test\" \033[1;37mwill delete all tasks from the list test.\n\n";
        std::cout << "\033[1;37mNumbers can also be used to make things faster.\n";
        std::cout << "\033[1;32mEg: \"todo change status n x y to c\"\033[1;37m where x is list number and y is task number.\n";
        std::cout << "Exception to the 2 number rule is \033[1;32m\"todo delete list n x\"\n\033[0;37m";

        exit(0);
    }
    else if (strcmp(argv[1], "yeet") == 0){ // why should i explain this?
        std::cout << "consider this an easter egg (this command was used for testing purposes)\n";
        exit(0);
    }
    else{
        error();
    }
}

int main(int argc, char** argv){

    //listsFolder is in $HOME/.lists, you might notice that there are 2 variables that do the same thing, not my fault, blame g++
    listsFolder = getenv("HOME");
    listsFolder += "/.lists/";
	
    // checks if .lists dir even exists, if not makes it
	DIR* dir = opendir(listsFolder.c_str()); 
	if (!dir)
		mkdir(listsFolder.c_str(), 0777);
	closedir(dir);

    if (argc == 1) // program needs args to actually work
        error();
    else{
        argParser(argc, argv);
    }

    return 0;
}
