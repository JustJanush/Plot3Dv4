/****h* ScPovPlot3D/TextExt.inc
* PURPOSE
*   Text object extensions,
*     image:./imgs/StringsUTF.jpg
*
*   Fig.[TextExt] Example of implementation of this module. At top text containing
*   polish national characters is displayed. Do not even try to pronounciate it ;)
*   |html <hr width=50% align="left">
* VERSION
*  4.0.0, tested on PovRay 3.7.
* AUTHOR
*  Janusz Opi≥a Ph.D.
*   jmo{at}agh.edu.pl, janusz.opila{at}gmail.com
*   Dept. of Applied Informatics,
*   href:https://www.facebook.com/KatedraInformatykiStosowanejWZAGH/
*   href:http://kis.zarz.agh.edu.pl/
*   AGH University of Science & Technology, Cracow, Poland href:http://www.zarz.agh.edu.pl/English/index.asp
*   Maintained by Janusz Opi≥a Ph.D.
*   Homepage: href:http://scpovplot3d.sourceforge.net
* COPYRIGHT
*  GNU GPL v.3 License
*  (c) 2012-now by Janusz Opi≥a Ph.D.
*  AGH University of Science and Technology
* HISTORY                                       
*  2018-02-03, 'tau' added
*  2017-09-25, some german "umlauts" added 
*  2017-04-12, 'Ø' letter corrected, minor changes
*  2017-02-03, macro added GetTextExtent() - returns appr. horizontal text size
*  2016-02-17, 3.2.0.3 - nabla unicode character correction
*  2016-01-29, 3.2.0.2 - macros TrimStr(), TrimStrC() added.
*  2016-01-28, 3.2.0.1 - minor corrections, added some symbols
****
  end of RoboDoc comment
****/

#version 3.7;
#ifndef(TextExt_INC)
#declare TextExt_INC = "2018-09-17"; // last edition date

#declare _FileName   = "TextExt.inc";
#declare _FileVerMaj = "4.0.0";
#declare _FileVerMin = TextExt_INC;
#debug concat("\n[==> ", _FileName,", ver: ", _FileVerMaj, ", build:  ", _FileVerMin, " <==]\n")

#ifndef (Debug) // Just 4 safety
   #declare Debug=0;
#end

#include "strings.inc"
#include "functions.inc"
#include "shapes.inc"
#include "CommonDefs.inc"

/****d* TextExt.inc/FormattingConstants [  ]
* PURPOSE
*   Escape sequences for parsing subsystem. Introductory stage, no debugging messages!
*
*   WARNING: may change in next releases (but i hope don't)!
* SOURCE
*/
// `a  - (LEFT apostrophe) 'ogonkified' letter - here polish accented as of now
//  _  - sub-script next character
//  ^  - super-script next character
// { } - block of super/sub-script (direct brackets on regular line)
// .   - inserts dot. Surprised? Utilizes superellipsoid object on regular line, raw dot in indexes.
// " " - regular space (just hit 'Space')
//  `| - vertical bar in indexes, but..
//  |  - short space on regular line
// double `` means single `  (not in super/sub-script)
// double ^^ means single ^  (not in super/sub-script)
// double \\ means single \
//   you can also insert magic codes defined above as _pi_
//   but you need to use concat() function to succeed
/*******/


/****iv* TextExt.inc/SubSupScale, SubShift, SupShift, UpPos, DnPos, SpcStub, _DotScalingFactor, _Dp, _HCur, _VCur, _TLen, _RFac, __Cnt
* PURPOSE
*   Helper variables
* SOURCE
*/
#declare SubSupScale = 0.7;  // lower and upper indexes are scaled by this factor from base font.
#declare SubShift    = 0.3;  // relative down shift of subscripts
#declare SupShift    = 0.8;  // relative up shift of superscripts
#declare UpPos       =  SupShift;  // vertical position of cursor - superscripts
#declare DnPos       = -SubShift;  // vertical position of cursor - subscripts
#declare _Dp         = 0.1;        // Depth of characters
#declare SpcStub     = "-";         // default fake space character
#declare _DotScalingFactor = 0.75; // "dot" size. You don't need to change this - usually.
#declare _HCur = 0.00; // current horizontal position of cursor
#declare _VCur = 0.00; // current vertical position of cursor
#declare _TLn  = 0;    // length of string parsed constant during parsing
#declare _RFac = 0.69; // reduction factor for indexes
#declare __Cnt = 0.00; // accounts calls to ParseNext() macro as POVRay doesn't implements "static" variables
#declare __BCnt= 0.00; // {} block level counter
#declare _tbox = 0.00; // size of parsed text block
/*******/

// internal array, just for debugging purposes
#declare __Prf = array[7] {"", ">>",
                               "--->>",
                               "------>>",
                               "--------->>",
                               "------------>>",
                               "--------------->>"};


/****iv* TextExt.inc/_ArNap [helper array]
* PURPOSE
*   Helper variable stores string parsed
* SOURCE
*/
#declare _ArNap = array[1];
/*******/



/****v* TextExt.inc/Fonts, _FLibera,  _FCourierI,  _FTimes, _FSegoa, _FArialUni, _FArialRond, _FCooper,  _FCorsiva
* PURPOSE
*   Helper variables
* SOURCE
*/
#declare _FLibera    = "LiberationMono-Regular.ttf";   // Liberation Mono, Regular
#declare _FCourierI  = "couri.ttf";                   // italic Courier
#declare _FTimes     = "times.ttf";                   // Times New Roman
#declare _FSegoa     = "segoesc.ttf";                 // Segoa
#declare _FArialUni  = "arialuni.ttf";                // Arial - Unicode
#declare _FArial     = "arial.ttf";                   // Arial - regular - no PL signs ;c
#declare _FArialRond = "ARLRDBD.TTF"                  // Arial Rounded MT Bold - no PL signs ;c
#declare _FCooper    = "COOPBL.TTF";                  // Cooper Black - no PL signs ;c
#declare _FCorsiva   = "MTCORSVA.TTF";                // MonoType Corsiva
//----- shall be continued....
/*******/


/****f* TextExt.inc/SetLDpth
* PURPOSE
*  Sets "depth" of the text object
* SYNOPSIS
*/
#macro SetLDpth(_d ) // string _spc declares depth of character(s)
/*
 * INPUTS
 *  _d - positive float
 *******/
   #declare _Dp = _d;
#end

/****f* TextExt.inc/SetSpcStub
* PURPOSE
*  Sets character imitiating 'space'
* SYNOPSIS
*/
#macro SetSpcStub(_spc ) // string _spc declares space width character(s)
/*
 * INPUTS
 *  string _spc - may be "-" or "."  or even "asd"
 *******/
   #declare SpcStub = _spc;
#end


/// initial default safe value
/****f* TextExt.inc/Set_TFont, _FF
* PURPOSE
*  Defines font for textual elements of the scene
*    WARNING: Font must exist in the host operating system
*    default setting "arial.ttf" should be safe on Windows, as well as on some GNU/Linux distros
*    You may also use Helvetica on GNU\Linux systems, as it should exist by default
* SYNOPSIS
*/
#declare _FF  = _FArial; // Default font for texts object handled by this tool macros set
#macro Set_TFont(_TxtFnt)
/*
 * INPUTS
 *  _FF - string containing name of the font, ex. "Arial.ttf"
 *
 *  WARNING: One have to use it BEFORE parsing call to Text() macros!
 * SEE ALSO
 *  Set_TextFont ()
 ****/
    #declare _FF = _TxtFnt;
#end


/****v* TextExt.inc/UTFCodes
* PURPOSE
*   Constants with UTF codes of some common characters including polish national characters. Depends on existence of definition of given glyph.
*
*   Description in comments.
* SOURCE
*/
//==========[ PL_pl 'ogonki' ]==========
#declare _a_   = "\u0105";  // π
#declare _A_   = "\u0104";  // •
#declare _c_   = "\u0107";  // Ê
#declare _C_   = "\u0106";  // ∆
#declare _e_   = "\u0119";  // Í
#declare _E_   = "\u0118";  //  
#declare _l_   = "\u0142";  // ≥
#declare _L_   = "\u0141";  // £
#declare _n_   = "\u0144";  // Ò
#declare _N_   = "\u0143";  // —
#declare _o_   = "\u00F3";  // Û
#declare _O_   = "\u00D3";  // ”
#declare _s_   = "\u015B";  // ú
#declare _S_   = "\u015A";  // å
#declare _X_   = "\u0179";  // è
#declare _x_   = "\u017A";  // ü
#declare _Z_   = "\u017B";  // Ø
#declare _z_   = "\u017C";  // ø
// selected DE
#declare _u_   = "\u00FC";  // ¸
#declare _U_   = "\u00DC";  // ‹
#declare _oe_  = "\u00F6";  // ˆ
#declare _OE_  = "\u00D6";  // ÷

//
#declare _at_     = "\u0040";  // @
#declare _spc_    = "\u0020";  // just Space
#declare _Ao_     = "\u00C5";  // Angstrem
#declare _mu_     = "\u00B5";  // greek mu
#declare _cdot_   = "\u00B7";  // multiplication sign
#declare _pi_     = "\u03C0";  // pi sign
#declare _phi_    = "\u03c6";  // italic small phi 
#declare _ro_     = "\u03C1";  // rho
#declare _tau_    = "\u03C4";  // tau
#declare _tht_    = "\u03D1";  // theta small
#declare _deg_    = "\u00B0";  // degree sign
#declare _par_    = "\u00A7";  // paragraph ß
#declare _promil_ = "\u2030";  // promile â

// these symbols may depend on font applied
#declare _inSet_  = "\u2208";  // belongs to the set
#declare _diff_   = "\u00B9";  // difference (as in "2 different from 3")
#declare _BgDt_   = "\u00B7";  // the big DOT
#declare _idnt_   = "\u00BA";  // identity
#declare _LePa_   = "\uFD3E";  // big Ornate left parenthesis
#declare _RiPa_   = "\uFD3F";  // big Ornate right parenthesis
#declare _inf_    = "\u221E";  // infinity symbol
#declare _plumin_ = "\u00B1";  // plus/minus ±
#declare _fromTo_ = "\u00F7";  // from-to math symbol, AKA division operator
#declare _nabla_  = "\u2207";  // nabla operator (gradient, divergence)
#declare _pdiff_  = "\u2202";  // partial difference
#declare _nabl2_  = "\u2206";  // Laplace operator (nabla^2)

// currencies (dollar is simply '$')
#declare _pnd_ = "\u00A3"; // Pound sign
#declare _yen_ = "\u00A5"; // Yen sign
#declare _eur_ = "\u20AC"; // EURo sign

// selected superscripts
#declare _0p_  = "\u2070"; // upper index '0' (power '0')
#declare _1p_  = "\u00B9"; // upper index '1' (ex. as in valence value in chemistry)
#declare _2p_  = "\u00B2"; // power '2'
#declare _3p_  = "\u00B3"; // power '3'
#declare _4p_  = "\u2074"; // power '4'
#declare _5p_  = "\u2075"; // power '5'
#declare _6p_  = "\u2076"; // power '6'
#declare _7p_  = "\u2077"; // power '7'
#declare _8p_  = "\u2078"; // power '8'
#declare _9p_  = "\u2079"; // power '9' // in fact rarely met
#declare _ip_  = "\u2071"; // power 'i'
#declare _np_  = "\u207F"; // power 'n'

// quotients
#declare _14_  = "\u00BC"; //  1/4 one quarter
#declare _12_  = "\u00BD"; //  1/2 one half
#declare _34_  = "\u00BE"; //  3/4 three quarter

// digits and selected subscripts (tiny letters )
#declare _0s_ = "\u2080"; // index '0'
#declare _1s_ = "\u2081"; // index '1'
#declare _2s_ = "\u2082"; // index '2'
#declare _3s_ = "\u2083"; // index '3'
#declare _4s_ = "\u2084"; // index '4'
#declare _5s_ = "\u2085"; // index '5'
#declare _6s_ = "\u2086"; // index '6'
#declare _7s_ = "\u2087"; // index '7'
#declare _8s_ = "\u2088"; // index '8'
#declare _9s_ = "\u2089"; // index '9'
#declare _is_ = "\u1D62"; // index 'i'
#declare _rs_ = "\u1D63"; // index 'r'
#declare _ks_ = "\u2096"; // index 'k'
#declare _ls_ = "\u2097"; // index 'l'
#declare _ms_ = "\u2098"; // index 'm'
#declare _ns_ = "\u2099"; // index 'n'
#declare _os_ = "\u2092"; // index 'o'
#declare _ps_ = "\u209A"; // index 'p'
#declare _ss_ = "\u209B"; // index 's'
#declare _ts_ = "\u209C"; // index 't'
#declare _xs_ = "\u2093"; // index 'x'

/*
 ******/

/****f* TextExt.inc/TrimStr
* PURPOSE
*  Removes trailing spaces from the string passed.
*  Spaces are expected before and after the core text.
* EXAMPLE
*  #declare SomeStr = "    ABC DE  ";
*  #declare SomeStr = TrimStr(SomeStr);
*  Now string SomeStr equals: "ABC DE". Intern spaces are preserved.
* SYNOPSIS
*/
#macro TrimStr ( _s )
/*
* INPUTS
*  string - text to deal with.
* OUTPUTS
*  string - stripped text.
* SEE ALSO
*  TrimStrC
*
******/
    #declare _t = ""; // empty string
    // skip leading spaces
    #local _i = 1;
    #local _L =strlen(_s);
    #while (substr(_s, _i, 1)=" " & _i <= _L)
       #local _i = _i+1;
    #end
    #local _j = _L;
    #while (substr(_s, _j, 1)=" " & _j > 0)
       #local _j = _j-1;
    #end
//    "...A..DE.." _L = 10
//     1234567890   5 chars to retrieve
//     _i=4, _j=8
    #declare _t = substr(_s, _i, _j-_i+1);
#if (Debug>1) #debug concat("\ni=", str(_i,2,0)," j=", str(_j,2,0), "; \"", _s,"\"=>>\""_t, "\"\n") #end
    _t
#end

/****f* TextExt.inc/TrimStrC
* PURPOSE
*  Removes trailing spaces from the string passed.
*  Spaces are expected before and after the core text.
* EXAMPLE
*  #declare SomeStr = "    ABC DE  ";
*  #declare SomeStr = TrimStr(SomeStr);
*  Now string SomeStr equals: "ABC DE". Intern spaces are preserved.
* SYNOPSIS
*/
#macro TrimStrC ( _s, _c )
/*
* INPUTS
*  string - text to deal with.
*  char   - charcter to skip over
* OUTPUTS
*  string - stripped text.
* SEE ALSO
*  TrimStr
*
******/
// skip leading chars '_c'
    #local _i = 1;
    #local _L =strlen(_s);
    #while (substr(_s, _i, 1)=_c & _i <= _L)
       #local _i = _i+1;
    #end
// cut off trailing '_c's
    #local _j = _L;
    #while (substr(_s, _j, 1)=_c & _j > 0)
       #local _j = _j-1;
    #end
//    "...A..DE.." _L = 10
//     1234567890   5 chars to retrieve
//     _i=4, _j=8
    #declare _t = substr(_s, _i, _j-_i+1);
#if (Debug>1) #debug concat("\ni=", str(_i,2,0)," j=", str(_j,2,0), "; \"", _s,"\"=>>\""_t, "\"\n") #end
    _t
#end



/****M* TextExt.inc/UnicodifyPL  [ (_In) ]
* PURPOSE
*  Parses string  _In passed to makro and returns copy of it as _Out with subscripts inserted
*   WARNING: not every font defines polish "Ogonki".
*   WARNING2: this is rather obsolete
*   Usage: prepend each polish national character changed to latin
*   base with " ` " (left apostrophe). Ex. "Ø" =>    "`Z"
*   To insert left apostrophe just put it in twice: "``" => "`"
* SYNOPSIS
*/
#macro UnicodifyPL(_In) // ~a => π and so on.
/*
 * INPUTS
 *  _In - string to parse, returns result of parsing as well
 * SEE ALSO
 *  Text ()
 ****/
    #declare _Tmp = "";
    #local i = 1;
    #local _Ln = strlen(_In);
    #while (i<=_Ln)
       #local _Chr=substr(_In, i, 1);
       #if (_Chr="`") // left(!) apostrophe
           #local i = min(i+1, _Ln);
           #local _Chr=substr(_In, i, 1);
           #if         (_Chr="a")  #declare _Chr=_a_;
               #elseif (_Chr="A")  #declare _Chr=_A_;
               #elseif (_Chr="c")  #declare _Chr=_c_;
               #elseif (_Chr="C")  #declare _Chr=_C_;
               #elseif (_Chr="e")  #declare _Chr=_e_;
               #elseif (_Chr="E")  #declare _Chr=_E_;
               #elseif (_Chr="l")  #declare _Chr=_l_;
               #elseif (_Chr="L")  #declare _Chr=_L_;
               #elseif (_Chr="n")  #declare _Chr=_n_;
               #elseif (_Chr="N")  #declare _Chr=_N_;
               #elseif (_Chr="o")  #declare _Chr=_o_;
               #elseif (_Chr="O")  #declare _Chr=_O_;
               #elseif (_Chr="s")  #declare _Chr=_s_;
               #elseif (_Chr="S")  #declare _Chr=_S_;
               #elseif (_Chr="z")  #declare _Chr=_z_;
               #elseif (_Chr="Z")  #declare _Chr=_Z_;
               #elseif (_Chr="x")  #declare _Chr=_x_;
               #elseif (_Chr="X")  #declare _Chr=_X_;
// selecte DE letters
               #elseif (_Chr="u") #declare _Chr=_u_ ;  // ¸
               #elseif (_Chr="U") #declare _Chr=_U_ ;  // ‹
               #elseif (_Chr="Û") #declare _Chr=_oe_;  // ˆ
               #elseif (_Chr="”") #declare _Chr=_OE_;  // ÷

               #else  // thats OK, insert this character
           #end  // #if..#end
       #end
       #declare _Tmp = concat(_Tmp, _Chr); //
       #if (Debug>__Stable)
         #debug concat("[UnicodifyPL > " str(i,4,0) ": " _Tmp, "\n")
       #end
       #local i=i+1;
    #end
    #declare _In = _Tmp;
#end

/****f* TextExt.inc/GetTextExtent  [ (None) ]
* PURPOSE
*   Returns horizontal (x-axis) text block extent in POV units
* SYNOPSIS
*/
#macro GetTextExtent()
/*
 * INPUTS
 *  None
 * SEE ALSO
 *  None
 ****/
   _tbox
#end

/****f* TextExt.inc/InsrtChr  [ (string, float, float, float) ]
* PURPOSE
*   Inserts single char into scene "as is" without processing
* SYNOPSIS
*/
#macro InsrtChr(_Cc, _hc, _vc, _sc)
/*
 * INPUTS
 *  _Cc - single character string to put, returns result of parsing as well
 *  _hc - horizonatl cursor
 *  _vc - vertical cursor
 *  _sc - scaling factor
 * SEE ALSO
 *  ParseText ()
 ****/
    text{ttf _FF, _Cc, _Dp, 0    // _Dp is global
        scale <_sc, _sc, 1>
        translate <_hc, _vc, 0>
    }
    #declare _tbox = _hc + Text_Width(_FF, _Cc, _sc, 0);
    #declare _hc = _hc + Text_Space(_FF, _Cc, _sc, 0);   
#end

/****M* TextExt.inc/Text  [ (string) ]
* PURPOSE
*  Recursively parses string  _Nap passed to makro and returns part of it but with super-scripts inserted as solitary characters
*   front-end to parsing macro
*   REMARK: This should work as expected with ANY font
*   Usage: Formats string due to special sequences
*   Ex: prepend subscript with underscore "_", longer subscripts enclose in curly paranthesis _{j,i_2} and so on
* SYNOPSIS
*/
#macro Text( _Nap ) // front-end to TextExt() macro
/*
 * INPUTS
 *  _Nap - string to parse, returns remainder of non parsed input.
 * SEE ALSO
 *  UnicodifyPL ()
 ****/
   UnicodifyPL(_Nap)     // first replace all polish characters but only ONCE
   #declare __Cnt = 0.0; // increment recurrence counter

   #declare _TLn   = strlen(_Nap); // _TLn is global: Total length of string after replacement
   #ifdef (_ArNap)       // destroy previous version
       #undef _ArNap
   #end
   #declare _ArNap = array[_TLn+1]; // one more character, the first one is numbered '0' //thus indexation is simplified...
   #declare _ArNap[0] = "";         // .. to simplify indexing later. So first character is indexed as '1'
                                    //    and last character is adressed as _ArNap[_Ln]. Wow!
   #for(i, 1, _TLn)                 // consecutive characters are counted from [1:_TLn]
       #declare _ArNap[i] = substr(_Nap, i, 1);
   #end
//// start recursive parsing - may be long, boring & tedious
   TextExt(1, 0, 0, 1) // from first character, no additional spacing (,0,0,), normal size
//// eventually DONE!
#end // Text() macro
/*******************/

/****M* TextExt.inc/TextExt  [ (int, float, float, float) ]
* PURPOSE
*  Recursively parses string  _Nap passed to makro and returns part of it but with super-scripts inserted as solitary characters
*   Usually one don't need to use this macro directly. It is designed to be called indirectly by Text () front macro.
*   REMARK: This should work as expected with ANY font.
*   Usage: Formats string due to escape sequences
*   Prepend subscript with underscore "_", longer subscripts enclose in curly paranthesis _{j,i_2} and so on
*   works recursively
* SYNOPSIS
*/
#macro TextExt( _ps, _hc, _vc, _sc)
// string to parse is stored in a global array, see INPUTS section
// character - any UTF character
// Text = [["" | "{" | "#" | character ] | ... ]
//  # command prefix
// ## - '#' itself
// #v - make FOLLOWING character or characters enclosed in curly brackets {} a vector ex. #vF - Force vector
// `  - /left apostrophe/ - accented letter (polish 'π' for example)
// {}
// ^
// _
// |   - short space
// " " - regular space

/*
 * INPUTS
 *  _ps  - string to parse is stored in global array _ArrNap[], returns (by ref) position of next char, to  parse
 *  _hc  - horizontal position of cursor
 *  _vc  - vertical position of cursor
 *  _sc  - local size modifier. Final size of text should be set in scene file (presumedly by scale<> statement ).
 * SEE ALSO
 *  UnicodifyPL ()
 *  Text ()
 ****/
object{
  union{
    #if (Debug>__Stable) #debug concat( "Entering Text() macro at level =", str( __Cnt,3,0), "\n") #end
    #local _Pos  = _ps;
    #declare __Cnt = __Cnt+1; // increment recurrence counter
    #if (Debug>__Stable) #debug concat( "Now parsing Text() macro at level =", str( __Cnt,3,0), "\n") #end
    #while (_Pos <= _TLn)  // no longer than length of string - string is placed from _ps=1.._TLn
       #if (__Cnt<8)
         #local __tc = __Cnt;
       #else
         #local __tc = 7;
       #end
       #if (Debug>__Stable) #debug concat(__Prf[__tc], " Pos = " str( _Pos,3,0) "  "   _ArNap[_Pos], " \n") #end
       #local _Chr=_ArNap[_Pos];
// =================================================================
       #if ((_Chr="{") & (_Pos<_TLn)) // just move to next character & continue
          #local _Pos = _Pos + 1;
          #if (__Cnt > 1)
               TextExt(_Pos, _hc, _vc, _sc) // rub your tommy
          #end
// =================================================================
       #elseif (_Chr="}") // reset and go ahead
          #if (Debug>__Init) #debug concat( __Prf[__Cnt], " __Cnt= ", str(__Cnt,4,0), " end of {} block met\n") #end
          #if (__Cnt > 1)
              #break
          #else
             #local _Pos = _Pos + 1;
          #end  // end of block in index otherwise just continue with next char
// =================================================================
       #elseif ((_Chr="#") & (_Pos<_TLn)) // ... & next character exist! There is command prefix, command id passed as next character, some commands are longer
          #local _Pos = _Pos + 1;         //  next character is a command or special character itself
          #local _Chr = _ArNap[_Pos];   // then inspect next character
          // ==========================
          // parsing command - CASE is ImPortanT!
          // #o - insert overlaid indexes as shown below.
          // Command should have following structure: #o{Upper}{Lower} <- [[character | block_of_characters][character | block_of_characters]]
          //
          //  2    Upper      Upper
          // X    X       OR       X
          //  i    Lower      Lower
          // ==========================
          #if ((_Chr="o")  & (_Pos<_TLn)) // ... & next character exist!
              // ======================
              #local _Pos = _Pos + 1;
              #local _c = _ArNap[_Pos];  // get next character & check if = '#' if not - continue
              #local _Uh = _hc;
              // upper index: read one or more until right '}' recursively
              #if (Debug>__Init) #debug concat(__Prf[__Cnt], "command #o: upper" str(_Pos, 4, 0), "\n") #end
              #if (_c != "{") // OK, it is single upper index
                  #local Upper = object {InsrtChr(_c, _Uh, _vc+_RFac*UpPos*_sc, _RFac*_sc)};
              #else
                  #local _Pos = _Pos + 1;
                  #local Upper = object {TextExt(_Pos, _Uh, _vc+_RFac*UpPos*_sc, _RFac*_sc)}; //  {block} - so go recursive
              #end
              #if (Debug>__Init) #debug concat(__Prf[__Cnt], "command #o: upper" str(_Pos, 4, 0), "\n") #end
              // created upper index as (plain as of now) text object

//========================================================
              #local _Pos = _Pos + 1;
              #local _c = _ArNap[_Pos];  // get next character & check if = '#' if not - continue
              #local _Lh = _hc;
              // lower index: read one or more until right '}' recursively
              #if (Debug>__Init) #debug concat(__Prf[__Cnt], "command #o: lower" str(_Pos, 4, 0), "\n") #end
              #if (_c != "{") // OK, it is single lower index
                  #local Lower = object {InsrtChr(_c, _Lh, _vc+_RFac*DnPos*_sc, _RFac*_sc)};
              #else
                  #local _Pos = _Pos + 1;
                  #local Lower = object {TextExt(_Pos, _Lh, _vc+_RFac*DnPos*_sc, _RFac*_sc)}; //  {block} - so go recursive
              #end
              #if (Debug>__Init) #debug concat(__Prf[__Cnt], "command #o: lower" str(_Pos, 4, 0), "\n") #end
              // created lower index as (plain as of now) text object

//========================================================
              object {Upper}
              object {Lower}
              #local _hc = max(_Uh, _Lh) + Text_Space(_FF, "|", _RFac*_sc, <0,0,0>);   // Additional adjustement update _HCur position

              #local _Pos = _Pos + 1;
          #elseif (_Chr="v")  // command: #v just "vectorizes" next symbol
              #local _Pos = _Pos + 1; // not implemented yet - just go next
          #else // just inserts unrecognized character, also '#' or '|'

              InsrtChr(_Chr, _hc, _vc, _sc)
              #local _Pos = _Pos + 1;
          #end
// ===[ subscripts ]==============================================================
       #elseif ((_Chr="_") & (_Pos<_TLn)) // ... & next character exist!
          #local _Pos = _Pos + 1;
          #local _Chr=_ArNap[_Pos]; // then inspect next character
          #if (_Chr="{")  // oh, there is {..} segment, go recursive
              #if (Debug>__Init) #debug concat( __Prf[__Cnt], " __Cnt= ", str(__Cnt,4,0), " {_} block begin\n")    #end
              TextExt(_Pos, _hc, _vc+_RFac*DnPos*_sc, _RFac*_sc) // single char. or {block}
              #if (Debug>__Init) #debug concat( __Prf[__Cnt], " __Cnt= ", str(__Cnt,4,0), " {_} block finished\n") #end
          #else  // insert single character as subscript
              InsrtChr(_Chr, _hc, _vc+_RFac*DnPos*_sc, _RFac*_sc)
              #local _Pos = _Pos + 1;
          #end
// ===[ superscripts ]==============================================================
       #elseif ((_Chr="^") & (_Pos<_TLn))   // ... & next character exist!
          #local _Pos = _Pos + 1;
          #local _Chr = _ArNap[_Pos]; // then inspect next character
          #if (_Chr="{")  // oh, there is {..} segment, go recursive
              #if (Debug>__Init) #debug concat( __Prf[__Cnt], " __Cnt= ", str(__Cnt,4,0), " {^} block begin\n")    #end
              TextExt(_Pos, _hc, _vc+_RFac*UpPos*_sc, _RFac*_sc) // single char. or {block}
              #if (Debug>__Init) #debug concat( __Prf[__Cnt], " __Cnt= ", str(__Cnt,4,0), " {^} block finished\n") #end
          #else  // insert single character as subscript
              InsrtChr(_Chr, _hc, _vc+_RFac*UpPos*_sc, _RFac*_sc)
              #local _Pos = _Pos + 1;
          #end
// ===[ real & important characters ]==============================================================
       #elseif (_Chr=" ")
          #local _hc = _hc + Text_Width(_FF, SpcStub, _sc, 0);
          #local _Pos = _Pos + 1;
// =================================================================
       #elseif (_Chr=".") // dot replacement
          #local   _DSpc  = _DotScalingFactor*Text_Width(_FF, ".", _sc, <0,0,0>); // arbitrary adjustment
          superellipsoid{ <1, 0.3> // full stop
                        scale<_DSpc, _DSpc, _Dp/2>
                        translate < _hc + 0.25*_DSpc, _vc, _Dp/2>
          }
          #local _Pos = _Pos + 1;
          #local _hc = _hc + 3*_DSpc;

// =================================================================
       #elseif (_Chr="|") // short space
          #local _DSpc  = Text_Width(_FF, "|", _sc, <0,0,0>); //
              #local _Pos = _Pos + 1;
              #local _hc = _hc + _DSpc+ _DSpc;
// =================================================================
       #else              // real insertion of character(s)
          InsrtChr(_Chr, _hc, _vc, _sc)
          #local _Pos = _Pos + 1;
       #end  // main #if statement
    #end   // #while()
  }
  #declare __Cnt = __Cnt-1; // Finally decrease recurrence counter
  #local _ps  = _Pos;

}
#end // #macro  ()



#end // #ifndef

