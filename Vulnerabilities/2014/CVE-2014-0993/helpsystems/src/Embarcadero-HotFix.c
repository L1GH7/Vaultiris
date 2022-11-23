/*************************************************************/

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

/*************************************************************/

#define asm _asm

#pragma comment(lib, "ADVAPI32.LIB")

/*************************************************************/

int protect_embarcadero ( int , int );

HANDLE OpenProcessWithPrivileges ( int , int , int );
int patch_function ( HANDLE , void * , unsigned int , unsigned char * );
void *get_program_address ( int , HANDLE );
void *get_first_executable_section ( HANDLE , void * );
void assembly_call ( HANDLE , unsigned int , unsigned int );
void *find_pattern ( unsigned int , unsigned char * , unsigned int , unsigned char * );

void _vuln_checker ( void );
void sub_vuln_checker ( unsigned int );
char *get_message ( void );
char *get_title ( void );
void _vuln_checker_end ( void );

/*************************************************************/

int protect_embarcadero ( int pid , int verbose )
{
  MEMORY_BASIC_INFORMATION mem_information;
  HANDLE hprocess;
  unsigned int reads;
  unsigned int cont;
  unsigned char inst [ 7 ];
  unsigned char *pattern = ( unsigned char * ) "\x8B\x4B\x20\x0F\xB6\x7D\xDF";
  unsigned int address;
  int program_protected = FALSE;
  void *program_address;
  void *data = NULL;
  int ret;

/* Opening process */
  if ( ( hprocess = OpenProcessWithPrivileges ( PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE , FALSE ,  pid ) ) == NULL )
  {
  /* If this aplication is launched standalone */
    if ( verbose == TRUE )
    {
      printf ( "Process error\n" );  
    }

    return ( -1 );
  }

/* Getting the ".text" of the executable section */
  program_address = get_program_address ( pid , hprocess );
//  printf ( "program address = %x\n" , program_address );

/* If the executable was found */
  if ( program_address != NULL )
  {
  /* Getting section information */
    ret = VirtualQueryEx ( hprocess , program_address , &mem_information , sizeof ( MEMORY_BASIC_INFORMATION ) );

  /* If the code could be read */
    if ( ret != 0 )
    {
    /* If the code is EXECUTABLE */
      if ( mem_information.Protect & 0xf0 )
      {
      /* Allocating memory for find */
        data = malloc ( mem_information.RegionSize );

      /* If the memory could be allocated */
        if ( data != NULL )
        {
        /* Cleaning the memory */
          memset ( data , 0 , mem_information.RegionSize );

        /* Reading memory from the process */
          ret = ReadProcessMemory ( hprocess , mem_information.BaseAddress , data , mem_information.RegionSize , ( DWORD * ) &reads );

        /* Searching vulnerable function */
          for ( cont = 0 ; cont < mem_information.RegionSize - 7 ; cont ++ )
          {
          /* Address to read */
            address = ( unsigned int ) mem_information.BaseAddress + cont;

          /* Reading next bytes */
            memcpy ( ( void * ) inst , ( void * ) ( ( unsigned int ) data + cont ) , 7 );

          /* If the memory pattern was found */
            if ( memcmp ( ( void * ) inst , ( void * ) pattern , 7 ) == 0 )
            {
              patch_function ( hprocess , ( void * ) address , 7 , pattern );

            /* If this aplication is launched standalone */
              if ( verbose == TRUE )
              {
                printf ( "[x] Pattern found at %.8x\n" , address );
                printf ( "[x] Protection activated\n" );
              }

              program_protected = TRUE;
              break;
            }
          }

        /* Freeing memory */
          free ( data );
        }
      }
    }
  }

/* If the pattern doesn't exist */
  if ( program_protected == FALSE )
  {
  /* If this aplication is launched standalone */
    if ( verbose == TRUE )
    {
      printf ( "[ ] Error: Vulnerable code not found\n" );
    }
  }

/* Closing process */
  CloseHandle ( hprocess );

/* Returning the result */
  return ( program_protected );
}

/****************************************************************************/

HANDLE OpenProcessWithPrivileges ( int access , int inherite , int pid )
{
  TOKEN_PRIVILEGES new_token_privileges;
  unsigned int token_handle;
  HANDLE ret;

/* Pido permiso como debugger */
  LookupPrivilegeValueA ( NULL , SE_DEBUG_NAME , &new_token_privileges.Privileges [ 0 ].Luid );

/* Abro el token */
  OpenProcessToken ( GetCurrentProcess () , TOKEN_ADJUST_PRIVILEGES , ( void ** ) &token_handle );

/* Nuevos valores de privilegio */
  new_token_privileges.PrivilegeCount = 1;
  new_token_privileges.Privileges [ 0 ].Attributes = SE_PRIVILEGE_ENABLED;

/* Ajusto los privilegios */
  AdjustTokenPrivileges ( ( void * ) token_handle , FALSE , &new_token_privileges , sizeof ( new_token_privileges ) , NULL , NULL );

/* Abro el proceso */
  ret = OpenProcess ( access , inherite , pid );

  return ( ret );
}

/*************************************************************/

void *get_program_address ( int pid , HANDLE hprocess )
{
  MODULEENTRY32 module;
  HANDLE handle;
  void *address = NULL;

/* System snapshot */
  handle = CreateToolhelp32Snapshot ( TH32CS_SNAPALL , pid );

/* If the handle is VALID */
  if ( handle != NULL )
  {
  /* Initial settings */
    module.dwSize = sizeof ( MODULEENTRY32 );

  /* The first module is always the MAIN EXECUTABLE */
    if ( Module32First ( handle , &module ) == TRUE )
    {
    /* Base module */
      address = get_first_executable_section ( hprocess , module.modBaseAddr );
    }

  /* Closing handle */
    CloseHandle ( handle );
  }

  return ( address );
}

/*************************************************************/

void *get_first_executable_section ( HANDLE hprocess , void *base_address )
{
  MEMORY_BASIC_INFORMATION mem_info;
  void *address = NULL;

/* While memory mapped */
  while ( VirtualQueryEx ( hprocess , base_address , &mem_info , sizeof ( MEMORY_BASIC_INFORMATION ) ) != 0 )
  {
  /* If the memory is executable */
    if ( mem_info.Protect & 0xf0 )
    {
    /* Address to SCAN */
      address = base_address;

    /* Stop finding */
      break;
    }
  /* If the memory is NOT executable */
    else
    {
    /* Next memory section */
      base_address = ( void * ) ( ( unsigned int ) base_address + mem_info.RegionSize );
    }
  }

  return ( address );
}

/*************************************************************/

void assembly_call ( HANDLE hprocess , unsigned int source , unsigned int destination )
{
  unsigned long escritos;
  int distancia;
  char buffer [ 5 ];

/* Fabrico la instruccion */
  buffer [ 0 ] = 0xe8;

/* Calculo la distancia */
  * ( int * ) &buffer [ 1 ] = ( int ) destination - ( int ) ( source + 5 );

/* Escribo la instruccion */
  WriteProcessMemory ( hprocess , ( void * ) source , buffer , 5 , &escritos );
}

/*************************************************************/

int patch_function ( HANDLE hprocess , void *address , unsigned int size , unsigned char *pattern )
{
  void *stub_address;
  void *pMessageBox;
  void *pExitProcess;
  void *p;
  unsigned int shellcode_size;
  unsigned char *shellcode;
  unsigned long writes;
  unsigned int cont;
  int ret = TRUE;

/* Loading "user32.dll" */
  LoadLibrary ( "user32.dll" );

/* Getting function address */
  pMessageBox = GetProcAddress ( GetModuleHandle ( "user32.dll" ) , "MessageBoxA" );
  pExitProcess = GetProcAddress ( GetModuleHandle ( "kernel32.dll" ) , "ExitProcess" );

/* Allocating memory for patch */
  stub_address = VirtualAllocEx ( hprocess , NULL , 0x1000 , 0x3000 , PAGE_EXECUTE_READ );

/* Patching function addresses */
  shellcode_size = ( unsigned int ) _vuln_checker_end - ( unsigned int ) _vuln_checker;
  shellcode = ( unsigned char * ) malloc ( shellcode_size );
  memcpy ( shellcode , ( void * ) _vuln_checker , shellcode_size );

/* Patching addresses */
  p = find_pattern ( shellcode_size , shellcode , 4 , ( unsigned char * ) "\x33\x33\x33\x33" );
  * ( unsigned int * ) p = ( unsigned int ) pMessageBox;
  p = find_pattern ( shellcode_size , shellcode , 4 , ( unsigned char * ) "\x44\x44\x44\x44" );
  * ( unsigned int * ) p = ( unsigned int ) pExitProcess;

/* Patching strings */
  p = find_pattern ( shellcode_size , shellcode , 4 , ( unsigned char * ) "\x55\x55\x55\x55" );
  strcpy ( ( char * ) p , "Please check: \"http://www.coresecurity.com/advisories\"\r\n\r\nThis program will be terminated ..." );
  p = find_pattern ( shellcode_size , shellcode , 4 , ( unsigned char * ) "\x66\x66\x66\x66" );
  strcpy ( ( char * ) p , "Embarcadero exploitation detected !" );

/* Copying the original instruction */
  WriteProcessMemory ( hprocess , stub_address , ( void * ) pattern , size , &writes );

/* Copying the vulnerability checker */
  WriteProcessMemory ( hprocess , ( void * ) ( ( unsigned int ) stub_address + size ) , ( void * ) shellcode , shellcode_size , &writes );

/* Padding original instructions */
  for ( cont = 0 ; cont < size ; cont ++ )
  {
  /* Writing a NOP */
    WriteProcessMemory ( hprocess , ( void * ) ( ( unsigned int ) address + cont ) , ( void * ) "\x90" , 1 , &writes );
  }

/* Patching the original instruction */
  assembly_call ( hprocess , ( unsigned int ) address , ( unsigned int ) stub_address );

  return ( ret );
}

/*************************************************************/

void *find_pattern ( unsigned int shellcode_size , unsigned char *shellcode , unsigned int size , unsigned char *pattern )
{
  void *address = NULL;
  unsigned int cont;

/* Finding pattern */
  for ( cont = 0 ; cont < shellcode_size ; cont ++ )
  {
  /* It it's the pattern */
    if ( memcmp ( shellcode + cont , pattern , size ) == 0 )
    {
    /* Pattern found */
      address = shellcode + cont;

    /* Stop finding */
      break;
    }
  }

  return ( address );
}

/*************************************************************/

__declspec ( naked ) void _vuln_checker ( void )
{
  asm pushad
  asm push ecx
  asm call sub_vuln_checker
  asm add esp,4
  asm popad
  asm ret
}

/*************************************************************/

void sub_vuln_checker ( unsigned int param )
{
  int ( WINAPI *myMessageBox ) ( int , char * , char * , int );
  int ( WINAPI *myExitProcess ) ( int );

/* Asignations to be patched */
  myMessageBox = ( int ( WINAPI * ) ( int , char * , char * , int ) ) 0x33333333;
  myExitProcess = ( int ( WINAPI * ) ( int ) ) 0x44444444;

/* If the param is INVALID */
  if ( param > 0x100 )
  {
  /* Message to user */
    myMessageBox ( 0 , get_message () , get_title () , MB_ICONERROR | MB_SERVICE_NOTIFICATION );

  /* Finishing process */
    myExitProcess ( 0 );
  }
}

/*************************************************************/

__declspec ( naked ) char *get_message ( void )
{
  asm call tag1;

/* Cookie */
  asm __emit 0x55;
  asm __emit 0x55;
  asm __emit 0x55;
  asm __emit 0x55;

/* Padding for strings */
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop

  asm tag1:
  asm pop eax
  asm ret
//  return ( ( char * ) _EAX );
}

/*************************************************************/

__declspec ( naked ) char *get_title ( void )
{
  asm call tag2;

/* Cookie */
  asm __emit 0x66;
  asm __emit 0x66;
  asm __emit 0x66;
  asm __emit 0x66;

/* Padding for strings */
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop
  asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop asm nop

  asm tag2:
  asm pop eax
  asm ret

//  return ( ( char * ) _EAX );
}

/*************************************************************/

__declspec ( naked ) void _vuln_checker_end ( void )
{
}

/*************************************************************/
