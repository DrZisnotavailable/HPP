// circuitSatisfiability.cc – Deel 1 met MPI_Reduce
#include <iostream>
#include <climits>
#include <mpi.h>
#include <cstdlib>

using std::cout;
using std::endl;

#define SIZE 32
#define EXTRACT_BIT(n, i) ((n & (1U << i)) ? 1 : 0)

int checkCircuit(int, bool *);

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int id, numProcs;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    bool *v = (bool*)malloc(sizeof(bool) * SIZE);
    unsigned int combination;
    int localCount = 0, globalCount = 0;

    // Start tijdmeting
    double startTime = MPI_Wtime();

    // Iedere process doorloopt zijn eigen segment:
    // hier simpelweg de volledige range (0..UINT_MAX) per process
    for (combination = 0; combination < UINT_MAX; ++combination) {
        for (int i = 0; i < SIZE; ++i)
            v[i] = EXTRACT_BIT(combination, i);
        localCount += checkCircuit(id, v);
    }

    // Reduce alle localCounts tot één globalCount op rank 0
    MPI_Reduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double totalTime = MPI_Wtime() - startTime;

    // Iedere process kan z'n eigen tijdshow doen:
    cout << "Process " << id
         << " finished in " << totalTime << " seconds." << endl;

    // Alleen rank 0 toont het globale aantal oplossingen
    if (id == 0) {
        cout << "A total of " << globalCount
             << " solutions were found." << endl;
    }

    free(v);
    MPI_Finalize();
    return 0;
}

int checkCircuit(int id, bool *v)
{
    if (( (v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3])
       && (!v[3] || !v[4]) && (v[4] || !v[5])
       && (v[5] || !v[6]) && (v[5] || v[6])
       && (v[6] || !v[15]) && (v[7] || !v[8])
       && (!v[7] || !v[13]) && (v[8] || v[9])
       && (v[8] || !v[9]) && (!v[9] || !v[10])
       && (v[9] || v[11]) && (v[10] || v[11])
       && (v[12] || v[13]) && (v[13] || !v[14])
       && (v[14] || v[15]) )
     && ( (v[16] || v[17]) && (!v[17] || !v[19]) && (v[18] || v[19])
       && (!v[19] || !v[20]) && (v[20] || !v[21])
       && (v[21] || !v[22]) && (v[21] || v[22])
       && (v[22] || !v[31]) && (v[23] || !v[24])
       && (!v[23] || !v[29]) && (v[24] || v[25])
       && (v[24] || !v[25]) && (!v[25] || !v[26])
       && (v[25] || v[27]) && (v[26] || v[27])
       && (v[28] || v[29]) && (v[29] || !v[30])
       && (v[30] || v[31]) )) {
        return 1;
    }
    return 0;
}
