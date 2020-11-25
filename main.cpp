#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

#include "encryption.h"

#define LL (long long)
typedef uint64_t ptr_t;

HANDLE pHandle;
ptr_t gameManager, profileManager, gameCamera;

enum SkeletonBone : ptr_t
{
    Bone_Head    = 0x1070, // Head Node
    Bone_Spine   = 0x1090, // Spine Node
    Bone_Neck    = 0x10F0  // Neck Node
    //TODO: Find other ones for skeleton reconstruction (entInfo + from 0x170 to 0x10FC)
};

typedef struct
{
    // Spatial information
    float headPos[3];
    float frontVector[3];
    unsigned int health;
} Player_t;

/*
 * Swaps the y and z coordinate
 */
inline void swapCoordinates(float* vec) {
    float temp = vec[1];
    vec[1] = vec[2];
    vec[2] = temp;
}


#define OFFSET_GAME_MGR 0x5E019A8  
#define OFFSET_PROFILE_MGR 0x5E32C50 
#define OFFSET_ENTITY_LIST 0xB0
#define OFFSET_ENTITY_LIST_COUNT 0xB8
#define OFFSET_ENTITY_INFO 0x50

void decryptEntityList(ptr_t* ent_list_dest, ptr_t ent_list_encrypted)
{
    ptr_t temp = (ent_list_encrypted + 0x0FFFFFFFFFFFFFFCA);
    *ent_list_dest = (temp >> 0x11 | temp << 0x2F) + 0x0FFFFFFFFFFFFFFA8;
}

int getEntityCount()
{
    ptr_t entListCountEncrypted, entListCountAddr;
    ReadProcessMemory(pHandle, (void*) (gameManager + OFFSET_ENTITY_LIST_COUNT), &entListCountEncrypted, sizeof(entListCountEncrypted), 0);
    decryptEntityList(&entListCountAddr, entListCountEncrypted); // Same encryption mechanism is used
    return (int) (entListCountAddr ^ 0x18C0000000);
}
 
ptr_t getEntityInfo(ptr_t entityBase)
{
	/*
		The entity info pointer is encrypted and is decrypted by the following operations:
		RainbowSix!GameBlocks::GBPredefinedId::operator=+0x1e112d4:
		00007ff6`8a255040       mov     rcx, qword ptr [rax]    ; Load encrypted pointer
		00007ff6`8a255044       rol     rcx, 1                  ; rotate rcx by one bit to the left
		00007ff6`8a255047       add     rcx, 0FFFFFFFFFFFFFFAEh ; Add the value 0x0FFFFFFFFFFFFFFAE to the pointer
		00007ff6`8a25504b       mov     rax, 845C1A3CE5D1D3B7h  ; rax = key2
		00007ff6`8a255055       xor     rax, rcx                ; Finally, decrypt using the key2 inside the rax register
		00007ff6`8a255058       ret
	 */
    ptr_t entityInfoEncrypted;
    ReadProcessMemory(pHandle, (void*) (entityBase + OFFSET_ENTITY_INFO), &entityInfoEncrypted, sizeof(ptr_t), 0);
    return (__ROL8__(entityInfoEncrypted, 1) + 0x0FFFFFFFFFFFFFFAE) ^ 0x845C1A3CE5D1D3B7;
}

void getEntityBonePosition(ptr_t entityBase, SkeletonBone bone, float* destVec)
{
    ptr_t entityInfoInstance = getEntityInfo(entityBase); //TODO: precache entity info pointers
    ReadProcessMemory(pHandle, (void*) (entityInfoInstance + bone), destVec, 3 * sizeof(float), 0);
    swapCoordinates(destVec);
}

int getEntityHealth(ptr_t entityBase)
{
    int health;
    ptr_t entityInfo = getEntityInfo(entityBase);
    ptr_t healthPtr;
    ReadProcessMemory(pHandle, (void*) (entityInfo + 0x018), &healthPtr, sizeof(ptr_t), 0);
    ReadProcessMemory(pHandle, (void*) (healthPtr  + 0x0D8), &healthPtr, sizeof(ptr_t), 0);
    ReadProcessMemory(pHandle, (void*) (healthPtr  + 0x008), &healthPtr, sizeof(ptr_t), 0);
    ReadProcessMemory(pHandle, (void*) (healthPtr  + 0x170), &health, sizeof(int), 0);
    return health;
}

void getViewTranslation(float* destVec)
{
    ReadProcessMemory(pHandle, (void*) (gameCamera + 0x7D0), destVec, sizeof(float) * 3, 0);
    swapCoordinates(destVec);
}

void getViewFrontVector(float* destVec)
{
    ReadProcessMemory(pHandle, (void*) (gameCamera + 0x7C0), destVec, sizeof(float) * 3, 0);
    swapCoordinates(destVec);
}

float getViewFOVHorizontal()
{
    float fovX;
    ReadProcessMemory(pHandle, (void*) (gameCamera + 0x7E0), &fovX, sizeof(float), 0);
    return fovX;
}

float getViewFOVVertical()
{
    float fovY;
    ReadProcessMemory(pHandle, (void*) (gameCamera + 0x7F4), &fovY, sizeof(float), 0);
    return fovY;
}

int main() 
{
    HWND windowHandle = FindWindow(NULL, "Rainbow Six");
    if (!windowHandle) return -1;

    DWORD pid;
    GetWindowThreadProcessId(windowHandle, &pid);
    pHandle = OpenProcess(PROCESS_VM_READ, 0, pid);
    if (!pHandle) return -1;

    // Overlay
    ptr_t overlayPlayerArrayAddr = 0x1e1bbbcc3a0;
    windowHandle = FindWindow(NULL, "Z:\\C++\\LxCrystal\\Overlay\\build\\Debug\\OpenBook.exe");
    if (!windowHandle) return -1;
    GetWindowThreadProcessId(windowHandle, &pid);
    HANDLE pOverlay = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, 0, pid);
    if (!pOverlay)
    {
        printf("Make sure overlay is running.\n");
        getchar();
        return -1;
    }
 
    // Game's base address
    uint64_t base = 0x7FF687DF0000;
   
    // Read game manager
    ReadProcessMemory(pHandle, (void*) (base + OFFSET_GAME_MGR), &gameManager, sizeof(ptr_t), 0);
    printf("[+] GameManager (Heap): %llx\n", LL gameManager);

    // Read profile manager
    ReadProcessMemory(pHandle, (void*) (base + OFFSET_PROFILE_MGR), &profileManager, sizeof(ptr_t), 0);
    printf("[+] ProfileManager (Heap): %llx %llx\n", LL profileManager, LL (base + OFFSET_PROFILE_MGR));

    // Read game camera
    ptr_t temp;
    ReadProcessMemory(pHandle, (void*) (profileManager + 0x88), &temp, sizeof(ptr_t), 0);
    ReadProcessMemory(pHandle, (void*) (temp + 0x0), &temp, sizeof(ptr_t), 0);
    ReadProcessMemory(pHandle, (void*) (temp + 0x220), &temp, sizeof(ptr_t), 0);
    ReadProcessMemory(pHandle, (void*) (temp + 0x410), &gameCamera, sizeof(ptr_t), 0);
    float fovX = getViewFOVHorizontal();
    float fovY = getViewFOVVertical();
    printf("[+] Game FOV: Horizontal: %f, Vertical: %f\n", fovX, fovY);

    // Ent list
    uint64_t entityListEncrypted, entityList;
    ReadProcessMemory(pHandle, (void*) (gameManager + OFFSET_ENTITY_LIST), &entityListEncrypted, sizeof(entityListEncrypted), 0);
    decryptEntityList(&entityList, entityListEncrypted);
    printf("[?] EntList (Enc): %llx\n", LL entityListEncrypted);
    printf("[+] EntList (Dec): %llx\n", LL entityList);

    getchar();

    while (true)
    {
        system("cls");
        printf("Entity count: %d\n", getEntityCount());

        Player_t player;
        getViewTranslation(player.headPos);
        getViewFrontVector(player.frontVector);
        player.health = 100;
        printf("Camera pos: %f %f %f\n", player.headPos[0], player.headPos[1], player.headPos[2]);
        printf("Camera front vector: %f %f %f\n", player.frontVector[0], player.frontVector[1], player.frontVector[2]);
        WriteProcessMemory(pOverlay, (void*) overlayPlayerArrayAddr, &player, sizeof(Player_t), 0);

        Player_t currentPlayer;
        for (int idx = 0; idx < getEntityCount(); idx++)
        {
            ptr_t entity;
            ReadProcessMemory(pHandle, (void*) (entityList + 0x8 * idx), &entity, sizeof(entity), 0);

            ptr_t entityInfo = getEntityInfo(entity);
            getEntityBonePosition(entity, Bone_Head, currentPlayer.headPos);
            currentPlayer.health = getEntityHealth(entity);
            WriteProcessMemory(pOverlay, (void*) (overlayPlayerArrayAddr + (sizeof(Player_t) * (1 + idx))), &currentPlayer, sizeof(Player_t), 0);
            printf("Entity %d: Info: %llx Bone: %f %f %f Health: %d\n", idx, LL entityInfo, currentPlayer.headPos[0], currentPlayer.headPos[1], currentPlayer.headPos[2], currentPlayer.health);
        }
    }
    
    getchar();
    return 0;
}
