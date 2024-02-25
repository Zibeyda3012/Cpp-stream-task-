#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int id;
class Contact
{
	string _name;
	string _surname;
	string _relation;
	string _number;
	int _id;

public:

	Contact(string name, string surname, string relation, string number)
	{
		_id = id++;
		SetName(name);
		SetNumber(number);
		SetSurname(surname);
		SetRelation(relation);
	}

	Contact(const Contact& other)
	{
		this->_name = other._name;
		this->_surname = other._surname;
		this->_relation = other._relation;
		this->_number = other._number;
	}

	void SetName(string name)
	{
		if (name.size() < 3)
			throw length_error("name must contain at least 3 symbols");
		else
		{
			if (name[0] >= 65 && name[0] <= 90)
				_name = name;
			else
				throw invalid_argument("first symbol must be uppercase letter");
		}

	}

	void SetSurname(string surname)
	{
		if (surname.size() < 5)
			throw length_error("surname must contain at least 3 symbols");
		else
			_surname = surname;
	}

	void SetRelation(string relation)
	{
		if (relation.size() < 1)
			throw invalid_argument("raletion is not given");
		else
			_relation = relation;
	}

	void SetNumber(string number)
	{
		if (number.size() < 10)
			throw length_error("phone number must contain at leats 8 digits");
		else
			_number = number;
	}

	string GetName() { return _name; }
	string GetSurname() { return _surname; }
	string GetRealtion() { return _relation; }
	string GetNumber() { return _number; }
	int GetId() { return _id; }

	void Show()
	{
		cout << endl;
		cout << "Name: " << _name << endl;
		cout << "Surname: " << _surname << endl;
		cout << "Relation: " << _relation << endl;
		cout << "Number: " << _number << endl;

	}

};


void ReadIdFromFile(string file)
{
	ifstream id_file(file, ios::in);
	int _id = 0;

	if (id_file.is_open())
	{
		id_file >> _id;
		id_file.close();

		id = _id;
	}
	else
		cout << "file is not opened" << endl;

}

void WriteIdToFile(string file, int size)
{
	ofstream id_file(file, ios::in);

	if (id_file.is_open())
	{
		if (size == 0)
			id_file << 0;
		else
			id_file << id;

		id_file.close();
	}

	else
		cout << "file is not opened" << endl;
}


class DataBase {

	Contact** contacts;
	int size;
public:
	DataBase()
	{
		contacts = nullptr;
		size = 0;
	}

	int GetSize() { return size; }

	Contact** ReadAllContactsFromFile(string file)
	{
		ifstream f(file, ios::in);

		if (f.is_open())
		{
			string name = "";
			string surname = "";
			string relation = "";
			string number = "";
			int id = 0;

			int size = 0;

			Contact** contacts = new Contact * [size] {};

			while (!f.eof())
			{
				f >> id;
				if (!f.eof())
				{
					f >> name;
					f >> surname;
					f >> relation;
					f >> number;

					Contact* contact = new Contact(name, surname, relation, number);
					AddContact(contact);
				}
			}
			f.close();
			return contacts;

		}

		else
		{
			cout << "file is not opened";
			return nullptr;
		}
	}

	void WriteContactsToFile(string file)
	{
		ofstream f(file, ios::out);

		if (!f.is_open())
		{
			cout << "file is not opened" << endl;
			return;
		}

		if (contacts != nullptr)
		{
			for (size_t i = 0; i < size; i++)
				f << contacts[i]->GetId() << " " << contacts[i]->GetName() << " " << contacts[i]->GetSurname() << " " << contacts[i]->GetRealtion() << " " << contacts[i]->GetNumber() << endl;

			f.close();
		}
		else
			cout << "contacts are null";
	}

	void AddContact(Contact* contact)
	{
		if (this->size == 0)
		{
			this->contacts = new Contact * [1];
			contacts[0] = contact;
			size++;
		}

		else
		{
			Contact** nContacts = new Contact * [size + 1];
			for (size_t i = 0; i < size; i++)
			{
				nContacts[i] = contacts[i];
			}
			nContacts[size] = contact;
			size++;

			delete[] contacts;
			contacts = nContacts;
		}
	}

	bool checkContact(string name)
	{
		for (size_t i = 0; i < size; i++)
			if (contacts[i]->GetName() == name)
				return true;

		return false;

	}

	int checkCountOfName(string name)
	{
		int count = 0;
		for (size_t i = 0; i < size; i++)
			if (contacts[i]->GetName() == name)
				count++;

		return count;
	}

	void DeleteByName(string name)
	{
		if (checkContact(name))
		{
			int count = checkCountOfName(name);
			Contact** nContacts = new Contact * [size - count];
			int index = 0;

			for (size_t i = 0; i < size; i++)
			{
				if (contacts[i]->GetName() == name) continue;
				else
					nContacts[index++] = contacts[i];
			}

			size -= count;
			delete[] contacts;
			contacts = nContacts;

		}

		else
			cout << "name not found" << endl;

	}

	void SearchByName(string name)
	{
		if (checkContact(name))
			for (size_t i = 0; i < size; i++)
				if (contacts[i]->GetName() == name)
					contacts[i]->Show();

				else
					cout << "name not found" << endl;
	}

	void ShowAllContacts()
	{
		if (size > 0)
			for (size_t i = 0; i < size; i++)
				contacts[i]->Show();
		else
		{
			cout << "there is no contact in database" << endl;
			return;
		}

	}

	~DataBase()
	{
		if (contacts != nullptr)
		{
			for (size_t i = 0; i < size; i++)
			{
				delete contacts[i];
				contacts[i] = nullptr;
			}

			delete[] contacts;
		}
	}
};


void printMenu()
{
	cout << "========================" << endl;
	cout << "1 => Add Contact" << endl;
	cout << "2 => Delete Contact" << endl;
	cout << "3 => Search Contact" << endl;
	cout << "4 => Show All Contacts" << endl;
	cout << "0 => Exit" << endl;
	cout << "========================" << endl;

}

int main()
{
	DataBase db;
	ifstream file1("Contacts.txt", ios::in);
	ofstream file2("Id_File.txt", ios::out);

	//file2 << 0;

	ReadIdFromFile("Id_File.txt");
	db.ReadAllContactsFromFile("Contacts.txt");


	while (true)
	{
		int choice;
		printMenu();

		cout << "enter your choice: ";
		cin >> choice;

		if (choice == 1)
		{
			system("cls");
			cin.ignore();

			string name;
			string surname;
			string relation;
			string number;

			try
			{
				cout << "enter name: ";
				cin >> name;

				cout << "enter surname: ";
				cin >> surname;

				cout << "relation: ";
				cin >> relation;

				cout << "number: ";
				cin >> number;

				Contact* contact = new Contact(name, surname, relation, number);
				db.AddContact(contact);
			}
			catch (exception ex)
			{
				cout << ex.what() << endl;
			}
			catch (...)
			{
				cout << "Error happened" << endl;
			}
		}

		else if (choice == 2)
		{
			system("cls");
			cin.ignore();
			string name;

			try
			{
				cout << "enter name: ";
				cin >> name;

				db.DeleteByName(name);

			}
			catch (exception ex)
			{
				cout << ex.what() << endl;
			}

			catch (...)
			{
				cout << "Error happened" << endl;
			}
		}

		else if (choice == 3)
		{
			system("cls");
			cin.ignore();
			string name;

			try
			{
				cout << "enter name: ";
				cin >> name;

				db.SearchByName(name);

			}
			catch (exception ex)
			{
				cout << ex.what() << endl;
			}

			catch (...)
			{
				cout << "Error happened" << endl;
			}
		}

		else if (choice == 4)
		{
			system("cls");
			db.ShowAllContacts();
		}

		else if (choice == 0)
		{
			db.WriteContactsToFile("Contacts.txt");
			WriteIdToFile("Id_File.txt", db.GetSize());
			system("cls");
			cout << "Bye Bye...";
			break;
		}

		else
		{
			cout << "incorrect input" << endl;
			break;
		}
	}
}