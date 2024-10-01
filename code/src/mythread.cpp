#include "mythread.h"
#include <iostream>

// Helper function to calculate factorial
int factorial(int n)
{
    if (n < 0 ) {
        return 0;
    }
    return !n ? 1 : n * factorial(n - 1);
}

std::vector<int> getPermutation(std::vector<int> sequence, int k) {
    int n = sequence.size();
    std::vector<int> permutation;           // To store the k-th permutation
    std::vector<int> availableElements = sequence; // Copy of the original sequence

    for (int i = 0; i < n; ++i) {
        int fact = factorial(n - 1 - i);  // Calculate (n-1)! to determine the block size
        int index = (k / fact) % availableElements.size();             // Determine which element to select
        permutation.push_back(availableElements[index]);  // Select element at 'index'
        availableElements.erase(availableElements.begin() + index); // Remove selected element
        k = k % fact;                     // Update k to find the next element
    }

    return permutation;
}

// Function to calculate how much the vector is sorted (percentage of sorted adjacent elements)
double howMuchSorted(const std::vector<int>& arr) {
    int sortedCount = 0;
    int totalComparisons = arr.size() - 1;

    if(arr.size() == 1) return 1.0;

    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i] >= arr[i - 1]) {
            sortedCount++;
        }
    }

    return static_cast<double>(sortedCount) / totalComparisons;
}

void bogosort(std::vector<int> seq, ThreadManager* pManager, QVector<int> *result)
{

    srand (time(NULL));

    int maxSequences = factorial(seq.size());
    double lastProgress = 0;

    while(!pManager->finished){

        int k = rand() % maxSequences;

        seq = getPermutation(seq, k);

        double percentFinished = howMuchSorted(seq);

        if (percentFinished > lastProgress) {
           pManager->incrementPercentComputed(percentFinished);
           lastProgress = percentFinished;
        }

        if(percentFinished > 0.99){

            pManager->finished = true;

            for(size_t i = 0; i < seq.size(); i++)
                result->append(seq[i]);
        }
    }
}
