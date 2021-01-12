//~~~~~~~~~~~~~~~~~Header files required for program~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <unistd.h>
#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<stdio.h>
#include<stack>
#include<ctime>
#define current getcwd
using namespace std;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~get_date()~~~~~~~~~~~~~~~~~~~
INPUT: none
OUTPUT:return string containing the system date in YYYY-MM-DD format
*/
string get_date()
{
	time_t now=time(0);				
	tm *ltm= localtime(&now);
	int month=(1+ltm->tm_mon);
	int day=ltm->tm_mday;
	int year=1900+ltm->tm_year;
	string date="";
	date+=to_string(year)+"-";
	string d,m;
	if(day<10)
	{
		d="0"+to_string(day);
	}
	else{
		d=to_string(day);
	}
	if(month<10)
	{
		m="0"+to_string(month);
	}
	else{
		m=to_string(month);
	}
	date+=m+"-"+d;
	return date;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Class of a todo list 
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
class todo_list{
	fstream todo,done;
	string in,out,temp;
public:
	todo_list();
	void add(string);
	void ls();
	void help();
	int count_left(string);
	string rem(int);
	void del(int);
	void done1(int);
	void report();
	~todo_list();
};
/*
~~~~~~~~~~~~~~~~~~~~~~~~ todo_list()~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Constructor for getting the file paths in current directory and opening the files in append and reading mode
*/
todo_list :: todo_list()
{
	char buff[FILENAME_MAX];
	current(buff,FILENAME_MAX);
    string pwd=buff;
    in=pwd+"/todo.txt";
    out=pwd+"/done.txt";
  	temp=pwd+"/temp.txt";
    todo.open(in,ios::app|ios::in);
    done.open(out,ios::app|ios::in);
}
/*
~~~~~~~~~~~~~~~~~~~~~~~~~ ~todo_list()~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Destructor to close the files
*/
todo_list :: ~todo_list()
{
	todo.close();
	done.close();
}
/*
~~~~~~~~~~~~~~~~ add(string)~~~~~~~~~~~~~~~~~~~
	INPUT: a string containing the todo item
	OUTPUT: Tells wheter a todo is added successfully
*/
void todo_list :: add(string entry)
{
	/*move file pointer to end to input new data, to prevetn overwritng over older data*/
	todo.seekp(0,ios::end);
	todo<<entry<<"\n";
	cout<<"Added todo: "<<'"'<<entry<<'"'<<endl; 
}
/*
~~~~~~~~~~~~~~~~~~~~~~ ls()~~~~~~~~~~~~~~~~~~~~~~~~~
	Shows the elements in the list in order of the latest item on top
	INPUT: none
	OUTPUT: list of the remaining todos
*/
void todo_list :: ls()
{
	todo.seekp(0,ios::beg);
	stack<string> s;
	string str;
	int i=0;
	/*use a stack to store the todos so the elements are in LIFO order*/
	while(getline(todo,str))
	{
		s.push(str);
		i++;
	}
	if(i==0)
		cout<<"There are no pending todos!\n";
	else
	while(s.empty()==false)
	{
		cout<<"["<<i--<<"] "<<s.top()<<endl;
		s.pop();
	}
}
/*
~~~~~~~~~~~~~~~~~~~~~~~~~help()~~~~~~~~~~~~~~~~~~~~~~~~~~
 Prints how the program functions for different inputs
	INPUT:none
	OUTPUT:List of functions
*/
void todo_list :: help()
{
	cout<<"Usage :-";
	cout<<"\n$ ./todo add \"todo item\"  # Add a new todo";
	cout<<"\n$ ./todo ls               # Show remaining todos";
	cout<<"\n$ ./todo del NUMBER       # Delete a todo";
	cout<<"\n$ ./todo done NUMBER      # Complete a todo";
	cout<<"\n$ ./todo help             # Show usage";
	cout<<"\n$ ./todo report           # Statistics\n";
}
/*
~~~~~~~~~~~~~~~~~~~~~~~~~count_left(string) ~~~~~~~~~~~~~~~~~~~~~~~~~~
It is a function to return the number of item present in a given file using the file path
	INPUT:String containing path of the file
	OUTPUT:an integer containing the number of records in file
*/
int todo_list :: count_left(string file)
{
	int count=0;
	ifstream f(file);
	string s;
	f.seekg(0,ios::beg);
	while(getline(f,s))
	{
		count++;
	}
	f.seekg(0,ios::beg);
	return count;
}
/*
~~~~~~~~~~~~~~~~~~~~~~~~~ rem(int) ~~~~~~~~~~~~~~~~~~~~~~~~~~
	this function removes the todolist item gven the number of the element and returns the removed element
	INPUT:number of the todo list item to delete
	OUTPUT:string containing the todo item which was deleted
*/
string todo_list :: rem(int no)
{
	int j=0;
	ofstream out(temp);
	string s,rim;
	todo.seekg(0,ios::beg);
	while(getline(todo,s))
	{
		++j;
		if(no!=j)
		{
			out<<s<<"\n";
		}
		else{
			rim=s;
		}
	}
	remove(in.c_str());
	rename(temp.c_str(),in.c_str());
	return rim;
}
/*
~~~~~~~~~~~~~~~~~~~~~~~~~ del(int) ~~~~~~~~~~~~~~~~~~~~~~~~~~
it deletes element corresponding to number from todo.txt file
	INPUT:number of the element to be deleted
	OUTPUT:if the element is present it is deleted or else an error is displayed
*/
void todo_list :: del(int no)
{
	int j=count_left(in);
	if(j<no || no<=0)
		cout<<"Error: todo #"<<no <<" does not exist. Nothing deleted.\n";
	else{
		rem(no);
		cout<<"Deleted todo #"<<no<<"\n";\
	}
}
/*
~~~~~~~~~~~~~~~~~~~~~~~~~ done1(int) ~~~~~~~~~~~~~~~~~~~~~~~~~~
Marks the numbered todo as done and puts it in done file
	INPUT:The number of the todo to be deleted
	OUTPUT:if the element is present we get success message and otherwise an error
*/
void todo_list :: done1(int no)
{
	int j=count_left(in);
	if(j<no || no<=0)
		cout<<"Error: todo #"<<no <<" does not exist.\n";
	else{
		string complete;
		complete=rem(no);
		done.seekg(0,ios::beg);
		done<<"x "<<get_date()<<" "<<complete<<"\n";
		cout<<"Marked todo #"<<no<<" as done.\n";
	}	
}
/*
~~~~~~~~~~~~~~~~~~~~~~~~~ report()  ~~~~~~~~~~~~~~~~~~~~~~~~~~
it dislays the items which are pending and completed
	INPUT:none
	OUTPUT:it prints the number of completed and pending todos
*/
void todo_list :: report()
{
	int pending=count_left(in);
	int complete=count_left(out);
	cout<<get_date()<<" Pending : "<<pending<<" Completed : "<<complete<<endl;
}
// main function
int main(int argc, char* argv[])
{
	todo_list list;
	// switch case which checs the option which has been given
    if(argc>1){
    switch(argv[1][0])
    {
    	case 'a':
    		if(argc==3)
    			list.add(argv[2]);
    		// insufficient arguments
    		else
    			cout<<"Error: Missing todo string. Nothing added!\n";
    		break;
    	case 'l':
    		list.ls();
    		break;
    	case 'd':
    		if(argv[1][1]=='e')
    		{
    			if(argc==3)
    			list.del(stoi(argv[2]));
    		// insufficient arguments
    		else
    			cout<<"Error: Missing NUMBER for deleting todo.";
    		}
    		else{
    			if(argc==3)
    				list.done1(stoi(argv[2]));
    			// insufficient arguments
    			else
    				cout<<"Error: Missing NUMBER for marking todo as done.\n";
    		}
    		break;
    	case 'h':
    		list.help();
    		break;
    	case 'r':
    		list.report();
    		break;
    	default:
    		list.help();
    		break;
    }}
    else
    	// print help if no argunment passed
    	list.help();

}