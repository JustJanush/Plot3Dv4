/****h* ScPovPlot3D/PeriodicTable.inc
* PURPOSE
*   This helper module contains definition of periodic table. The table is designed
*   for  use with Potential.inc and VectorField.inc modules by now. It can be overriden 
*   if required.
*    |html <hr width=50% align="left">
* VERSION
*  3.1.0.#, tested on PovRay 3.7.
* AUTHOR
*  Janusz Opi³a Ph.D.
*   jmo{at}agh.edu.pl, janusz.opila{at}gmail.com
*   Dept. of Applied Informatics,
*   href:https://www.facebook.com/KatedraInformatykiStosowanejWZAGH/
*   href:http://kis.zarz.agh.edu.pl/
*   AGH University of Science & Technology, Cracow, Poland href:http://www.zarz.agh.edu.pl/English/index.asp
*   Maintained by Janusz Opi³a‚a Ph.D.
*   Homepage: href:http://scpovplot3d.sourceforge.net
* COPYRIGHT
*  GNU GPL v.3 License
*  (c) 2012-now by Janusz Opi³a‚a Ph.D.
*  AGH University of Science and Technology
*
****
  end of RoboDoc comment
****/

#ifndef(PeriodicTable_INC)
#declare PeriodicTable_INC = "2016-01-25";
#version 3.7;

#local _FileName   = "PeriodicTable.INC";
#local _FileVerMaj = "3.1.0.0";
#local _FileVerMin = PeriodicTable_INC;
#debug concat("\n[==> ", _FileName,", ver: ", _FileVerMaj, ", build:  ", _FileVerMin, " <==]\n")

#ifndef (Debug) // Just 4 safety
   #declare Debug=0;
#end

#include "colors.inc"
#include "textures.inc"
#include "Cameras.inc"
#include "ColorMaps.inc"

// dictionary of elements  - external interface 
#declare _NElements = 9; // 1.._NElements
#declare PeriodicTable = array[_NElements+1][6]{ 
   // R,   R_m,     LJ epsilon Hue,  Sat, Lumia -- all these values may be overridden on demand of course
    {   9,  1.00,     1.000,    0,    0,   0.0} // number of elements in the table ;)
   ,{0.31,  0.60,     1.000,    0,    1,   0.5} //  1  H - hydrogen 
   ,{1.00,  2.00,     1.000,    0,    1,   0.5} //  2 He - helium
   ,{1.00,  2.00,     1.000,    0,    1,   0.5} //  3 Li - lithium
   ,{1.00,  2.00,     1.000,    0,    1,   0.5} //  4 Be - berilium
   ,{1.00,  2.00,     1.000,    0,    1,   0.5} //  5  B - bor 
   ,{0.70,  1.40,     1.000,    0,    0,   0.1} //  6  C - Carbon
   ,{0.65,  0.30,     1.000,    0,    1,   0.5} //  7  N - nitrogen 
   ,{0.60,  0.12,     1.000,    0,    1,   0.5} //  8  O - Oxygen                                                      
   ,{0.60,  1.00,     1.000,    0,    1,   0.5} //  9  F - Fluor
};                
                       
#declare _PT = PeriodicTable; // internal representation                       

#end // #ifndef(PeriodicTable_INC)