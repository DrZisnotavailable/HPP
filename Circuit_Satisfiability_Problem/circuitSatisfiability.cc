/* circuitSatifiability.cpp solves the Circuit Satisfiability
 *  Problem using a brute-force sequential solution.
 *
 *   The particular circuit being tested is "wired" into the
 *   logic of function 'checkCircuit'. All combinations of
 *   inputs that satisfy the circuit are printed.
 *
 *   16-bit version by Michael J. Quinn, Sept 2002.
 *   Extended to 32 bits by Joel C. Adams, Sept 2013.
 *   Adapted for HU-HPP by Huib Aldewereld, 2020.
 *   Adapted for C++ by Stephan van der Feest, 2022.
 */

#include <iostream>
#include <climits>
#include <mpi.h>
#include <chrono>
#include <cmath>

#define SIZE 32

/** EXTRACT_BIT is a macro that extracts the ith bit of number n.
 *
 * parameters: n, a number;
 *             i, the position of the bit we want to know.
 *
 * return: 1 if 'i'th bit of 'n' is 1; 0 otherwise
 */
#define EXTRACT_BIT(n, i) ((n & (1 << i)) ? 1 : 0)

int checkCircuit(int, bool *);

int main (int argc, char *argv[])
{
   unsigned int i, combination; // loop variable (32 bits)
   int id = -1, numProcesses = -1;
   unsigned int start = 0, stop = 0, chunkSize = 0;
   int count = 0, localCount = 0;        // number of solutions

   bool *v = (bool *)malloc(sizeof(bool) * SIZE); /* Each element is one of the 32 bits */

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &id);
   MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

   chunkSize = UINT_MAX / numProcesses;
   start = id * chunkSize;
   stop = start + chunkSize;

   std::cout << std::endl << "Process " << id << '/' << numProcesses << " is going from [start: " << start << ", stop: " << stop << "]. Over total of N=" << UINT_MAX << ". With a chunkSize of "<< chunkSize << std::endl;

   std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now(); 

   for (combination = start; combination < stop; combination++)
   {
      for (i = 0; i < SIZE; i++)
         v[i] = EXTRACT_BIT(combination, i);

      localCount += checkCircuit(id, v);
   }

   MPI_Reduce(&localCount, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   MPI_Finalize();

   std::cout << "Process " << id << " finished." << std::endl;

   if(id == 0){
      std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now(); 
      std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
      std::cout << "\nElasped time = " << time_span.count() << " sec" << std::endl;
      std::cout << "A total of " << count << " solutions were found.\n" << std::endl;

   }
   return 0;
}

/* checkCircuit() checks the circuit for a given input.
 * parameters: id, the id of the process checking;
 *             bits, the (long) rep. of the input being checked.
 *
 * output: the binary rep. of bits if the circuit outputs 1
 * return: 1 if the circuit outputs 1; 0 otherwise.
 */
int checkCircuit(int id, bool *v)
{
   if ( ( (v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3])
       && (!v[3] || !v[4]) && (v[4] || !v[5])
       && (v[5] || !v[6]) && (v[5] || v[6])
       && (v[6] || !v[15]) && (v[7] || !v[8])
       && (!v[7] || !v[13]) && (v[8] || v[9])
       && (v[8] || !v[9]) && (!v[9] || !v[10])
       && (v[9] || v[11]) && (v[10] || v[11])
       && (v[12] || v[13]) && (v[13] || !v[14])
       && (v[14] || v[15]) )
       &&
          ( (v[16] || v[17]) && (!v[17] || !v[19]) && (v[18] || v[19])
       && (!v[19] || !v[20]) && (v[20] || !v[21])
       && (v[21] || !v[22]) && (v[21] || v[22])
       && (v[22] || !v[31]) && (v[23] || !v[24])
       && (!v[23] || !v[29]) && (v[24] || v[25])
       && (v[24] || !v[25]) && (!v[25] || !v[26])
       && (v[25] || v[27]) && (v[26] || v[27])
       && (v[28] || v[29]) && (v[29] || !v[30])
       && (v[30] || v[31]) ) )
   {

      return 1;
   } else {
      return 0;
   }
}