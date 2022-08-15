#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include "trollsurfacebig.h"

void PutPix(uint32_t x, uint32_t y, uint64_t color, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *gopInfo)
{
    uint32_t PPS = (uint32_t)gopInfo->PixelsPerScanLine;
    uint32_t yes = (uint32_t)4;    
    uintptr_t testing = (uintptr_t)((gop->Mode->FrameBufferBase) + (yes * PPS * y + 2 * x));
    *(uintptr_t*)testing = color;
}

void drawImage(uint32_t *icon, int posx, int posy, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *gopInfo)
{
    int x = posx;
    int y = posy;
    for (int i = 0; icon[i] != 3; i++)
    {
        if(icon[i] == 2 || icon[i] == '2')
        {
            x = posx;
            y++;
        }
        else
        {
            PutPix(x, y, icon[i], gop, gopInfo);
            x+=3;
        }
    }
}

EFI_STATUS EFIAPI UefiMain (IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE  *SystemTable)
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_STATUS status = SystemTable->BootServices->LocateProtocol(&gopGuid, NULL, (void**)&gop);
    Print(L"STATUS = %s", status);
    
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *gopInfo;
    UINTN* infoSize = (UINTN*)gop->Mode->SizeOfInfo;
    gop->SetMode(gop, gop->Mode->MaxMode);
    gop->QueryMode(gop, gop->Mode->Mode, infoSize, &gopInfo);
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

    Print(L"TrollBoot loading successful!\n\n");

    Print(L"Width = %lu\nheight = %lud\nMaxMode = %lud", 
    gopInfo->HorizontalResolution, gopInfo->VerticalResolution, gop->Mode->MaxMode);

    int width  = gopInfo->HorizontalResolution;
    int height = gopInfo->VerticalResolution;

    drawImage(g_trollsurfacebig_data, width / 2 + 300, height / 2 - 300, gop, gopInfo);

    while (1);
}