/*************************************************************/

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

#include "list.cpp"
#include "Embarcadero-HotFix.c"

/*************************************************************/

int get_processes ( List & , List & );
void refresh_processes ( List & , List & , List & );
void protect_processes ( List & , List & , List & , List & );
int protect_process ( int );
void block_processes ( List & , List & );
char *get_process_name ( int );
unsigned int get_process_uptime ( int );

/*************************************************************/

#define MAX_TRIES   3
#define GOOD_UPTIME 3

/*************************************************************/

void main ( void )
{
  List black_list_checks;
  List black_list;
  List processes;
  List uptimes;
  unsigned int cont;

/* Intro message */
  printf ( "\n*** Embarcadero-Workaround PoC ***\n" );
  printf ( "Created by Nicolas A. Economou\n" );
  printf ( "Special thanks to Marcos Accossatto\n" );
  printf ( "Core Security Technologies, Buenos Aires, Argentina (2014)\n" );
  printf ( "\n" );
  Sleep ( 2000 );

/* Monitoreo la lista de procesos todo el tiempo */
  for ( cont = 0 ; cont < 0xffffffff ; cont ++ )
  {
  /* Getting processes */
    get_processes ( processes , uptimes );

  /* Merging list */
    refresh_processes ( black_list , black_list_checks , processes );

  /* Looking for new vulnerable programs */
    protect_processes ( black_list , black_list_checks , processes , uptimes );

  /* If it's the first time */
    if ( cont == 0 )
    {
    /* No more scannings for black listed processes */
      block_processes ( black_list , black_list_checks );
    }

  /* A delay for the next try */
    Sleep ( 1000 );
  }
}

/*************************************************************/

int get_processes ( List &processes , List &uptimes )
{
  PROCESSENTRY32 process;
  HANDLE handle;
  HANDLE phandle;
  int mypid;
  int ret = TRUE;

/* Getting my process ID */
  mypid = GetCurrentProcessId ();

/* Cleaning list */
  processes.Clear ();
  uptimes.Clear ();

/* Getting process list */
  handle = CreateToolhelp32Snapshot ( TH32CS_SNAPALL , 0 );

/* Initializing structure */
  process.dwSize = sizeof ( PROCESSENTRY32 );

/* Getting first process */
  Process32First ( handle , &process );

/* Adding the PID */
  processes.Add ( ( void * ) process.th32ProcessID );

/* Getting the uptime of this process */
  uptimes.Add ( ( void * ) get_process_uptime ( process.th32ProcessID ) );

/* Getting the rest of the processes */
  while ( Process32Next ( handle , &process ) == TRUE )
  {
  /* If it's not me */
    if ( mypid != process.th32ProcessID )
    {
    /* Adding the PID */
      processes.Add ( ( void * ) process.th32ProcessID );

    /* Getting the uptime of this process */
      uptimes.Add ( ( void * ) get_process_uptime ( process.th32ProcessID ) );
    }
  }

/* Ordering process list */
  processes.SortCouple ( uptimes );

/* Closing handle */
  CloseHandle ( handle );

  return ( ret );
}

/*************************************************************/

void refresh_processes ( List &black_list , List &black_list_checks , List &processes )
{
  unsigned int cont;

/* Walking the list */
  for ( cont = 0 ; cont < black_list.Len () ; cont ++ )
  {
  /* If the process DIED */
    if ( processes.Find ( black_list.Get ( cont ) ) == FALSE )
    {
    /* Deleting this OLD PID */
      black_list.Delete ( cont );
      black_list_checks.Delete ( cont );

    /* Compensating the element extraction */
      cont --;
    }
  }
}

/*************************************************************/

void protect_processes ( List &black_list , List &black_list_checks , List &processes , List &uptimes )
{
  unsigned int checking_counter;
  unsigned int uptime;
  unsigned int cont;
  unsigned int pos;
  int pid;
  int ret;

/* Walking process list */
  for ( cont = 0 ; cont < processes.Len () ; cont ++ )
  {
  /* Next PID */
    pid = ( int ) processes.Get ( cont );

  /* Process Uptime */
    uptime = ( unsigned int ) uptimes.Get ( cont );

//    printf ( "%i: %u seconds\n" , pid , uptime );

  /* If the UPTIME is ENOUGH */
    if ( uptime >= GOOD_UPTIME )
    {
    /* If the process is in the BLACK LIST */
      if ( black_list.Find ( ( void * ) pid ) == TRUE )
      {
      /* Getting the position in the list */
        black_list.GetPos ( ( void * ) pid , &pos );

      /* Getting the times that the process was attempted to be preotected */
        checking_counter = ( unsigned int ) black_list_checks.Get ( pos );

      /* If the "time to protect" is fine */
        if ( checking_counter < MAX_TRIES )
        {
        /* Trying to protect the process */
          ret = protect_process ( pid );

        /* If the process could be protected */
          if ( ret == 1 )
          {
          /* Process protected */
            black_list_checks.Set ( pos , ( void * ) MAX_TRIES );
          }
          else
          {
          /* Incrementing the counter protection tries */
            black_list_checks.Set ( pos , ( void * ) ( checking_counter + 1 ) );
          }
        }
      }
      else
      {
      /* Trying to protect the process */
        ret = protect_process ( pid );

      /* Adding the process to the BLACK LIST */
        black_list.Add ( ( void * ) pid );

      /* If the process could be protected */
        if ( ret == 1 )
        {
        /* Process protected */
          black_list_checks.Add ( ( void * ) MAX_TRIES );
        }
      /* If the process couldn't be protected */
        else
        {
        /* To be scanned again */
          black_list_checks.Add ( ( void * ) 1 );
        }
      }
    }
  }
}

/*************************************************************/

int protect_process ( int pid )
{
  char cmd [ 256 ];
  char *message;
  int ret;

/* Trying to protect the process */
//  printf ( "[x] Scanning PID:%i ... " , pid );
  printf ( "[x] Scanning %s ... " , get_process_name ( pid ) );
  ret = protect_embarcadero ( pid , FALSE );

/* If the pattern was found */
  if ( ret == 1 )
  {
    message = "PROTECTION ACTIVATED !";
  }
/* If the pattern wasn't found */
  else if ( ret == 0 )
  {
    message = "VULNERABLE CODE NOT FOUND";
  }
/* If the process could't be open */
  else
  {
    message = "PROCESS ERROR";
  }

/* The result */
  printf ( "%s\n" , message );

  return ( ret );
}

/*************************************************************/

void block_processes ( List &black_list , List &black_list_checks )
{
  unsigned int cont;

/* Walking the black list processes */
  for ( cont = 0 ; cont < black_list.Len () ; cont ++ )
  {
  /* Setting the MAX TRIES NUMBER */
    black_list_checks.Set ( cont , ( void * ) MAX_TRIES );
  }
}

/*************************************************************/

char *get_process_name ( int pid )
{
  PROCESSENTRY32 process;
  HANDLE handle;
  static char pname [ 256 ];
  unsigned int ssize;
  unsigned int cont;
  int ret = FALSE;
  int res;

/* Initializing the process name */
  memset ( pname , 0 , sizeof ( pname ) );
  strcpy ( pname , "no_name" );

/* Inicializo la estructura */
  process.dwSize = sizeof ( PROCESSENTRY32 );

/* Imagen del sistema */
  handle = CreateToolhelp32Snapshot ( TH32CS_SNAPPROCESS , 0 );

/* Listo todos los procesos del sistema */
  res = Process32First ( handle , &process );

/* Mientras pueda listar procesos */
  while ( res == TRUE )
  {
  /* Si es el proceso que hizo el llamado */
    if ( process.th32ProcessID == pid )
    {
    /* Copio el nombre del proceso */
      strncpy ( pname , process.szExeFile , sizeof ( pname ) );

    /* Retorno OK */
      ret = TRUE;

    /* Dejo de buscar */
      break;
    }

  /* Sigo listando procesos */
    res = Process32Next ( handle , &process );
  }

/* Closing handle */
  CloseHandle ( handle );

/* Length of the process name */
  ssize = strlen ( pname );

/* If the process name is short */
  if ( ssize < 20 )
  {
  /* Padding the rest */
    memset ( &pname [ ssize ] , ' ' , 20 - ssize );
  }

  return ( pname );
}

/*************************************************************/

unsigned int get_process_uptime ( int pid )
{
  unsigned int uptime = 0xffffffff;
  unsigned __int64 time1;
  unsigned __int64 time2;
  HANDLE hprocess;
  FILETIME ct;
  FILETIME et;
  FILETIME kt;
  FILETIME ut;
  FILETIME st;

/* Opening process */
  hprocess = OpenProcess ( PROCESS_QUERY_INFORMATION , FALSE , pid );

/* If the process could be open */
  if ( hprocess != NULL )
  {
  /* Getting the process uptime */
    GetProcessTimes ( hprocess , &ct , &et , &kt , &ut );

  /* Getting the current time */
    GetSystemTimeAsFileTime ( &st );

  /* Casting time */
    time1 = ( * ( unsigned __int64 * ) &ct ) / 10000000;
    time2 = ( * ( unsigned __int64 * ) &st ) / 10000000;

  /* Returning the UPTIME */
    uptime = ( unsigned int ) ( time2 - time1 );

  /* Closing process */
    CloseHandle ( hprocess );
  }

  return ( uptime );
}

/*************************************************************/
