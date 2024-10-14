#include "mythread.h"
#include <iostream>

std::vector<int> getPermutation(std::vector<int> sequence, int k, int fact) {
    int n = sequence.size();
    std::vector<int> permutation;           // To store the k-th permutation
    std::vector<int> availableElements = sequence; // Copy of the original sequence

    for (int i = 0; i < n; ++i) {
        int index = (k / fact) % availableElements.size();             // Determine which element to select
        permutation.push_back(availableElements[index]);  // Select element at 'index'
        availableElements.erase(availableElements.begin() + index); // Remove selected element
        k = k % fact;                   // Update k to find the next element
        if (i < n - 1) {
            fact = fact / (n - 1 - i);
        }
    }

    return permutation;
}

void bogosort(std::vector<int> seq, 
                ThreadManager* pManager, 
                QVector<int> *result, 
                const int start,
                const int end,
                const int maxSequences)
{
    std::vector<int> permutedSeq(seq.size());

    for (int k = start; k < end && !pManager->finished; ++k) {
        permutedSeq = getPermutation(seq,k,maxSequences / seq.size());

        if ((k % (maxSequences / 100)) == 0) {
            pManager->incrementPercentComputed(0.1);
        }

        if (std::is_sorted(permutedSeq.begin(), permutedSeq.end())) {

            pManager->finished.store(true);

            pManager->qMutex.lock();

            if(result->size() == 0){

                for (size_t i = 0; i < permutedSeq.size(); i++) {
                    result->append(permutedSeq[i]);
                }
            }

            pManager->qMutex.unlock();

            break;
        }
    }
}
