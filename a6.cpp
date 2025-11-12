#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int knapsack(int W, vector<int> &weights, vector<int> &utilities, int N, vector<int> &chosenItems) {
    vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= N; i++) {
        for (int w = 1; w <= W; w++) {
            if (weights[i - 1] <= w) {
                dp[i][w] = max(utilities[i - 1] + dp[i - 1][w - weights[i - 1]], dp[i - 1][w]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    int w = W;
    for (int i = N; i > 0 && w > 0; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            chosenItems.push_back(i); 
            w -= weights[i - 1];
        }
    }

    return dp[N][W];
}

int main() {
    int N, W;
    cout << "Enter number of items: ";
    cin >> N;

    cout << "Enter truck capacity (in kg): ";
    cin >> W;

    vector<int> weights(N), utilities(N);
    cout << "\nEnter weight and utility of each item:\n";
    for (int i = 0; i < N; i++) {
        cout << "Item " << i + 1 << " (weight utility): ";
        cin >> weights[i] >> utilities[i];
    }

    vector<int> chosenItems;
    int maxUtility = knapsack(W, weights, utilities, N, chosenItems);

    cout << "\nMaximum total utility value = " << maxUtility << endl;
    cout << "Items chosen (1-based index): ";

    reverse(chosenItems.begin(), chosenItems.end());
    for (int item : chosenItems)
        cout << item << " ";
    cout << endl;

    cout << "\nChosen items details:\n";
    cout << "Item\tWeight\tUtility\n";
    for (int item : chosenItems) {
        cout << item << "\t" << weights[item - 1] << "\t" << utilities[item - 1] << endl;
    }

    return 0;
}


