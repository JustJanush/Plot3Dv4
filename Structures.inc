/****h* ScPovPlot3D/Structures.inc
* PURPOSE
*   Very important toolkit defining some handy ...
*   TODO: still a lot!
*   Several useful ....... This library silently includes system library file(s) .... and
*     image:./imgs/Water.png
*
*   Fig.[H2O] Scene employing finishes declared in this library
*
*    |html <hr width=50% align="left">
* VERSION
*  3.2.0.2, tested on PovRay 3.7.
* AUTHOR
*  Janusz Opi�a Ph.D.
*   jmo{at}agh.edu.pl, janusz.opila{at}gmail.com
*   Dept. of Applied Informatics,
*   href:https://www.facebook.com/KatedraInformatykiStosowanejWZAGH/
*   href:http://kis.zarz.agh.edu.pl/
*   AGH University of Science & Technology, Cracow, Poland href:http://www.zarz.agh.edu.pl/English/index.asp
*   Maintained by Janusz Opi�a�a Ph.D.
*   Homepage: href:http://scpovplot3d.sourceforge.net
* COPYRIGHT
*  GNU GPL v.3 License
*  (c) 2012-now by Janusz Opi�a�a Ph.D.
*  AGH University of Science and Technology
* HISTORY
*  2016-01-29/3.2.0.1 - macro ArMerge () added
****
  end of RoboDoc comment
****/

#version 3.7;
#ifndef(Structures_INC)
#declare Structures_INC = "2016-02-03"; // last editing date

#declare _FileName   = "Structures.inc";
#declare _FileVerMaj = "3.2.0.2";
#declare _FileVerMin = Structures_INC;
#debug concat("\n[==> ", _FileName,", ver: ", _FileVerMaj, ", build:  ", _FileVerMin, " <==]\n")

#ifndef (Debug) // Just 4 safety
   #declare Debug=0;
#end

#include "math.inc"
#include "colors.inc"  // Standard pre-defined colors
#include "textures.inc"
#include "stones1.inc" // a lot of stone textures: T_Stone1 - T_Stone44
#include "metals.inc"
#include "shapes.inc"

// Remove as many as possible;)
#include "CommonDefs.inc"    // base library
#include "scFinish.inc"      // finishes
#include "PeriodicTable.inc" // data: periodic table ;) surprised?

//================[ PTube structure ]===============
#declare epsiqeff  = 10000;     // epsilon Lenarda-Jones'a - initial setting
#declare HOdist    = 1.0;
#declare RHyd      = HOdist/5;  
#declare RmLJ      = 2*RHyd;    // LJ well radius

#declare MaxN      = 10;
#declare MaxB      = 13;
#declare PTube_Atms = array[MaxN+1][8]  // charge, x,y,z, R, R_m, epsilon, multifunction see below
//                             [..][7] color component: [..][0]>0 Red; <0 Blue; (0;1)-hue component of HSL color; >=1-element number in Mendelejew table, may encode radius as well, 1-H, 2=He, ....
                 { // POVRay coordinates         
                   //          1/0    2/1   3/2   4/3    5/4   6/5    7/6    8/7
                           { MaxN,       0, 0,       0,    0,    0,        0, 0}, // MaxN=[0][0] - number of molecules in the table:[1..MaxN]
                           { -1, -2*HOdist, 0,  HOdist, RHyd, RmLJ, epsiqeff, 0}, //  1                       6 - 7 - 8 - 9 - 10 /+/ |  
                           { -1,   -HOdist, 0,  HOdist, RHyd, RmLJ, epsiqeff, 0}, //  2  q, x,y,z             |   |   |   |    |     |
                           { -1,         0, 0,  HOdist, RHyd, RmLJ, epsiqeff, 0}, //  3  q, x,y,z             1 - 2 - 3 - 4 -  5 /-/ |
                           { -1,    HOdist, 0,  HOdist, RHyd, RmLJ, epsiqeff, 0}, //  4                                              V +Z
                           { -1,  2*HOdist, 0,  HOdist, RHyd, RmLJ, epsiqeff, 0}, //  5
                         //====================================// q, x,y,z
                           {  1, -2*HOdist, 0, -HOdist, RHyd, RmLJ, epsiqeff, 0}, //  6
                           {  1,   -HOdist, 0, -HOdist, RHyd, RmLJ, epsiqeff, 0}, //  7  q, x,y,z
                           {  1,         0, 0, -HOdist, RHyd, RmLJ, epsiqeff, 0}, //  8  q, x,y,z
                           {  1,    HOdist, 0, -HOdist, RHyd, RmLJ, epsiqeff, 0}, //  9
                           {  1,  2*HOdist, 0, -HOdist, RHyd, RmLJ, epsiqeff, 0}, // 10
                 };        

#declare PTube_Bnds = array[MaxB+1][3] 
                 { 
                           {MaxB, 0, 0}, // holds number of bonds [1..MaxB]
                           {1,    6, 0}, // nums of two atoms, then bondage type (single, double, ionic, covalent, ...)
                           {2,    7, 0},
                           {3,    8, 0},
                           {4,    9, 0},
                           {5,   10, 0},
                         //=============\\
                           {1,    2, 2},
                           {2,    3, 2},
                           {3,    4, 2},
                           {4,    5, 2},
                         //=============\\ 
                           {6,    7, 2},
                           {7,    8, 2},
                           {8,    9, 2},
                           {9,   10, 2},
                 };
//================[ PTube structure END ]===============

//================[ H2O structure ]===============
#declare ROxy   = 0.66;      //  modified Covalent oxygen radius [A]
#declare RHyd   = 0.31;      //  modified Covalent hydrogen radius[A]
#declare HOdist = 0.9584;    // H-O distance in water molecule [A],other value: 1.013[A] (http://www.chem.uw.edu.pl/people/AMyslinski/cw3/h2oa.htm)
#declare epsiqeff  = 10000;  // epsilon Lenarda-Jones'a - initial setting
#declare RmLJ      = RHyd/7; // r_m Lenarda-Jones'a, LJ well radius     - initial setting

// do zakodowania w tablic� H2O          // 104o27'-1.82 [rad] (http://www.chem.uw.edu.pl/people/AMyslinski/cw3/h2oa.htm)
#declare dAngleHOH = 104.45;             //  angle between H-O bonds in [deg]
#declare AngleHOH  = radians(dAngleHOH); //  angle between H-O bonds in [rd]
#declare sAngleHOH = sin(AngleHOH);           
#declare cAngleHOH = cos(AngleHOH);           

#declare H1 = <HOdist, 0, 0>;     // 1. and 2. hydrogen
#declare H2 = <HOdist*cAngleHOH, 0, -HOdist*sAngleHOH>;
                       
#declare H2O_Atoms = array[4][8] {  // charge, x,y,z, R, R_m, epsilon, multifunction see below
//                             [..][7] color component: 
//                                     if [..][0]>0 Red; <0 Blue;
//                                     if (0;1) - hue component of HSL color; 
//                                     if >=1   - element number in Mendelejew table, thus may encode radius as well, 1=H, 2=He, 3=Li, 4=....
// table uses POVRay coordinates         
//        1          2  3        4     5     6         7  8
       {  3,         0, 0,       0,    0,    0,        0, 0}, // MaxN=[0][0] - number of molecules in the table:[1..MaxN]
       { -2,         0, 0,       0, _PT[8][0], _PT[8][1], _PT[8][2], 8}, //  oxygen  
       {  1,      H1.x, H1.y, H1.z, _PT[1][0], _PT[1][1], _PT[1][2], 1}, //  H_1
       {  1,      H2.x, H2.y, H2.z, _PT[1][0], _PT[1][1], _PT[1][2], 1}, //  H_2,    
};        

#declare H2O_Bonds = array[3][3] { 
   {2,  0, 0}, // holds number of bonds [1..MaxB]
   {1,  2, 0}, // nums of two atoms, then bondage type (single, double, ionic, covalent, ...)
   {1,  3, 0},
};
//================[ H2O structure END ]===============


//======[ single graphene BEGIN ]=======
#declare  AngleCC = radians(120.0);     //  angle between H-O bonds in [rd]
#declare sAngleCC = sin(AngleCC);           
#declare cAngleCC = cos(AngleCC);           
#declare CCdist   = 1.42;               //  H-O distance in water molecule [A]
#declare sCC      = sAngleCC*CCdist;    //  sos(x)*D
#declare cCC      = cAngleCC*CCdist;    //  cos(x)*D
                                        //                      3                                 o   o   o
                                        //                     / \                               / \ / \ / \
#declare C1 = < 0, 0, 0>;               // 1.           ^     2   4                             o   o   o
#declare C2 = < CCdist,     0,      0>; // 2.           |+X   |   |              -Z             |   |   |
#declare C3 = < CCdist-cCC, 0,   -sCC>; // 3.           |     1   5   120[deg] --->             o   o   o
#declare C4 = < CCdist,     0, -2*sCC>; // 4.           |      \ /                               \ / \ / \ /
#declare C5 = <      0,     0, -2*sCC>; // 5.                   6                                 o   o   o
#declare C6 = <    cCC,     0,   -sCC>; // 6.

#declare Graphene_Atoms = array[7][8]{  // charge, x,y,z, R, R_m, epsilon, multifunction see below
//                             [..][7] color component: [..][0]>0 Red; <0 Blue; (0;1)-hue component of HSL color; >=1-element number in Mendelejew table, may encode radius as well, 1-H, 2=He, ....
// POVRay coordinates         
//        0          1     2     3          4          5          6   7
       {  6,         0,    0,    0,         0,         0,         0,  0}, // MaxN=[0][0] - number of molecules in the table:[1..MaxN]
       {  0,      C1.x, C1.y, C1.z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  C_1
       {  0,      C2.x, C2.y, C2.z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  C_2
       {  0,      C3.x, C3.y, C3.z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  C_3,    
       {  0,      C4.x, C4.y, C4.z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  C_4, 
       {  0,      C5.x, C5.y, C5.z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  C_5
       {  0,      C6.x, C6.y, C6.z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  C_6
};        

#declare Graphene_Bonds = array[7][3] { 
       {6,  0, 0}, // holds number of bonds [1..MaxB]
       {1,  2, 0}, // nums of two atoms, then bondage type (single, double, ionic, covalent, ...)
       {2,  3, 0},
       {3,  4, 0}, // nums of two atoms, then bondage type (single, double, ionic, covalent, ...)
       {4,  5, 0},
       {5,  6, 0}, // nums of two atoms, then bondage type (single, double, ionic, covalent, ...)
       {6,  1, 0},
};

//======[ graphene END ]=======

#declare _v1 = C5-C1;
#declare _v2 = C3-C6+C2-C1;
//
//   +----> j*_v2
//   |
//   |
//   v
// i*_v1
#declare Graphene2_Atoms = array[41][8] { // charge, x,y,z, R, R_m, epsilon, multifunction see below
//                             [..][7] color component: [..][0]>0 Red; <0 Blue; (0;1)-hue component of HSL color; >=1-element number in Mendelejew table, may encode radius as well, 1-H, 2=He, ....
// POVRay coordinates         
//        0          1     2     3          4          5          6   7
       { 40,         0,    0,    0,         0,         0,         0,  0}, // MaxN=[0][0] - number of molecules in the table:[1..MaxN]
// base cell                                                                        
       {  0,      C1.x, C1.y, C1.z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  C_1 "elementary cell"        o3
       {  0,      C2.x, C2.y, C2.z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  C_2                       2o    
       {  0,      C3.x, C3.y, C3.z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  C_3,                      1o    
       {  0,      C6.x, C6.y, C6.z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  C_4,                         o4
// 1st row arm-chair                           
       {  0,      (C1+_v1).x, (C1+_v1).y, (C1+_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 
       {  0,      (C2+_v1).x, (C2+_v1).y, (C2+_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C6+_v1).x, (C6+_v1).y, (C6+_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C3+_v1).x, (C3+_v1).y, (C3+_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 
       
       {  0,      (C1-_v1).x, (C1-_v1).y, (C1-_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 
       {  0,      (C2-_v1).x, (C2-_v1).y, (C2-_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C6-_v1).x, (C6-_v1).y, (C6-_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C3-_v1).x, (C3-_v1).y, (C3-_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 

       {  0,      (C1+2*_v1).x, (C1+2*_v1).y, (C1+2*_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 
       {  0,      (C2+2*_v1).x, (C2+2*_v1).y, (C2+2*_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C6+2*_v1).x, (C6+2*_v1).y, (C6+2*_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C3+2*_v1).x, (C3+2*_v1).y, (C3+2*_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 
       
       {  0,      (C1-2*_v1).x, (C1-2*_v1).y, (C1-2*_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 
       {  0,      (C2-2*_v1).x, (C2-2*_v1).y, (C2-2*_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C6-2*_v1).x, (C6-2*_v1).y, (C6-2*_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C3-2*_v1).x, (C3-2*_v1).y, (C3-2*_v1).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 

// 2nd row zig-zag
       {  0,      (C1+_v2).x, (C1+_v2).y, (C1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 
       {  0,      (C2+_v2).x, (C2+_v2).y, (C2+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C6+_v2).x, (C6+_v2).y, (C6+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C3+_v2).x, (C3+_v2).y, (C3+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 

       {  0,      (C1+_v1+_v2).x, (C1+_v1+_v2).y, (C1+_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 
       {  0,      (C2+_v1+_v2).x, (C2+_v1+_v2).y, (C2+_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C6+_v1+_v2).x, (C6+_v1+_v2).y, (C6+_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C3+_v1+_v2).x, (C3+_v1+_v2).y, (C3+_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 
       
       {  0,      (C1-_v1+_v2).x, (C1-_v1+_v2).y, (C1-_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 
       {  0,      (C2-_v1+_v2).x, (C2-_v1+_v2).y, (C2-_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C6-_v1+_v2).x, (C6-_v1+_v2).y, (C6-_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C3-_v1+_v2).x, (C3-_v1+_v2).y, (C3-_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 

       {  0,      (C1+2*_v1+_v2).x, (C1+2*_v1+_v2).y, (C1+2*_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 
       {  0,      (C2+2*_v1+_v2).x, (C2+2*_v1+_v2).y, (C2+2*_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C6+2*_v1+_v2).x, (C6+2*_v1+_v2).y, (C6+2*_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C3+2*_v1+_v2).x, (C3+2*_v1+_v2).y, (C3+2*_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 
       
       {  0,      (C1-2*_v1+_v2).x, (C1-2*_v1+_v2).y, (C1-2*_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 
       {  0,      (C2-2*_v1+_v2).x, (C2-2*_v1+_v2).y, (C2-2*_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C6-2*_v1+_v2).x, (C6-2*_v1+_v2).y, (C6-2*_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  
       {  0,      (C3-2*_v1+_v2).x, (C3-2*_v1+_v2).y, (C3-2*_v1+_v2).z, _PT[6][0], _PT[6][1], _PT[6][2],  6}, //  , 
};        

#declare Graphene2_Bonds = array[7][3] {
       {6,  0, 0}, // holds number of bonds [1..MaxB]
       {1,  2, 0}, // nums of two atoms, then bondage type (single, double, ionic, covalent, ...)
       {2,  3, 0},
       {1,  4, 0},
       {4,  5, 0},
       {5,  6, 0},
       {6,  3, 0},                           
};              

//======[ graphene2 END ]=======


// additional every single atom/bondage zoom >0, no DISTANCE is changed!
#declare _mzoom = 1; #macro SetMoleculeZoom(_ZM) #declare _mzoom=_ZM; #end

/****f* Structures.inc/ArMerge
* PURPOSE
*  general double index array (KxN and LxN) merger _A[K][N]+_B[L][N]
*
* SYNOPSIS
*/
#macro ArMerge(_A, _B)  // 
/*
* INPUTS
*  garray   - base data structure 
*  garray   - data structure appended
* OUTPUTS
*  garray   - merged structure, ex. use: #declare A = StrMerge(A, B); #declare A = StrMerge(C, B);
* SEE ALSO
*  StrMerge, BndMerge
*
******/
// macro body
// #end

   #declare _t = array[dimension_size(_A,1)+dimension_size(_B,1)][dimension_size(_A,2)];
   #for(i, 0, dimension_size(_A,1)-1)
      #for(j, 0, dimension_size(_B,2)-1)
        #declare _t[i][j]=_A[i][j];       
      #end
   #end                     
   #local _i = 0;
   #for(i, dimension_size(_A,1),dimension_size(_A,1)+dimension_size(_B,1)-1)
      #for(j, 0, dimension_size(_B,2)-1)
        #declare _t[i][j]=_B[_i][j];       
      #end
      #local _i = _i+1;
   #end        
   #declare _t[0][0]=dimension_size(_A,1)+dimension_size(_B,1)-1; 
   _t
#end 


///====[ manipulation library begin ]========
/****M* Structures.inc/CreateStruct
* PURPOSE
*  Makes POVRay ready object
* SYNOPSIS
*/
#macro CreateStruct (_PTs, _BNs, _D) // molecules table, bonds table, base diameter of bond, may be modified due to bondage type
/*
* INPUTS
*  garray - structure
*  garray - bonds
* OUTPUTS
*  POVRay object ready to render
* SEE ALSO
*  StrTrans, StrMerge, StrRotate, BndMerge, ArMerge
*
******/
#merge{ 
#declare _MxN = _PTs[0][0];
#if (_MxN>2) #merge{ #end // trick to avoid CSG warning   
  #for(i, 1, _MxN) 
     sphere{<_PTs[i][1], _PTs[i][2], _PTs[i][3]>, _mzoom*_PTs[i][4]
             texture{
                 #if (_PTs[i][0] > 0)
                     pigment{color  abs(_PTs[i][0])*Red/3 } 
                 #elseif (_PTs[i][0] < 0) 
                      pigment{color abs(_PTs[i][0])*Blue/3}
                 #else  // analyze type of element from periodic table or moderate Hue
                      #declare _H = _PTs[i][7];
                      #if (_H=0) // just gray
                         pigment{color rgb<.5,.5,.25>}
                      #elseif(_H<1)   
                         #declare cl = CHSL2RGB(<_H*180, 1, .3 .6, 1.0>); // light gray opaque
                         pigment{ color rgbt <cl.red, cl.green, cl.blue, 0>}
                      #else // appearence defined in periodic table                      
                         #declare el = _PTs[i][7];       
                         #declare cl = CHSL2RGB(<PeriodicTable[el][3],PeriodicTable[el][4],PeriodicTable[el][5], .6, 1.0>); // light gray opaque
                         // pigment{ color rgbt <cl.red, cl.green, cl.blue, 0>}
                         pigment{ color rgbt <1, 1, 0, 0>} // yellow is set temporary carbon hack
                      #end   
                 #end      
                 // finish{emission .61 reflection metallic phong .4} 
                 // finish{ scDullMirror emission .7 }
                 finish{ Dull }
             }    
     }
  #end                      
#if (_MxN>2) } #end  // trick to avoid CSG warning   


// OR: Bright_Bronze, NBglass, Tinny_Brass, Soft_Silver, NBoldglass, T_Yellow_Glass, T_Copper_4A, T_Gold_1B 
#declare _MxB = _BNs[0][0]; // ! avoid glassy interior  or transparent texture
#if (_MxB>2) #merge{ #end   // trick to avoid CSG warning   
  #for(i, 1, _MxB) 
     cylinder{<_PTs[_BNs[i][0]][1], _PTs[_BNs[i][0]][2], _PTs[_BNs[i][0]][3]>, 
              <_PTs[_BNs[i][1]][1], _PTs[_BNs[i][1]][2], _PTs[_BNs[i][1]][3]>, 
                #switch(_BNs[i][2])                                              // bondage type
                    #case(0)     _mzoom*_D texture{ pigment{color    Silver} finish{Dull}} #break; // "normal"
                    #case(1) 1.5*_mzoom*_D texture{ pigment{color  Maroon/2} finish{Dull}} #break; // "double normal"
                    #case(2) 2.0*_mzoom*_D texture{ pigment{color   Green/3} finish{Dull}} #break; // "ionic"                  
                    #case(3) 1.3*_mzoom*_D texture{ pigment{color rgbf <0.0,.5,.5,0>} finish{Dull}} #break; // "hydrogen bond"                  
                    #else        _mzoom*_D texture{ pigment{color    Silver} finish{Dull}} #break; // ...and so on ;)                                              
                #end
     }    
  #end      
#if (_MxB>2) } #end  // trick to avoid CSG warning   
}  
#end // CreateStruct()

/****M* Structures.inc/StrMerge
* PURPOSE
*  Merges two molecule or bonds structures
*
* SYNOPSIS
*/
#macro StrMerge(_A, _B)
/*
* INPUTS
*  garray   - base data structure 
*  garray   - data structure appended
* OUTPUTS
*  garray   - merged structure, ex. use: #declare A = StrMerge(A, B); #declare A = StrMerge(C, B);
* SEE ALSO
*  StrRotate, StrTrans
*
******/
   #declare _t = array[dimension_size(_A,1)+dimension_size(_B,1)-1][dimension_size(_B,2)];
   #for(i, 1, dimension_size(_A, 1)-1)
      #for(j, 0, dimension_size(_A, 2)-1)
        #declare _t[i][j] = _A[i][j];       
      #end  
   #end                     
   #local _i = 1;
   #for(i, dimension_size(_A, 1),dimension_size(_A,1)+dimension_size(_B,1)-2)
      #for(j, 0, dimension_size(_A, 2)-1)
        #declare _t[i][j]=_B[_i][j];       
      #end  
      #local _i = _i+1;
   #end           
   #declare _t[0][0] = _A[0][0]+_B[0][0];
   _t
#end 

/****M* Structures.inc/BndMerge
* PURPOSE
*  Merges two bonds structures
*
* SYNOPSIS
*/
#macro BndMerge(_A, _B, _S)
/*
* INPUTS
*  garray   - base data structure 
*  garray   - data structure appended
*  uinteger - number of ATOMS in base structure, referred by bonds in matrix _A
* OUTPUTS
*  garray   - merged structure, ex. use: #declare A = StrMerge(A, B); #declare A = StrMerge(C, B);
* SEE ALSO
*  StrRotate, StrTrans
*
******/
   #declare _t = array[dimension_size(_A,1)+dimension_size(_B,1)-1][dimension_size(_B,2)];
   #local _dim = dimension_size(_A,1);
   #for(i, 0, _dim-1)
      #for(j, 0, dimension_size(_A, 2)-1)
        #declare _t[i][j]=_A[i][j];       
      #end  
   #end                     
   #local _i = 1;         
   #for(i, _dim,_dim+dimension_size(_B, 1)-2)
      #declare _t[i][0]=_B[_i][0]+_S; // index shift        
      #declare _t[i][1]=_B[_i][1]+_S; // due to larger structure      
      #declare _t[i][2]=_B[_i][2];    // bond type, no index shift   
      #local _i = _i+1;
   #end
   #declare _t[0][0] = _A[0][0]+_B[0][0];
   _t
#end 

/*
0 0  {2, 0, 0}, // holds number of bonds [1..MaxB]
1 1  {1, 2, 0}, // nums of two atoms, then bondage type (single, double, ionic, covalent, ...)
2 2  {1, 3, 0},

0 -  {2, 0, 0}, // holds number of bonds [1..MaxB]
1 3  {1, 2, 0}, // nums of two atoms, then bondage type (single, double, ionic, covalent, ...)
2 4  {1, 3, 0},
         
0 -  {2, 0, 0}, // holds number of bonds [1..MaxB]
1 5  {1, 2, 0}, // nums of two atoms, then bondage type (single, double, ionic, covalent, ...)
2 6  {1, 3, 0},
====================
0
1   1   A        
2   2   B        1/2
3   3   B        1/3
      
0
1   4   A
2   5   B        4/5
3   6   B        4/6

0
1   7   A
2   8   B        7/8
3   9   B        7/9
*/


/****M* Structures.inc/StrTrans
* PURPOSE
*  Rotates given array 
*
* SYNOPSIS
*/
#macro StrTrans(_A, _V)
/*
* INPUTS
*  garray   - data structure to operate on
*  3Dvector - rotation axis (might be: x, y, z, x+y, <1,2,1.>)
*  float    - rotation amount in degrees
* OUTPUTS
*  garray   - rotated structure
* SEE ALSO
*  StrMerge, StrRotate
*
******/
   #for(i, 1, dimension_size(_A,1)-1)
       #declare _A[i][1]=_A[i][1]+_V.x;       
       #declare _A[i][2]=_A[i][2]+_V.y;   
       #declare _A[i][3]=_A[i][3]+_V.z;   
   #end                     
#end   

/****M* Structures.inc/StrRotate
* PURPOSE
*  Rotates given array _A around given axis _V by amount _deg degrees
*
* SYNOPSIS
*/
#macro StrRotate ( _A, _V, _deg )
/*
* INPUTS
*  garray   - data structure to operate on
*  3Dvector - rotation axis (might be: x, y, z, x+y, <1,2,1.>)
*  float    - rotation amount in degrees
* OUTPUTS
*  garray   - rotated structure
* SEE ALSO
*  StrMerge, StrTrans
*
******/
   #for(i, 1, dimension_size(_A,1)-1)
       #local _Vr = <_A[i][1], _A[i][2], _A[i][3]>;
       #local _Vr = vaxis_rotate(_Vr,_V,_deg);
       #declare _A[i][1]= _Vr.x;       
       #declare _A[i][2]= _Vr.y;   
       #declare _A[i][3]= _Vr.z;   
   #end                     
#end


/* Water ice simple six molecule flat cluster.  
   This library part, do NOT remove!
 #declare Hbond = 1.86; // [A] hydrogen bond
 #declare tbnd = H2O_Bonds; // 1
 #declare tarr = H2O_Atoms;        
 #declare vp = (Hbond+HOdist)*< .5,0,-sin(radians(60))>;
 #declare vm = (Hbond+HOdist)*<-.5,0,-sin(radians(60))>;
 #declare vx = (Hbond+HOdist)*<1,0,0>;
 
 StrRotate(tarr, y, -dAngleHOH) // 1st
 #declare tmpr = H2O_Atoms; // 2nd
   StrRotate(tmpr, y, -dAngleHOH+60 )
   StrTrans(tmpr, vx)//<Hbond+HOdist, 0, 0>)
 #declare tbnd = BndMerge( tbnd, H2O_Bonds,  dimension_size(tarr,1)-1);   // 4
 #declare tarr = StrMerge( tarr, tmpr);  

 #declare tmpr = H2O_Atoms; // 3rd
   StrRotate(tmpr, y, -dAngleHOH+120 )
   StrTrans(tmpr, vx+vp)
 #declare tbnd = BndMerge( tbnd, H2O_Bonds, dimension_size(tarr,1)-1);    // 7
 #declare tarr = StrMerge( tarr, tmpr);  

 #declare tmpr = H2O_Atoms; // 4th
   StrRotate(tmpr, y, -dAngleHOH+180  )
   StrTrans(tmpr, vx+vp+vm)
 #declare tbnd = BndMerge( tbnd, H2O_Bonds, dimension_size(tarr,1)-1);    // 10
 #declare tarr = StrMerge( tarr, tmpr);  

 #declare tmpr = H2O_Atoms; // 5th
   StrRotate(tmpr, y, -dAngleHOH+240  )
   StrTrans(tmpr, vp+vm)
 #declare tbnd = BndMerge( tbnd, H2O_Bonds, dimension_size(tarr,1)-1);    // 13
 #declare tarr = StrMerge( tarr, tmpr);  

 #declare tmpr = H2O_Atoms; // 6th
   StrRotate(tmpr, y, -dAngleHOH+300  )
   StrTrans(tmpr, vm)
 #declare tbnd = BndMerge( tbnd, H2O_Bonds, dimension_size(tarr,1)-1);   // 16
 #declare tarr = StrMerge( tarr, tmpr);                                     
 
 #declare HB = array[6][3]{
    { 1,18,3}  //  1
   ,{ 4, 3,3}  //  4
   ,{ 7, 6,3}  //  7
   ,{10, 9,3}  // 10
   ,{13,12,3}  // 13
   ,{16,15,3}  // 16
 };

 #declare tbnd = ArMerge( tbnd, HB );

// StrTrans(tarr, <-4, 0, 4>)
 StrRotate(tarr, y, 25 )

 #declare Molecule = object{CreateStruct( tarr, tbnd, 0.07)}; // *** PTube, Bonds, .05)};7
 object{Molecule} 
*/                                  

#end // put this line on the very end!

// temporary trashcan
//============[ Water molecule ]============
//duplicate
//#declare AngleHOH = radians(104.45);     //  angle between H-O bonds in [rd]
//#declare sAngleHOH = sin(AngleHOH);           
//#declare cAngleHOH = cos(AngleHOH);           

//#declare H1 = <HOdist, 0, 0>;    // 1. and 2. hydrogen
//#declare H2 = <HOdist*cAngleHOH, 0, -HOdist*sAngleHOH>;
                       
//#declare H2O_atoms = array[4][8]  // charge, x,y,z, R, R_m, epsilon, multifunction see below
//                             [..][7] color component: [..][0]>0 Red; <0 Blue; (0;1)-hue component of HSL color; >=1-element number in Mendelejew table, may encode radius as well, 1-H, 2=He, ....
//                 { // POVRay coordinates         
//                   //          1      2     3     4       5     6     7       8
//                           {  3,         0, 0,       0,    0,    0,        0, 0}, // MaxN=[0][0] - number of molecules in the table:[1..MaxN]
//                           { -2,         0, 0,       0, RHyd, RmLJ, epsiqeff, 0}, //  oxygen                  6 - 7 - 8 - 9 - 10 /+/ |  
//                           {  1,      H1.x, H1.y, H1.z, RHyd, RmLJ, epsiqeff, 0}, //  H_1
//                           {  1,      H2.x, H2.y, H2.z, RHyd, RmLJ, epsiqeff, 0}, //  H_2
//                 };        
//============================================================================================                       

//#declare WaterMolecule = merge {
//      sphere{ H1, RHyd texture{ pigment{color  Red/2} finish{emission .71 reflection metallic phong .4} }}
//      cylinder {0, H1, RHyd/2.3 pigment{color Silver} } 
//      sphere{ 0, ROxy texture{  pigment{color Blue/2} finish{emission .71 reflection metallic phong .4} }}
//      cylinder {0, H2, RHyd/2.3 pigment{color Silver} }
//      sphere{ H2, RHyd texture{ pigment{color  Red/2} finish{emission .71 reflection metallic phong .4} }}
//} /// end merge{}  

//#declare WaterV0 = function(x,y,z, R){kqcoef*(
//                         -2/sqrt(pow(   x,2)+ pow(y,2) + pow(z,2))+      // oxygen y=0;
//                          1/sqrt(pow(x-R,2) + pow(y,2) + pow(z,2))+      // hydrogen y = 0; at <R, 0, 0>
//                          1/sqrt(pow((x-R*cAngleHOH),2) 
//                            + pow(y,2)  // hydrogen at <-R, 0, R>
//                            + pow((z+R*sAngleHOH),2))
//                          )
//                };
//
                             
