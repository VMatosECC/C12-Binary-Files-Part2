// C12-Binary-Files-Part2.cpp 
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Structures ------------------------------------------------
struct Person
{
    int    id;
    char   name[30];
    int    age;
    char   data[10];

    void print()
    {
        cout << "[ ID: " << id
            << ", Name: " << name
            << ", Age: " << age
            << ", Data: " << data << "]" << endl;
    }
};

// Function Prototypes ---------------------------------------
void makeMemoryDatabase(vector<Person>& people);
void writeBinaryFile(vector<Person>& people, string filename);
void readBinaryFile(vector<Person>& people, string filename);
void showDatabase(vector<Person>& people);
void sequentialSearch(fstream& binFile, int key);

// Main Program -----------------------------------------------
void experiment01();
void experiment02();
void experiment03();


int main()
{
    experiment01();     //Read text file, make memory resident database, write binary file
    experiment02();     //Num records in binary file, read backwards every other record, seach by ID
                        //also apply Binary search by ID
    experiment03();     //add a record to the binary file, show the new record, and show all records

    cout << "\nAll done!\n";
}

// Function Definitions ---------------------------------------
void experiment03()
{
    //Open the binary file for reading and writing
    string filename = "c:/temp/springfieldPeople.bin";
    fstream binFile(filename, ios::in | ios::out | ios::binary);
    if (!binFile) {
        cout << "Error opening binary file " << filename << endl;
        exit(1);
    }
    //How many records are in the binary file?
    int numRecords = 0;
    binFile.clear();
    binFile.seekg(0, ios::end);
    numRecords = binFile.tellg() / sizeof(Person);
    cout << "[experiment03] Number of records in the binary file: " << numRecords << endl;
    //Add a new record to the binary file
    Person p;
    p.id = 200;
    strcpy_s(p.name, "Jimbo Jones");
    p.age = 33;
    strcpy_s(p.data, "data200");
    binFile.seekp(0, ios::end);
    binFile.write((char*)&p, sizeof(Person));
    cout << "New record added to the binary file: " << endl;
    p.print();
    //Show all records in the binary file
    cout << "\nShowing all records in the binary file: " << endl;
    binFile.clear();
    binFile.seekg(0, ios::beg);
    while (binFile.read((char*)&p, sizeof(Person)))
    {
        p.print();
    }
    binFile.close();
}



void binarySearch(fstream& binFile, int key) {
    //We assume the file exists and is open
    Person p;
    binFile.clear();
    binFile.seekg(0, ios::end);
    int numRecords = binFile.tellg() / sizeof(Person);
    int low = 0;
    int high = numRecords - 1;
    int mid;
    bool found = false;
    while (low <= high)
    {
        mid = (low + high) / 2;
        binFile.seekg(mid * sizeof(Person), ios::beg);
        binFile.read((char*)&p, sizeof(Person));
        if (p.id == key)
        {
            cout << "Record found: " << endl;
            p.print();
            found = true;
            break;
        }
        else if (p.id < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    if (!found)
    {
        cout << "Record not found." << endl;
    }
}

void sequentialSearch(fstream& binFile, int key) {
    //We assume the file exists and is open
    Person p;
    binFile.clear();
    binFile.seekg(0, ios::beg);
    bool found = false;
    while (binFile.read((char*)&p, sizeof(Person)))
    {
        if (p.id == key)
        {
            cout << "Record found: " << endl;
            p.print();
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "Record not found." << endl;
    }
}



void experiment02()
{
    //We assume that the binary file exists
    string filename = "c:/temp/springfieldPeople.bin";
    fstream binFile(filename, ios::in | ios::out | ios::binary);
    if (!binFile) {
        cout << "Error opening ninary file " << filename << endl;
        exit(1);
    }
    //How many records are in the binary file?
    int numRecords = 0;
    binFile.seekg(0, ios::end);
    numRecords = binFile.tellg() / sizeof(Person);
    cout << "Number of records in the binary file: " << numRecords << endl;
    //Print every other record
    Person p;
    cout << "\nShowing every other record in the binary file (backwards): " << endl;
    for (int i = numRecords - 1; i >= 0; i -= 2)
    {
        binFile.seekg(i * sizeof(Person), ios::beg);
        binFile.read((char*)&p, sizeof(Person));
        cout << i << "\t";
        p.print();
    }
    //Ask user for a key, apply sequential search, and show the record
    int key;
    do {
        cout << "\n[SEQUENTIAL SEARCH] Enter an ID key to show the record [0 to end]: ";
        cin >> key;
        if (key == 0) break;
        sequentialSearch(binFile, key);
    } while (true);

    //Ask user for a key, apply binary search, and show the record
    do {
        cout << "\n[BINARY SEARCH] Enter an ID key to show the record [0 to end]: ";
        cin >> key;
        if (key == 0) break;
        binarySearch(binFile, key);
    } while (true);
}
void experiment01()
{
    //Create a binary file from a vector of Person objects
    string filename = "c:/temp/springfieldPeople.bin";
    vector<Person> people;
    makeMemoryDatabase(people);
    showDatabase(people);
    writeBinaryFile(people, filename);
}



void makeMemoryDatabase(vector<Person>& people)
{
    //Read csv file and create a vector of Person objects
    
    ifstream fin("c:/temp/springfieldPeople.txt");
    if (!fin)
    {
        cout << "Error opening file." << endl;
        exit(1);
    }   

    Person p;
    string strId, strName, strAge, strData;
    while (getline(fin, strId, ',')){
        getline(fin, strName, ',');
        getline(fin, strAge, ',');
        getline(fin, strData);
        p.id = stoi(strId);
        strcpy_s(p.name, strName.c_str());
        p.age = stoi(strAge);   
        strcpy_s(p.data, strData.c_str());
        people.push_back(p);
    }
    fin.close();
}

void showDatabase(vector<Person>& people)
{
    cout << "People Database (Memory Resident) - Size: " << people.size() << endl;
    for (int i = 0; i < people.size(); i++)
    {
        cout << i << "\t";
        people[i].print();
    }
}

void writeBinaryFile(vector<Person>& people, string filename)
{
    //Use the memory database to create a persisten binary file image
    fstream fout(filename, ios::out | ios::binary);
    if (!fout)
    {
        cout << "Error opening binary file." << endl;
        exit(1);
    }

    for (int i = 0; i < people.size(); i++)
    {
        fout.write((char*)&people[i], sizeof(Person));
    }
    fout.close();
    cout << "Binary file created => " << filename << endl;
}

void readBinaryFile(vector<Person>& people, string filename)
{
    //Read the binary file and create a memory database
    fstream fin(filename, ios::in | ios::binary);
    if (!fin)
    {
        cout << "Error opening binary file." << endl;
        exit(1);
    }

    Person p;
    while (fin.read((char*)&p, sizeof(Person)))
    {
        people.push_back(p);
    }
    fin.close();
}