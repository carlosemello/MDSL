// main.cpp

#include "MDSLControl.h"

// PROTOTYPES

void GenerateMDSLFile(string fileName, int n_students);
void OpenMDSLFile(string fileName);
void OpenMDSLFile(string fileName, string structList);

int main(int argc, const char * argv[])
{
    string command;
    
    if(argc < 3)
    {
        cout << endl << "[USAGE]: classroom <command> <file-name> [num-students / structure-list]" << endl;
        exit(0);
    }
    
    command = argv[1];
    
    if(command == "write")
    {
        if(argc == 3)
        {
            GenerateMDSLFile(argv[2], 10);
            exit(0);
        }
        
        if(argc == 4)
        {
            GenerateMDSLFile(argv[2], atoi(argv[3]));
            exit(0);
        }
    }
    
    if(command == "read")
    {
        if(argc == 3)
        {
            OpenMDSLFile(argv[2]);
            exit(0);
        }
        
        if(argc == 4)
        {
            OpenMDSLFile(argv[2], argv[3]);
            exit(0);
        }
    }

    return 0;
}


void GenerateMDSLFile(string fileName, int n_students)
{
    MDSLControl myControl;
    EP ep = EPzeroERROR;
    long mainId, teatcherId, studentId, nameId, gradesId;
    int count = 0;

    // create classroom control...
    mainId = myControl.CreateMainElement("class");
    
    // hire teacher...
    ep = myControl.AddNewElement(mainId, "teacher", "");
    teatcherId = myControl.GetID(mainId, "teacher", count);
    ep = myControl.AddNewElement(teatcherId, "name", "");
    nameId = myControl.GetID(teatcherId, "name", 0);
    ep = myControl.AddNewElement(nameId, "first", "Professor");
    ep = myControl.AddNewElement(nameId, "last", "Wonder");
    
    // enroll students...
    while (count < n_students)
    {
        ep = myControl.AddNewElement(mainId, "student", "");
        if(ep == EPzeroERROR)
        {
            studentId = myControl.GetID(mainId, "student", count);
            ep = myControl.AddNewElement(studentId, "name", "");
            ep = myControl.AddNewElement(studentId, "grades", "");
            nameId = myControl.GetID(studentId, "name", 0);
            ep = myControl.AddNewElement(nameId, "first", "Excellent");
            ep = myControl.AddNewElement(nameId, "last", "Student");
            gradesId = myControl.GetID(studentId, "grades", 0);
            ep = myControl.AddNewElement(gradesId, "art", "10.0");
            ep = myControl.AddNewElement(gradesId, "math", "10.0");
        }
        count++;
    }
    
    myControl.WriteToFile(fileName);
}

void OpenMDSLFile(string fileName)
{
    OpenMDSLFile(fileName, "");
}

void OpenMDSLFile(string fileName, string structList)
{
    MDSLControl myControl;
    EP ep = EPzeroERROR;
    long mainId, nameId, gradesId, tempId;
    long  numDeAlunos;
  
    cout << "Openning Document..." << endl;
    ep = myControl.OpenFile(fileName, structList);
    if(ep != EPzeroERROR)
        cout << endl << "Failed to open document:\t\terror: " << ep << endl << endl;
    else
    {
        cout << endl << "Initiating data extraction..." << endl << endl;
        
        // the first step is to obtain the main element's id...
        mainId = myControl.GetMainID();
        cout << endl << "main element id = " << mainId;
        cout << endl << "main element name = " << myControl.GetTag(mainId);
        
        // then we get the teacher's name ids...
        tempId = myControl.GetID(mainId, "teacher", 0);
        nameId = myControl.GetID(tempId, "name", 0);
        
        // and extract the teacher's firs and last names with
        // the ids obtained in the previous step...
        cout << endl << endl;
        cout << "Teacher: " << myControl.GetDataString(myControl.GetID(nameId, "first", 0));
        cout << " " << myControl.GetDataString(myControl.GetID(nameId, "last", 0));
        cout << endl;
        
        // after that we verify the number of students in the class...
        numDeAlunos = myControl.GetNumberOfElements(mainId, "student");
        
        cout << "Number of students in the class: " << numDeAlunos;
        cout << endl;
        cout << endl << "List of Students" << endl;
        
        // if there are any students...
        if(numDeAlunos > 0)
        {
            for(long i = 0; i < numDeAlunos; i++)
            {
                // we fetch an id for each student,...
                tempId = myControl.GetID(mainId, "student", i);
                // thes obtain the ids for each of their names...
                nameId = myControl.GetID(tempId, "name", 0);
                // and finally reach the name and last name data for each one...
                cout << endl << myControl.GetDataString(myControl.GetID(nameId, "first", 0));
                cout << " "<< myControl.GetDataString(myControl.GetID(nameId, "last", 0));
                // and also the grades for each subject...
                gradesId = myControl.GetID(tempId, "grades", 0);
                cout << endl << "\tGrades: " << endl;
                cout << "\t\tMath: " << myControl.GetDataString(myControl.GetID(gradesId, "math", 0));
                cout << endl << "\t\tArt: " << myControl.GetDataString(myControl.GetID(gradesId, "art", 0));
            }
        }
        
        cout << endl;
    }
}


