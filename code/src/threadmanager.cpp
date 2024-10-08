
#include <QRandomGenerator>
#include <iostream>

#include <pcosynchro/pcothread.h>

#include "threadmanager.h"
#include "mythread.h"



ThreadManager::ThreadManager(QObject *parent) :
    QObject(parent)
{}


/**
 * @brief Cette fonction trie une séquence générée aléatoirement
 * @param seq séquence à trier
 * @param nbThreads le nombre de threads à lancer
 * @return séquence triée
 */
std::vector<int> ThreadManager::startSorting(
        std::vector<int> seq,
        unsigned int nbThreads
)
{
    std::vector<PcoThread *> threads(nbThreads);
    finished = false;
    QVector<int> result;
    std::vector<int> finalResult;
    this->nbThreads = nbThreads;
    
    // TODO création des threads et du vecteur de résultats
    // TODO lancement des threads avec la fonction Bogosort
    // TODO arrêt des threads et récupération du tableau trié
    // TODO retourner le tableau trié

    int maxSequences = factorial(seq.size());
    int sequencesPerThread = maxSequences / nbThreads;

    for(unsigned int i = 0; i < nbThreads; i++){
        int start = i * sequencesPerThread;
        int end = (i == nbThreads - 1)? maxSequences : (i + 1) * sequencesPerThread;

        threads[i] = new PcoThread(
                    bogosort,
                    seq,
                    this,
                    &result,
                    start,
                    end,
                    maxSequences
                    );
    }

    for(const auto &thread: threads){
        thread->join();
        delete thread;
    }

    for(int num : result)
            finalResult.push_back(num);


    return finalResult;
}


void ThreadManager::incrementPercentComputed(double increment)
{
    emit sig_incrementPercentComputed(increment / nbThreads);
}

// Helper function to calculate factorial
int ThreadManager::factorial(int n)
{
    if (n < 0 ) {
        return 0;
    }
    return !n ? 1 : n * factorial(n - 1);
}
