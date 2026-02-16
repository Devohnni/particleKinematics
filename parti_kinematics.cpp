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

using namespace symcalc;

bool negativeZeroCheck(double time, void (*func)()){
    if (time <= 0){
        std::cout << "Invalid input please try again." << std::endl;
        func();
        return false;
    }
    return true;
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
        double coefficient, power;
        std::cout << "Enter coefficient for term " << (i + 1) << ": ";
        if (!(std::cin >> coefficient))
            return position;
        std::cout << "Enter power for term " << (i + 1) << ": ";
        if (!(std::cin >> power))
            return position;
        position = position + (Equation(coefficient) * t.pow(Equation(power)));
    }

    std::cout << "The " << name << " equation is: " << position << std::endl;
    return position;
}

void calculateAverageVelocity () {
    double finalPos, startPos, finalTime, startTime;
    std::cout << "\n--- Calclate Average Velocity ---" << std::endl;
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

    double avgVeloctiy = (finalPos - startPos) / totalTime;

    std::cout << "The Average Velocity is " << avgVeloctiy << " m/s." << std::endl;
    std::cout << "Would you like to calculate the Velocity Vector? (y/n) ";
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

        double VeloctiyX = (finalPosX - startPosX) / timeChange;

        double VeloctiyY = (finalPosY - startPosY) / timeChange;
        double VeloctiyVector = std::sqrt(std::pow(VeloctiyX, 2) + std::pow(VeloctiyY, 2));

        double VeloctiyAngle = (std::atan2(VeloctiyY, VeloctiyX)) * 180 / Constants::Pi();

        std::cout << "Velocity is " << VeloctiyVector << " m/s." << "\n" << "At an angle " << VeloctiyAngle << " degrees." << std::endl;
    }
    else if (choice == 'n') return;
    else std::cout << "Invalid choice. Please try again." << std::endl;
}

void calculateInstantaneousVelocity () {
    Equation t("t");

    std::cout << "\n--- Calclate Instantaneous Velocity ---" << std::endl;

    Equation position = termCall();

    Equation dsdt = position.derivative();

    std::cout << "The derivative of the position equation is: " << dsdt << std::endl;

    double time;
    std::cout << "Enter the time: ";

    if (!negativeZeroCheck(time, calculateAverageVelocity))return;

    double instantaneousVelocity = dsdt.eval({{t, time}});

    if (instantaneousVelocity == std::numeric_limits<double>::infinity()){
        std::cout << "That is approaching infinity you mong!";
    }
    else std::cout << "The instantaneous velocity at time " << time << " is: " << instantaneousVelocity << "m/s" <<std::endl;

}

void calculateInstantaneousAcceleration(){
    Equation t("t");
    std::cout << "\n--- Calclate Instantaneous Acceleration ---" << std::endl;
    
    Equation position = termCall();

    Equation d2sdfinalTime = position.derivative(2);

    std::cout << "The second derivative of the position equation is: " << d2sdfinalTime << std::endl;

    double time;
    std::cout << "Enter the time: ";
    if (!negativeZeroCheck(time, calculateAverageVelocity))return;

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
        std::cout << "Velocity Equation derived: " << velocity << std::endl;

        double initialTime, finalTime, runTime;
        std::cout << "Enter Start Time (s): ";
        std::cin >> initialTime;
        std::cout << "Enter Final Time (s): ";
        std::cin >> finalTime;
        runTime = finalTime - initialTime;
        if (!negativeZeroCheck(runTime, calculateAverageVelocity))return;

        double initialVelocity = velocity.eval({{t, initialTime}});
        double finalVelocity = velocity.eval({{t, finalTime}});

        double avgAccel = (finalVelocity - initialVelocity) / (finalTime - initialTime);
        std::cout << "Average Acceleration is: " << avgAccel << " m/s^2" << std::endl;
        
        return;
    }
    else if (choice == 'y'){
        std::cout << "\n Please enter the Velocity Equation details:" << std::endl;
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

        double VeloctiyX = (finalVelX - startVelX) / timeChange;

        double VeloctiyY = (finalVelY - startVelY) / timeChange;
        double VeloctiyVector = std::sqrt(std::pow(VeloctiyX, 2) + std::pow(VeloctiyY, 2));

        double VeloctiyAngle = (std::atan2(VeloctiyY, VeloctiyX)) * 180 / Constants::Pi();

        std::cout << "Velocity is " << VeloctiyVector << " m/s2." << "\n" << "At an angle " << VeloctiyAngle << " degrees." << std::endl;
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
