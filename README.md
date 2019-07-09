# todo
**if you can help me fix the code please put up an issue, i'd appreciate the help**
<br> *i am also really bad at making .md files so sorry if it is that bad*

## How to install
git clone https://github.com/xDprinDx/todo.git
<br> cd todo
<br> sudo make install

## How to use
### Commands
* **tasks (list name)**
<br>if you don't input a list name it will output all tasks from all list names, if there is a list name it will output all of it's tasks
* **lists**
<br>outputs all lists. This can't accept any other args
* **add**
  * **list (list name)**
    <br>this will add a list with the given name
    <br>*Eg: todo add list foo*<br>
  * **task (task name) to (list name)**
    <br>this will add a task to the list specified
    <br>*Eg: todo add task foo to bar*
* **change**
  * **status**
    <br>This will change the status of an already existing task
    <br>States can be: n (not complete), i (in progress), c (complete) and u (unknown)
    <br>*Eg: todo change status of foo from bar to i*
  * **name**
    * **task**
    <br>This will change an already existing task's name
    <br>*Eg: todo change task name foo from bar to test*
    * **list**
    <br>This will change an already existing list's name
    <br>*Eg: todo change list name foo to bar*
  * **description/desc**
  <br>This changes the description of a task
  <br>*Eg: todo change desc of foo from bar to test*
* **remove/delete**
  * **list**
  <br>This deletes an already existing list
  <br>*Eg: todo delete list foo*
  * **task**
  <br>This deletes a task from a list
  <br>*Eg: todo delete task foo from bar*
  * **description/desc**
  <br>This makes description equal to nothing
  <br>*Eg: todo delete description foo from bar*
  
### **NOTE**
if you want to use key words in a task/list/desc name put an '/' before it.

<br> <br> if you want to select all tasks/lists put "\*". *Eg: todo delete task \\\* from foo*, this will delete all tasks from foo. Exception to this rule being *todo tasks* as it already prints all tasks if not list is inputed.

<br> <br> you can also use the numbers that are listed in *todo tasks*, most of the time it is 2 numbers like "*todo change desc n x y to (new desc)*", where x is list number and y is task number. If one number will be used for a command then it is the list number that you should input, like "*todo change list name n 1 to bar*".
