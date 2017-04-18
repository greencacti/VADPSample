/*
 ============================================================================
 Name        : VADPSample.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <vixDiskLib.h>

#define VIXDISKLIB_VERSION_MAJOR 6
#define VIXDISKLIB_VERSION_MINOR 5

static void
LogFunc(const char *fmt, va_list args)
{
   printf("Log: ");
   vprintf(fmt, args);
}

static void
WarnFunc(const char *fmt, va_list args)
{
   printf("Warning: ");
   vprintf(fmt, args);
}

static void
PanicFunc(const char *fmt, va_list args)
{
   printf("Panic: ");
   vprintf(fmt, args);
   exit(10);
}

int main(int argc, char* argv[]) {
	VixError vixError;
	VixDiskLibConnection connection;
	VixDiskLibConnectParams cnxParams = {0};
	VixDiskLibInfo *info = NULL;
	VixDiskLibHandle diskHandle;

    vixError = VixDiskLib_InitEx(VIXDISKLIB_VERSION_MAJOR,
                                 VIXDISKLIB_VERSION_MINOR,
                                 &LogFunc, &WarnFunc, &PanicFunc,
                                 "/usr/lib/vmware-vix-disklib-distrib/lib64",
                                 NULL);
    if(vixError != VIX_OK) {
    	printf("Error Code: %d", (int)vixError);
    	goto ERROR;
    }

    cnxParams.specType = VIXDISKLIB_SPEC_VMX;
    cnxParams.spec.vmxSpec = "Photon/Photon.vmx?dcPath=DC1&dsName=datastore3";
    cnxParams.serverName = "10.110.170.194";
    cnxParams.thumbPrint = "48:54:B9:C8:E6:42:FD:E0:F8:73:6A:9B:15:FE:F0:77:A4:F4:EC:75";
    cnxParams.credType = VIXDISKLIB_CRED_UID;
    cnxParams.creds.uid.userName = "administrator@vsphere.local";
    cnxParams.creds.uid.password = "ca$hc0w";
    cnxParams.port = 443;
    cnxParams.nfcHostPort = 902;
    vixError = VixDiskLib_Connect(&cnxParams,
                                  &connection);
    if(vixError != VIX_OK) {
    	printf("Error Code: %d", (int)vixError);
    	goto ERROR;
    }

    vixError = VixDiskLib_Open(connection,
                               "[datastore3] Photon/Photon.vmdk",
                               VIXDISKLIB_FLAG_OPEN_READ_ONLY,
                               &diskHandle);
    if(vixError != VIX_OK) {
    	printf("Error Code: %d", (int)vixError);
    	goto ERROR;
    }

    vixError = VixDiskLib_GetInfo(diskHandle, &info);
    if(vixError != VIX_OK) {
    	printf("Error Code: %d", (int)vixError);
    	goto ERROR;
    }
    printf("size is %ld\n", info->capacity);

ERROR:
    VixDiskLib_Disconnect(connection);
    VixDiskLib_Exit();
    return 0;
}
