#include <iostream>
#include <iomanip>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <vector>
#include <numeric>
#include <string>
#include <limits>
#include <symcalc/symcalc.hpp>
#include <fstream>
#include <cstdlib>

using namespace symcalc;

bool negativeZeroCheck(double time, void (*func)()){
    if (time <= 0){
        std::cout << "Invalid input please try again." << std::endl;
        func();
        return false;
    }
    return true;
}

struct Vector2D {
    double x;
    double y;
    double angle;
    double magnitude;
};

void vectorToCSV (const Vector2D& vector, const std::string filename = "vectorData.csv"){
    std::ifstream checkFile(filename);
    bool fileExists = checkFile.good();
    checkFile.close();

    std::ofstream outFile;
    outFile.open(filename, std::ios::app);

    if(outFile.is_open()){
        if(!fileExists){
            outFile << "X_Component,Y_Component,Magnitude,Angle_Degrees\n";
        }
        outFile << vector.x << "," << vector.y << "," << vector.magnitude << "," << vector.angle << "\n";
        outFile.close();
    }
    else {
        std::cerr << "Unable to open file for writing!"<< std::endl;
    }

}

void equationToCSV (Equation equation, const std::string filename = "equationData.csv"){
    Equation t("t");

    std::ofstream outFile;
    outFile.open(filename);

    double startTime, endTime, timeStep;
    std::cout << "Enter Start Time (s): ";
    std::cin >> startTime;
    std::cout << "Enter End Time (s): ";
    std::cin >> endTime;
    std::cout << "Enter Time Step (s): ";
    std::cin >> timeStep;

    if (outFile.is_open())
    {
        outFile << "T_Component, S_Component, V_Component, A_Component\n";
        for (double tComponent = startTime; tComponent <= endTime; tComponent += timeStep){
            double sComponent = equation.eval({{t, tComponent}});
            double vComponent = equation.derivative().eval({{t, tComponent}});
            double aComponent = equation.derivative(2).eval({{t, tComponent}});
            outFile << tComponent << "," << sComponent << "," << vComponent << "," << aComponent << "\n";
        }
        outFile.close();
        std::cout << "Launching Python script..." << std::endl;
        std::system("python3 /Users/devon/C++/partiKinematics/graphing.py");
    }
    else
    {
        std::cerr << "Unable to open file for writing!" << std::endl;
    }
}

Vector2D createVector(double velocityX, double velocityY){
    Vector2D vector;
    vector.x = velocityX;
    vector.y = velocityY;
    vector.magnitude = std::sqrt(std::pow(velocityX, 2) + std::pow(velocityY, 2));
    vector.angle = (std::atan2(velocityY, velocityX)) * 180 / Constants::Pi();
    return vector;
}

Equation termCall(std::string name = "position"){
    Equation t("t");
    Equation position(0.0);
    int terms;
    std::cout << "How many terms are in the " << name << " equation? (e.g. 3 for 5t^2 + 2t + 1): ";

    if (!(std::cin >> terms))
        return position;

    for (int i = 0; i < terms; i++)
    {
        int type;
        std::cout << "Enter term type (sin[1], cos[2], polynomial[3]) for term " << (i + 1) << ": ";
        std::cin >> type;
        Equation term(0.0);
        if (type == 1) {
            double coefficientOutter, coefficientInner;
            std::cout << "Enter coefficient (A) for A*sin(B*t): ";
            std::cin >> coefficientOutter;
            std::cout << "Enter inner coefficient (B) for A*sin(B*t): ";
            std::cin >> coefficientInner;
            term = (Equation(coefficientOutter) * sin(Equation(coefficientInner) * t));
        } 
        else if (type == 2) {
            double coefficientOutter, coefficientInner;
            std::cout << "Enter coefficient (A) for A*cos(B*t): ";
            std::cin >> coefficientOutter;
            std::cout << "Enter inner coefficient (B) for A*cos(B*t): ";
            std::cin >> coefficientInner;
            term = (Equation(coefficientOutter) * cos(Equation(coefficientInner) * t));
        }
        else if (type == 3) {
            double coefficient, power;
            std::cout << "Enter coefficient for term " << (i + 1) << ": ";
            if (!(std::cin >> coefficient)) return position;
            std::cout << "Enter power for term " << (i + 1) << ": ";
            if (!(std::cin >> power)) return position;
            term = (Equation(coefficient) * t.pow(Equation(power)));
        }
        else {
            std::cout << "Invalid term type. Please try again." << std::endl;
            return position;
        }
        position = position + term;
    }

    std::cout << "The " << name << " equation is: " << position << std::endl;
    return position;
}

void calculateAverageVelocity () {
    double finalPos, startPos, finalTime, startTime;
    std::cout << "\n--- Calclate Average velocity ---" << std::endl;
    std::cout << "Enter Start Positon (m) : ";
    std::cin >> startPos;
    std::cout << "Enter Final Positon (m) : ";
    std::cin >> finalPos;
    std::cout << "Enter Start Time (s) : ";
    std::cin >> startTime;
    std::cout << "Enter Final Time (s) : ";
    std::cin >> finalTime;

    double totalTime = (finalTime - startTime);

    if (!negativeZeroCheck(totalTime, calculateAverageVelocity)) return;

    double avgvelocity = (finalPos - startPos) / totalTime;

    std::cout << "The Average velocity is " << avgvelocity << " m/s." << std::endl;
    std::cout << "Would you like to calculate the velocity Vector? (y/n) ";
    char choice;
    std::cin >> choice;
    if (choice == 'y'){
        double finalPosX, startPosX, finalPosY, startPosY, timeChange;
        std::cout << "\nWhat is the start position of X (m) : ";
        std::cin >> startPosX;
        std::cout << "What is the final position of X (m) : ";
        std::cin >> finalPosX;
        std::cout << "What is the start position of Y (m) : ";
        std::cin >> startPosY;
        std::cout << "What is the final position of Y (m) : ";
        std::cin >> finalPosY;
        std::cout << "What is the change in time (s) : ";
        std::cin >> timeChange;

        double velocityX = (finalPosX - startPosX) / timeChange;

        double velocityY = (finalPosY - startPosY) / timeChange;

        Vector2D velocityVector = createVector(velocityX, velocityY);

        std::cout << "velocity is " << velocityVector.magnitude << " m/s." << "\n" << "At an angle " << velocityVector.angle << " degrees." << std::endl;

        Vector2D robotvelocity = createVector(velocityX, velocityY);
        vectorToCSV(robotvelocity);
    }
    else if (choice == 'n') return;
    else std::cout << "Invalid choice. Please try again." << std::endl;
}

void calculateInstantaneousVelocity () {
    Equation t("t");

    std::cout << "\n--- Calclate Instantaneous velocity ---" << std::endl;

    Equation position = termCall();

    Equation dsdt = position.derivative();

    std::cout << "The derivative of the position equation is: " << dsdt << std::endl;

    equationToCSV(position);

    double time;
    std::cout << "Enter the time: ";
    std::cin >> time;
    
    if (!negativeZeroCheck(time, calculateInstantaneousVelocity)) return;

    double instantaneousvelocity = dsdt.eval({{t, time}});

    if (instantaneousvelocity == std::numeric_limits<double>::infinity()){
        std::cout << "That is approaching infinity you mong!";
    }
    else std::cout << "The instantaneous velocity at time " << time << " is: " << instantaneousvelocity << "m/s" <<std::endl;

}

void calculateInstantaneousAcceleration(){
    Equation t("t");
    std::cout << "\n--- Calclate Instantaneous Acceleration ---" << std::endl;
    
    Equation position = termCall();

    Equation dsdt = position.derivative();
    Equation d2sdfinalTime = position.derivative(2);

    std::cout << "The first derivative of the poisition equation is:" << dsdt << std::endl;
    std::cout << "The second derivative of the position equation is: " << d2sdfinalTime << std::endl;

    equationToCSV(position);

    double time;
    std::cout << "Enter the time: ";
    std::cin >> time;
    
    if (!negativeZeroCheck(time, calculateInstantaneousAcceleration)) return;

    double instantaneousAcceleration = d2sdfinalTime.eval({{t, time}});



    if (instantaneousAcceleration == std::numeric_limits<double>::infinity()){
        std::cout << "That is approaching infinity you mong!";
    }
    else
        std::cout << "The instantaneous acceleration at time " << time << " is: " << instantaneousAcceleration << "m/s^2" << std::endl;
}

void calculateAverageAcceleration(){
    Equation t("t");
    std::cout << "\n--- Calclate Average Acceleration ---" << std::endl;
    std::cout << "Do you have an equation in terms of velocity? (y/n) ";
    char choice;
    std::cin >> choice;
    if (choice == 'n'){
        std::cout << "\n Please enter the Position Equation details:" << std::endl;
        Equation position = termCall();
        Equation velocity = position.derivative();
        std::cout << "velocity Equation derived: " << velocity << std::endl;

        double initialTime, finalTime, runTime;
        std::cout << "Enter Start Time (s): ";
        std::cin >> initialTime;
        std::cout << "Enter Final Time (s): ";
        std::cin >> finalTime;
        runTime = finalTime - initialTime;
        if (!negativeZeroCheck(runTime, calculateAverageAcceleration))return;

        double initialvelocity = velocity.eval({{t, initialTime}});
        double finalvelocity = velocity.eval({{t, finalTime}});

        double avgAccel = (finalvelocity - initialvelocity) / (finalTime - initialTime);
        std::cout << "Average Acceleration is: " << avgAccel << " m/s^2" << std::endl;
        
        return;
    }
    else if (choice == 'y'){
        std::cout << "\n Please enter the velocity Equation details:" << std::endl;
        Equation velocity = termCall("velocity");


        double initialTime, finalTime;
        std::cout << "Enter Start Time (s): ";
        if (!(std::cin >> initialTime)) return;
        std::cout << "Enter Final Time (s): ";
        if (!(std::cin >> finalTime)) return;

        double velocityStart = velocity.eval({{t, initialTime}});
        double velcoityFinal = velocity.eval({{t, finalTime}});

        double averageAcceleration = (velcoityFinal - velocityStart) / (finalTime - initialTime);
        std::cout << "Average Acceleration is: " << averageAcceleration << " m/s^2" << std::endl;
    }
    else std::cout << "Invalid choice. Please try again." << std::endl;

    std::cout << "Would you like to calculate the Acceleration Vector? (y/n) ";
    std::cin >> choice;
    if (choice == 'y'){
        double finalVelX, startVelX, finalVelY, startVelY, timeChange;
        std::cout << "\nWhat is the start position of X (m) : ";
        std::cin >> startVelX;
        std::cout << "What is the final position of X (m) : ";
        std::cin >> finalVelX;
        std::cout << "What is the start position of Y (m) : ";
        std::cin >> startVelY;
        std::cout << "What is the final position of Y (m) : ";
        std::cin >> finalVelY;
        std::cout << "What is the change in time (s) : ";
        std::cin >> timeChange;
        if (timeChange <= 0) {
            std::cout << "Invalid input please try again.";
            return;
        }

        double velocityX = (finalVelX - startVelX) / timeChange;

        double velocityY = (finalVelY - startVelY) / timeChange;

        Vector2D velocityVector = createVector(velocityX, velocityY);

        std::cout << "velocity is " << velocityVector.magnitude << " m/s2." << "\n" << "At an angle " << velocityVector.angle << " degrees." << std::endl;
        Vector2D robotvelocity = createVector(velocityX, velocityY);
        vectorToCSV(robotvelocity);
    }
    else if (choice == 'n') return;
    else std::cout << "Invalid choice. Please try again." << std::endl;
}

int main(){
    int choice;
    while (true) {
        std::cout << "\n--- Kinematics Calculator ---" << std::endl;
        std::cout << "What are we calculating today my liege?" << std::endl;
        std::cout << "1. Calculate Average Velocity" << std::endl;
        std::cout << "2. Calculate Instantaneous Velocity" << std::endl;
        std::cout << "3. Calculate Instantaneous Acceleration" << std::endl;
        std::cout << "4. Calculate Average Acceleration" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Enter your choice: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        if (choice == 1) calculateAverageVelocity();
        else if (choice == 2) calculateInstantaneousVelocity();
        else if (choice == 3) calculateInstantaneousAcceleration();
        else if (choice == 4) calculateAverageAcceleration();
        else if (choice == 5) break;
        else std::cout << "Invalid choice. Please try again." << std::endl;
    }

    return 0;
}