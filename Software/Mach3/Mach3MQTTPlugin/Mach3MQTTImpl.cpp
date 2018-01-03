// ProtoImpl.cpp

#include "stdafx.h"
#include "XMLProfile.h"

#include "Mach3MQTTImpl.h"
#include "dbg.h"
#include "MQTTCommunicator.h"

// ===========================================================================
//
// Global variables for this code
//
// ===========================================================================

MQTTCommunicator* mqttComm;
CString mqttServer;

double* localDro1 = NULL;
double* localDro2 = NULL;

//============================================================================
//
// Here are the routines which are called by Mach3
// They are the core of every plugin
//
//============================================================================

//----------------------------------------------------------------------------



// Used for destruction of variables prior to exit.
// Called as Mach3 shuts down.

void cleanUp() {
	DbgMsg(("myCleanUp entry"));

    KillTimer(NULL, 1);

    if (mqttComm)
        delete mqttComm;

    if (localDro1)
        delete localDro1;

    if (localDro2)
        delete localDro2;

	DbgMsg(("myCleanUp exit"));
}

//----------------------------------------------------------------------------

// initial access to Mach profile when enumerating available plugins
// Returns the second half on the "pluging id" for the list in Operator
// Proto adds the file name at the start of the string

CString	profileInit(CString name, CXMLProfile *DevProf) {
	DbgMsg(("myProfileInit entry"));

    mqttServer = DevProf->GetProfileString("Preferences", "MQTTServer", "127.0.0.1");

	DbgMsg(("myProfileInit exit"));

	return("MQTT Plugin - PtD Ver 1.0");
}

//----------------------------------------------------------------------------

// called during Mach initialisation. You can influence subsequent init by actions here
// **** Not used in typical device plugin

void initControl() {
	DbgMsg(("myInitControl entry"));


	DbgMsg(("myInitControl exit"));
}

//----------------------------------------------------------------------------

void postInitControl() {
	// called when mach fully set up so all data can be used but initialization  outcomes not affected
	DbgMsg(("myPostInitControl entry"));

    mqttComm = new MQTTCommunicator(mqttServer);
    mqttComm->connect();
    // mqttComm->subscribe();
	DbgMsg(("myPostInitControl exit"));
}

//----------------------------------------------------------------------------

// Called to configure the device
// Has read/write access to Mach XML profile to remember what it needs to.
void readConfig(CXMLProfile *DevProf) {
	DbgMsg(("myConfig entry"));


	DbgMsg(("myConfig exit"));
}

//----------------------------------------------------------------------------

// 10Hz update loop

void update() {
    DbgMsg(("*"));
}
//----------------------------------------------------------------------------

// called at 40Hz. This is used for Higher Res timing needs.
// only use as required, it can use alot of system resources.

void initializeDroBuffer(double** buffer, double* source, unsigned int sourceSizeInBytes) {
    unsigned int doubleSize = sizeof(double);

    *buffer = new double[sourceSizeInBytes / doubleSize];
    memcpy(*buffer, source, sourceSizeInBytes);
}

void highSpeedUpdate() {
    if (!trajectoryControl) return;

    if (!localDro1) {
        initializeDroBuffer(&localDro1, trajectoryControl->OEMDROs, sizeof(trajectoryControl->OEMDROs));
    } else {
        for (int i = 0; i < sizeof(trajectoryControl->OEMDROs) / sizeof(double); i++) {
            if (trajectoryControl->OEMDROs[i] != localDro1[i]) {
                localDro1[i] = trajectoryControl->OEMDROs[i];
                if (mqttComm)
                    mqttComm->machDroChanged(i + 200, localDro1[i]); // TODO?? How much is it?
            }
        }
    }

    if (!localDro2) {
        initializeDroBuffer(&localDro2, trajectoryControl->OEMDROs2, sizeof(trajectoryControl->OEMDROs2));
    } else {
        for (int i = 0; i < sizeof(trajectoryControl->OEMDROs2) / sizeof(double); i++) {
            if (trajectoryControl->OEMDROs2[i] != localDro2[i]) {
                localDro2[i] = trajectoryControl->OEMDROs2[i];
                if (mqttComm)
                    mqttComm->machDroChanged(i + 455, localDro2[i]);
            }
        }
    }
}

//----------------------------------------------------------------------------

// ======================================================================================================
//
// The Helpers follow
//
//=======================================================================================================
