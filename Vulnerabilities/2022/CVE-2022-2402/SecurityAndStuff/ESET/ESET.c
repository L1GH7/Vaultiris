#include <windows.h>
#include <ntstatus.h>
#include <stdio.h>
#include <winternl.h>

#define PTR_ADD_OFFSET(Pointer, Offset) ((PVOID)((ULONG_PTR)(Pointer) + (ULONG_PTR)(Offset)))
#define PTR_SUB_OFFSET(Pointer, Offset) ((PVOID)((ULONG_PTR)(Pointer) - (ULONG_PTR)(Offset)))
#define ALIGN_UP_BY(Address, Align) (((ULONG_PTR)(Address) + (Align) - 1) & ~((Align) - 1))
#define ALIGN_UP_POINTER_BY(Pointer, Align) ((PVOID)ALIGN_UP_BY(Pointer, Align))
#define ALIGN_UP(Address, Type) ALIGN_UP_BY(Address, sizeof(Type))
#define ALIGN_UP_POINTER(Pointer, Type) ((PVOID)ALIGN_UP(Pointer, Type))
#define ALIGN_DOWN_BY(Address, Align) ((ULONG_PTR)(Address) & ~((ULONG_PTR)(Align) - 1))
#define ALIGN_DOWN_POINTER_BY(Pointer, Align) ((PVOID)ALIGN_DOWN_BY(Pointer, Align))
#define ALIGN_DOWN(Address, Type) ALIGN_DOWN_BY(Address, sizeof(Type))
#define ALIGN_DOWN_POINTER(Pointer, Type) ((PVOID)ALIGN_DOWN(Pointer, Type))
#define PH_FIRST_OBJECT_TYPE(ObjectTypes) \
    PTR_ADD_OFFSET(ObjectTypes, ALIGN_UP(sizeof(OBJECT_TYPES_INFORMATION), ULONG_PTR))

#define PH_NEXT_OBJECT_TYPE(ObjectType) \
    PTR_ADD_OFFSET(ObjectType, sizeof(OBJECT_TYPE_INFORMATION) + \
    ALIGN_UP(ObjectType->TypeName.MaximumLength, ULONG_PTR))

typedef struct _OBJECT_NAME_INFORMATION
{
    UNICODE_STRING Name;
} OBJECT_NAME_INFORMATION, * POBJECT_NAME_INFORMATION;

int main()
{
    system("pause");
    __debugbreak();
    ULONG bufferSize = sizeof(OBJECT_NAME_INFORMATION) + (MAX_PATH * sizeof(WCHAR));
    POBJECT_NAME_INFORMATION buffer = calloc(bufferSize, 1);
    HANDLE hDevice = CreateFile(L"\\\\.\\DLPFdePub_CDO", GENERIC_READ, 0, NULL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, NULL);
    while ((NtQueryObject(
        hDevice,
        1,
        buffer,
        bufferSize,
        &bufferSize
    )) == STATUS_INFO_LENGTH_MISMATCH)
    {
        free(buffer);
        bufferSize *= 2;
        buffer = calloc(bufferSize, sizeof(char));
    }
    free(buffer);
}
