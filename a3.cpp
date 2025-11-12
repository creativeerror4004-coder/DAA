#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Structure to represent each relief item
struct Item {
    string name;
    double weight;
    double value;
    bool divisible;  // true = can take fraction (like food/water), false = must take whole (like medicine)
};

// Comparator to sort items by value-to-weight ratio (descending)
bool compare(Item a, Item b) {
    double r1 = a.value / a.weight;
    double r2 = b.value / b.weight;
    return r1 > r2;
}

// Fractional Knapsack function
double fractionalKnapsack(double W, vector<Item> &items) {
    // Sort items by decreasing value/weight ratio
    sort(items.begin(), items.end(), compare);

    double totalValue = 0.0; // total utility value
    double remainingWeight = W;

    cout << "\nSelected items for transport:\n";
    cout << "-----------------------------------------\n";
    cout << "Name\t\tTaken(kg)\tValue\n";

    for (auto &item : items) {
        if (remainingWeight <= 0)
            break;

        if (item.weight <= remainingWeight) {
            // Take the whole item
            totalValue += item.value;
            remainingWeight -= item.weight;
            cout << item.name << "\t\t" << item.weight << "\t\t" << item.value << endl;
        } 
        else {
            // If divisible, take a fraction
            if (item.divisible) {
                double fraction = remainingWeight / item.weight;
                double valueTaken = item.value * fraction;
                totalValue += valueTaken;
                cout << item.name << "\t\t" << remainingWeight << "\t\t" << valueTaken << " (partial)" << endl;
                remainingWeight = 0; // boat is full
            } 
            else {
                // Skip indivisible item if it can't fit
                continue;
            }
        }
    }

    cout << "-----------------------------------------\n";
    cout << "Total utility value carried: " << totalValue << endl;
    return totalValue;
}

// Main function
int main() {
    int n;
    double W;

    cout << "Enter number of items: ";
    cin >> n;

    vector<Item> items(n);

    cout << "Enter details of each item:\n";
    cout << "(name weight value divisible[1=yes,0=no])\n";
    for (int i = 0; i < n; i++) {
        cin >> items[i].name >> items[i].weight >> items[i].value;
        int div;
        cin >> div;
        items[i].divisible = (div == 1);
    }

    cout << "Enter maximum weight capacity of boat: ";
    cin >> W;

    fractionalKnapsack(W, items);

    return 0;
}
