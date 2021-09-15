// Project 1: DNA Profiling
// By: Luis Campos
//
// Creative Component: command = "list"
// List command outputs the list of names inside the database as well
// as the total number of names in the database. "list" will output
// the list only when a database is loaded, otherwise the output will
// be "No database loaded."

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "ourvector.h"

using namespace std;

// struct for name & dna values
struct T {
    string name;
    ourvector<double> dna;
};

struct STR {
    ourvector<ourvector<char>> str;
};

// Function prototypes
void parseLine(string line, T &newData);
void display(ourvector<T> &data);
void load_db(string fileName, ourvector<T> &data);
void load_dna(string fileName, ourvector<T> &dnaData);
int commaCount = 0;

// parses line, seperated by comma
void parseLine(string line, T &newData) {
    stringstream ss(line);
    getline(ss, newData.name, ',');
    int count = 0;
    while (!ss.eof()) {
        if (count < commaCount) {
            string dna1;
            getline(ss, dna1, ',');
            newData.dna.push_back(stod(dna1));
            count++;
        }
    }
}
// loads database file, stores in vector
void load_db(string fileName, ourvector<T> &data) {
    ifstream inFile(fileName);
    string line;
    getline(inFile, line, '\n');
     // counts number of commas aka number of DNA strands
    char ch = ',';
    for (int i = 0; (i = line.find(ch, i)) != string::npos; i++) {
        commaCount++;
    }
    while (!inFile.eof()) {
        getline(inFile, line, '\n');
         //
        if (!inFile.fail()) {
            T newData;
            parseLine(line, newData);
            data.push_back(newData);
        }
    }
}
// loads dna file, stores in vector
void load_dna(string fileName, ourvector<T> &dnaData) {
    ifstream inFile(fileName);
    string line;
    T newdnaData;
    getline(inFile, line, '\n');
    parseLine(line, newdnaData);
    dnaData.push_back(newdnaData);
}
// displays every element in vector
void display(ourvector<T> &data) {
    for (auto st : data) {
        cout << st.name << " ";
        for (auto dna : st.dna) {
            cout << dna << " ";
        }
        cout << endl;
    }
    return;
}
// used to parse STR
void STRparse(string &STRchar, string &STRstring, ourvector<string> &
strData) {
    for (int i=0; i < STRstring.length(); ++i) {
            STRchar = STRstring.at(i);
            strData.push_back(STRchar);
        }
    strData.push_back(" ");
}
// stores STR data
void storeSTR(string fileName, ourvector<string> &strData,
ourvector<T> &dnaData) {
    ifstream inFile(fileName);
    string STRstring;
    string STRchar;
    getline(inFile, STRstring, ',');
    for (int i = 0; i < (commaCount - 1); i++) {
        getline(inFile, STRstring, ',');
        STRparse(STRchar, STRstring, strData);
    }
    getline(inFile, STRstring, '\n');
    STRparse(STRchar, STRstring, strData);
}
// bool function returns true if entire STR exists, false otherwise
bool match(string &STRstring, ourvector<T> &dnaData, int idx) {
    for (int i = 0; i < STRstring.size();) {
        for (auto st : dnaData) {
            if (idx >= (st.name).size()) {  // idx out of range
                return false;
            } else if (STRstring.at(i) != (st.name).at(idx)) { // char !=
                return false;
            }
        }
        i++;
        idx++;
    }
    return true;
}
// count the number of STR inside DNA
int STRcount(string &str, ourvector<string> &strData, ourvector<T> &
dnaData) {
    int forCount = 0;
    int count = 0;
    int j = 0;
    bool consec = 0;
    bool end = 0;
    for (auto st : dnaData) {
            for (int i = 0; i < ((st.name).size());) {
                forCount++;
                 // check if char matches first char of str
                if ((st.name).at(i) == str.at(0)) {
                    if (match(str, dnaData, i) == true) {
                        if (consec == true) {
                            count++;
                        }
                        i = i + str.size();  // update index position
                        consec = true;  // set consecutive to true
                    } else {
                         // advance to next char
                        i++;
                        consec = 0;
                    }
                } else {
                     // advance to next char in dna
                    i++;
                    consec = 0;
                }
            }
        }
    return count+1;
}
// process dna function, shows STR count
void process(ourvector<string> &strData, ourvector<T> &dnaData,
ourvector<double> &result) {
    ourvector<string> STRstring;
    string fullData;
    int i = 0;
    for (auto st : strData) {
        fullData += strData[i];
        i++;
		}
		stringstream s_stream(fullData);  // create string stream from the string
		for (int i = 0; i < commaCount; i++) {
					string substr;
					getline(s_stream, substr, ' ');  // get first string delimited by comma
					cout << substr << ": " << STRcount(substr, strData, dnaData) <<
					endl;
					result.push_back(STRcount(substr, strData, dnaData));
		}
	}
// search function to check if STR matches any name in database
void search(ourvector<T> &data, ourvector<T> &dnaData, ourvector<double> &
result) {
  ourvector<double> newData;
  string comp1 = "";
  string comp2 = "";
  ourvector<string> nameList;
  int nameCount = 0;
  int i = 0;
  int k = 0;
  for (auto st : data) {
        nameList.push_back(st.name);
        for (auto dna : st.dna) {
            newData.push_back(dna);
        }
  }
  for (auto st : newData) {
    comp1 += newData[i];
    i++;
  }
  for (auto st : result) {
    comp2 += result[k];
    k++;
  }
  int j = 0;
  for (auto st : nameList) {
    nameCount++;
    j++;
  }
  int pos = 0;
  int index;
		// check if STR counts match anyone from database
   if ((index = comp1.find(comp2, pos)) != string::npos) {
      cout << "Found in database! DNA matches: " << nameList[index / 
      commaCount] << endl;
      pos = index + 1;  // new position is from next element of index
   } else {
    cout << "Not found in database." << endl;
  }
}
// creative component function: list
void list(ourvector<T> &data) {
	int count = 0;
	int i = 0;
	cout << "\nDatabase names list:" << endl;
	for (auto st : data) {
		cout << st.name << endl;
		count++;
	}
	cout << "\nTotal # of people in database: " << count << endl;
}

// main
int main() {
    ourvector<T> data;
    ourvector<T> dnaData;
    ourvector<string> strData;
    ourvector<double> result;
    string command = "";
    string fileName = "";
    bool dnaLoaded = 0;
    bool dnaProcessed = 0;
    bool dbLoaded = 0;
    cout << "Welcome to the DNA Profiling Application." << endl;
    while (command != "#") {
        cout << "Enter command or # to exit: ";
        cin >> command;
         // load_db command
        if (command == "load_db") {
            cin >> fileName;
            if (fileName == "small.txt" || fileName == "large.txt") {
                load_db(fileName, data);
                cout << "Loading database..." << endl;
                dbLoaded = true;
                storeSTR(fileName, strData, dnaData);
            } else {
                cout << "Loading database..." << endl;
                cout << "Error: unable to open '" << fileName << "'" <<
                endl;
            }
         // load_dna command
        } else if (command == "load_dna") {
             // cout << "DNA TIME" << endl;
            cin >> fileName;
            if (fileName == "1.txt" || fileName == "2.txt" || fileName ==
            "3.txt" || fileName == "4.txt" || fileName == "5.txt" ||
            fileName == "6.txt" || fileName == "7.txt" || fileName ==
            "8.txt" || fileName == "9.txt" || fileName == "10.txt" ||
            fileName == "11.txt" || fileName == "12.txt" || fileName ==
            "13.txt" || fileName == "14.txt" || fileName == "15.txt" ||
            fileName == "16.txt" || fileName == "17.txt" || fileName ==
            "18.txt" || fileName == "19.txt" || fileName == "20.txt") {
                cout << "Loading DNA..." << endl;
                dnaLoaded = true;
                load_dna(fileName, dnaData);
            } else {
                cout << "Loading DNA..." << endl;
                cout << "Error: unable to open '" << fileName << "'" <<
                endl;
            }
					// process
        } else if (command == "process") {
          if (dbLoaded == false) {
            cout << "No database loaded." << endl;
          } else if (dnaLoaded == false) {
            cout << "No DNA loaded." << endl;
          } else {
            cout << "Processing DNA..." << endl;
						dnaProcessed = true;
          }
						// search
        } else if (command == "search") {
          if (dbLoaded == false) {
            cout << "No database loaded." << endl;
          } else if (dnaLoaded == false) {
            cout << "No DNA loaded" << endl;
          } else if (dnaProcessed == false){
						cout << "No DNA processed." << endl;
					} else {
            cout << "Searching database..." << endl;

            search(data, dnaData, result);
          }
						// display
        } else if (command == "display") {
             // database loaded?
            if (dbLoaded == true) {
                cout << "Database loaded:" << endl;
                display(data);
            } else if (dbLoaded == false) {
                cout << "No database loaded." << endl;
            }
             // dna loaded?
            if (dnaLoaded == true) {
                cout << "DNA loaded:" << endl;
                display(dnaData);
            } else if (dnaLoaded == false) {
                cout << "\nNo DNA loaded." << endl;
            }
             // dna processed?
            if (dnaProcessed == true) {
                 // DNA Processed, STR counts
                cout << "DNA processed, STR counts:" << endl;
                process(strData, dnaData, result);
            } else if (dnaProcessed == false) {
                cout << "\nNo DNA has been processed." << endl;
            }
					// list
        } else if (command == "list") {
					if (dbLoaded == true) {
						list(data);
					} else {
						cout << "No database loaded." << endl;
					}
				}
    }
    return 0;
}