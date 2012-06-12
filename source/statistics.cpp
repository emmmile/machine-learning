#include <iostream>
#include "population.hpp"
#include "living-tm.hpp"
#include "nm.hpp"
using namespace std;



int main() {
  typedef living_tm<N, M> ltm;

  double pc = 0.9;
  double pm = 0.8;
  int totalTrials = 500;
  int generations[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };
  int plotPoints = sizeof(generations) / sizeof(int);

  printf( "generations\tpc\tpm\taverageSteps\taverageExplored\tbestFound%%\n" );
  for ( int j = 0; j < plotPoints; ++j ) {
    double average = 0;
    double averageExplored = 0;
    int timesBestFound = 0;

    for ( int i = 0; i < totalTrials; i++ ) {
      // the seed changes at every trial
      population<ltm> p( i + (j << 4), 400, true, pc, pm );
      p.run( generations[j] );
      if ( !p.get_best().get_state().ishalt() ) {
        cout << "NOT Halted!!!\n";
        exit( 1 );
      } else {
        average += p.get_best().get_nb_shifts();
        averageExplored += p.get_explored();
        if ( p.get_best().get_nb_shifts() == slimits<N,M>::upper )
          timesBestFound++;
      }
    }


    printf( "%d\t%.2lf\t%.2lf\t%.2lf\t%.0lf\t%.0lf\n", generations[j], pc, pm, average / totalTrials,
            averageExplored / totalTrials, 100.0 * timesBestFound / double(totalTrials) );
  }


  return 0;
}
