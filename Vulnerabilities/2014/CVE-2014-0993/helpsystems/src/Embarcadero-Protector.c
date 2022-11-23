/*************************************************************/

#include <windows.h>
#include <stdio.h>

#include "Embarcadero-HotFix.c"

/*************************************************************/

int main ( int argc , char *argv [] )
{
  int verbose = TRUE;
  int pid;

/* Checking parameters */
  if ( argc != 2 )
  {
  /* If a special parameter is received */
    if ( ( argc == 3 ) && ( strcmp ( argv [ 2 ] , "-no_verbose" ) == 0 ) )
    {
    /* No prints */
      verbose = FALSE;
    }
    else
    {
    /* Error message */
      printf ( "\n*** Embarcadero-Workaround PoC ***\n" );
      printf ( "Created by Nicolas A. Economou\n" );
      printf ( "Special thanks to Marcos Accossatto\n" );
      printf ( "Core Security Technologies, Buenos Aires, Argentina (2014)\n" );
      printf ( "\n" );
      printf ( "Use: %s pid\n" , argv [ 0 ] );
      return ( 0 );
    }
  }

/* Obtaining the PID */
  sscanf ( argv [ 1 ] , "%u" , &pid );

/* Protection the program */
  protect_embarcadero ( pid , verbose );

  return ( 1 );
}

/*************************************************************/
