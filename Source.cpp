#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

const int MAX_STUDENTS = 20;
const int MAX_ADMINS = 10;

// ---Classes---
struct Student {
	string ID;
	string first_name;
	string second_name;
	string email;
	string password;

	// Constructor for easy initialization
	Student(string id, string fname, string sname, string em, string pwd)
		: ID(id), first_name(fname), second_name(sname), email(em), password(pwd) {
	}

	// Member function to display student info
	void display()  {
		cout << "ID: " << ID << "\nName: " << first_name << " "
			<< second_name << "\nEmail: " << email << "\n\n";
	}
};

vector<Student> loadStudentsFile(const string& filename) {
	vector<Student> students;
	ifstream file(filename);

	if (!file.is_open()) {
		cerr << "Error openning file ~" << filename << endl;
		return students;
	}

	string line;
	getline(file, line); // Skips the header row in the csv file

	while (getline(file, line)) {
		stringstream ss(line);
		string token;
		vector<string> tokens;

		while (getline(ss, token, ',')) {
			tokens.push_back(token);
		}
		if (tokens.size() == 5) {
			students.emplace_back(
				tokens[0], // ID
				tokens[1], // first name
				tokens[2], // second name
				tokens[3], // email
				tokens[4] // password
			);
		}

	}
	file.close();

	return students;
}

struct Admin {
	string ID[MAX_ADMINS] = {};
	string name[MAX_ADMINS] = { "Mohamed", "Salah", "Yassin" };
	string email[MAX_ADMINS] = {};
	string password[MAX_ADMINS] = { "admin123", "admin456", "admin789" };

};

// Global Function


// ---Functions--- //
void signInStudent(vector<Student>& students, string status);
void signInAdmin(vector<Student>& students, string status);

// ---Admin Functions--- //
void searchStudent(vector<Student>& students);
void addStudent(vector<Student>& students);
//void displayStudentInfo(vector<Student>& students ,int index);



//Main Program
int main() {

	vector<Student> students = loadStudentsFile("students.csv");

	cout << "Loaded " << students.size() << " students\n";


	


	cout << "\t >Student Information System< \t" << endl;
	cout << "\t (SIS) \t" << endl;
	cout << "\t***********************************" << endl;

	cout << "\t1 ->Log in(Student) \n";
	cout << "\t2 ->Log in(Admin) " << endl;

	int choice1; // To choose sign up or login.


	string logStatus; // To choose admin or student

	do
	{
		cin >> choice1;
		switch (choice1)
		{

		case 1:
			cout << "Log in as a student: ";
			logStatus = "Student";
			signInStudent(students, logStatus);
			break;

		case 2:
			cout << "Log in as an admin: ";
			logStatus = "Admin";
			signInAdmin(students, logStatus);
			break;

		default:
			cout << "Incorrect input! Try again \n";
			break;
		}
	} while (choice1 < 0 || choice1 > 2);

	return 0;
}

void signInStudent(vector<Student>& students, string status){
	string username;
	string pass;
	cout << "Enter your username: "; cin >> username;
	cout << "Enter your password: "; cin >> pass;

	bool found = false;

	for (int i = 0; i < MAX_STUDENTS; i++) {
		if (students[i].first_name == username) {
			found = true;
			if (students[i].password == pass) {
				cout << "Access Granted!" << endl;
				return;
			}
			else {
				cout << "Password is incorrect.";
			}

		}
	}

	if (!found) {
		cout << "Username is incorrect";
	}
}

void signInAdmin(vector<Student>& students, string status) {
	string username;
	string pass;
	cout << "Enter your username: "; 
	cin >> username;

	cout << "Enter your password: "; 
	cin >> pass;

	Admin admins;
	bool found = false;
	int choice = 0;

	for (int i = 0; i < MAX_STUDENTS; i++) {
		if (admins.name[i] == username) {
			found = true;
			if (admins.password[i] == pass) {
				cout << "Access Granted!" << endl;

				cout << "Welcome Admin, what do you wish to do? \n";
				cout << "1 -> Display Students\n";
				cout << "2 -> Search Student\n";

				do
				{
					cin >> choice;
					switch (choice)
					{
					case 1:
						cout << "---Display Students---\n";
						for (auto& student : students) {
							student.display();
						}
						break;

					case 2:
						cout << "---Search---\n";
						searchStudent(students);
						break;
					default:
						cout << "Incorrect input! Try again \n";
						break;
					}
				} while (choice < 0 || choice > 2);

			}
			else {
				cout << "Password is incorrect.\n";
			}

		}
	}

	if (!found) {
		cout << "Username is incorrect\n";
	}
}

void searchStudent(vector<Student>& students) {
	cout << "Search student by:\n";
	cout << "1 -> ID\n";
	cout << "2 -> Name\n";
	cout << "3 -> email\n";

	int choice = 0;
	string idToFind;
	string usertofind;
	string emailtofind;
	bool idfound = false;
	bool namefound = false;
	bool emailfound = false;

	do
	{
		cin >> choice;
		switch (choice)
		{
		case 1:
			   // ---> will be changed for efficiency
			cout << "Enter ID: ";
			cin >> idToFind;
			for (auto& student : students) {
				if (student.ID == idToFind) {
					idfound = true;
					student.display();
				}
			}
			if (!idfound) {
				cout << "ID cannot be found.";
			}
			break;

		case 2:
			cout << "Enter firstname: ";
			cin >> usertofind;
			for (auto& student : students) {
				if (student.first_name == usertofind) {
					namefound = true;
					student.display();
				}
			}
			
			if (!namefound) {
				cout << "User cannot be found.";
			}
			break;

		case 3:
			cout << "Enter email: ";
			cin >> emailtofind;
			for (auto& student : students) {
				if (student.email == idToFind) {
					emailfound = true;
					student.display();
				}
			}

			if (!emailfound) {
				cout << "User cannot be found.";
			}
			break;

			break;

		default:
			cout << "Incorrect input, try again.";
			break;
		}
	} while (choice < 0 || choice > 3);



}

void addStudent(vector<Student>& students) {

}