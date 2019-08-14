#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>  // system, 
#include <ctime>
#include <cmath> // abs
#include <iomanip>
#include <vector>
#include <direct.h> 	// mkdir
#include <conio.h>

#define read_from_file read_from_file

using namespace std;

struct UserInfo{
	string username; string fullname; string gender; string birthdate; string address;
	string town; string telno; string nationality; string occupation;
	string securityQuestion; string securityAnswer; string password;
};

struct AccountInfo{
	string owner, account, balance, interestMode, account_type;
};

struct Devs{
	string name, id, group;
};

struct CurrentlyActive {
	string userName = ""; UserInfo userInfo; AccountInfo accountInfo;
};

string intro = "";
string mainOptions[] = {"Login", "Create User", "Credits", "Exit"}; //"1. Login\n2. Create User\n99. Exit\n";
string loggedInOptions[] = {"Create Accont", "Check Balance", "Deposit", "Withdraw", "Transfer", 
							"Change Info", "Change Password", "Deactivate Account"};

const vector<char> whitespaces = {' ', '\t', '\n'};
CurrentlyActive currentlyActive;

// Joins items from the array with a given size and returns a formatted string. Ex. 1.Login
string join(string joiner, string joinee[], int size, int start){
	string main = "";
	for(int i =0; i < size; i++)
	{
		main += to_string(i+start) + ". " + joinee[i] + joiner;
	}
	return main;
}

// Joins items from a vector with of any size and returns a string
string join(string joiner, vector<string> joinee) {
	string main = "";
	for(string s : joinee){
		main += s + joiner;
	}
	return main;
}

string makeItTitle(string text){
	string preSuff = "", title;
	preSuff.resize(text.size() + 20, '-');
	title = preSuff + "\n";
	title.resize(text.size() + 31);
	title += text + "\n" + preSuff;
	return title;
}

/**
 * mode = 0, key, value method for updating certaing informations. If line starts with key, update on the value after the key.
 * mode = 1, if line starts with key, update entire line,
 */
void updateLine(string file_name, string key, string lineUpdate, int mode) {
	ofstream writer;
	ifstream reader;
	writer.open(file_name + ".copy", ios::out);
	reader.open(file_name, ios::in);;
	if (reader.fail()){
		return;
	}
	if (writer.fail()){
		return;
	}
	string line;
	while(!reader.eof())
	{
		getline(reader, line);
		if(line.substr(0, key.size()) == key)
		{
			if (mode == 0) writer<<key<<":"<<lineUpdate<<endl;
			else if (mode == 1) writer<<lineUpdate;
		}else{
			writer<<line<<endl;
		}
	}
	reader.close();
	writer.close();
	remove(file_name.c_str());
	rename((file_name+".copy").c_str(), file_name.c_str());
}

string trim(string str){
	if (str.size() == 0) return "";
	int minIndex = 0, maxIndex = str.size() - 1;
	while(str.at(minIndex) == ' ' || str.at(minIndex) == '\n'){
		minIndex++;
		if (minIndex == maxIndex) return "";
	}

	while(str.at(maxIndex) == ' ' || str.at(maxIndex) == '\n'){
		maxIndex--;
		if (minIndex == maxIndex) return "";
	}

	return str.substr(minIndex, maxIndex+1);
}

string getValue(string file_name, string key){
	ifstream reader;
	reader.open(file_name);
	if (reader.fail()) {
		return "";
	}
	string line;
	while(!reader.eof())
	{
		getline(reader, line);
		if(line.substr(0, key.size()) == key){
			string value = line.substr(key.size() + (key.at(key.size() -1 ) == ':' ? 0 : 1));
			reader.close();
			return value;
		}
	}
	reader.close();
	return "";
}

bool authenticate(string user, string key) {
	string path = ".\\Data\\Users\\" + user + "\\PersonalInfo.txt";
	string actualPass = getValue(path, "Password");
	return actualPass == key;
}

int write_to_file(string file_name, string content, ios_base::openmode mode){
	ofstream writer;
	writer.open(file_name, mode);
	if (writer.fail()){
		return -1;
	}
	writer<<content;
	writer.close();
	return 0;
}

vector<string> read_from_file(string file_name) {
	vector<string> mainText;
	ifstream reader;
	reader.open(file_name, ios::in);
	if (reader.fail()) {
		return mainText;
	}
	string line;

	while(!reader.eof())
	{
		getline(reader, line);
		if(line.size() > 0) mainText.push_back(line);
	}
	reader.close();
	return mainText;
}

int check_if_account_no_exists(string account_no){
	ifstream reader;
	reader.open("data\\accounts.dat");
	if(reader.fail()){
		reader.close();
		return -1;
	}
	string account;
	while(!reader.eof())
	{
		getline(reader, account);
		account = account.substr(0, account.find(" "));
		if (account == account_no) {
			reader.close();
			return 1;
		}
	}
	reader.close();
	return 0;
}

int check_if_username_exists(string userName){
	ifstream reader;
	reader.open("data\\users.dat");
	if(reader.fail()){
		reader.close();
		return -1;
	}
	string user;
	while(!reader.eof())
	{
		getline(reader, user);
		user = user.substr(0, user.find(" "));
		if (user == userName) {
			reader.close();
			return 1;
		}
	}
	reader.close();
	return 0;
}

string generate_account_no(){
	string account = "";
	do
	{
		account = "";
		while(account.size() < 10) {
			account += to_string(abs(rand()));
		}
	} while (check_if_account_no_exists(account.substr(0, 10)) == 1);
	return account.substr(0, 10);
}

int wordCount(string text, string findee, int count) {
	int value;
	if ((value = text.find(findee)) != -1){
		count++;
		return wordCount(text.substr(value+1), findee, count);
	}else{
		return ++count;
	}
}

void create_new_user(){
	cout<<"\n\nWe see that you are new around here.\nWe will ask you a couple of questions then you will be good to go."<<endl;
	cout<<"It will only take a couple of minutes"<<endl;
	cout<<"\n-----------Enter ? for help----------\n"<<endl;

	string userName,fullname, gender, birthdate, address, town, telNo, nationality, occupation, security_question,  security_answer, accountType, password, passConfirm;
	
	string security_questions[]  = {
								"What was your childhood nickname?",
								"In what city did you meet your spouse/significant other?", 
								"What is the name of your favorite childhood friend?",
								"What is the middle name of your youngest child?",
								"What is your youngest sister's name?",
								"What school did you attend for sixth grade?"
								};
	string help[] {
		"Enter your full name (Including Grand father's).", "Select your Gender\n\tMale\n\tFemale", "Enter you birth date", 
		"Enter your current Address", "Town", "Enter your phone number", "Enter you nationality", "Enter your current Occupation",
		"Select a Security Question\n\t" + join("\n\t", security_questions, 6, 1), "Enter the answer for your security question", 
	};
	string questions[] = {"Full Name", "Gender (M or F)", "Birth Date (DD-MM-YYYY)", "Address", "Town", "Tel. No", "Nationality",
	"Occupation", "Security Question", "Security Answer"};
	string *vars[] = { &fullname, &gender, &birthdate, &address, &town, &telNo, &nationality, &occupation,
	&security_question,  &security_answer};
	// Get Unique username
	do
	{
		cout<<setw(23)<<setiosflags(ios::left)<<"User Name"<<": ";
		getline(cin, userName);
	} while (check_if_username_exists(userName) == 1 && cout<<"\nThe user name is not available.\n"<<endl);

	cout<<endl<<makeItTitle("Personal Informations")<<endl<<endl;
	// Get Personal Informations
	for (int i = 0; i < 10; i++)
	{
		if (i == 8){
			cout<<"\nYou have completed the personal information section. Proceed to Section II\n"<<endl;
			system("pause");
			system("cls");
			cout<<endl<<makeItTitle("Security Informations")<<endl<<endl;
			cout<<endl<<help[i]<<endl<<endl;
		}
		cout<<setw(23)<<setiosflags(ios::left)<<questions[i]<<": ";
		getline(cin, *(vars[i]));

		if(*(vars[i]) == "?"){
			cout<<help[i]<<endl;
			i--;
		}else if(*(vars[i]) == "~"){
			return;
		}else if(i == 8){
			int index;
			istringstream(trim(*(vars[i]))) >> index;
			*(vars[i]) = security_questions[index-1];
		}else if(i == 0) {
			int names = wordCount(fullname, " ", 0);
			if (names < 3){
				cout<<"\nYou must enter your grandfather's name.\n"<<endl;
				i--;
			}
		}
	}
	// Get Password from user.
	do
	{
		cout<<setw(23)<<setiosflags(ios::left)<<"Password"<<": ";
		cin>>password;
		cout<<setw(23)<<setiosflags(ios::left)<<"Confirm Password"<<": ";
		cin>>passConfirm;
		cin.ignore();
	} while (password != passConfirm && cout<<"\nThe Passwords do not match. Try Again.\n"<<endl);

	time_t now = time(0);
	// string account_no = generate_account_no();
	string title = "Ms";
	if (gender == "M"){
		title = "Mr";
	}

	cout<<"\nThanks "<<title<<". "<<fullname<<" for your cooperation\n"<<endl;
	string content =
	"UserName:" + userName +
	"\nFull Name:" + fullname +
	"\nGender:" + gender +
	"\nBirth Date:" + birthdate +
	"\nAddress:" + address +
	"\nTown:" + town +
	"\nTelNo:" + telNo +
	"\nNationality:" + nationality +
	"\nOccupation:" + occupation +
	"\nSecurity Question:" + security_question +
	"\nSecurity Answer:" + security_answer +
	"\nPassword:" + password +
	"\nAccount Created On:" + ctime(&now) +
	"\nLast Logged On:Never";

	//Create Directory for the new user.
	string path = "Data\\Users\\" + userName;
	mkdir(path.c_str());

	// Create a new User Profile
	write_to_file("Data\\Users\\" + userName + "\\PersonalInfo.txt", content, ios::out);

	// Update Users.dat file
	content = userName + " " + to_string(time(0)) + "\n";
	write_to_file("Data\\users.dat", content, ios::app);

	content = "Notifications:0";
	write_to_file("Data\\Users\\" + userName + "\\notifications.dat", content, ios::out);
	system("pause");
	system("cls");
}

void create_new_account(string userName){
	cout<<"\n-----------Enter ? for help----------\n"<<endl;
	string path = "Data\\Users\\" + userName + "\\Accounts";
	mkdir(path.c_str());
	// mode = Interest Free or Conventional Banking
	string balance,interestMode, account_type, password;
	string *vars[] = { &account_type, &balance, &interestMode};
	string questions[] = {"Account Type (S or C)", "Initial Balance (> 50)", 
						"Interest Mode"};
	string help[] = { "\tSavings\n\tCurrent", "Enter you Initial Deposit to create your account", 
					  "\tConventional (With Interest)\n\tInterest Free Banking"}; 
	// Authenticate First
	int count = 0; // Counts the number of trials.
	do
	{
		if(count > 3){
			cout<<"\nYou have exceeded your maximum number trials. Over and Out. Ciao\n"<<endl;
			return;
		}
		count++;
		cout<<"Password: ";
		getline(cin, password);
	} while (!authenticate(userName, password) && cout<<"\nInvalid User Name/Password combination.\n"<<endl);

	// Get Personal Informations
	for (int i = 0; i < 3; i++)
	{
		cout<<setw(22)<<setiosflags(ios::left)<<questions[i]<<": ";
		getline(cin,*(vars[i]));
		if (*(vars[i]) == "?"){
			cout<<endl<<help[i]<<endl<<endl;
			i--;
		}else if (*(vars[i]) == "~"){
			return;
		}
	}

	string account_no = generate_account_no();

	/**Writing Files to disk */
	// string content = account_no + " " + to_string(time(0)) + "\n";
	string content = account_no + " " + userName + "\n";
	write_to_file("Data\\accounts.dat", content, ios::app);
	content = "UserName:" + userName + "\n" +
			  "AccountNo:" + account_no + "\n" +
			  "InterestMode:" + interestMode + "\n" +
			  "Balance:" + balance + "\n" +
			  "LastDeposit:"+ to_string(time(0));

	write_to_file("Data\\Users\\" + userName + "\\Accounts\\" + account_no + ".dat", content, ios::out);
	write_to_file("Data\\Users\\" + userName + "\\accounts.dat", account_no + "\n", ios::app);

	cout<<"\nYour account has been created successfully!"<<endl;
	cout<<"Your Account No is: "<<account_no<<endl<<endl;
}

UserInfo getUserInfo(string userName, bool isActive){

	if(currentlyActive.userName == userName) return currentlyActive.userInfo;
	UserInfo info;
	string path = "Data\\Users\\" + userName + "\\Personalinfo.txt";
	info.username = userName;
	info.password = getValue(path, "Password");
	info.gender = getValue(path, "Gender");
	info.fullname = getValue(path, "Full Name");
	info.birthdate = getValue(path, "Birth Date");
	info.address = getValue(path, "Address");
	info.town = getValue(path, "Town");
	info.telno = getValue(path, "TelNo");
	info.nationality = getValue(path, "Nationality");
	info.occupation = getValue(path, "Occupation");
	info.securityQuestion = getValue(path, "Security Question");
	info.securityAnswer = getValue(path, "Security Answer");

	if(isActive){
		currentlyActive.userInfo = info;
		currentlyActive.userName = userName;
	}

	return info;
}

double getBalance(string username, string account) {
	string accountPath = "Data\\Users\\" + username + "\\Accounts\\" + account + ".dat";
	string balance = trim(getValue(accountPath, "Balance:"));
	string lastDeposited = trim(getValue(accountPath, "LastDeposit"));
	string interestMode = trim(getValue(accountPath, "InterestMode"));

	long lastDepo; double interest = 0.0;
	if (interestMode == "Conventional" or interestMode == "C")
	{
		interest = .07;
	}
	istringstream(lastDeposited) >> lastDepo;
	int months = (time(0) - lastDepo)/3600;

	double bal;
	istringstream(balance) >> bal;
	return bal*(pow(1 + interest, months));
}

void change_balance(string userName, string account, double amount) {

	string accountPath = "Data\\Users\\" + userName + "\\Accounts\\" + account + ".dat";
	double currentBalance = getBalance(userName, account);
	double newBalance = currentBalance + amount;
	updateLine(accountPath, "Balance", to_string(newBalance), 0);
	updateLine(accountPath, "LastDeposit", to_string(time(0)), 0);
}

string getOwnerOfAccount(string account){
	ifstream reader;
	reader.open("Data\\accounts.dat", ios::in);
	if(reader.fail()){
		return "";
	}
	string line; bool isNext = false;
	while(reader>>line)
	{
		if(isNext){
			reader.close();
			return trim(line);
		}
		if (trim(line) == account){
			isNext = true;
		}
	}
	reader.close();
	return "";
}

void checkYourBalance(string userName){
	string opt; int option;
	vector<string> accountsList = read_from_file("Data\\Users\\" + userName + "\\accounts.dat");
	if (accountsList.empty()){
		cout<<"\nYou have no active account. Create an account first.\n"<<endl;
		cout<<"\nDo you want to create new account? (Y or N) ";
		string response;
		getline(cin, response);
		if (response.at(0) == 'Y' or response.at(0) == 'y') {
			system("cls");
			cout<<makeItTitle("Creating New Account")<<endl<<endl;
			create_new_account(userName);
			accountsList = read_from_file("Data\\Users\\" + userName + "\\accounts.dat");
		}else return;
	}
	for (int i = 0; i < accountsList.size(); i++) {cout<<i+1<<": "<<accountsList[i]<<endl;}
	do
	{
		cout<<"Select: ";
		getline(cin, opt);
		istringstream(trim(opt)) >> option;
	} while (option > accountsList.size() && cout<<"Please Selected a number in the given range"<<endl);
	
	cout<<"\nAccount No: "<<accountsList[option-1]<<endl;
	long currentBalance = getBalance(userName, accountsList[option-1]);
	cout<<"Your Current Balance is: "<<currentBalance<<" Br.\n"<<endl;
}

void makeDeposit(string userName) {
	double deposit; string depositString;
	string opt; int option;
	vector<string> accountsList = read_from_file("Data\\Users\\" + userName + "\\accounts.dat");
	if (accountsList.empty()){
		cout<<"You have not active account. Create an account first."<<endl;
		cout<<"\nDo you want to create new account? (Y or N) ";
		string response;
		getline(cin, response);
		if (response.at(0) == 'Y' or response.at(0) == 'y') {
			system("cls");
			cout<<makeItTitle("Creating New Account")<<endl<<endl;
			create_new_account(userName);
			accountsList = read_from_file("Data\\Users\\" + userName + "\\accounts.dat");
		}else{
			return;
		}
	}
	for (int i = 0; i < accountsList.size(); i++) {cout<<i+1<<": "<<accountsList[i]<<endl;}
	do
	{
		cout<<"Select: ";
		getline(cin, opt);
		istringstream(trim(opt)) >> option;
	} while (option > accountsList.size() && cout<<"Please Selected a number in the given range"<<endl);
	double currentBalance = getBalance(userName, accountsList[option -1]);
	cout<<"\nYour Current Balance is: "<<currentBalance<<endl;
	cout<<"How Much do you want to deposit? ";
	getline(cin, depositString);

	istringstream(trim(depositString)) >> deposit;
	// cout<<"Been Here"<<endl;
	deposit = abs(deposit);
	change_balance(userName, accountsList[option-1], deposit);
	cout<<"\nYou have successfully deposited Br."<<deposit<<" to "<<accountsList[option-1]<<endl;
	cout<<"Your new balance is "<<getBalance(userName, accountsList[option - 1])<<endl<<endl;
}

void makeTransfer(string userName) {
	string opt; int option;
	string transferString; double transferAmount;
	vector<string> accountsList = read_from_file("Data\\Users\\" + userName + "\\accounts.dat");
	if (accountsList.empty()){
		cout<<"You have not active account. Create an account first."<<endl;
		cout<<"\nDo you want to create new account? (Y or N) ";
		string response;
		getline(cin, response);
		if (response.at(0) == 'Y' or response.at(0) == 'y') {
			system("cls");
			cout<<makeItTitle("Creating New Account")<<endl<<endl;
			create_new_account(userName);
			accountsList = read_from_file("Data\\Users\\" + userName + "\\accounts.dat");
		}else return;

	}
	for (int i = 0; i < accountsList.size(); i++) {cout<<i+1<<": "<<accountsList[i]<<endl;}
	
	
	do
	{
		cout<<"Select an account to transfer from: ";
		getline(cin, opt);
		istringstream(trim(opt)) >> option;
	} while (option > accountsList.size() && cout<<"Please Selected a number in the given range"<<endl);
	double currentBalance = getBalance(userName, accountsList[option -1]);
	string recieverAccount, recieverUser;
	cout<<"Whom To? Enter the reciever's account number: ";
	getline(cin, recieverAccount);
	// cin.ignore();
	recieverUser = getOwnerOfAccount(recieverAccount);
	do
	{
		cout<<"How much do you want to transfer (<"<<currentBalance-50<<")? ";
		getline(cin, transferString);
		istringstream(transferString) >> transferAmount;
		// cin.ignore();
	} while (transferAmount > currentBalance - 50 && cout<<"\nYou just entered more than the allowed maximum.\n"<<endl);
	change_balance(userName, accountsList[option -1], -transferAmount);
	change_balance(recieverUser, recieverAccount, transferAmount);

	time_t now = time(0);
	string notification = "Title:Transfer Notification\n" +
						  string("Date:") + string(ctime(&now)) + "\n" + 
						  "Sender User:" + userName + "\n" +
						  "Sender Account:" + accountsList[option - 1] + "\n" +
						  "Reciever User:" + recieverUser + "\n" +
						  "Reciever Account:" + recieverAccount + "\n" +
						  "Amount:" + to_string(transferAmount) + "\n" +
						  "#Closed:\n";
	int notifications, notificationInput;
	istringstream(getValue("Data\\Users\\" + recieverUser + "\\notifications.dat", "Notifications")) >> notifications;
	updateLine("Data\\Users\\" + recieverUser + "\\notifications.dat", "Notifications", to_string(notifications + 1), 0);

	write_to_file("Data\\Users\\" + recieverUser + "\\notifications.dat", notification, ios::app);
	
	cout<<"\nYou have successfully transfered Br."<<transferAmount<<" from "<<accountsList[option-1]
		<<" to "<<recieverAccount<<endl;
	cout<<"Your new balance is "<<getBalance(userName, accountsList[option - 1])<<endl<<endl;
}

void makeWithdrawal(string userName){
	int withdraw; string withdrawString;
	string opt; int option;
	vector<string> accountsList = read_from_file("Data\\Users\\" + userName + "\\accounts.dat");
	if (accountsList.empty()){
		cout<<"You have not active account. Create an account first."<<endl;
		cout<<"\nDo you want to create new account? (Y or N) ";
		string response;
		getline(cin, response);
		if (response.at(0) == 'Y' or response.at(0) == 'y') {
			system("cls");
			cout<<makeItTitle("Creating New Account")<<endl<<endl;
			create_new_account(userName);
			accountsList = read_from_file("Data\\Users\\" + userName + "\\accounts.dat");
		}else return;
	}
	for (int i = 0; i < accountsList.size(); i++) {cout<<i+1<<": "<<accountsList[i]<<endl;}
	do
	{
		cout<<"Select: ";
		getline(cin, opt);
		istringstream(trim(opt)) >> option;
	} while (option > accountsList.size() && cout<<"Please Selected a number in the given range"<<endl);
	double currentBalance = getBalance(userName, accountsList[option -1]);
	cout<<"\nYour Current Balance is: "<<currentBalance<<endl;

	do
	{
		cout<<"How much do you want to withdraw (<"<<currentBalance - 50<<")? ";
		getline(cin, withdrawString);
		istringstream(withdrawString) >> withdraw;
		// cin.ignore();
	} while ((withdraw % 100 != 0 && cout<<"\nYou can only withdraw in 100s. Thanks\n"<<endl) ||
				 (withdraw > currentBalance - 50 && cout<<"\nYou just entered more than the allowed maximum.\n"<<endl));
	change_balance(userName, accountsList[option - 1], -abs(withdraw));
	cout<<"\nYou have successfully withdrawn Br."<<withdraw<<" from "<<accountsList[option-1]<<endl;
	cout<<"Your new balance is "<<getBalance(userName, accountsList[option - 1])<<endl<<endl;
}

void changeInfo(string userName){
	vector<string> info = read_from_file("Data\\Users\\" + userName + "\\PersonalInfo.txt");
	for (int i = 0; i < info.size(); i++)
	{
		cout<<setw(2)<<i+1<<". "<<info[i]<<endl;
	}
	cout<<endl<<setw(2)<<99<<". Done changing!\n"<<endl;
	
	int option; string opt;
	do
	{
		option = 99;
		cout<<"Which one do you want to change? ";
		getline(cin, opt);
		if(trim(opt) == "" or trim(opt) == "99") break;
		istringstream(opt) >> option;

		
		string newValue, oldValue;
		oldValue = info[option - 1].substr(info[option - 1].find(":") + 1);
		string key = info[option - 1].substr(0, info[option - 1].find(":"));

		if (key == "Password" or key == "UserName" or key == "Account Created On" or key == "Last Logged On"){
			cout<<"\nYou can't change this. Maybe something else!\n"<<endl;
			continue;
		}
		
		do
		{
			cout<<"New "<<key<<": ";
			getline(cin, newValue);
		} while (key == "Full Name" && wordCount(trim(newValue), " ", 0) < 3 && cout<<"\nYou must enter your grandfather's name.\n"<<endl);
		updateLine("Data\\Users\\" + userName + "\\PersonalInfo.txt", key, newValue, 0);
		cout<<"\nChanged your "<<key<<" from "<<oldValue<<" to "<<newValue<<" successfully!\n"<<endl;
		
	} while (option != 99);
	
	system("cls");
	cout<<makeItTitle("Updated Information")<<endl;
	info = read_from_file("Data\\Users\\" + userName + "\\PersonalInfo.txt");
	for (int i = 0; i < info.size(); i++) cout<<setw(2)<<i+1<<". "<<info[i]<<endl;
}

void changePassword(string userName, string password){
	string curPass, newPass, newPassConfirm;
	bool valid = false;
	do{
		cout<<"Current Password: ";
		cin>>curPass;
		// getline(cin, curPass);
		cout<<"New Password: ";
		getline(cin, newPass);
		cin>>newPass;
		cout<<"Confirm Password: ";
		// getline(cin, newPassConfirm);
		cin>>newPassConfirm;
		valid = curPass == password && newPass == newPassConfirm;
		cin.ignore();
	}while (!valid && cout<<"\nSomething went wrong. Try again!\n"<<endl);
	updateLine("Data\\Users\\" + userName + "\\PersonalInfo.txt", "Password", newPass, 0);
	cout<<"\nYour password has been changed successfully!\n";

}

void displayNotification(string userName, int &total){
	string path = "Data\\Users\\" + userName + "\\notifications.dat";
	vector<string> allLine = read_from_file(path);
	int count = 0;
	bool inNotification = false;
	for (string line : allLine){
		if (line.substr(0, 5) == "Title"){
			inNotification = true;
			cout<<makeItTitle(getValue(path, "Title"))<<endl;
		} else if (trim(line) == "#Closed:"){
			inNotification = false;
			cout<<"~~~~~~~Notification #"<<++count<<" out of "<<total<<endl;
			system("Pause");
			system("cls");
		}else if (inNotification){
			cout<<line.substr(0, line.find(":"))<<": "<<line.substr(line.find(":") + 1)<<endl;
		}
	}
	write_to_file(path, "Notifications:0", ios::out);
	total = 0;
}

void closeAccount(string userName)
{
	int withdraw; string withdrawString;
	string opt; int option;
	vector<string> accountsList = read_from_file("Data\\Users\\" + userName + "\\accounts.dat");
	if (accountsList.empty()){
		cout<<"\nYou have not active account already\n"<<endl;
		return;
	}
	for (int i = 0; i < accountsList.size(); i++) {cout<<i+1<<": "<<accountsList[i]<<endl;}
	do
	{
		cout<<"Select: ";
		getline(cin, opt);
		istringstream(trim(opt)) >> option;
	} while (option > accountsList.size() && cout<<"Please Selected a number in the given range"<<endl);
	istringstream(opt) >> option;
	// double currentBalance = getBalance(userName, accountsList[option -1]);
	// cout<<"\nYour Current Balance is: "<<currentBalance<<endl;
	cout<<"\nAre you sure? This can not be undone. (Y or N) ";
	getline(cin, opt);
	if (opt.at(0) == 'Y' || opt.at(0) == 'y'){
		remove( ("Data\\Users\\" + userName + "\\Accounts\\" + accountsList[option - 1] + ".dat").c_str());
		updateLine("Data\\accounts.dat", accountsList[option - 1], "", 1);
		updateLine("Data\\Users\\" + userName +  "\\accounts.dat", accountsList[option - 1], "", 1);
		cout<<"\nYou have closed your account successfully.\n"<<endl;
	}
}

void prompt(UserInfo info){
	int input; string inputString;
	string title = "Mr.";
	string userName = info.username, password = info.password;
	if(info.gender.at(0) == 'F' or info.gender.at(0) == 'f') title = "Ms.";
	// Check and Display Notification
	int notifications, notificationInput; string notifyString;
	istringstream(getValue("Data\\Users\\" + userName + "\\notifications.dat", "Notifications")) >> notifications;
	if (notifications > 0){
		cout<<"\nYou have "<<notifications<<" new notifications.\n\t1. Open\n\t2. Next Time\nSelect: ";
		getline(cin, notifyString);
		istringstream(notifyString) >> notificationInput;
		if (notificationInput == 1){
			system("cls");
			cout<<makeItTitle("Displaying Notification")<<endl;
			displayNotification(userName, notifications);
		}
		system("cls");
	}
	do
	{
		cout<<makeItTitle("Welcome, " + title + info.fullname)<<endl;
		cout<<"\nWhat do you want to do?\n"<<endl;

		istringstream(getValue("Data\\Users\\" + userName + "\\notifications.dat", "Notifications")) >> notifications;
		cout<<"1. Notifications("<<notifications<<")"<<endl;
		cout<<join("\n", loggedInOptions, 8, 2)<<endl;
		cout<<99<<" Back"<<endl;
		cout<<"Select: ";
		getline(cin, inputString);
		istringstream(inputString) >> input;
		system("cls");
		if (input == 1){ // Notification
			cout<<makeItTitle("Displaying Notification")<<endl;
			if(notifications > 0){
				displayNotification(userName, notifications);
			}else{
				cout<<"You have no new Notification"<<endl;
				system("pause");
			}
		} else if (input == 2){ // Creating Account
			cout<<makeItTitle("Creating New Account")<<endl;
			create_new_account(info.username);
		} else if (input == 3){ // Check Balance
			cout<<makeItTitle("Checking Balance")<<endl;
			checkYourBalance(userName);
			// system("pasue");
		} else if (input == 4){ // Deposit
			cout<<makeItTitle("Deposit")<<endl;
			makeDeposit(userName);
			// system("pasue");
		} else if (input == 5){ // Withdraw
			cout<<makeItTitle("Withdraw")<<endl;
			makeWithdrawal(userName);
			// system("pasue");
		} else if(input == 6){ // Transfer
			cout<<makeItTitle("Transfer")<<endl;
			makeTransfer(userName);
		} else if (input == 7){ // Change Info
			cout<<makeItTitle("Current Information")<<endl;
			changeInfo(userName);
			// system("pause");
		} else if (input == 8){ // Change Password
			cout<<makeItTitle("Change Password")<<endl;
			changePassword(userName, password);
			// system("pause");
		} else if (input == 9){
			cout<<makeItTitle("Deactivate Account")<<endl;
			closeAccount(userName);
		}
		if (input != 99 && input != 1) system("pause");
		system("cls");
	} while (input != 99);
}

void login() {
	cout<<makeItTitle("Login")<<endl;
	string userName, password;
	int count = 0;
	do
	{
		if(count > 2){
			cout<<"\nYou have exceeded your maximum number trials. Over and Out. Ciao\n"<<endl;
			system("pause");
			system("cls");
			return;
		}
		count++;
		cout<<"User Name: ";
		cin>>userName;
		cout<<"Password: ";
		cin>>password;
		cin.ignore();
	} while (!authenticate(userName, password) && cout<<"\nInvalid User Name/Password combination.\n\n");
	system("cls");
	time_t now = time(0);
	updateLine("Data\\Users\\" + userName + "\\PersonalInfo.txt", "Last Logged On", ctime(&now), 0);
	UserInfo info = getUserInfo(userName, true);
	prompt(info);
}

void displayCredits(){
	Devs developers[] = {
		{"Yahya Fati", "UGR\\17776\\11", "7"},
		{"Filmawi Fekadu", "UGR\\17776\\11", "7"},
		{"Natnael Abe", "UGR\\17776\\11", "7"},
		{"Sara Abdelkadir", "UGR\\17776\\11", "7"},
		{"Tesfaye Feyisa", "UGR\\17719\\11", "7"}
	};

	string tableborder= "";
	tableborder.resize(50, '-');
	cout<<makeItTitle("Credits")<<endl;
	
	cout<<endl<<tableborder<<endl;
	cout<<setiosflags(ios::left)<<setw(33)<<"    Full Name"
		<<setiosflags(ios::left)<<setw(10)<<"ID"
		<<setiosflags(ios::left)<<setw(20)<<"Group"<<endl;
	
	cout<<tableborder<<endl;
	int i = 0;
	for(Devs d : developers){
		cout<<++i<<". "<<setiosflags(ios::left)<<setw(25)<<d.name<<setw(17)<<d.id<<setw(5)<<d.group<<endl;
		cout<<tableborder<<endl;
	}
	system("pause");
	system("cls");
}

void initializeFirstTime() {
	mkdir("Data");
	mkdir("Data\\Users");
}

int main(){
	initializeFirstTime();
	system("cls");
	string input;
	srand(time(NULL));
	srand(rand() % 100);
	do
	{
		int randNum = rand()%(11);
		intro = join("\n", read_from_file("Logos\\" + to_string(randNum) + ".txt"));
		cout<<intro<<"\n";
		cout<<'\t'<<join("\n\t", mainOptions, 3, 1)<<"\n\t99. Exit\nSelect: ";
		getline(cin, input);
		if (input == "1"){
			system("cls");
			login();
		}else if(input == "2")
		{
			system("cls");
			cout<<makeItTitle("Creating new user")<<endl<<endl;
			create_new_user();
		}else if(input == "3"){
			system("cls");
			displayCredits();
			system("cls");
		}
	} while (input != "99");
	cout<<"\nHope to see you soon. Ciao!\n"<<endl;
	return 0;
}