#include <iostream>
#include <fstream>                
using namespace std;

//class for Courses
class Course
{
public:
        Course(); // default constructor
        void get(string& name, string& time, string& number,
        char& g, int& h) const;
	//to only get the course number and hours
	void getRequirements(string& number, int& h);
        void set(string name, string time, string number,
        char g, int h);
	//to only set the course number and hours
	void setRequirements(string number, int h);
        void print() const;
	void printRequirements() const;
private:
        string courseName;
        string courseTime;
        string courseNumber;
        char grade;
        int hours;
};

//Constructor Course to intialized the variables
Course::Course()
{
        courseName = "Programming Foundations I";
        courseTime = "Fall 2015";
        courseNumber = "CSCE 2004";
        grade = 'A';
        hours = 4;
}

//print member function
void Course::print() const
{
        cout << courseName << endl;
        cout << courseTime << endl;
        cout << courseNumber << endl;
        cout << grade << endl;
        cout << hours << endl;
}
//print to print only the requirement courses
void Course::printRequirements() const
{
	cout << courseNumber << endl;
	cout << hours << endl;
}

//set member function
void Course::set(string name, string time, string number, char g, int h) 
{
        courseName = name;
        courseTime = time;
        courseNumber = number;
        grade = g;
        hours = h;
}

//set member to set the Requirement classes
void Course::setRequirements(string number, int h)
{
	courseNumber = number;
	hours = h;
}

//get member function
void Course::get(string& name, string& time, string& number, char& g, int& h) const
{
        name = courseName;
        time = courseTime;
        number = courseNumber;
        g = grade;
        h = hours;
}

//get member to get the Requirement classes
void Course::getRequirements(string& number, int& h)
{
	number = courseNumber;
	h = hours;
}
//class for Requirements
class Requirement
{
public:
	Requirement();
	void get(string& groups, string& subgroups, int& hours) const;
	//to get the information from the Course class
	void getCourses(string courseNum[], int hours[], int& index);
	void set(string groups, string subgroups, int hours);
	//to set the information from the Course class
	void setCourses(string courseNum, int hours, int index);
	bool validate(Course uniqueCourses[], Course usedCourses[], int& numUsedCourses, int numUniqueCourses);
	void print() const;
	void printSubgroup() const;
private:
	static const int INDEX = 200;
	string group;
	string subgroup;
	int requiredHours;
	Course courses[INDEX];
	int numCourses;
};
//constructor
Requirement::Requirement()
{
	group = "University Core";
	subgroup = "Fine Arts";
	requiredHours = 3;
	numCourses = 0;
}
//getter for the course class
void Requirement::getCourses(string courseNum[], int hour[], int& index)
{
	index = numCourses;
	string number;
	int h;
	for(int i = 0; i < numCourses; i++)
	{
		courses[i].getRequirements(number, h);
		courseNum[i] = number;
		hour[i] = h;
	}
}
//print method function
void Requirement::print() const
{
	cout << group << endl;
	cout << subgroup << endl;
	cout << requiredHours << endl;
	for(int k = 0; k < numCourses; k++)
	{
		courses[k].printRequirements();
	}
}
//to only print the subgroup
void Requirement::printSubgroup() const
{
	cout << subgroup << endl;
}
//getter to get group, subgroup and required Hours
void Requirement::get(string& groups, string& subgroups, int& hours) const
{
	groups = group;
	subgroups = subgroup;
	hours = requiredHours;
}
//setter to set group, subgroup and required hours
void Requirement::set(string groups, string subgroups, int hours) 
{
	group = groups;
	subgroup = subgroups;
	requiredHours = hours;
}
//setter to set the Course class
void Requirement::setCourses(string courseNum, int hours, int index)
{
	courses[index].setRequirements(courseNum, hours);
	numCourses = index + 1;
}
//method function to perform the degree audit
bool Requirement::validate(Course uniqueCourses[], Course usedCourses[], int& numUsedCourses, int numUniqueCourses)
{
	string number, usedNumber;
	string name, time;
	char grade;
	int hour, usedHours;
	int countHours = 0;
	for(int i = 0; i < numUniqueCourses; i++)
	{
		uniqueCourses[i].get(name, time, number, grade, hour);
		bool checkUsed = true;
		//to make sure the course has not already been used
		for(int j = 0; j < numUsedCourses; j++)
		{
			usedCourses[j].getRequirements(usedNumber, usedHours);
			if(number == usedNumber)
				checkUsed = false;
		}
		if(checkUsed)
		{
			string reqNumber;
			int reqHours;
			//loop to check the course against all of the possible courses
			for(int k = 0; k < numCourses; k++)
			{
				courses[k].getRequirements(reqNumber, reqHours);
				if(number == reqNumber)
				{
					countHours += hour;
					usedCourses[numUsedCourses].set(name, time, number, grade, hour);
					numUsedCourses++;
				}
				//checking to see if the required hours have been met
				if(countHours >= requiredHours)
				{
					return true;
				}
			}
		}
	}
	return false;
}
//function to read in the requirement file
bool readingRequirements(const char filename[], Requirement requirements[],
                         int capacity, int& n)
{
	ifstream din;
	din.open( filename );
	string group, subgroup;
	string courseNum;
	int hours, requiredHours;
	//checking to see if the file opened
	if(!din)
	{
		cout << "That file does not exist" << endl;
		return false;
	}
	getline(din, group);
	int requirementIndex = 0;
	//loop to assign the information to the variables
	while(!din.eof())
	{
		//error checking
		if(requirementIndex > capacity)
		{
			cout << "You went over capacity for the array" << endl; 
			return false;
		}
		int courseIndex = 0;
		getline(din, subgroup);
		din >> requiredHours;
		requirements[requirementIndex].set(group, subgroup, requiredHours);
		getline(din, courseNum);
		getline(din, courseNum);
		while(!din.eof() && courseNum != "")
		{
			din >> hours;
			requirements[requirementIndex].setCourses(courseNum, hours, courseIndex);
			courseIndex++;
			getline(din, courseNum);
			getline(din, courseNum);
		}
		requirementIndex++;
		getline(din, group);
	}
	n = requirementIndex;
	//closing the file
	din.close();
	return true;
}

//function to separate the University Requirements from the other requirements
void getUniversityRequirement (Requirement requirements[], Requirement universityRequirement[], int n, int& numUniversityRequirement)
{
	const int SIZE = 200;
	string group, subgroup;
	string number[SIZE];
	int requiredHours;
	int hours[SIZE];
	int index;
	for(int i = 0; i < n; i++)
	{
		requirements[i].get(group, subgroup, requiredHours);
		requirements[i].getCourses(number, hours, index);
		if (group == "University Core")
		{
			universityRequirement[numUniversityRequirement].set(group, subgroup, requiredHours);
			for(int j = 0; j < index; j++)
			{
				universityRequirement[numUniversityRequirement].setCourses(number[j], hours[j], j);
			}
			numUniversityRequirement++;
		}
	}
}
//function to get unique courses
void getUniqueCourses(Course courses[], Course uniqueCourses[], int numCourses, int& numUnique)
{
	string name, time, number;
	char grade;
	int hours;
	string uniqueName, uniqueTime, uniqueNumber;
	char uniqueGrade;
	int uniqueHours;
	bool checkUnique;
	for(int i = 0; i < numCourses; i++)
	{
		checkUnique = true;
		courses[i].get(name, time, number, grade, hours);
		for(int j = 0; j < numUnique; j++)
		{
			uniqueCourses[j].get(uniqueName, uniqueTime, uniqueNumber, uniqueGrade, uniqueHours);
			if(uniqueNumber == number)
			{
				if(uniqueGrade == 'D' && (grade == 'C' || grade == 'B' || grade == 'A'))
					uniqueCourses[j].set(name, time, number, grade, hours);
				else if(uniqueGrade == 'C' && (grade == 'B' || grade == 'A'))
					uniqueCourses[j].set(name, time, number, grade, hours);
				else if(uniqueGrade == 'B' && grade == 'A')
					uniqueCourses[j].set(name, time, number, grade, hours);
				checkUnique = false;
			}
		}
		if(checkUnique)
		{
			if(grade == 'A' || grade == 'B' || grade == 'C' || grade == 'D')
			{
				uniqueCourses[numUnique].set(name, time, number, grade, hours);
				numUnique++;
			}
		}
	}
}

//function to read in the file
bool reading(const char filename[], Course courses[], int& n,
             int capacity)
{
	ifstream din;
	din.open( filename );
	string name, time, number;
	char grade;
	int hours;
	//checking to see if the file opened
	if(!din)
	{
		cout << "That file does not exist" << endl;
		return false;
	}
	din >> n;
	//checking to make sure the file does not go over capacity
	if(n > capacity)
	{
		cout << "Insufficient space in the arrays." << endl;
		return false;
	}
	//loop to assign the information to the variables
	for (int i = 0; i < n; i++)
	{
		getline(din, name);
		getline(din, name);
		getline(din, time);
		getline(din, number);
		din >> grade;
		din >> hours;
		//setting the values to the courses class
		courses[i].set(name, time, number, grade, hours);
	}
	//closing the file
	din.close();
	return true;
}

//function to write the class into a file
bool writing(const char filename[], Course courses[], int n)
{
        ofstream dout;
        dout.open( filename );
        //making sure the file opens
        if(!dout)
                return false;
        dout << n << endl;
	string name;
	string time;
	string number;
	char grade;
	int hours;
        //loop to write in the file
        for (int i = 0; i < n; i++)
        {
		//retrieving the information from the class
		courses[i].get(name, time, number, grade, hours);
		dout << name << endl;
		dout << time << endl;
		dout << number << endl;
		dout << grade << endl;
		dout << hours << endl;
        }
        //closing the file           
        dout.close();
        return true;       
}

//gpa function
double gpa(int n, const Course courses[])
{
	double gradePoints = 0;
	double totalHours = 0;
	string name, time, number;
	char grade;
	int hour;
	//loop to sum the grade points and credit hours
	for(int index = 0; index < n; index++)
	{
		courses[index].get(name, time, number, grade, hour);
		if(grade == 'A')
			gradePoints = gradePoints + (hour * 4.0);
		else if(grade == 'B')
			gradePoints = gradePoints + (hour * 3.0);
		else if(grade == 'C')
			gradePoints = gradePoints + (hour * 2.0);
		else if(grade == 'D')
			gradePoints = gradePoints + (hour * 1.0);
		else if(grade == 'F')
			gradePoints = gradePoints + (hour * 0.0);

		if(grade != 'W' && grade != 'I')
			totalHours = totalHours + hour;
	}
	double gpaEquation = 0;
	//error checking and calculating the GPA
	if(totalHours == 0)
		cout << "Cannot divide by 0." << endl;
	else
		gpaEquation = gradePoints / totalHours;
	//if statements to output the GPA
        if ( gpaEquation >= 3.0)
		cout << "Excellent! Your GPA is " << gpaEquation << endl;
	else if ( gpaEquation > 2.5)                 
		cout << "Not bad, but you could work harder. Your GPA is " << gpaEquation << endl;
	else if (gpaEquation >= 2.0) 
		cout << "Put a bit more effort into it. Your GPA is " << gpaEquation << endl;
	else if (gpaEquation >= 0.0) 
		cout << "That's terrible! Do you even go to class? Your GPA is " << gpaEquation << endl;
	return gpaEquation;
}
//function to calculate the GPA for a semester
double semesterGpa(int n, const Course courses[],  string semester)
{
	double gradePoints = 0;
	double totalHours = 0;
	string name, time, number;
	char grade;
	int hours;
	for(int index = 0; index < n; index++)
	{
		courses[index].get(name, time, number, grade, hours);
		//checking the semester and adding up the grade points and credit hours
		if(time == semester)
		{
			if(grade == 'A')
				gradePoints = gradePoints + (hours * 4.0);
			else if(grade == 'B')
				gradePoints = gradePoints + (hours * 3.0);
			else if(grade == 'C')
				gradePoints = gradePoints + (hours * 2.0);
			else if(grade == 'D')
				gradePoints = gradePoints + (hours * 1.0);
			else if(grade == 'F')
				gradePoints = gradePoints + (hours * 0.0);

			if(grade != 'W' && grade != 'I')
				totalHours = totalHours + hours;
		}
	}
	double gpaEquation = 0;
	//calculating the GPA
	if(totalHours == 0)
		cout << " You cannot divide by 0." << endl;
	else
		gpaEquation = gradePoints / totalHours;
	//if statements to output the GPA
	if ( gpaEquation >= 3.0)
		cout << "Excellent! Your GPA is " << gpaEquation << endl;
	else if ( gpaEquation > 2.5)                 
		cout << "Not bad, but you could work harder. Your GPA is " << gpaEquation << endl;
	else if (gpaEquation >= 2.0) 
		cout << "Put a bit more effort into it. Your GPA is " << gpaEquation << endl;
	else if (gpaEquation >= 0.0) 
		cout << "That's terrible! Do you even go to class? Your GPA is " << gpaEquation << endl;
	
	return gpaEquation;
}
//function to sum the credit hours with letter grade D
int DRule(int n, Course courses[])
{
	int totalHours = 0;
	string name, time, number;
	char grade;
	int hours;
	//loop to sum the hours
	for(int index = 0; index < n; index++)
	{	
		courses[index].get(name, time, number, grade, hours);
		if(grade == 'D')
			totalHours += hours;
	}
	cout << "The total credit hours with the letter grade D is: " << totalHours << endl;
	return totalHours;
}
//function to print the courses
void print(int n, Course courses[])
{
	cout << n << endl;                                        
	//loop to output each input                        
	for(int index = 0; index < n; index++)                        
	{
		//printing the class
		courses[index].print();
	}
}
//function to read in a course
void getCourse(int n, Course& courses)
{
	string name, time, number;
	char grade;
	int hour;
	//Command to enter course name
	cout << "Enter course name for class " << n << endl;
	getline (cin, name);
	getline (cin, name);

	//Command to enter course semester
	cout << "Enter the semester for the course " << n <<  endl;        
	getline (cin, time);        

	//Command to enter course number        
	cout << "Enter course number for class " <<  n << endl;        
	getline (cin, number);        

	//Command to enter the letter grade                     
	cout << "Input letter grade(A, B, C, D, F, I, or W) of class " << n <<endl;        
	cin >> grade;        
	//Loop to check if the user input a valid letter        
	while (grade != 'A' && grade != 'B' && grade != 'C' && grade != 'D' && grade != 'F' && grade != 'W' && grade != 'I')
	{        
		cout << "You did not input a valid response, please input A, B, C, D, or F." << endl;    
        	cin >> grade;        
	}

	//Command to enter the credit hours        
	cout << "Input number of credit hours " << n << endl;        
	cin >> hour;
	//Loop to check if the user input a valid number of credit hours
	while (hour < 1 || hour > 5)
	{
		cout << "Your did not input a valid respose, please input 1, 2, 3, 4, or 5" << endl;
		cin >> hour;
	}
	courses.set(name, time, number, grade, hour);
}
//function to output the menu
char menu()
{
	char menuInput = 'w';
	//menu interface
	cout << "\n"
		"Welcome to the interactive menu-driven part of the GPA and Course storage program\n"
		"Please enter the character next to the choice you wish to pick.\n"
		"Here are your options:\n"
		"A(a) . Compute the GPA of all courses\n"
		"B(b) . List all courses\n"
		"C(c) . Compute the total credit hours of the courses with grade D\n"
		"D(d) . Compute the GPA for a particular semester\n"
		"E(e) . Add another course to the course list\n"
		"F(f) . Delete a course on the course list\n"
		"G(g) . Print University Requirements\n"
		"H(h) . Preform degree audit for University Core\n"
		"Q(q) . Quit the program\n"
		"\n"
		"\n"                    
		"Please choose one of the above\n";             
	cin >> menuInput;
	return menuInput;
}

//function to delete a course
void deleteCourse(int& n, Course courses[])   
{
	int choice;
	cout << "Select from 1 to " << n << " for the course to be deleted" << endl;
	cin >> choice;
	//error checking to make sure the user can only select valid courses
	while (choice < 1 || choice > n)
		cout << "Please select a number from 1 to " << n << "for the course to be deleted" << endl;
	cout << "Course number " << choice << endl;
	courses[choice - 1].print();
	cout << "Would you like to delete the course above?\n"
		"Please enter y for yes and n for no\n"; 
	char confirm = 'x';
	//confirming that the user wants to delete the course
	cin >> confirm;
	//error checking to make sure they input y or n
	while (confirm != 'y' && confirm != 'n')
	{
		cout << "You did not enter y or n\n"
			"Would you like to delete the course above?\n"
			"Please enter y for yes and n for no\n"; 
		cin >> confirm; 
	} 
	if (confirm == 'y')
	{
		n = n - 1;
		//loop to delete a course
		for(int i = choice - 1; i < n; i++)
		{
			courses[i] = courses[i + 1];
		}
		cout << "Here is the updated list of courses" << endl;
		print(n, courses);
	}
	if (confirm == 'n')
		cout << "You did not delete the course." << endl;
}
//main function
int main()
{
	const int SIZE = 200;
	int classes;
	Course courses[SIZE];
	string filename;
	bool fileTest;
	char fileChoice;
	int capacity = SIZE;
	Requirement requirements[SIZE];
	Requirement universityRequirements[SIZE];
	int numRequirements = 0;
	int numUniversityRequirements = 0;
	//whether the user wants to input their own file or not
        cout << "Would you like to use the default file to get the requirements?\n"
		"Please enter y for yes and n for no\n";
	cin >> fileChoice;
	//error checking to make sure the user inputs a y or n
	while (fileChoice != 'y' && fileChoice != 'n')
	{
		cout << "You did not enter a y or n\n"
			"Would you like to use the default file\n"
			"Please enter y for yes and n for no\n";
		cin >> fileChoice;
	}
	if (fileChoice == 'y')
	{
		filename = "requirements.txt";
		fileTest = readingRequirements(filename.c_str(), requirements, capacity, numRequirements);
		//stops the program if the file does not open
		if (fileTest == false)
		{
			cout << "Terminating program" << endl;
			return 1;
		}
	}
	else if (fileChoice == 'n')
	{
		cout << "Enter the file name" << endl;
		cin >> filename;
		fileTest = readingRequirements(filename.c_str(), requirements, capacity, numRequirements);
		if (fileTest == false)
		{
			cout << "Terminating program" << endl;
			return 1;
		}
	}
	getUniversityRequirement(requirements, universityRequirements, numRequirements, numUniversityRequirements);

	cout << "Would you like to read courses taken from a file?\n"
		"Please enter y for yes and n for no\n";
	cin >> fileChoice;
	//error checking to make sure the user inputs a y or n
	while (fileChoice != 'y' && fileChoice != 'n')
	{
		cout << "You did not enter a y or n\n"
			"Would you like to read courses taken from a file?\n"
			"Please enter y for yes and n for no\n";
		cin >> fileChoice;
	}
	if ( fileChoice == 'y')
	{
		cout << "Would you like to use the default file\n"
			"Please enter y for yes and n for no\n";
		cin >> fileChoice;
		//error checking to make sure the user inputs a y or n
		while (fileChoice != 'y' && fileChoice != 'n')
		{
			cout << "You did not enter a y or n\n"
				"Would you like to use the default file\n"
				"Please enter y for yes and n for no\n";
			cin >> fileChoice;
		}

		if (fileChoice == 'y')
		{
			filename = "courses.txt";
			fileTest = reading(filename.c_str(), courses, classes, capacity);
			//stops the program if the file does not open
			if (fileTest == false)
			{
				cout << "Terminating program" << endl;
				return 0;
			}
			else if (fileTest == true)
			{
				//printing out the courses
				print(classes, courses);
			}
		}
		else if (fileChoice == 'n')
		{
			cout << "Enter the file name" << endl;
			cin >> filename;
			fileTest = reading(filename.c_str(), courses, classes, capacity);
			if (fileTest == false)
			{
				cout << "Terminating program" << endl;
				return 0;
			}
			else if (fileTest == true)
			{	
				print(classes, courses);
			}
		}
	}
	else if (fileChoice == 'n')
	{
		//Command to input the number of classes to calculate
		cout << "Input how many classes you want to calculate." << endl;
		cin >> classes;
		while(classes > SIZE || classes < 1)
		{
			cout << "You cannot input 0 or a negative number.\n"
				"You cannot input more than 10 classes.\n"
				"Please input how many classes you want to calculate.\n";
			cin >> classes;
		}

		//Loop to input class information
		for(int i = 0; i < classes; i++)
		{
			getCourse(i + 1, courses[i]);
		}
	}
	char menuInput = 'w';
	while(menuInput != 'Q' && menuInput != 'q')
	{
		menuInput = menu();
		if( menuInput == 'A' || menuInput == 'a')
			gpa(classes, courses);
		else if(menuInput == 'B' || menuInput == 'b')
			print(classes, courses);
		else if(menuInput == 'C' || menuInput == 'c')
			DRule(classes, courses);
		else if(menuInput == 'D' || menuInput == 'd')
		{
			string semester;
			cout << "Please input the semester you want to calculate the GPA for." << endl;
			getline (cin, semester);
			getline (cin, semester);
			semesterGpa(classes, courses, semester);
		}
		else if(menuInput == 'E' || menuInput == 'e')
		{
			classes++;
			if(classes <= SIZE)
			{
				int index = classes - 1;
				getCourse(classes, courses[index]);
			}
			else 
			{
				classes--;
				cout << "You cannot enter more than " << SIZE << "  courses" << endl;
			}
		}
		else if(menuInput == 'F' || menuInput == 'f')
		{
			deleteCourse(classes, courses);
		}
		//printing the university requirements
		else if(menuInput == 'G' || menuInput == 'g')
		{
			for(int i = 0; i < numUniversityRequirements; i++)
			{
				universityRequirements[i].print();
			}
		}
		//performing the degree audit
		else if(menuInput == 'H' || menuInput == 'h')
		{
			Course uniqueCourses[SIZE];
			Course usedCourses[SIZE];
			int numUsedCourses = 0;
			int numUniqueCourses = 0;
			//get a list of unique courses
			getUniqueCourses(courses, uniqueCourses, classes, numUniqueCourses);
			bool validate = true;
			//performing the degree audit
			for(int i = 0; i < numUniversityRequirements; i++)
			{
				if(universityRequirements[i].validate(uniqueCourses, usedCourses, numUsedCourses, numUniqueCourses))
				{
					cout << "Passed requirement for ";
					universityRequirements[i].printSubgroup();
				}
				else
				{
					validate = false;
					cout << "You did not pass the requirement for ";
					universityRequirements[i].printSubgroup();
				}
			}
			if(validate)
				cout << "Congratulations, you passed all of the requirements" << endl;
			else
				cout << "You did not pass all fo the requirements" << endl;
		}
		else if(menuInput == 'Q' || menuInput == 'q')
		{
			filename = "courses.txt";
			writing(filename.c_str(), courses, classes);
			cout << "Program shutting down." << endl;
			writing("checking.txt", courses, classes);
		}
		else
			cout << "You did not enter an A, a, B, b, C, c, D, d, E, e, F, f, Q, or q, please try again" << endl;
			//making sure that the user enters a valid response
	}
	return 0;
}
