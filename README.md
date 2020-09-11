# ObjectController (OC)
## About
The ObjectController (OC) has been developed as part of Michael Joyce’s PhD with the Fire Safety Engineering Group at the University of Greenwich. Michael’s thesis is entitled *Representing the Movement of Objects within Evacuation Simulation Models*. Details on the theoretical model used in this software can be found in his thesis (currently in the write-up stage).

This application was built on top of the HEPTAD software developed by Michael Joyce and presented in the paper *Hospital Evacuation Planning Tool for Assistance Devices (HEPTAD)* - DOI: https://doi.org/10.1002/fam.2856


## Use cases
This software is to be used with **buildingEXODUS** to represent the movement of "generic objects" such as wheelchairs, hospital beds, stretchers and luggage. Put simply, the OC is designed to represent any moveable object that doesn't fit well with the grid-like node structure of EXODUS due to its size or kinematic constraints. The software can also be used as a stand-alone application to simulate the movement of objects through geometries (with no people). To use with EXODUS a TCP link can be established. To use as a stand-alone application, an OpenGL window can be used as a simple editor (see **[opengl_controls.txt](/OpenGL_Stuff/opengl_controls.txt)** for a list of controls).

## How to use
1. Insure **init.csv** is in the same folder as the exe and edit any settings there before launching the OC.

1. Ensure **freeglut.dll** and **glew32.dll** are in the same folder as the exe.

1. Run **ObjectController.exe** and run a version of buildingEXODUS that works with the OC.

1. Load the desired geometry and scenario (with objects) into EXODUS.

1. Drag and drop **sync_obj.sfe** into EXODUS to initiate TCP link and sync the geometry, people and objects.

1. Wait until the OC says *OC ready for simulation!!*.

1. Run the simulation in EXODUS.


## Config
You can edit the initial settings in the OC by editing **init.csv**. This includes defining new types of object (`<ADD_PREFAB_22>`, `<ADD_VERTEX_3>`, `<ADD_EXT_ATT_POINT_12>`, `<ADD_INT_ATT_POINT_8>`), changing the calibration settings for velocity obstacle generation (`<OPTION_14>`), turning logs on or off (`<MODE_5>`) and changing the run mode (`<MODE_5>`). The most important thing to note is that by default OC runs with TCP on and OpenGL on. If you would like to switch them off, change the corresponding column in the `<MODE_5>` command to 0. At least one of TCP or OpenGL must be active, otherwise there is no way to control the OC.


## Contributors
Thanks to Peter Lawrence for supervising my PhD and for creating the version of EXODUS (with others at FSEG) that works with the OC.


## Contact
For more information, contact **mike1joy1@gmail.com**.
