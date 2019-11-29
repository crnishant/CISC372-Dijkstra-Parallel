# include <stdlib.h>
# include <stdio.h>
# include <time.h>
#include <omp.h>
#include <stdlib.h>
# define NV 1000

int main ( int argc, char **argv );
void gen_random_graph(int ohd[NV][NV], int n);
int *dijkstra_distance ( int ohd[NV][NV] );
void find_nearest ( int mind[NV], int connected[NV], int *d, int *v );
void init ( int ohd[NV][NV] );
void timestamp ( void );
void update_mind ( int mv, int connected[NV], int ohd[NV][NV], int mind[NV] );

int main ( int argc, char **argv )
{
  
  //int i;
  int i4_huge = 2147483647;
  //int j;
  
  int *mind;
  int ohd[NV][NV];
  
  //  timestamp ( );
    init ( ohd );
    double st = omp_get_wtime();
   mind = dijkstra_distance ( ohd );
   double runtime = omp_get_wtime() - st;
printf(" total: %f s\n", runtime);
  
  free ( mind );
  /*
  fprintf ( stdout, "\n" );
  fprintf ( stdout, "DIJKSTRA\n" );
  fprintf ( stdout, "  Normal end of execution.\n" );

  fprintf ( stdout, "\n" );
  //  timestamp ( );
*/
  return 0;
}

void gen_random_graph(int ohd[NV][NV], int n)
{
    //int adj_matrix[n][n];
    for(int u = 0; u < n; u++)
    {
        for (int v = 0; v < n; v++)
        {
            	ohd[u][v] = rand() % 100 + 1;
		ohd[v][u] = ohd[u][v];
		//printf("%d\n", ohd[u][v]);
        }
    }

}

int *dijkstra_distance ( int ohd[NV][NV] )
{
  int *connected;
  int i;
  int md;
  int *mind;
  int mv;
  int step; 
  connected = ( int * ) malloc ( NV * sizeof ( int ) );
  connected[0] = 1;
  for ( i = 1; i < NV; i++ )
  {
    connected[i] = 0;
  }
  mind = ( int * ) malloc ( NV * sizeof ( int ) );

  for ( i = 0; i < NV; i++ )
  {
    mind[i] = ohd[0][i];
  }
  for ( step = 1; step < NV; step++ )
  {
    find_nearest ( mind, connected, &md, &mv );

    if ( mv == - 1 )
    {
      fprintf ( stdout, "\n" );
      fprintf ( stdout, "DIJKSTRA_DISTANCE - Warning!\n" );
      fprintf ( stdout, "  Search terminated early.\n" );
      fprintf ( stdout, "  Graph might not be connected.\n" );
      break;
    }
    connected[mv] = 1;
    update_mind ( mv, connected, ohd, mind );
  }
  free ( connected );
  return mind;
}
void find_nearest ( int mind[NV], int connected[NV], int *d, int *v )
{
  int i;
  int i4_huge = 2147483647;

  *d = i4_huge;
  *v = -1;
  for ( i = 0; i < NV; i++ )
  {
    if ( !connected[i] && mind[i] < *d )
    {
      *d = mind[i];
      *v = i;
    }
  }
  return;
}

void init ( int ohd[NV][NV] )
{
  int i;
  int i4_huge = 2147483647;
  int j;

  for ( i = 0; i < NV; i++ )  
  {
    for ( j = 0; j < NV; j++ )
    {
      if ( i == j )
      {
        ohd[i][i] = 0;
      }
      else
      {
        ohd[i][j] = i4_huge;
      }
    }
  }

  gen_random_graph(ohd, NV);

  return;
}
void timestamp ( void )
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  time_t now;

  now = time ( NULL );
  tm = localtime ( &now );

  strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

  printf ( "%s\n", time_buffer );

  return;
# undef TIME_SIZE
}

void update_mind ( int mv, int connected[NV], int ohd[NV][NV], int mind[NV] )
{
  int i;
  int i4_huge = 2147483647;

  for ( i = 0; i < NV; i++ )
  {
    if ( !connected[i] )
    {
      if ( ohd[mv][i] < i4_huge )
      {
        if ( mind[mv] + ohd[mv][i] < mind[i] )  
        {
          mind[i] = mind[mv] + ohd[mv][i];
        }
      }
    }
  }
  return;
}



