/****h* ScPovPlot3D/Lights.inc
* PURPOSE
*   Very important toolkit defining some handy cinematic lights
*   TODO: still a lot!
*   |html <hr width=50% align="left">
* VERSION
*  4.1.1, 2024-02-25 - tested on PovRay 3.7.0
* AUTHOR
*  Janusz Opi³a Ph.D.
*   jmo{at}agh.edu.pl, janusz.opila{at}gmail.com
*   Dept. of Applied Informatics, 
*   href:https://www.facebook.com/KatedraInformatykiStosowanejWZAGH/
*   href:http://kis.zarz.agh.edu.pl/
*   AGH University of Science & Technology, Cracow, Poland href:http://www.zarz.agh.edu.pl/English/index.asp
*   Maintained by Janusz Opi³a Ph.D.
*   Homepage: http://scpovplot3d.sourceforge.net   
* COPYRIGHT
*  GNU GPL v.3 License
*  (c) 2012-now by Janusz Opi³a Ph.D.         
*  AGH University of Science and Technology
* HISTORY
*  2024-02-25, minor code cleaning 
*  2023-12-21, v4.1.1.0, Initial release with Panel_LED() only
****
  end of RoboDoc comment
****/        

#version 3.7;      
#ifndef(Lights_INC)
#declare Lights_INC = "2024-02-25"; // last modification date

// common includes & definitions
#declare _FileName   = "Lights.inc";
#declare _FileVerMaj = "4.1.0";
#declare _FileVerMin = Lights_INC;

#declare _ModuleDescription_Came = concat("\n[==> ", _FileName,", ver: ", _FileVerMaj, ", build:  ", _FileVerMin, " <==]\n");
#debug   _ModuleDescription_Came

#ifndef (_ShowStand)  #declare _ShowStand = true; #end  

/****f* Potential.inc/ShowStand (), _ShowStand
* PURPOSE
*  turns visibility of stand and fremae of LED Panels ON, ie. they deliver area lights only
* SYNOPSIS
*/                                                 
#macro ShowStand()
/*
* INPUTS
*  no inputs
* OUTPUTS
*  sets parameter _ShowStand to yes
* SEE ALSO
*  HideStand ()
*
******/
    #declare _ShowStand = true;
#end
 
/****f* Potential.inc/HideStand (), _ShowStand
* PURPOSE
*  turns visibility of stand and fremae of LED Panels OFF, ie. they deliver area lights only
* SYNOPSIS
*/                                                 
#macro HideStand()  
/*
* INPUTS
*  no inputs
* OUTPUTS
*  sets parameter _ShowStand to no
* SEE ALSO
*  ShowStand ()
*
******/
    #declare _ShowStand = false;
#end

//=========================
/****M* Cameras.inc/Panel_LED 
* PURPOSE
*  Element of main camera subsystem. 
*  Ought to be placed before scene description as macros could be informed about camera position: 
*  this is for future smart and sophisticated extensions ;) Must be followed by call to CameraOn () macro
* SYNOPSIS                        
*/ 
#macro Panel_LED(_clr,        // valid color expression
                 _D, _w, _h   // depth, width, height
                 _nx, _ny,    // number of diods
                 _fi, _r, _z, // stand coordinates vs. +z
                 _ro, _pi     // orientation and pitch of panel vs. +z 
       )  
/*
 * INPUTS
 *  _clr,         valid color expression
 *  _D, _w, _h    float, float, float - body of panel depth, width, height
 *  _nx, _ny,     int, int - number of diods
 *  _fi, _r, _z,  float, float, float - stand coordinates - z>=0, rel. to z+ direction
 *  _ro, _pi      float, float - orientation and pitch of panel -90<_pi<90 rel. to z+ direction
 *  ref: https://en.wikipedia.org/wiki/Aircraft_principal_axes       
 * SEE ALSO
 *  CameraOn ()
 *  setPerspective ()
 *  SetOrthographic ()
 *  SetCameraTarget ()
 ****/                    
#declare _p = union{

union{
 box{ //screen
    <-_w/2, 0.05*_h,    0 >
    < _w/2, 1.05*_h, -_D/4> 
    texture{
      pigment{ color srgb 1 }
      finish{ scDullMirror emission 1  }
    }
 }                                     
 
 box{ // body
    <-_w/2*1.05,     0.0,  -_D*0.05>
    < _w/2*1.05,  1.1*_h,  -_D*1.05> 
    texture{pigment {color srgb <1,1,1>*0} finish { phong 1 reflection 0.00}}
 }            
 
 cylinder{ <-_D, 0, -_D*0.55>, <_D, 0, -_D*0.55>  _D/1.5} 
 
 light_source {
      <0, _h/2, 0.01>         // light's position (translated below)
      color _clr          // light's color
      area_light
      <_w, 0, 0> <0, _h, 0>   // lights spread out across this distance (x * z)
      _nx, _ny                // total number of lights in grid (4x*4z = 16 lights)
      //adaptive 0            // 0,1,2,3...
      // jitter                  // adds random softening of light
      // circular                // make the shape of the light circular
      area_illumination on
//      shadowless
      //orient                  // orient light                
 }

 //translate _D/4*y     
 translate _D*0.55*z
 #if (abs(_pi) >  95)   #local _pi =  95; #end
 #if (abs(_pi) < -95)   #local _pi = -95; #end
 rotate _pi*x      
 rotate (_ro + _fi)*y        
 translate -_D*0.55*z
      
}      
  // primitive stand      
  union{
     cylinder{ <0,  -_z+0.01, -_D*0.55>, <0,      0, -_D*0.55>  _D/2}   // stick
     cylinder{ <0,       -_z, -_D*0.55>, <0, -_z+_D, -_D*0.55>   sqrt(_w*_h)/2.5}   // flat base
     texture{pigment {color srgb <1,1,1>*0} finish { phong 1 reflection 0.00}}
     translate <0, 0, 0>   // <x y z> position of light
  }
  
  translate <0, _z, -_r>   // <x y z> position of light
  rotate -_fi*y
}      
           
  object{ _p  
     #if ( ! _ShowStand)
       no_image  no_shadow  no_reflection 
     #end
  }
 // object{ _p }                      
#end

#else
 DebugMsg(concat("Module \"", _FileName, "recalled!\n",1)
 #ifndef (_CallStack )
   #declare _CallStack = ">Call stack:\n"  
 #end                         
 DebugMsg(_CallStack, 1)
 #declare _CallStack = concat(_CallStack, _ModuleDescription_Pote,"\n")
 DebugMsg(_CallStack, 1)

#end // #ifndef(Lights_INC)