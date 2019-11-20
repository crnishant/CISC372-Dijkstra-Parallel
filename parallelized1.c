# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <omp.h>

# define NV 1000

int main ( int argc, char **argv );
int *dijkstra_distance ( int ohd[NV][NV] );
void find_nearest ( int s, int e, int mind[NV], int connected[NV], int *d, 
  int *v );
void init ( int ohd[NV][NV] );
void timestamp ( void );
void update_mind ( int s, int e, int mv, int connected[NV], int ohd[NV][NV], 
  int mind[NV] );
void gen_random_graph(int ohd[NV][NV], int n);

/******************************************************************************/

int main ( int argc, char **argv )

/******************************************************************************/
/*
  Purpose:

    MAIN runs an example of Dijkstra's minimum distance algorithm.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Author:

    Original C version by Norm Matloff, CS Dept, UC Davis.
    This C version by John Burkardt.
*/
{
  int i;
  int i4_huge = 2147483647;
  int j;
  int *mind;
  int ohd[NV][NV];

  timestamp ( ) ;

  init ( ohd );

/*
  Carry out the algorithm.
*/
  double st = omp_get_wtime();

  mind = dijkstra_distance ( ohd );

 double runtime = omp_get_wtime() - st;
printf(" total: %f s\n", runtime);  
/*
  Print the results.
*/
/*
  fprintf ( stdout, "\n" );
  fprintf ( stdout, "  Minimum distances from node 0:\n");
  fprintf ( stdout, "\n" );
  for ( i = 0; i < NV; i++ )
  {
    fprintf ( stdout, "  %2d  %2d\n", i, mind[i] );
  }
*/
  free ( mind );

  timestamp ( );

  return 0;
}
/******************************************************************************/

void gen_random_graph(int ohd[NV][NV], int n)
{
	for (int u = 0; u < n; u++)
	{
		for (int v = 0; v < n; v++)
		{
			int random = rand() % 5 + 1;
			if(random == 1){
				ohd[u][v] = rand() % 100 + 1;
				ohd[v][u] = ohd[u][v];
			}
		}
	}

}


int *dijkstra_distance ( int ohd[NV][NV]  )
/*
    DIJKSTRA_DISTANCE uses Dijkstra's minimum distance algorithm.
*/
{
  int *connected;
  int i;
  int i4_huge = 2147483647;
  int md;
  int *mind;
  int mv;
  int my_first;
  int my_id;
  int my_last;
  int my_md;
  int my_mv;
  int my_step;
  int nth;

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

  # pragma omp parallel private ( my_first, my_id, my_last, my_md, my_mv, my_step ) \
  shared ( connected, md, mind, mv, nth, ohd )
  {
    my_id = omp_get_thread_num ( );
    nth = omp_get_num_threads ( ); 
    my_first =   (   my_id       * NV ) / nth;
    my_last  =   ( ( my_id + 1 ) * NV ) / nth - 1;

    fprintf ( stdout, "  P%d:  First=%d  Last=%d\n", my_id, my_first, my_last );

    for ( my_step = 1; my_step < NV; my_step++ )
    {
      # pragma omp single 
      {
        md = i4_huge;
        mv = -1; 
      }

      find_nearest ( my_first, my_last, mind, connected, &my_md, &my_mv );

      # pragma omp critical
      {
        if ( my_md < md )  
        {
          md = my_md;
          mv = my_mv;
        }
      }

      # pragma omp barrier

      # pragma omp single 
      {
        if ( mv != - 1 )
        {
          connected[mv] = 1;
        }
      }

      # pragma omp barrier

      if ( mv != -1 )
      {
        update_mind ( my_first, my_last, mv, connected, ohd, mind );
      }

      #pragma omp barrier
    }

  }

  free ( connected );

  return mind;
}

void find_nearest ( int s, int e, int mind[NV], int connected[NV], int *d, 
  int *v )

/*
    FIND_NEAREST finds the nearest unconnected node.
*/
{
  int i;
  int i4_huge = 2147483647;

  *d = i4_huge;
  *v = -1;

  //#pragma omp parallel for default(none) private(s, i, e, d, v) shared(mind, connected)
  #pragma omp parallel for default(none) shared(s, i, e, d, v, mind, connected)
  for ( i = s; i <= e; i++ )
  {
    if ( !connected[i] && ( mind[i] < *d ) )
    {
      *d = mind[i];
      *v = i;
    }
  }
  return;
}

void init ( int ohd[NV][NV] )
/*
    INIT initializes the problem data.
*/
{
  int i;
  int i4_huge = 2147483647;
  //int j;

  //#pragma omp parallel for default(none) private(i, i4_huge) shared(ohd)
  #pragma omp parallel for default(none) shared(i, i4_huge, ohd)
  for ( i = 0; i < NV; i++ )  
  {
    for ( int j = 0; j < NV; j++ )
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
/*
    TIMESTAMP prints the current YMDHMS date as a time stamp.
*/
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


void update_mind ( int s, int e, int mv, int connected[NV], int ohd[NV][NV], int mind[NV] )
/*
    UPDATE_MIND updates the minimum distance vector.
*/
{
  int i;
  int i4_huge = 2147483647;

  //#pragma omp parallel for default(none) private(s, i, e, mv, i4_huge) shared(ohd, mind, connected)
  #pragma omp parallel for default(none) shared(s, i, e, mv, i4_huge, ohd, mind, connected)
  for ( i = s; i <= e; i++ )
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
