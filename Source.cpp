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

struct Admin {
	string ID;
	string first_name;
	string second_name;
	string email;
	string password;
	string title;

	// Constructor for easy initialization
	Admin(string id, string fname, string sname, string em, string pwd, string ti)
		: ID(id), first_name(fname), second_name(sname), email(em), password(pwd), title(ti) {
	}


	Admin() = default;
};

//Loading files
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

	vector<Admin> loadAdminsFile(const string& filename) {
		vector<Admin> admins;
		ifstream file(filename);

		if (!file.is_open()) {
			cerr << "Error openning file ~" << filename << endl;
			return admins;
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
			if (tokens.size() == 6) {
				admins.emplace_back(
					tokens[0], // ID
					tokens[1], // first name
					tokens[2], // second name
					tokens[3], // email
					tokens[4], // password
					tokens[5] // title ex: "Professor"
				);
			}

		}
		file.close();

		return admins;
	}

// ---Functions--- //
void mainPage(vector<Student>& students);
void adminSignIn(vector<Admin>& admins, vector<Student>& students, string status);
void studentSignIn(vector<Student>& students, string status);
void studentPanel(vector<Student>& students, string status);
bool adminPanel(vector<Admin>& admins,vector<Student>& students, string status);



// ---Admin Functions--- //
bool searchStudent(vector<Student>& students);
void addStudent(vector<Student>& students);

//Main Program
int main() {
	vector<Student> students = loadStudentsFile("students.csv");
	vector<Admin> admins = loadAdminsFile("admins.csv");

	while (true) {
		cout << "\t >Student Information System< \t" << endl;
		cout << "\t (SIS) \t" << endl;
		cout << "\t***********************************" << endl;
		cout << "\t1 -> Log in(Student) \n";
		cout << "\t2 -> Log in(Admin) \n\n";
		cout << "\t0 -> Exit" << endl;

		int choice1;
		cin >> choice1;

		if (choice1 == 0) break;

		switch (choice1) {
		case 1:
			studentSignIn(students, "Student");
			break;
		case 2:
			adminSignIn(admins, students, "Admin");
			break;
		default:
			cout << "Incorrect input! Try again \n";
			break;
		}
	}
	return 0;
}

void mainPage(vector<Student>& students) {
	
}

void studentSignIn(vector<Student>& students, string status) {
	while (true) {
		string username;
		string pass;

		cout << "\n--- Student Sign In ---\n";
		cout << "Enter your username (or '0' to go back): ";
		cin >> username;

		// Check if user wants to go back
		if (username == "0") {
			return; // Return to main menu
		}

		cout << "Enter your password: ";
		cin >> pass;

		bool loggedIn = false;
		for (auto& student : students) {
			if (student.first_name == username && student.password == pass) {
				loggedIn = true;
				cout << "\nWelcome " << student.first_name << " " << student.second_name << endl;
				studentPanel(students, status);
				return; // Return after successful login and panel exit
			}
		}

		if (!loggedIn) {
			cout << "\nIncorrect username or password.\n";
			cout << "1 -> Try again\n";
			cout << "0 -> Go back\n";

			int retryChoice;
			cin >> retryChoice;

			if (retryChoice == 0) {
				return; // Go back to main menu
			}
			// Otherwise loop continues for another try
		}
	}
}

void adminSignIn(vector<Admin>& admins, vector<Student>& students, string status) {
	while (true) {
		string username;
		string pass;

		cout << "\n--- Admin Sign In ---\n";
		cout << "Enter your username (or '0' to go back): ";
		cin >> username;

		// Check if user wants to go back
		if (username == "0") {
			return; // Return to main menu
		}

		cout << "Enter your password: ";
		cin >> pass;

		bool loggedIn = false;
		for (auto& admin : admins) {
			if (admin.first_name == username && admin.password == pass) {
				loggedIn = true;
				cout << "\nWelcome " << admin.first_name << " " << admin.second_name << endl;
				adminPanel(admins, students, status);
				return; // Return after successful login and panel exit
			}
		}

		if (!loggedIn) {
			cout << "\nIncorrect username or password.\n";
			cout << "1 -> Try again\n";
			cout << "0 -> Go back\n";

			int retryChoice;
			cin >> retryChoice;

			if (retryChoice == 0) {
				return; // Go back to main menu
			}
			// Otherwise loop continues for another try
		}
	}
}

void studentPanel(vector<Student>& students, string status){
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

	
}

bool adminPanel(vector<Admin>& admins, vector<Student>& students, string status) {
	while (true) {  // Main admin panel loop
		cout << "\n=== ADMIN PANEL ===\n";
		cout << "1 -> Display All Students\n";
		cout << "2 -> Search Student\n";
		cout << "3 -> Add New Student\n";
		cout << "\n0 -> Log Out\n";
		cout << "Enter your choice: ";

		int choice;
		cin >> choice;

		switch (choice) {
		case 1:  // Display all students
			cout << "\n--- ALL STUDENTS ---\n";
			if (students.empty()) {
				cout << "No students found.\n";
			}
			else {
				for (auto& student : students) {
					student.display();
				}
			}
			break;

		case 2:  // Search student
			searchStudent(students);  // Will handle its own back navigation
			break;

		case 3:  // Add new student
			addStudent(students); // Will handle its own back navigation
			break;

		case 0:  // Log out
			cout << "\nLogging out...\n";
			return true;  // Exits admin panel and returns to main menu

		default:
			cout << "Invalid choice! Please try again.\n";
			break;
		}

		// Pause before showing menu again (except when coming back from search)
		if (choice != 2) {
			cout << "\nPress Enter to continue...";
			cin.ignore();
			cin.get();
		}
	}
}

bool searchStudent(vector<Student>& students) {
	while (true) {  // Main loop for the search menu
		cout << "\n--- Search Student ---\n";
		cout << "1 -> Search by ID\n";
		cout << "2 -> Search by Name\n";
		cout << "3 -> Search by Email\n";
		cout << "\n0 -> Back to Admin Panel\n";
		cout << "\nEnter your choice: ";

		int choice;
		cin >> choice;

		if (choice == 0) {
			return true;  // Signal to go back to admin panel
		}

		string searchTerm;
		bool found = false;

		switch (choice) {
		case 1:  // Search by ID
			cout << "Enter student ID: ";
			cin >> searchTerm;
			for (auto& student : students) {
				if (student.ID == searchTerm) {
					found = true;
					cout << "\n--- Student Found ---\n";
					student.display();
				}
			}
			break;

		case 2:  // Search by Name
			cout << "Enter student first name: ";
			cin >> searchTerm;
			for (auto& student : students) {
				if (student.first_name == searchTerm) {
					found = true;
					cout << "\n--- Student Found ---\n";
					student.display();
				}
			}
			break;

		case 3:  // Search by Email
			cout << "Enter student email: ";
			cin >> searchTerm;
			for (auto& student : students) {
				if (student.email == searchTerm) {
					found = true;
					cout << "\n--- Student Found ---\n";
					student.display();
				}
			}
			break;

		default:
			cout << "Invalid choice! Please try again.\n";
			continue;  // Skip the rest and show menu again
		}

		if (!found && (choice >= 1 && choice <= 3)) {
			cout << "No student found with matching criteria.\n";
		}

		// After search results, give option to search again or go back
		cout << "\n1 -> Search again\n";
		cout << "0 -> Back to Admin Panel\n";
		cout << "Enter your choice: ";

		int continueChoice;
		cin >> continueChoice;

		if (continueChoice == 0) {
			return true;  // Signal to go back to admin panel
		}
	}
}



void addStudent(vector<Student>& students) {
	while (true) {  // Main loop for retry capability
		cout << "\n--- ADD NEW STUDENT ---\n";
		cout << "(Enter '0' at any time to cancel)\n";

		// 1. Get Input with Validation
		string id, fname, sname, email, pwd, pwdConfirm;

		// ID Input with duplicate validation
		bool idValid = false;
		while (!idValid) {
			cout << "Student ID: ";
			cin >> id;

			if (id == "0") return; // To cancel

			bool duplicatefound = false;
			for (const auto& student : students) {
				if (student.ID == id) {
					duplicatefound = true;
					break;
				}
			}
			if (duplicatefound) {
				cerr << "Error, ID already exists!\n";
				cout << "1. Try again\n";
				cout << "0. Cancel\n";
				
				int retryChoice;
				cin >> retryChoice;

				if (retryChoice == 0) return; // if 1, it will continue
			}
			else {
				idValid = true;
			}

		}

		// Name Input
		cout << "First Name: ";
		cin >> fname;
		if (fname == "0") return;

		cout << "Last Name: ";
		cin >> sname;
		if (sname == "0") return;

		// Email Input with Validation
		while (true) {
			cout << "Email: ";
			cin >> email;
			if (email == "0") return;

			if (email.find('@') == string::npos || email.find('.') == string::npos) {
				cout << "Invalid email format! (Must contain @ and .)\n";
				continue;
			}

			// Check for duplicate email
			bool emailExists = false;
			for (const auto& s : students) {
				if (s.email == email) {
					emailExists = true;
					break;
				}
			}
			if (!emailExists) break;
			cout << "!!Email already registered!\n";
		}

		// Password Input
		while (true) {
			cout << "Password: ";
			cin >> pwd;
			if (pwd == "0") return;
			break;
		}

		// 2. Save to Memory and File
		students.emplace_back(id, fname, sname, email, pwd);

		ofstream file("students.csv", ios::app);
		if (file.is_open()) {
			if (file.tellp() == 0) {  // If empty file
				file << "ID,FirstName,LastName,Email,Password\n";
			}
			file << id << "," << fname << "," << sname << ","
				<< email << "," << pwd << "\n";
			cout << " --> Student added successfully!\n";
		}
		else {
			cerr << "!!Error saving to file! Student added to current session only.\n";
		}
		file.close();

		// 3. Ask to add another
		cout << "\nAdd another student?\n";
		cout << "1. Yes\n";
		cout << "0. No (return to admin panel)\n";
		int choice;
		cin >> choice;
		if (choice == 0) break;
	}
}
