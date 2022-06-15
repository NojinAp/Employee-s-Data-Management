/*
----------------------------------
    Employee’s Data Management
----------------------------------
*/


#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <occi.h> 

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

struct Employee
{
	int employeeNumber;
	char lastName[50];
	char firstName[50];
	char email[100];
	char phone[50];
	char extension[10];
	int reportsTo[100];
	char jobTitle[50];
	char city[50];
};

//functions definition
int getint(const char* valid = nullptr);
int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp);
void displayEmployee(Connection* conn, struct Employee emp);
void insertEmployee(Connection* conn, struct Employee emp);
void updateEmployee(Connection* conn, int employeeNumber);
void deleteEmployee(Connection* conn, int employeeNumber);

//prompts the user to enter a valid integer
int getint(const char* valid)
{
	int select, i;
	if (valid)
	{
		cout << valid;
	}
	do
	{
		i = 0;
		cin >> select;
		if (cin.fail())
		{
			cout << "Invalid Entry, Please enter an integer: ";
			cin.clear();
			cin.ignore(10000, '\n');
			i = 1;
		}

		else
		{
			char newLine;
			newLine = cin.get();

			if (newLine != '\n')
			{
				cout << "Enter only an Integer: ";
				cin.clear();
				cin.ignore(10000, '\n');
				i = 1;
			}
			
		}

	} while (i == 1);
	return select;
}

int menu()
{
	cout << "******* HR Menu *******" << endl;
	cout << "1) Find Employee" << endl;
	cout << "2) Employees Report" << endl;
	cout << "3) Add Employee" << endl;
	cout << "4) Update Employee" << endl;
	cout << "5) Remove Employee" << endl;
	cout << "0) Exit" << endl;
	int option = getint("Enter a number from 0 to 5: ");
	return option;
}

//finds the information of a specific employee
int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp)
{
	Statement* statement = nullptr;
	ResultSet* result = nullptr;
	int flag;

	try
	{
		statement = conn->createStatement();
		result = statement->executeQuery("SELECT * FROM employees e, offices o WHERE employeenumber = " + to_string(employeeNumber) + "AND o.officecode = e.officecode");
		if (result->next())
		{
			emp->employeeNumber = result->getInt(1);

			strcpy(emp->lastName, result->getString(2).c_str());

			strcpy(emp->firstName, result->getString(3).c_str());

			strcpy(emp->email, result->getString(5).c_str());

			strcpy(emp->phone, result->getString(11).c_str());

			strcpy(emp->extension, result->getString(4).c_str());

			*emp->reportsTo = result->getInt(7);

			strcpy(emp->jobTitle, result->getString(8).c_str());

			strcpy(emp->city, result->getString(10).c_str());

			strcpy(emp->phone, result->getString(11).c_str());

			flag = 1;
		}
		else
		{
			flag = 0;
		}

		conn->terminateStatement(statement);
	}

	catch (SQLException& sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
	}

	return flag;
}

//shows the information an employee
void displayEmployee(Connection* conn, struct Employee emp)
{
	cout << endl;
	cout << "-------------- Employee Information -------------" << endl;
	cout << "Employee Number = " << emp.employeeNumber << endl;
	cout << "Last Name = " << emp.lastName << endl;
	cout << "First Name = " << emp.firstName << endl;
	cout << "Email = " << emp.email << endl;
	cout << "phone = " << emp.phone << endl;
	cout << "Extension = " << emp.extension << endl;
	cout << "reportsTo = " << *emp.reportsTo << endl;
	cout << "Job Title = " << emp.jobTitle << endl;
	cout << "city = " << emp.city << endl << endl;
}

//shows the information of all of the employees
void displayAllEmployees(Connection* conn)
{
	Statement* statement = nullptr;
	ResultSet* result = nullptr;
	try
	{
		statement = conn->createStatement();
		//specifying the query
		result = statement->executeQuery("SELECT e.employeenumber, e.firstname || ' '|| e.lastname, e.email, offices.phone, e.extension, m.firstname || ' ' || m.lastname FROM employees e JOIN offices ON offices.officecode = e.officecode LEFT JOIN employees m ON m.employeenumber = e.reportsto ORDER BY e.employeenumber");
		cout << endl;
		cout << "E	 Employee Name			Email			   Phone	  Ext	      Manager Name" << endl;
		cout << "-------------------------------------------------------------------------------------------------------------------" << endl;

		if (result->next())
		{
			do
			{
				cout.setf(ios::left);
				cout.width(9);
				cout << result->getInt(1);
				cout.width(19);
				cout << result->getString(2);
				cout.width(35);
				cout << result->getString(3);
				cout.width(18);
				cout << result->getString(4);
				cout.width(13);
				cout << result->getString(5);
				cout.width(17);
				cout << result->getString(6);
				cout.unsetf(ios::left);
				cout << "\n";

			} while (result->next());
			cout << "\n";
		}

		else
		{
			cout << "There is no employees’ information to be displayed." << "\n";
		}
		conn->terminateStatement(statement);
	}

	catch (SQLException& sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << "\n";
	}
}

//inserts the information of an employee into the table
void insertEmployee(Connection* conn, struct Employee emp)
{
	Statement* statement = nullptr;
	ResultSet* result = nullptr;
	try
	{
		cout << "Last Name: ";
		cin >> emp.lastName;

		cout << "First Name: ";
		cin >> emp.firstName;

		cout << "Email: ";
		cin >> emp.email;
		
		cout << "extension: ";
		cin >> emp.extension;

		cout << "Job Title: ";
		cin >> emp.jobTitle;

		cout << "City: ";
		cin >> emp.city;

		statement = conn->createStatement();
		//specifying the query
		result = statement->executeQuery("INSERT INTO employees (employeeNumber, lastName, firstName, extension, email, officeCode, reportsTo, jobTitle) VALUES ( " + to_string(emp.employeeNumber) + ", '" + emp.lastName + "'" + ", '" + emp.firstName + "', '" + emp.extension + "'" + ", '" + emp.email + "'" + ", 1" + ", 1002" + ", '" + emp.jobTitle + "')");
		cout << "The new employee is added successfully." << endl << endl;
		conn->terminateStatement(statement);
	}

	catch (SQLException& sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << "\n";
	}
}

//updates the information of an employee
void updateEmployee(Connection* conn, int employeeNumber) {
	Statement* statement = nullptr;
	ResultSet* result = nullptr;
	Employee emp;

	try
	{
		statement = conn->createStatement();
		cout << "New Extension: ";
		cin >> emp.extension;
		cin.clear();
		string query = "UPDATE employees SET extension = '";
		query.append(emp.extension);
		query.append("' WHERE employeeNumber = " + to_string(employeeNumber));

		//specifying the query
		result = statement->executeQuery(query);
		cout << "The employee's extension is updated successfully." << endl << endl;
		conn->terminateStatement(statement);

	}

	catch (SQLException& sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << "\n";
	}
}

//deletes the information of an employee
void deleteEmployee(Connection* conn, int employeeNumber) {
	Statement* statement = nullptr;
	ResultSet* result = nullptr;
	Employee emp;

	try
	{
		statement = conn->createStatement();
		//specifying the query
		result = statement->executeQuery("DELETE FROM employees WHERE employeeNumber = " + to_string(employeeNumber));
		cout << "The employee is deleted." << endl << endl;
		conn->terminateStatement(statement);

	}

	catch (SQLException& sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << "\n";
	}
}

int main()
{
	Environment* environment = nullptr;
	Connection* connection = nullptr;
	string db_name = "DBS211_ms2";
	string user = "dbs211_221za02";
	string password = "19589165";
	string server = "myoracle12c.senecacollege.ca:1521/oracle12c";

	try
	{
		environment = Environment::createEnvironment(Environment::DEFAULT);
		connection = environment->createConnection(user, password, server);
		Employee employee{};
		int selection;
		do
		{
			selection = menu();
			if (selection > 5 || selection < 0)
			{
				cout << "Value must be between 0 and 5" << endl << endl;
			}

			else if (selection == 1)
			{
				int empNu = getint("Enter Employee Number: ");
				int val = findEmployee(connection, empNu, &employee);

				if (val != 0)
				{
					displayEmployee(connection, employee);
				}

				else
				{
					cout << "Employee " << empNu << " does not exist." << endl << endl;
				}
			}

			else if (selection == 2)
			{
				displayAllEmployees(connection);
			}

			else if (selection == 3) 
			{
				int empNu = getint("Employee Number: ");
				int val = findEmployee(connection, empNu, &employee);
				employee.employeeNumber = empNu;

				if (val == 0) 
				{
					insertEmployee(connection, employee);
				}

				else
				{
					cout << "An employee with the same employee number exists." << endl << endl;
				}
			}

			else if (selection == 4) 
			{
				int empNu = getint("Employee Number: ");
				int val = findEmployee(connection, empNu, &employee);
				employee.employeeNumber = empNu;

				if (val == 0) 
				{
					cout << "Employee number does not exist." << endl << endl;
				}

				else 
				{
					updateEmployee(connection, employee.employeeNumber);
				}
			}

			else if (selection == 5)
			{
				int empNu = getint("Employee Number: ");
				int val = findEmployee(connection, empNu, &employee);
				employee.employeeNumber = empNu;

				if (val == 0) 
				{
					cout << "The employee does not exist." << endl << endl;
				}

				else 
				{
					deleteEmployee(connection, employee.employeeNumber);
				}
			}

		} while (selection != 0);
	}
	catch (SQLException& sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
	environment->terminateConnection(connection);
	Environment::terminateEnvironment(environment);

	return 0;
}