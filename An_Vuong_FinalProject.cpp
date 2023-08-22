// An_Vuong_FinalProject.cpp : Defines the entry point for the application.
//


#include <iostream>    
#include <stdlib.h>  
#include <time.h>      
#include <vector>
#include <map>
#include <String.h>
#include <String>
#include <random>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


#include "An_Vuong_FinalProject.h"

using namespace std;

struct Test
{
    string testDate;
    bool testResult;
};

struct Patient
{
    string name = "";
    int age = 0;
    string postalCode = "";
    vector<Test> patientTestVector;

} patient;

Patient testPatient;
int testPatientIndex;

// Main function
int main(int argc, const char* argv[]) {
    srand(time(NULL));
    
    //create window
    sf::RenderWindow window{ {1500, 800},"TGUI window with sfml" };
    tgui::Gui gui{ window };

    //load the form
    gui.loadWidgetsFromFile("form.txt");
    
    //create gui
    auto nameText = gui.get<tgui::TextArea>("nameText");
    auto ageText = gui.get<tgui::TextArea>("ageText");
    auto pcText = gui.get<tgui::TextArea>("pcText");
    
    auto testDateText = gui.get<tgui::TextArea>("testDateText");
    auto testDateLabel = gui.get<tgui::Label>("testDate");
    auto testListLabel = gui.get<tgui::Label>("testListLabel");
    auto patientTestView = gui.get<tgui::ListView>("patientTestView");


    auto calButton = gui.get<tgui::Button>("calButton");
    auto calHotspotButton = gui.get<tgui::Button>("calHotspotButton");

    testDateText->setVisible(false);
    testDateLabel->setVisible(false);
    testListLabel->setVisible(false);
    patientTestView->setVisible(false);

    auto listView = gui.get<tgui::ListView>("listView");
    
    auto add = gui.get<tgui::Button>("add");
    auto remove = gui.get<tgui::Button>("remove");
    auto createTest = gui.get<tgui::Button>("createTestButton");

    vector<int> positiveTestGroups(20,0);

    createTest->setVisible(false);
    
    // create vector and map
    vector<Patient> patients;
    map<string, int> hotspots;
    
    //show child window
    calHotspotButton->onClick([&] {
        auto hotspotWindow = tgui::ChildWindow::create("hotspotWindow");
        hotspotWindow->setClientSize({ 600,600 });

        auto title = tgui::Label::create("Here's the list of numbers of positive tests per region: ");
        title->setPosition(10, 10);
        hotspotWindow->add(title);


        int i = 0;
        map<string, int>::iterator it;
        for (it = hotspots.begin(); it != hotspots.end(); it++)
        {
            string temp = it->first + ": " + to_string(it->second);
            auto tempLabel = tgui::Label::create(temp);
            cout << temp << endl;
            tempLabel->setPosition(10, (i * 25 + 30));
            hotspotWindow->add(tempLabel);
            i++;
        }

        gui.add(hotspotWindow);
        });
    calButton->onClick([&](){
        auto staWindow = tgui::ChildWindow::create("staWindow");
        staWindow->setClientSize({ 600,600 });
        

        int minAge = INT_MAX;
        int maxAge = 0;

        for(int i=0; i<patients.size();i++){ 
            minAge = min(patients[i].age, minAge);
            maxAge = max(patients[i].age, maxAge);  
        }

        int minGroup = minAge / 5 * 5;
        int maxGroup = maxAge / 5 * 5;

        auto title = tgui::Label::create("Here's the list of numbers of positive tests per age group: ");
        title->setPosition(10, 10);
        staWindow->add(title);

        for (int i = minGroup; i <= maxGroup; i += 5) {
            string temp = to_string(i) + "," + to_string(i + 4) + ": " + to_string(positiveTestGroups[i/5]);
            auto tempLabel = tgui::Label::create(temp);
            cout << temp << endl;
            tempLabel->setPosition(10, (i - minGroup) * 5 + 30);    
            staWindow->add(tempLabel);
        } 

        gui.add(staWindow);
    });

    // functions interact with users
    //add patient
    add->onClick([&]() {
        
        
        string name = nameText->getText().toStdString();
        patient.name = name;
        int age = ageText->getText().toInt();
        patient.age = age;
        string postalCode = pcText->getText().toStdString();
        patient.postalCode = postalCode;
        //add patient to a vector patients
        patients.push_back(patient);
        //add patient to list view
        listView->addItem(patient.name);

        if (hotspots.find(postalCode.substr(0,3)) == hotspots.end()) {
            hotspots[postalCode.substr(0,3)] = 0;
        }
     });

    //remove patient
    remove->onClick([&]() {

        string name = nameText->getText().toStdString();
        patient.name = name;
        int age = ageText->getText().toInt();
        patient.age = age;
        string postalCode = pcText->getText().toStdString();
        patient.postalCode = postalCode;

        auto patientPosition = -1;

        for (int i = 0; i < patients.size(); i++) {
            if (patients.at(i).name == patient.name) {
                patientPosition = i;
                break;
            }
        }

    
        //check if patient is registered 
        if (patientPosition != -1)
        {
            
            // Remove patient from map
            string postalCodePrefix = patient.postalCode.substr(0, 3); // take 3 first code of the postal code

            int numPositiveTests = 0;
            for (int i = 0; i < patients.at(patientPosition).patientTestVector.size(); i++) {
                if (patients.at(patientPosition).patientTestVector.at(i).testResult) {
                    numPositiveTests++;
                }
            }
            hotspots[postalCodePrefix] -= numPositiveTests;
            positiveTestGroups[patient.age / 5] -= numPositiveTests;
            

            // Remove patient from vector
           

            testDateText->setVisible(false);
            testDateLabel->setVisible(false);
            createTest->setVisible(false);
            testListLabel->setVisible(false);
            patientTestView->setVisible(false);
            listView->removeItem(patientPosition);
        }
        else
        {
            std::cout << "Patient not found" << std::endl;
        }
        
        });

    listView->onItemSelect([&] {
        testDateText->setVisible(true);
        testDateLabel->setVisible(true); 
        createTest->setVisible(true);
        testListLabel->setVisible(true);
        patientTestView->setVisible(true);

        patientTestView->removeAllItems();

        int itemIndex = listView->getSelectedItemIndex();
        try {
            testPatient.name = listView->getItem(itemIndex).toStdString();
            testPatientIndex = itemIndex;

            testListLabel->setText("Patient " + testPatient.name + " Test List: ");

            for (int i = 0; i < patients.at(itemIndex).patientTestVector.size(); i++) {
                Test tempTest = patients.at(itemIndex).patientTestVector.at(i);
                patientTestView->addItem("Test Date: " + tempTest.testDate + ", Result: " + (tempTest.testResult ? "Positive" : "Negative"));

            }
        }
        catch (exception e) {

        }
        
 
        });

    createTest->onClick([&] { 
        Test tempTest;
        auto testChance = rand() % 100 + 1;
        tempTest.testResult = false;

        if (testChance >= 95) {
            tempTest.testResult = true;
        }

        tempTest.testDate = testDateText->getText().toStdString();
        patients.at(testPatientIndex).patientTestVector.push_back(tempTest);
        if (tempTest.testResult) {
  
            positiveTestGroups[patients.at(testPatientIndex).age / 5] = positiveTestGroups[patients.at(testPatientIndex).age / 5] + 1;
            string postalCodePrefix = patients.at(testPatientIndex).postalCode.substr(0, 3);
            hotspots[postalCodePrefix]++;
            
        }
        
        patientTestView->addItem("Test Date: " + tempTest.testDate + ", Result: " + (tempTest.testResult ? "Positive" : "Negative"));
        
        });

    // Add an if to check whenever clicked outside of patient
    /*testDateText->setVisible(false);
    testDateLabel->setVisible(false);
    createTest->setVisible(false);
    testListLabel->setVisible(false);
    patientTestView->setVisible(false);*/

    /*listView->onDoubleClick([&](int index) {
        patients[index]-> 
        });*/
    
    

    gui.mainLoop();
    
    return 0;
}




