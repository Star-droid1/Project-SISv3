#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const int MAX_STUDENTS = 500;
const int MAX_ADMINS = 10;

// ---Classes---
struct Course {
	string courseCode;
	string courseName;
	float gpa;          // 0.0 to 4.0 scale
	int creditHours;
};
struct Student {
	string ID, national_id, gender, first_name, second_name, email, phone, password, program;
	vector<Course> courses;  // Each student's courses

	Student(string id, string nid, string g, string fn, string sn,string em, string ph, string pwd, string prgrm)
		: ID(id), national_id(nid), gender(g), first_name(fn),
		second_name(sn), email(em), phone(ph), password(pwd), program(prgrm) {
	}


	void display() {
		cout << "Student ID: " << ID << "\n"
			<< "Name: " << first_name << " " << second_name << "\n"
			<< "Program: " << program << "\n"
			<< "Email: " << email << "\n"
			<< "Phone Number: " << phone << '\n'
			<< "Courses:\n";

		for (const auto& course : courses) {
			cout << "- " << course.courseCode << " (" << course.courseName << "): "
				<< "GPA: " << course.gpa << ", Credits: " << course.creditHours << "\n";
		}
	}

	float calculateOverallGPA() {
		float totalPoints = 0;
		int totalCredits = 0;
		for (const auto& course : courses) {
			totalPoints += course.gpa * course.creditHours;
			totalCredits += course.creditHours;
		}
		return totalCredits > 0 ? totalPoints / totalCredits : 0.0f;
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
		cerr << "Error opening file: " << filename << endl;
		return students;
	}

	string line;
	getline(file, line); // Skip header row

	while (getline(file, line)) {
		stringstream ss(line);
		string token;
		vector<string> tokens;

		// Split line by commas
		while (getline(ss, token, ',')) {
			tokens.push_back(token);
		}

		if (tokens.size() >= 9) {
			// Construct Student with all basic info
			Student student(
				tokens[0], // ID
				tokens[1], // national_id
				tokens[2], // gender
				tokens[3], // first_name
				tokens[4], // second_name
				tokens[5], // email
				tokens[6], // phone
				tokens[7],  // password
				tokens[8] // program
			);

			// Load courses if they exist (9th column)
			if (tokens.size() > 9 && !tokens[9].empty()) {
				stringstream coursesStream(tokens[9]);
				string courseEntry;

				// Split course entries by semicolon
				while (getline(coursesStream, courseEntry, ';')) {
					stringstream courseStream(courseEntry);
					string coursePart;
					vector<string> courseParts;

					// Split course details by colon
					while (getline(courseStream, coursePart, ':')) {
						courseParts.push_back(coursePart);
					}

					if (courseParts.size() == 4) {
						Course course;
						course.courseCode = courseParts[0];
						course.courseName = courseParts[1];
						course.gpa = stof(courseParts[2]);
						course.creditHours = stoi(courseParts[3]);
						student.courses.push_back(course);
					}
				}
			}

			students.push_back(student);
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
void studentPanel(Student& student);
bool adminPanel(vector<Admin>& admins,vector<Student>& students, string status);
void saveStudentsToFile(vector<Student>& students, const string& filename);
void viewStudentCourses(Student& student);



// ---Admin Functions--- //
bool searchStudent(vector<Student>& students);
void addStudent(vector<Student>& students);
bool deleteStudent(Student& student, vector<Student>& students);
void modifyStudent(Student& student, vector<Student>& students);
void addCourseToStudent(Student& student, vector<Student>& students, const string& filename);
void manageCourses(Student& student, vector<Student>& students, const string& filename);

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

void studentSignIn(vector<Student>& students, string status) {
	while (true) {
		string email;
		string pass;

		cout << "\n--- Student Sign In ---\n";
		cout << "Enter your email (or '0' to go back): ";
		cin >> email;

		if (email == "0") {
			return;
		}

		cout << "Enter your password: ";
		cin >> pass;

		bool loggedIn = false;
		for (auto& student : students) {
			if (student.email == email && student.password == pass) {
				loggedIn = true;
				cout << "\nWelcome " << student.first_name << " " << student.second_name << endl;
				studentPanel(student);  // Pass the specific student to the panel
				return;
			}
		}

		if (!loggedIn) {
			cout << "\nIncorrect username or password.\n";
			cout << "1 -> Try again\n";
			cout << "0 -> Go back\n";

			int retryChoice;
			cin >> retryChoice;

			if (retryChoice == 0) {
				return;
			}
		}
	}
}

void adminSignIn(vector<Admin>& admins, vector<Student>& students, string status) {
	while (true) {
		string email;
		string pass;

		cout << "\n--- Admin Sign In ---\n";
		cout << "Enter your email address (or '0' to go back): ";
		cin >> email;

		// Check if user wants to go back
		if (email == "0") {
			return; // Return to main menu
		}

		cout << "Enter your password: ";
		cin >> pass;

		bool loggedIn = false;
		for (auto& admin : admins) {
			if (admin.email == email && admin.password == pass) {
				loggedIn = true;
				cout << "\nWelcome " << admin.first_name << " " << admin.second_name << endl;
				adminPanel(admins, students, status);
				return; // Return after successful login and panel exit
			}
		}
		if (!loggedIn) {
			cout << "\nIncorrect email addresss or password.\n";
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

void studentPanel(Student& student) {
	while (true) {
		cout << "\n=== STUDENT PANEL ===\n";

		cout << "1 -> View My Information\n";
		cout << "2 -> View My Courses\n";
		cout << "3 -> View My GPA\n";
		cout << "0 -> Log Out\n";
		cout << "Choice: ";

		int choice;
		cin >> choice;

		switch (choice) {
		case 1:  // View all information
			cout << "\n--- STUDENT INFORMATION ---\n";
			cout << "Student ID: " << student.ID << "\n";
			cout << "Name: " << student.first_name << " " << student.second_name << '\n';
			cout << "Program: " << student.program << '\n';
			cout << "National ID: " << student.national_id << '\n';
			cout << "Gender: " << student.gender << '\n';
			cout << "Email: " << student.email << '\n';
			cout << "Phone: " << student.phone << '\n';
			break;

		case 2:  // View courses
			viewStudentCourses(student);
			break;

		case 3:  // View GPA
			cout << "\nYour Overall GPA: " << student.calculateOverallGPA() << "\n";
			break;

		case 0:  // Log out
			cout << "\nLogging out...\n";
			return;

		default:
			cout << "Invalid choice! Please try again.\n";
		}

		cout << "\nPress Enter to continue...";
		cin.ignore();
		cin.get();
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

// Admin Functions
bool searchStudent(vector<Student>& students) {
	while (true) {
		cout << "\n--- Search Student ---\n";
		cout << "1. Search by ID\n";
		cout << "2. Search by Name\n";
		cout << "3. Search by Email\n";
		cout << "0. Back to Admin Panel\n";
		cout << "Choice: ";

		int choice;
		cin >> choice;
		cin.ignore(); // Clear input buffer

		if (choice == 0) return true;

		string searchTerm;
		vector<Student*> matches;

		switch (choice) {
		case 1: // Search by ID
			cout << "Enter student ID: ";
			getline(cin, searchTerm);
			for (auto& student : students) {
				if (student.ID == searchTerm) {
					matches.push_back(&student);
				}
			}
			break;

		case 2: // Search by Name
			cout << "Enter student name: ";
			getline(cin, searchTerm);
			for (auto& student : students) {
				string fullName = student.first_name + " " + student.second_name;
				if (fullName.find(searchTerm) != string::npos) {
					matches.push_back(&student);
				}
			}
			break;

		case 3: // Search by Email
			cout << "Enter student email: ";
			getline(cin, searchTerm);
			for (auto& student : students) {
				if (student.email == searchTerm) {
					matches.push_back(&student);
				}
			}
			break;

		default:
			cout << "Invalid choice!\n";
			continue;
		}

		if (matches.empty()) {
			cout << "No matching students found.\n";
			cout << "1. Search again\n";
			cout << "0. Back to Admin Panel -->| |\n";

			int retryChoice;
			cin >> retryChoice;
			if (retryChoice == 0) return true;
			continue;
		}

		// Display all matches
		cout << "\n--- Search Results (" << matches.size() << ") ---\n";
		for (size_t i = 0; i < matches.size(); i++) {
			cout << "[" << i + 1 << "] ";
			matches[i]->display();
			cout << "-----------------\n";
		}

		// Student selection
		cout << "\nSelect student (1-" << matches.size() << ") or 0 to search again: ";
		int selection;
		cin >> selection;
		cin.ignore();

		if (selection == 0) continue;
		if (selection < 1 || selection > matches.size()) {
			cout << "Invalid selection!\n";
			continue;
		}

		Student* selectedStudent = matches[selection - 1];

		// Action menu
		while (true) {
			cout << "\n--- Student Options ---\n";
			selectedStudent->display();
			cout << "\n1. Modify Student\n";
			cout << "2. Delete Student\n";
			cout << "3. Search Again\n";
			cout << "4. Manage Courses\n";
			cout << "0. Back to Admin Panel\n";
			cout << "Choice: ";

			int actionChoice;
			cin >> actionChoice;
			cin.ignore();

			switch (actionChoice) {
			case 1: // Modify
				modifyStudent(*selectedStudent, students);
				break;
			case 2: // Delete
				if (deleteStudent(*selectedStudent, students)) {
					return true; // Return to admin panel after deletion
				}
				break;
			case 3: // Search Again
				goto search_again; // Break out to restart search
			case 4:
				manageCourses(*selectedStudent, students, "students.csv");
				break;
			case 0: // Back
				return true;
			default:
				cout << "Invalid choice!\n";
				continue;
			}
			break;
		}

	search_again:;
	}
}

void addStudent(vector<Student>& students) {
	while (true) {
		cout << "\n--- ADD NEW STUDENT ---\n";
		cout << "(Enter '0' at any time to cancel)\n";

		// 1. Get Input with Validation
		string id, national_id, fname, sname,gender, email, phone, pwd, prgrm;

		// ID Input with duplicate validation
		bool idValid = false;
		while (!idValid) {
			cout << "Student ID: ";
			cin >> id;
			if (id == "0") return;

			bool duplicateFound = false;
			for (const auto& student : students) {
				if (student.ID == id) {
					duplicateFound = true;
					break;
				}
			}
			if (duplicateFound) {
				cerr << "Error, ID already exists!\n";
				cout << "1. Try again\n";
				cout << "0. Cancel\n";

				int retryChoice;
				cin >> retryChoice;
				if (retryChoice == 0) return;
			}
			else {
				idValid = true;
			}
		}

		// National ID input
		bool nationalidValid = false;
		while (!nationalidValid) {
			cout << "National ID: ";
			cin >> national_id;
			if (id == "0") return;

			bool duplicateFound = false;
			for (const auto& student : students) {
				if (student.national_id == national_id) {
					duplicateFound = true;
					break;
				}
			}
			if (duplicateFound) {
				cerr << "Error, National ID already exists!\n";
				cout << "1. Try again\n";
				cout << "0. Cancel\n";

				int retryChoice;
				cin >> retryChoice;
				if (retryChoice == 0) return;
			}
			else {
				nationalidValid = true;
			}
		}


		// Name Input
		cout << "First Name: ";
		cin >> fname;
		if (fname == "0") return;

		cout << "Last Name: ";
		cin >> sname;
		if (sname == "0") return;

		// Gender Input
		cout << "Enter Gender (M/F): ";
		bool validgender = false;
		while (!validgender) {
			cin >> gender;
			if (gender != "M" && gender != "F") {  // Changed || to &&
				cerr << "Wrong Input:\n";
				cout << "1. Try again\n";
				cout << "0. Cancel\n";
				int retryChoice;
				cin >> retryChoice;
				if (retryChoice == 0) return;
			}
			else {
				validgender = true;
			}
		}


		// Email Input
		cout << "Email: ";
		cin >> email;
		if (email == "0") return;

		// Phone Input
		while (true) {
			cout << "Phone Number (11 digits): ";
			cin >> phone;
			if (phone == "0") return;

			bool valid = true;
			// Check if all characters are digits
			for (char c : phone) {
				if (!isdigit(c)) {
					valid = false;
					break;
				}
			}

			// Check length
			if (!valid || phone.length() != 11) {
				cout << " Invalid phone number! Must be exactly 11 digits.\n";
				cout << "1. Try again\n";
				cout << "0. Cancel\n";

				int retryChoice;
				cin >> retryChoice;

				if (retryChoice == 0) return;
				continue;  // Will restart the phone input loop
			}
			break;  // Valid phone number entered
		}

		// Password Input
		cout << "Password: ";
		cin >> pwd;
		if (pwd == "0") return;

		// Program Input
		cout << "Program: ";
		cin >> prgrm;
		if (prgrm == "0") return;

		// 2. Save to Memory and File
		students.emplace_back(
			id,          // ID
			national_id, // national_id 
			gender,      // gender
			fname,       // first_name
			sname,       // second_name
			email,       // email
			phone,       // phone
			pwd,         // password
			prgrm		// program
		);

		ofstream file("students.csv", ios::app);
		if (file.is_open()) {
			if (file.tellp() == 0) {
				file << "ID,NationalID,Gender,FirstName,LastName,Email,Phone,Password,Courses\n";
			}
			file << id << ","
				<< national_id << ","
				<< gender << ","
				<< fname << ","
				<< sname << ","
				<< email << ","
				<< phone << ","
				<< pwd << "," // Empty courses for new student
				<< prgrm << ",\n";
			cout << "Student added successfully!\n";
		}
		else {
			cerr << "Error saving to file! Student added to current session only.\n";
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

bool deleteStudent(Student& student, vector<Student>& students) {
	cout << "\nCONFRIM DELETION OF: \n";
	student.display();
	cout << "\n!!This cannot be undone. Proceed?(1-Yes, 2-No)";

	int confirm;
	cin >> confirm;

	if (confirm == 1) {
		students.erase(remove_if(students.begin(), students.end(),
			[&](const Student& s) { return s.ID == student.ID; }),
			students.end());

		saveStudentsToFile(students, "students.csv");
		cout << "Student deleted successfully!\n";
		return true;
	}
	return false;
}

void modifyStudent(Student& student, vector<Student>& students) {
	while (true) {
		cout << "\n--- Modify Student " << student.ID << " ---\n";
		cout << "Current Information:\n";
		student.display();

		cout << "\nSelect field to modify:\n";
		cout << "1. National ID\n";
		cout << "2. Gender\n";
		cout << "3. First Name\n";
		cout << "4. Last Name\n";
		cout << "5. Email\n";
		cout << "6. Phone\n";
		cout << "7. Password\n";
		cout << "8. Program\n";
		cout << "9. Save Changes\n";
		cout << "0. Cancel Without Saving\n";
		cout << "Choice: ";

		int choice;
		cin >> choice;
		cin.ignore(); // Clear newline

		if (choice == 0) {
			cout << "Modification cancelled.\n";
			return;
		}
		if (choice == 9) break;

		string newValue;
		cout << "Current: ";
		switch (choice) {
		case 1: cout << student.national_id; break;
		case 2: cout << student.gender; break;
		case 3: cout << student.first_name; break;
		case 4: cout << student.second_name; break;
		case 5: cout << student.email; break;
		case 6: cout << student.phone; break;
		case 7: cout << student.password; break;
		case 8: cout << student.program; break;
		default: break;
		}
		cout << "\nEnter new value: ";
		getline(cin, newValue);

		// Validate and update field
		bool valid = true;
		switch (choice) {
		case 1: // National ID
			student.national_id = newValue;
			break;

		case 2: // Gender
			if (newValue == "M" || newValue == "F") {
				student.gender = newValue;
			}
			else {
				cout << "Invalid gender! Use M/F\n";
				valid = false;
			}
			break;

		case 3: // First Name
			student.first_name = newValue;
			break;

		case 4: // Last Name
			student.second_name = newValue;
			break;

		case 5: // Email
			if (newValue.find("@eng.asu.edu.eg") != string::npos) {
				// Check for duplicate email
				bool emailExists = false;
				for (const auto& s : students) {
					if (s.email == newValue && s.ID != student.ID) {
						emailExists = true;
						break;
					}
				}
				if (!emailExists) {
					student.email = newValue;
				}
				else {
					cout << "Email already in use by another student!\n";
					valid = false;
				}
			}
			else {
				cout << "Email must end with @eng.asu.edu.eg\n";
				valid = false;
			}
			break;

		case 6: // Phone
			if (newValue.length() == 11 &&
				all_of(newValue.begin(), newValue.end(), ::isdigit)) {
				student.phone = newValue;
			}
			else {
				cout << "Phone must be 11 digits\n";
				valid = false;
			}
			break;

		case 7: // Password
			student.password = newValue;
			break;
		case 8:
			student.program = newValue;
			break;

		default:
			cout << "Invalid choice!\n";
			valid = false;
		}

		if (valid) {
			cout << "Field updated successfully!\n";
		}
		else {
			cout << "Field not updated. Please try again.\n";
		}
	}

	// Save changes to file
	saveStudentsToFile(students, "students.csv");
	cout << "All changes saved to file!\n";
}

void saveStudentsToFile(vector<Student>& students, const string& filename) {
	ofstream file(filename);
	if (file.is_open()) {
		// Write header
		file << "ID,NationalID,Gender,FirstName,LastName,Email,Phone,Password,Courses\n";

		for (const auto& s : students) {
			// Write basic student info
			file << s.ID << ","
				<< s.national_id << ","
				<< s.gender << ","
				<< s.first_name << ","
				<< s.second_name << ","
				<< s.email << ","
				<< s.phone << ","
				<< s.password << ","
				<< s.program << ",";

			// Write courses in format: code:name:gpa:credits;code:name:gpa:credits
			for (size_t i = 0; i < s.courses.size(); i++) {
				const auto& course = s.courses[i];
				file << course.courseCode << ":"
					<< course.courseName << ":"
					<< course.gpa << ":"
					<< course.creditHours;

				if (i != s.courses.size() - 1) {
					file << ";";
				}
			}
			file << "\n";
		}
	}
	else {
		cerr << "Error saving changes to file!\n";
	}
}

void addCourseToStudent(Student& student, vector<Student>& students, const string& filename) {
	Course newCourse;
	cout << "Enter course code: ";
	cin >> newCourse.courseCode;
	cout << "Enter course name: ";
	cin.ignore();
	getline(cin, newCourse.courseName);
	// Validate gpa input
	while (true) {
		cout << "Enter GPA (0.0-4.0): ";
		cin >> newCourse.gpa;
		if (newCourse.gpa >= 0.0 && newCourse.gpa <= 4.0) {
			break;
		}
		cout << "Invalid GPA! Must be between 0.0 and 4.0\n";
	}
	// Validate credit hours input
	while (true) {
		cout << "Enter credit hours: ";
		cin >> newCourse.creditHours;
		if (newCourse.creditHours > 0) {
			break;
		}
		cout << "Invalid input, credit hours must be positive.";
	}

	student.courses.push_back(newCourse);
	cout << "Course added successfully!\n";

	// Save the changes immediately
	saveStudentsToFile(students, filename);
}

void manageCourses(Student& student, vector<Student>& students, const string& filename) {
	while (true) {
		cout << "\nCOURSE MANAGEMENT: " << student.first_name << "\n";
		student.display();
		cout << "\n1. Add Course\n"
			<< "2. Remove Course\n"
			<< "3. Back\n"
			<< "Choice: ";

		int choice;
		cin >> choice;

		if (choice == 1) {
			addCourseToStudent(student, students, filename);
		}
		else if (choice == 2) {
			// You could add a removeCourse function here
			cout << "Remove course functionality would go here\n";
		}
		else if (choice == 3) {
			break;
		}
		else {
			cout << "Invalid choice!\n";
		}
	}
}

void viewStudentCourses(Student& student) {
	cout << "\n--- YOUR COURSES ---\n";
	if (student.courses.empty()) {
		cout << "You are not enrolled in any courses.\n";
		return;
	}

	cout << "Course Code\tCourse Name\t\tGPA\tCredit Hours\n";
	cout << "----------------------------------------------------\n";
	for (const auto& course : student.courses) {
		cout << course.courseCode << "\t\t"
			<< course.courseName << "\t\t"
			<< course.gpa << "\t"
			<< course.creditHours << "\n";
	}
	cout << "----------------------------------------------------\n";
	cout << "Total Courses: " << student.courses.size() << "\n";
	cout << "Overall GPA: " << student.calculateOverallGPA() << "\n";
}
