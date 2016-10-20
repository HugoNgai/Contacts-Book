/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------
																	 ***Contacts Book*** 
Functions:
      1.search phone numbers by contact's name;
		  2.add new contacts;
		  3.update existing contacts;
		  4.delete existing contacts.
---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include<iostream>
#include<fstream>  
#include<stdlib.h>
#include<iomanip>
#include<string.h>

using namespace std;

class Book
{
public:
	void run();											//operating user interface
	Book();												//construtor
	~Book();

private:
	void changeData();									//change contacts data
	void deleteData();									//delete contacts data
	void updateData();									//update contacts data
	void addcontact();									//add new contacts
	void search();										//search contacts data
	fstream file;
};

//telephone numbers struct:save contacts and phone numbers
struct telephone
{
	bool deletetemp;									//the tag of delete
	char name[20];
	char num[20];
};

//construtor and ~
Book::Book()											//construtor
{
	ifstream infile("contacts.dat");
	if (!infile.is_open())
	{
		ofstream outfile("contacts.dat");
		if (!outfile.is_open())
			cout << "Open file error!";
		outfile.close();
	}
	else
		infile.close();

	file.open("contacts.dat", ios::in | ios::out | ios::binary);
	if (!file.is_open())
		cout << "Open file error!";
}

Book::~Book()											//release
{
	file.close();
}


//funtions:add phone numbers
void Book::addcontact()
{
	telephone tele;
	tele.deletetemp = false;

	cout << "Please input the contact's name first:";
	cin >> tele.name;
	cout << "Then input the phone number:";
	cin >> tele.num;

	file.write((char*)&tele, sizeof(telephone));
}


//functions:update user data
void Book::updateData()
{
	telephone tele;
	char name[20];
	cout << "Please input the name you want to change :";
	cin >> name;

	file.seekg(0);
	file.read((char*)&tele, sizeof(telephone));					//search name in the file 

	while (!file.eof())
	{
		if (strcmp(tele.name, name) == 0 && !tele.deletetemp)   //find the contact successfully
			break;
		file.read((char*)&tele, sizeof(telephone));
	}

	if (!file.eof())
	{
		cout << "The previous data is:" << endl;
		cout << "The data to be deleted is:" << endl;
		cout << "Contact Name:" << tele.name << endl;
		cout << "Phone Number:" << tele.num << endl;

		cout << "Please input the new telephone number:" << endl;
		cin >> tele.num;												//get the new number

		file.seekg(-(int)sizeof(telephone), ios::cur);                       //*************locate at the previous one in the file
		file.write((char*)&tele, sizeof(telephone));					//write in the new numbers

		cout << "Update successfully!" << endl;
	}
	else
	{
		cout << "Can't find this contact!" << endl;
		file.clear();													//clear the location history
	}
}

//funtion:search the contacts
void Book::search()
{
	telephone tele;
	char name[20];

	cout << "Please input the name of the contact you want to read:";
	cin >> name;

	file.seekg(0);
	file.read((char*)&tele, sizeof(telephone));

	while (!file.eof())
	{
		if (strcmp(tele.name, name) == 0 && !tele.deletetemp)				//compare the name to find the contact
			break;
		else
			file.read((char*)&tele, sizeof(telephone));
	}

	if (!file.eof())
	{
		cout << "The result:" << endl;
		cout << "Contact Name:" << tele.name << endl;
		cout << "Phone Number:" << tele.num << endl;
	}
	else
	{
		cout << "Can't find the contact!" << endl;
		file.clear();											//clear the location of history
	}
}

//funtion:delete the user data
void Book::deleteData()
{
	telephone tele;
	char name[20];

	cout << "Please input the contact's name you want to delete:" << endl;
	cin >> name;

	file.seekg(0);
	file.read((char*)&tele, sizeof(telephone));

	while (!file.eof())
	{
		if (strcmp(tele.name, name) == 0 && !tele.deletetemp)
			break;
		else
			file.read((char*)&tele, sizeof(telephone));
	}

	if (!file.eof())
	{
		char temp;
		cout << "The data to be deleted is:" << endl;
		cout << "Contact Name:" << tele.name << endl;
		cout << "Phone Number:" << tele.num << endl;
		cout << "Are you sure you want to delete it?(Y/N)";
		cin >> temp;

		if (temp == 'Y')
		{
			tele.deletetemp = true;
			file.seekg(-(int)sizeof(telephone), ios::cur);					//locate at the previous one in the data
			file.write((char*)&tele, sizeof(telephone));
			cout << "Deleted!" << endl;
		}
		else
			file.clear();												//clear the location history
	}
	else
	{
		cout << "Delete failed!";
		file.clear();													//clear the location history
	}
}

//funtion:change data,use to create a new output file
void Book::changeData()
{
	ofstream outfile("temp.dat", ios::app | ios::binary);               //create a new output file
	telephone tele;
	file.seekg(0);
	file.read((char*)&tele, sizeof(telephone));

	while (!file.eof())
	{
		if (!tele.deletetemp)											//the data isn't deleted
			outfile.write((char*)&tele, sizeof(telephone));
		else
			file.read((char*)&tele, sizeof(telephone));
	}
	file.close();
	outfile.close();

	//detele previous file and replace it with temp file
	remove("contacts.dat");
	rename("temp.dat", "contacts.dat");
	file.open("contacts.dat", ios::in | ios::out | ios::binary);			//open the new file again
}

//funtion:run the user interface
void Book::run()
{
	system("cls");
	int i;

	do
	{
		cout << setw(20) << "--------------------------------Contacts Book---------------------------------" << endl;
		cout << setw(20) << "------------------------------1.Add New Contact-------------------------------" << endl;
		cout << setw(20) << "------------------------------2.Delete Contact--------------------------------" << endl;
		cout << setw(20) << "------------------------------3.Search Contact--------------------------------" << endl;
		cout << setw(20) << "------------------------------4.Update Contact--------------------------------" << endl;
		cout << setw(20) << "------------------------------5.Close Contacts Book---------------------------" << endl;
		cin >> i;

		while (cin.get() != '\n');

		switch (i)
		{
		case 1:
			this->addcontact();
			break;
		case 2:
			this->deleteData();
			break;
		case 3:
			this->search();
			break;
		case 4:
			this->updateData();
			break;
		case 5:
			break;
		}
	} while (i != 5);
}


int main(void)
{
	Book Contact;
	Contact.run();
}
