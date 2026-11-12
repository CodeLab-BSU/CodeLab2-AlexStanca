#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

void collectUserData(double& carMiles, double& electricityKwh) {
    cout << "Monthly car miles travelled: ";
    cin >> carMiles;

    cout << "Monthly electricity usage (kWh): ";
    cin >> electricityKwh;
}

double calculateTotalEmissions(double carMiles, double electricityKwh) {
    return (carMiles * 0.25) + (electricityKwh * 0.20);
}

string classifyResult(double totalEmissions) {
    if (totalEmissions < 100) {
        return "Low";
    } else if (totalEmissions <= 300) {
        return "Moderate";
    } else {
        return "High";
    }
}

int main() {
    double carMiles, electricityKwh;
    collectUserData(carMiles, electricityKwh);

    double total = calculateTotalEmissions(carMiles, electricityKwh);

    cout << fixed << setprecision(2);
    cout << "\nTotal monthly emissions: " << total << " kg CO2\n";
    cout << "Classification: " << classifyResult(total) << '\n';

    return 0;
}