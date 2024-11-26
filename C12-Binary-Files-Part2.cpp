// C12-Binary-Files-Part2.cpp 
/*  This is the springfieldPeople.txt dataset -----------------------------
100,Homer Simpson,39,data100
105,Marge Simpson,36,data105
110,Bart Simpson,10,data110
115,Lisa Simpson,8,data115
120,Maggie Simpson,1,data120
125,Abraham Simpson,83,data125
130,Ned Flanders,60,data130
135,Maude Flanders,58,data135
140,Milhouse Van Houten,10,data140
145,Nelson Muntz,12,data145
150,Ralph Wiggum,8,data150
155,Seymour Skinner,44,data155
160,Edna Krabappel,41,data160
165,Apu Nahasapeemapetilon,45,data165
170,Moe Szyslak,50,data170
175,Barney Gumble,40,data175
180,Krusty the Clown,55,data180
185,Clancy Wiggum,45,data185
190,Waylon Smithers,40,data190
195,Montgomery Burns,104,data195

*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Structures --------------------------------------------------
struct Person
{
    int    id;
    char   name[30];    //using c-string instead of string
    int    age;
    char   data[10];    //arbitrary data field

    void print()
    {
        cout << "[ ID: " << id
            << ", Age: " << age
            << ", Data: " << data 
            << ", Name: " << name
            << "]" << endl;
    }
};

// Function Prototypes ---------------------------------------
void makeBinaryFile(string& csvFilePath, string& binFilePath);
void showBinaryFile(fstream& file, string caption = "");
int  countRecords(string filename);
void sequentialSearch(fstream& binFile, int key);
void binarySearch(fstream& binFile, int key);
fstream openBinaryFile(string filename, ios::openmode mode = ios::in | ios::out);

// Main Program ------------------------------------------------------------
void experiment01();    //Create a binary file from a csv text file
void experiment02();    //Tell how many records are there in the binary file.
void experiment03();    //Random access. Read backwards every other record
void experiment04();    //Add a record to the binary file.
void experiment05();    //Modify an existing record
void experiment06();    //Sequential search by ID
void experiment07();    //Binary search by ID
void experiment08();    //Delete a record from the binary file
void experiment09();    //Create an index to search by ID. Show the index



//add a record to the binary file, show the new record, and show all records


int main()
{
    //experiment01();     //Read csv text file, create an equivalent binary file
    //experiment02();     //Tell how many records are there in the binary file.
    //experiment03();     //Show random access of the binary file.
    //experiment04();     //Add a record to the binary file.
    //experiment05();     //Modify an existing record
    //experiment06();     //Search by ID using Linear Search
    //experiment07();     //Search by ID using Binary Search
    //experiment08();     //Delete a record from the binary file
    experiment09();     //Create an index to search by ID. Show the index
    cout << "\nAll done!\n";
}

// User-Defined Functions =========================================================

fstream openBinaryFile(string filename, ios::openmode mode)
{
    //Open the binary file for READ and WRITE. Default mode is ios::in | ios::out
    //Observe that if the file does not exist, ios::in will fail
    fstream file(filename, mode | ios::binary);
    if (!file) {
        cout << "Error opening binary file " << filename << endl;
        exit(1);
    }
    return file; //return the file object
}



//-------------------------------------------------------------------------------
void showBinaryFile(fstream& file, string caption)
{
    //Read records from binFile-display their content 
    cout << "\n" << caption << endl;    //show caption
    Person p;                           //create a Person object
    int pos = 0;                        //position counter 0,1,2, ...
    file.clear();                       //clear the eof flag
    file.seekg(0, ios::beg);            //move get-pointer to the top 

    while (file.read((char*)&p, sizeof(Person)))  //read a record   
    {
        cout << pos++ << "\t";  //display position and record content   
        p.print();
    }
    cout << endl;
    file.clear();               //courtesy - clear the eof flag
    file.seekg(0, ios::beg);    //courtesy - move the get-pointer to top of the file

}




//---------------------------------------------------------------------------
void binarySearch(fstream& binFile, int key) {
    //Precondition: binFile is open and sorted on ID field
    //Reset eof flag, set get-pointer to the beginning of the file
    binFile.clear();
    binFile.seekg(0, ios::end);

    //Determine the number of records in the binary file
    int numRecords = binFile.tellg() / sizeof(Person);

    int low = 0;
    int high = numRecords - 1;
    int mid;
    bool found = false;
    Person p;
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

//---------------------------------------------------------------------------
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

//---------------------------------------------------------------------------

int countRecords(string filename)
{
    ////Open the binary file for reading
    //fstream binFile(filename, ios::in | ios::binary);
    //if (!binFile) {
    //    cout << "Error opening binary file " << filename << endl;
    //    exit(1);
    //}

    //Open the binary file for reading & writing (call utility function)
    fstream binFile = openBinaryFile(filename);

    //Count the number of records in the binary file
    binFile.seekg(0, ios::end);
    int numRecords = binFile.tellg() / sizeof(Person);
    binFile.close();
    return numRecords;
}

//---------------------------------------------------------------------------
void makeBinaryFile(string& csvFilePath, string& binFilePath) {
    // Open the input CSV file
    ifstream csvFile(csvFilePath);
    if (!csvFile) {
        cerr << "Error opening CSV file. " << endl;
        exit(1);
    }

    // Declare the output binary file
    static fstream binFile(binFilePath, ios::out | ios::binary);

    // Read the CSV file and create Person objects
    Person p;
    string strId, strName, strAge, strData;

    while (getline(csvFile, strId, ',')) {
        getline(csvFile, strName, ',');
        getline(csvFile, strAge, ',');
        getline(csvFile, strData);

        p.id = stoi(strId);                 // Convert string to int
        p.age = stoi(strAge);               // Convert string to int
        strcpy_s(p.name, strName.c_str());  // Copy string to c-string
        strcpy_s(p.data, strData.c_str());  // Copy string to c-string

        binFile.write(reinterpret_cast<char*>(&p), sizeof(Person));  // Populate binFile
    }

    csvFile.close();
    binFile.close();                        // Commit changes made to the binary file                         
}

//---------------------------------------------------------------------------
void experiment01()
{
    //Create a binary file from a csv text file
    string csvFilePath = "c:/temp/springfieldPeople.txt";
    string binFilePath = "c:/temp/springfieldPeople.bin";
    makeBinaryFile(csvFilePath, binFilePath);
    cout << "Binary file created from the CSV file." << endl;
}

//---------------------------------------------------------------------------
void experiment02()
{
    //Determine the number of records in the binary file
    string filename = "c:/temp/springfieldPeople.bin";
    int numRecords = countRecords(filename);
    cout << filename << " has " << numRecords << " records " << endl;

}

//---------------------------------------------------------------------------
void experiment03()
{
    //Demonstrate the random access nature of binFile
    //Show backwards navigation, display every other record

    string binFileName = "c:/temp/springfieldPeople.bin";
    
    //How many records are in the binary file?
    int numRecords = countRecords(binFileName);     
    
    //Open the binary file for reading and writing
    fstream binFile(binFileName, ios::in | ios::out | ios::binary);
    if (!binFile) {
        cout << "Error opening binary file " << binFileName << endl;
        exit(1);
    }

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
    binFile.close();
}

//---------------------------------------------------------------------------
void experiment04()
{
    //Add a record to the binary file
    string filename = "c:/temp/springfieldPeople.bin";
    fstream binFile(filename, ios::in | ios::out | ios::binary);
    if (!binFile) {
        cout << "Error opening binary file " << filename << endl;
        exit(1);
    }

    //Add a new record to the binary file
    Person p;
    //Assemble the new record
    p.id = 200;
    p.age = 33;
    strcpy_s(p.name, "Jimbo Jones");
    strcpy_s(p.data, "data200");

    //Write the new record to the binary file
    //move the put-pointer to the end of the file
    binFile.seekp(0, ios::end);         
    binFile.write((char*)&p, sizeof(Person));
    
    cout << "\nNew record added to the binary file: " << endl;
    p.print();

    //Show all records in the binary file
    showBinaryFile(binFile, "Binary file after adding a new record (Jimbo)");
}

//---------------------------------------------------------------------------
void experiment05()
{
    //Modify an existing record - change Bart's data field to "newData"
    //Barts record is at position 2
    
    //Open the binary file for reading and writing
    string binFileName = "c:/temp/springfieldPeople.bin";
    fstream binFile(binFileName, ios::in | ios::out | ios::binary);
    if (!binFile) {
        cout << "Error opening binary file " << binFileName << endl;
        exit(1);
    }

    //Fetch and modify record at position 2 (Bart's record)
    Person p;
    int pos = 2;
    binFile.clear();    //clear the eof flag

    // set the get-pointer to position 2, the read the record
    binFile.seekg(pos * sizeof(Person), ios::beg);    
    binFile.read((char*)&p, sizeof(Person));

    //Show the 'OLD' record
    cout << "\n[step1] Modifying record at position " << pos << endl;
    p.print();

    //change the record's data field to "newData"
    strcpy_s(p.data, "NEWDATA");

    //Show the modified record
    cout << "\n[step2] Modified record at position " << pos << " modified: " << endl;
    p.print();

    //set the put-pointer to position 2 ('Bart's record)
    //write the modified record back to the file
    binFile.seekp(pos * sizeof(Person), ios::beg);        
    binFile.write((char*)&p, sizeof(Person));

    //Show all records in the binary file
    cout << "\n[step3] Binary file after changing rec. #2 (Bart's data): " << endl;
    showBinaryFile(binFile, "New database:");
    binFile.close();
}
//---------------------------------------------------------------------------
void experiment06()
{
    //Search by ID using Linear Search
    //Open the binary file for reading and writing
    string binFileName = "c:/temp/springfieldPeople.bin";
    fstream binFile(binFileName, ios::in | ios::out | ios::binary);
    if (!binFile) {
        cout << "Error opening binary file " << binFileName << endl;
        exit(1);
    }

    //Ask user for a key, apply sequential search, and show the record
    int key;
    do {
        cout << "\n[SEQUENTIAL SEARCH] Enter an ID key to show the record [0 to end]: ";
        cin >> key;
        if (key == 0) break;
        sequentialSearch(binFile, key);
    } while (true);
}
//---------------------------------------------------------------------------
void experiment07()
{
    //Search by ID using Binary Search

    //Open the binary file for reading and writing
    string binFileName = "c:/temp/springfieldPeople.bin";
    fstream binFile(binFileName, ios::in | ios::out | ios::binary);
    if (!binFile) {
        cout << "Error opening binary file " << binFileName << endl;
        exit(1);
    }

    //Ask user for a key, apply sequential search, and show the record
    int key;
    do {
        cout << "\n[BINARY SEARCH] Enter an ID key to show the record [0 to end]: ";
        cin >> key;
        if (key == 0) break;
        binarySearch(binFile, key);
    } while (true);

    binFile.close();
}
//---------------------------------------------------------------------------
void     experiment08()
{
    //Delete a record from the binary file
    //1. Open the binary file for reading and writing
    //2. Open a temporary file for writing
    //3. Copy all records except the one to be deleted
    //4. Close both files
    //5. Delete the original file
    //6. Rename the temporary file to the original file name

    string binFileName = "c:/temp/springfieldPeople.bin";
    string tempFileName = "c:/temp/tempFile.bin";

    fstream binFile = openBinaryFile(binFileName, ios::in);     //Open for reading
    fstream tempFile = openBinaryFile(tempFileName, ios::out);  //Open for writing

    //Delete Bart Simpson's record (ID = 110)
    int    key = 110;
    Person p;
    bool   found = false;
    while (binFile.read((char*)&p, sizeof(Person)))
    {
        if (p.id == key)
        {
            cout << "Record deleted: " << endl;
            p.print();
            found = true;
        }
        else
        {
            tempFile.write((char*)&p, sizeof(Person));
        }
    }
    if (!found)
    {
        cout << "Record not found." << endl;
    }
    //Close both files
    binFile.close();
    tempFile.close();

    //Delete the original file
    remove(binFileName.c_str());

    //Rename the temporary file to the original file name
    rename(tempFileName.c_str(), binFileName.c_str());

    //Show all records in the binary file
    binFile = openBinaryFile(binFileName);
    showBinaryFile(binFile, "Binary file after deleting Bart Simpson's record");

}
//---------------------------------------------------------------------------
void experiment09()
{
    //Create an index to search by ID. Show the index
    //Open the binary file for reading
    string binFileName = "c:/temp/springfieldPeople.bin";
    fstream binFile = openBinaryFile(binFileName, ios::in);

    //Create an index to search by ID (Done only once!)
    vector<int> index;
    Person p;
    int    pos = 0;
    //Read all records and populate the index
    while (binFile.read((char*)&p, sizeof(Person)))
    {
        cout << pos << "\t";    //for debugging
        p.print();              //for debugging
        index.push_back(p.id);
        pos++;
    }
    binFile.clear();            //clear the eof flag
    binFile.seekg(0, ios::beg); //move the get-pointer to the top of the file

    //Show the index
    cout << "\nIndex to search by ID: " << endl;
    cout << "Index\tID" << endl;
    for (int i = 0; i < index.size(); i++)
    {
        cout << i << "\t" << index[i] << endl;
    }

    do {
        //Ask user for a key, search the index (not the file!), and show the record
        int key;
        cout << "\n[SEARCH USING INDEX[ID] ] Enter an ID key to show the record [0 to end]: ";
        cin >> key;
        if (key == 0) break;
        //Using sequential search for now
        bool found = false;
        for (int i = 0; i < index.size(); i++)
        {
            if (index[i] == key)
            {
                found = true;
                cout << "Record found at position " << i << endl;
                binFile.seekg(i * sizeof(Person), ios::beg);    //move get-pointer to the record
                binFile.read((char*)&p, sizeof(Person));        //read the record
                p.print();                                      //display the record    
                break;
            }
        }
        if (!found)
        {
            cout << "Record not found." << endl;
        }
    } while (true);


    binFile.close();
}
//---------------------------------------------------------------------------