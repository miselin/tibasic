/*
 * Copyright (c) 2011 Matthew Iselin
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <map>
#include <string>

#include <string.h>

#include "tibasic.h"

using namespace std;

/// Describes a potential token to be read by the compiler
struct Token {
    /// The compiled byte for the token
	unsigned char data;

    /// The actual text to be converted by the interpreter
	const char* text;
};

/// A two byte token (0xBB, 0x7E and SysVar)
struct TwoByte {
	unsigned short data;
	const char* text;
};

/// Direct ASCII character to token conversion.
struct ConvertRule {
	char c;				// the character
	unsigned char tok;	// the equivalent token
};

/// References to lists defined after functions.
extern struct Token StandardTokens[199];
extern struct TwoByte CalcVars[302];
extern struct ConvertRule Replacements[39];

/// string -> token mapping
map<string, token_t> g_TokenLookup;

/// token -> string mapping
map<unsigned short, string> g_ReverseLookup;

/// Longest input string possible
size_t g_LongestInput = 0;

/// Shiny little template function that returns the size of an array.
template <typename T, int N> size_t arrayLen(T(&)[N]){return N;}

/// Initialises the token map
void initialiseTokens()
{
    // Iterate the main token list first.
    for(size_t i = 0; i < arrayLen(StandardTokens); i++)
    {
        token_t value;
        value.token = StandardTokens[i].data;
        value.sz = sizeof(StandardTokens[i].data);

        size_t len = strlen(StandardTokens[i].text);
        if(len > g_LongestInput)
            g_LongestInput = len;

        string s = string(StandardTokens[i].text);
        g_TokenLookup[s] = value;
        g_ReverseLookup[value.token] = s;
    }

    // Now iterate the two-byte tokens.
    for(size_t i = 0; i < (sizeof(CalcVars) / sizeof(Token)); i++)
    {
        token_t value;
        value.token = CalcVars[i].data;
        value.sz = sizeof(CalcVars[i].data);

        size_t len = strlen(CalcVars[i].text);
        if(len > g_LongestInput)
            g_LongestInput = len;
        
        string s = string(CalcVars[i].text);
        g_TokenLookup[s] = value;
        g_ReverseLookup[value.token] = s;
    }

    // Finally, iterate single-character tokens.
    for(size_t i = 0; i < (sizeof(Replacements) / sizeof(ConvertRule)); i++)
    {
        token_t value;
        value.token = Replacements[i].tok;
        value.sz = sizeof(Replacements[i].tok);

        char c[] = {Replacements[i].c, 0};
        string s = c;

        g_TokenLookup[s] = value;
        g_ReverseLookup[value.token] = s;
    }
}

size_t getLongestToken()
{
    return g_LongestInput;
}

/// Perform a lookup
bool lookupToken(string in, token_t &ret)
{
    if(in.length() > g_LongestInput)
        return false;

    if(g_TokenLookup.find(in) == g_TokenLookup.end())
        return false;

    ret = g_TokenLookup[in];

    return true;
}

bool lookupToken(unsigned short in, string &out)
{
    if(g_ReverseLookup.find(in) == g_ReverseLookup.end())
        return false;

    out = g_ReverseLookup[in];

    return true;
}

// Token List
#define TO_DMS			0x01
#define TO_DEC			0x02
#define TO_FRAC			0x03
#define STORE			0x04
#define BOXPLOT			0x05
#define LEFTSBRACK		0x06
#define RIGHTSBRACK		0x07
#define LEFTBRACE		0x08
#define RIGHTBRACE		0x09
#define RADIANS			0x0A
#define DEGREES			0x0B
#define INVERSE			0x0C
#define SQUARE			0x0D
#define TRANSPOSE		0x0E
#define CUBE			0x0F
#define LEFTBRACKET		0x10
#define RIGHTBRACKET	0x11
#define ROUND			0x12
#define PXLTEST			0x13
#define AUGMENT			0x14
#define ROWSWAP			0x15
#define ROWPLUS			0x16
#define STARROW			0x17
#define STARROWPLUS		0x18
#define MAX				0x19
#define MIN				0x1B
#define MEDIAN			0x1F
#define RANDM			0x20
#define MEAN			0x21
#define SOLVE			0x22
#define SEQFUNC			0x23
#define FNINT			0x24
#define NDERIV			0x25
#define FMIN			0x27
#define FMAX			0x28
#define SPACE			0x29
#define DOUBLEQUOTE		0x2A
#define COMMA			0x2B
#define IMAG_I			0x2C
#define FACTORIAL		0x2D
#define CUBICREG		0x2E
#define QUARTREG		0x2F
#define NUM_0			0x30
#define NUM_1			0x31
#define NUM_2			0x32
#define NUM_3			0x33
#define NUM_4			0x34
#define NUM_5			0x35
#define NUM_6			0x36
#define NUM_7			0x37
#define NUM_8			0x38
#define NUM_9			0x39
#define PERIOD			0x3A
#define EXPONENT		0x3B
#define LOGIC_OR		0x3C
#define LOGIC_XOR		0x3D
#define COLON			0x3E
#define HARD_RETURN		0x3F
#define LOGIC_AND		0x40
#define STR_A			0x41
#define STR_B			0x42
#define STR_C			0x43
#define STR_D			0x44
#define STR_E			0x45
#define STR_F			0x46
#define STR_G			0x47
#define STR_H			0x48
#define STR_I			0x49
#define STR_J			0x4A
#define STR_K			0x4B
#define STR_L			0x4C
#define STR_M			0x4D
#define STR_N			0x4E
#define STR_O			0x4F
#define STR_P			0x50
#define STR_Q			0x51
#define STR_R			0x52
#define STR_S			0x53
#define STR_T			0x54
#define STR_U			0x55
#define STR_V			0x56
#define STR_W			0x57
#define STR_X			0x58
#define STR_Y			0x59
#define STR_Z			0x5A
#define STR_THETA		0x5B
#define PROGRAM			0x5F

/** SysVar **/
#define RADIAN			0x64
#define DEGREE			0x65
#define NORMAL			0x66
#define SCI				0x67
#define ENG				0x68
#define FLOAT			0x69
#define TEST_EQUAL		0x6A
#define TEST_LESSTHAN	0x6B
#define TEST_HIGHTHAN	0x6C
#define TEST_LOREQU		0x6D
#define TEST_GOREQU		0x6E
#define TEST_NOTEQUAL	0x6F
#define ADD				0x70
#define SUBTRACT		0x71
#define ANSWER			0x72
#define FIX				0x73
#define HORIZ			0x74
#define FULL			0x75
#define FUNC			0x76
#define PARAM			0x77
#define POLAR			0x78
#define SEQ				0x79
#define INDPNTAUTO		0x7A
#define INDPNTASK		0x7B
#define DEPENDAUTO		0x7C
#define DEPENDASK		0x7D

/** 7E VARIABLES, ie. graph manipulation **/
#define BOX				0x7F
#define DIVIDE			0x80
#define DOT				0x81
#define MULTIPLY		0x82
#define DIVIDE_SLASH	0x83
#define TRACE			0x84
#define CLRDRAW			0x85
#define ZSTANDARD		0x86
#define ZTRIG			0x87
#define ZBOX			0x88
#define ZOOMIN			0x89
#define ZOOMOUT			0x8A
#define ZSQUARE			0x8B
#define ZINTEGER		0x8C
#define ZPREVIOUS		0x8D
#define ZDECIMAL		0x8E
#define ZOOMSTAT		0x8F
#define ZOOMRCL			0x90
#define PRINTSCREEN		0x91
#define ZOOMSTO			0x92
#define TEXTFUNC		0x93
#define NPR				0x94
#define NCR				0x95
#define FNON			0x96
#define FNOFF			0x97
#define STOREPIC		0x98
#define RECALLPIC		0x99
#define STOREGDB		0x9A
#define RECALLGDB		0x9B
#define LINE			0x9C
#define VERTICAL		0x9D
#define PTON			0x9E
#define PTOFF			0x9F
#define PTCHANGE		0xA0
#define PXLON			0xA1
#define PXLOFF			0xA2
#define PXLCHANGE		0xA3
#define SHADE			0xA4
#define CIRCLE			0xA5
#define HORIZONTAL		0xA6
#define TANGENT			0xA7
#define DRAWINV			0xA8
#define DRAWF			0xA9
#define RAND			0xAB
#define PI				0xAC
#define GETKEY			0xAD
#define APOSTROPHE		0xAE
#define QUESTIONMARK	0xAF

/** SysVar **/
#define NEGATIVE		0xB0
#define CONV_INT		0xB1
#define ABS				0xB2
#define DETERMINANT		0xB3
#define IDENTITY		0xB4
#define DIM				0xB5
#define SUM				0xB6
#define PROD			0xB7
#define NOT				0xB8
#define IPART			0xB9
#define FPART			0xBA

/** BB tokens **/
#define SQR_ROOT		0xBC
#define CUBE_ROOT		0xBD
#define NATLOG			0xBE
#define ETOPOWER		0xBF
#define LOGARITHM		0xC0
#define POWER10			0xC1
#define SINE			0xC2
#define INVSIN			0xC3
#define COSINE			0xC4
#define INVCOSINE		0xC5
#define TANGENT_TRIG	0xC6
#define INVTANGENT		0xC7
#define HYP_SINE		0xC8
#define HYP_ISINE		0xC9
#define HYP_COSINE		0xCA
#define HYP_ICOSINE		0xCB
#define HYP_TANGENT		0xCC
#define HYP_ITANGENT	0xCD
#define LOGIC_IF		0xCE
#define LOGIC_THEN		0xCF
#define LOGIC_ELSE		0xD0
#define CTL_WHILE		0xD1
#define CTL_REPEAT		0xD2
#define CTL_FOR			0xD3
#define CTL_END			0xD4
#define CTL_RETURN		0xD5
#define LABEL			0xD6
#define CTL_GOTO		0xD7
#define CTL_PAUSE		0xD8
#define CTL_STOP		0xD9
#define INCSKIPIFHIGH	0xDA
#define DECSKIPIFLOW	0xDB
#define INPUT			0xDC
#define PROMPT			0xDD
#define DISP			0xDE
#define DISPGRAPH		0xDF
#define OUTPUT			0xE0
#define CLRHOME			0xE1
#define FILL			0xE2
#define SORTA			0xE3
#define SORTD			0xE4
#define DISPTABLE		0xE5
#define MENU			0xE6
#define SEND			0xE7
#define GET				0xE8
#define PLOTSON			0xE9
#define PLOTSOFF		0xEA
#define LIST			0xEB
#define PLOT1			0xEC
#define PLOT2			0xED
#define PLOT3			0xEE
#define POWEROF			0xF0
#define XTHROOT			0xF1
#define VARSTATS_1		0xF2
#define VARSTATS_2		0xF3
#define LINREG1			0xF4
#define EXPREG			0xF5
#define LNREG			0xF6
#define PWRREG			0xF7
#define MEDMED			0xF8
#define QUADREG			0xF9
#define CLRLIST			0xFA
#define CLRTABLE		0xFB
#define HISTOGRAM		0xFC
#define XYLINE			0xFD
#define SCATTER			0xFE
#define LINREG2			0xFF

/** SYSTEM VARIABLES **/

// Matrices
#define MAT_A			0x005C
#define MAT_B			0x015C
#define MAT_C			0x025C
#define MAT_D			0x035C
#define MAT_E			0x045C
#define MAT_F			0x055C
#define MAT_G			0x065C
#define MAT_H			0x075C
#define MAT_I			0x085C
#define MAT_J			0x095C

// Lists
#define L1				0x005D
#define L2				0x015D
#define L3				0x025D
#define L4				0x035D
#define L5				0x045D
#define L6				0x055D
#define L7				0x065D
#define L8				0x075D
#define L9				0x085D
#define L0				0x095D

// Graph (function)
#define Y1				0x105E
#define Y2				0x115E
#define Y3				0x125E
#define Y4				0x135E
#define Y5				0x145E
#define Y6				0x155E
#define Y7				0x165E
#define Y8				0x175E
#define Y9				0x185E
#define Y0				0x195E

// Graph (parametric)
#define X1T				0x205E
#define Y1T				0x215E
#define X2T				0x225E
#define Y2T				0x235E
#define X3T				0x245E
#define Y3T				0x255E
#define X4T				0x265E
#define Y4T				0x275E
#define X5T				0x285E
#define Y5T				0x295E
#define X6T				0x2A5E
#define Y6T				0x2B5E

// Graph (polar)
#define R1				0x405E
#define R2				0x415E
#define R3				0x425E
#define R4				0x435E
#define R5				0x445E
#define R6				0x455E
#define SYSVAR_U		0x805E
#define SYSVAR_V		0x815E

// Pictures
#define PIC1			0x0060
#define PIC2			0x0160
#define PIC3			0x0260
#define PIC4			0x0360
#define PIC5			0x0460
#define PIC6			0x0560
#define PIC7			0x0660
#define PIC8			0x0760
#define PIC9			0x0860
#define PIC0			0x0960

// Graph databases
#define GDB1			0x0061
#define GDB2			0x0161
#define GDB3			0x0261
#define GDB4			0x0361
#define GDB5			0x0461
#define GDB6			0x0561
#define GDB7			0x0661
#define GDB8			0x0761
#define GDB9			0x0861
#define GDB0			0x0961

// Stat data
#define REGEQ			0x0162
#define STAT_N			0x0262
#define MEANX			0x0362
#define SUMX			0x0462
#define SUMXSQUARED		0x0562
#define SX				0x0662
#define SIGMAX			0x0762
#define MINX			0x0862
#define MAXX			0x0962
#define MINY			0x0A62
#define MAXY			0x0B02
#define MEANY			0x0C62
#define SUMY			0x0D62
#define SUMYSQUARED		0x0E62
#define SY				0x0F62
#define SIGMAY			0x1062
#define SUMXY			0x1162
#define SYSVAR_R		0x1262
#define MED				0x1362
#define Q1				0x1462
#define Q3				0x1562
#define SYSVAR_A		0x1662
#define SYSVAR_B		0x1762
#define SYSVAR_C		0x1862
#define SYSVAR_D		0x1962
#define SYSVAR_E		0x1A62
#define X1				0x1B62
#define X2				0x1C62
#define X3				0x1D62
#define Y1_1			0x1E62
#define Y2_1			0x1F62
#define Y3_1			0x2062
#define SYSVAR_N		0x2162
#define SYSVAR_P		0x2262
#define SYSVAR_Z		0x2362
#define SYSVAR_T		0x2462
#define CHISQUARED		0x2562
#define FIN				0x2662
#define DF				0x2762
#define PHAT			0x2862
#define PHAT1			0x2962
#define PHAT2			0x2A62
#define MEANX1			0x2B62
#define SX1				0x2C62
#define N1				0x2D62
#define MEANX2			0x2E62
#define SX2				0x2F62
#define N2				0x3062
#define SXP				0x3162
#define LOWER			0x3262
#define UPPER			0x3362
#define SYSVAR_S		0x3462
#define RSQUARED		0x3562
#define CAPRSQUARED		0x3662
#define DF2				0x3762 // not sure about this one
#define SS				0x3862
#define MS				0x3962
#define DF3				0x3A62 // again here?
#define SS1				0x3B62 // another double
#define MS1				0x3C62 //   "       "

// Graph data
#define ZXSCL			0x0063
#define ZYSCL			0x0163
#define XSCL			0x0263
#define YSCL			0x0363
#define UNSTART			0x0463
#define VNSTART			0x0563
#define UNINVERSE		0x0663
#define VNINVERSE		0x0763
#define ZUNSTART		0x0863
#define ZVNSTART		0x0963
#define XMIN			0x0A63
#define XMAX			0x0B63
#define YMIN			0x0C63
#define YMAX			0x0D63
#define TMIN			0x0E63
#define TMAX			0x0F63
#define THETAMIN		0x1063
#define THETAMAX		0x1163
#define ZXMIN			0x1263
#define ZXMAX			0x1363
#define ZYMIN			0x1463
#define ZYMAX			0x1563
#define ZTHETAMIN		0x1663
#define ZTHETAMAX		0x1763
#define ZTMIN			0x1863
#define ZTMAX			0x1963
#define TBLMIN			0x1A63
#define NMIN			0x1B63
#define ZNMIN			0x1C63
#define NMAX			0x1D63
#define ZNMAX			0x1E63
#define NSTART			0x1F63
#define ZNSTART			0x2063
#define DELTATABLE		0x2163
#define TSTEP			0x2263
#define THETASTEP		0x2363
#define ZTSTEP			0x2463
#define ZTHETASTEP		0x2563
#define DELTAX			0x2663
#define DELTAY			0x2763
#define XFACT			0x2863
#define YFACT			0x2963
#define TBLINPUT		0x2A63
#define SYSVAR_CAPN		0x2B63
#define IPERCENT		0x2C63
#define PV				0x2D63
#define PMT				0x2E63
#define FV				0x2F63
#define XRES			0x3063
#define ZXRES			0x3163

// Strings
#define STR1			0x00AA
#define STR2			0x01AA
#define STR3			0x02AA
#define STR4			0x03AA
#define STR5			0x04AA
#define STR6			0x05AA
#define STR7			0x06AA
#define STR8			0x07AA
#define STR9			0x08AA
#define STR0			0x09AA

/** 7E VARIABLES **/
#define SEQUENTIAL_7E	0x007E
#define SIMUL			0x017E
#define POLARGC			0x027E
#define RECTGC			0x037E
#define COORDON			0x047E
#define COORDOFF		0x057E
#define CONNECTED		0x067E
#define DOT_7E			0x077E
#define AXESON			0x087E
#define AXESOFF			0x097E
#define GRIDON			0x0A7E
#define GRIDOFF			0x0B7E
#define LABELON			0x0C7E
#define LABELOFF		0x0D7E
#define WEB				0x0E7E
#define TIME			0x0F7E
#define UVAXES			0x107E
#define VWAXES			0x117E
#define UWAXES			0x127E

/** BB VARIABLES **/
#define NPV				0x00BB
#define IRR				0x01BB
#define BAL				0x02BB
#define SUMPRN			0x03BB
#define SUMINT			0x04BB
#define TONOM			0x05BB
#define TOEFF			0x06BB
#define DBD				0x07BB
#define LCM				0x08BB
#define GCD				0x09BB
#define RANDINT			0x0ABB
#define RANDBIN			0x0BBB
#define SUB				0x0CBB
#define STDDEV			0x0DBB
#define VARIANCE		0x0EBB
#define INSTRING		0x0FBB
#define NORMALCDF		0x10BB
#define INVNORM			0x11BB
#define TCDF			0x12BB
#define XSQUCDF			0x13BB
#define FCDF			0x14BB
#define BINOMPDF		0x15BB
#define BINOMCDF		0x16BB
#define POISSONPDF		0x17BB
#define POISSONCDF		0x18BB
#define GEOMETPDF		0x19BB
#define GEOMETCDF		0x1ABB
#define NORMALPDF		0x1BBB
#define TPDF			0x1CBB
#define XSQUPDF			0x1DBB
#define FPDF			0x1EBB
#define RANDNORM		0x1FBB
#define TVM_PMT			0x20BB
#define TVM_I_PERCENT	0x21BB
#define TVM_PV			0x22BB
#define TVM_N			0x23BB
#define TVM_FV			0x24BB
#define CONJ			0x25BB
#define REAL			0x26BB
#define IMAG			0x27BB
#define ANGLE			0x28BB
#define CUMSUM			0x29BB
#define EXPR			0x2ABB
#define LENGTH			0x2BBB
#define DELTA_LIST		0x2CBB
#define REF				0x2DBB
#define RREF			0x2EBB
#define TORECT			0x2FBB
#define TOPOLAR			0x30BB
#define VAR_E			0x31BB
#define SINREG			0x32BB
#define LOGISTIC		0x33BB
#define LINREGTTEST		0x34BB
#define SHADENORM		0x35BB
#define SHADE_T			0x36BB
#define SHADEXSQU		0x37BB
#define SHADEF			0x38BB
#define MATRTOLIST		0x39BB
#define LISTTOMATR		0x3ABB
#define ZTEST			0x3BBB
#define TTEST			0x3CBB
#define TWO_SAMPZTEST	0x3DBB
#define ONE_PROPZTEST	0x3EBB
#define TWO_PROPZTEST	0x3FBB
#define XSQUTEST		0x40BB
#define ZINTERVAL		0x41BB
#define TWO_SAMPZINT	0x42BB
#define ONE_PROPZINT	0x43BB
#define TWO_PROPZINT	0x44BB
#define GRAPHSTYLE		0x45BB
#define TWO_SAMPTTEST	0x46BB
#define TWO_SAMPFTEST	0x47BB
#define TINTERVAL		0x48BB
#define TWO_SAMPTINT	0x49BB
#define SETUPEDITOR		0x4ABB
#define PMT_END			0x4BBB
#define PMT_BGN			0x4CBB
#define REAL_BB			0x4DBB
#define REPOWTHETAI		0x4EBB
#define APLUSBI			0x4FBB
#define EXPRON			0x50BB
#define EXPROFF			0x51BB
#define CLRALLLISTS		0x52BB
#define GETCALC			0x53BB
#define DELVAR			0x54BB
#define EQUTOSTRING		0x55BB
#define STRINGTOEQU		0x56BB
#define CLEARENTRIES	0x57BB
#define SELECT			0x58BB
#define ANOVA			0x59BB
#define MODBOXPLOT		0x5ABB
#define NORMPROBPLOT	0x5BBB

// Standard Tokens are any token that can be used anywhere in the line.
struct Token StandardTokens[] = {
	/** CONTROL page of PROGRAM EDITOR (press PRGM when EDITING a program) **/
	{ LOGIC_IF,			"If "		},
	{ LOGIC_THEN,		"Then"		},
	{ LOGIC_ELSE,		"Else"		},
	{ CTL_FOR,			"For("		},
	{ CTL_WHILE,		"While "	},
	{ CTL_REPEAT,		"Repeat "	},
	{ CTL_END,			"End"		},
	{ CTL_PAUSE,		"Pause "	},
	{ CTL_PAUSE,		"Pause"		},
	{ LABEL,			"Label "	},
	{ CTL_GOTO,			"Goto "		},
	{ INCSKIPIFHIGH,	"IS>("		},
	{ DECSKIPIFLOW,		"DS<("		},
	{ MENU,				"Menu("		},
	{ PROGRAM,			"prgm"		},
	{ CTL_RETURN,		"Return"	},
	{ CTL_STOP,			"Stop"		},

	/** I/O page **/
	{ INPUT,			"Input "	},
	{ PROMPT,			"Prompt "	},
	{ DISP,				"Disp "		},
	{ DISPGRAPH,		"DispGraph"	},
	{ DISPTABLE,		"DispTable"	},
	{ OUTPUT,			"Output("	},
	{ GETKEY,			"getKey"	},
	{ CLRHOME,			"ClrHome"	},
	{ CLRTABLE,			"ClrTable"	},

	/** GetCalc **/
	{ GET,				"Get("		},
	{ SEND,				"Send("		},

    /** Plots **/
    { PLOTSON,          "PlotsOn "  },
    { PLOTSOFF,         "PlotsOff " },

	/** Others **/
	{ TRACE,			"Trace"		},
	{ ZSTANDARD,		"ZStandard"	},
	{ ZTRIG,			"ZTrig"		},
	{ ZBOX,				"ZBox"		},
	{ ZOOMIN,			"ZoomIn"	},
	{ ZOOMOUT,			"ZoomOut"	},
	{ ZSQUARE,			"ZSquare"	},
	{ ZINTEGER,			"ZInteger"	},
	{ ZPREVIOUS,		"ZPrevious"	},
	{ ZDECIMAL,			"ZDecimal"	},
	{ ZOOMSTAT,			"ZoomStat"	},
	{ ZOOMRCL,			"ZoomRcl"	},
	{ ZOOMSTO,			"ZoomSto"	},
	{ TEXTFUNC,			"Text("		},
	{ STOREPIC,			"StorePic"	},
	{ RECALLPIC,		"RecallPic"	},
	{ STOREGDB,			"StoreGDB"	},

	/** OTHERS **/
	{ TO_DMS,			"->DMS"			},
	{ TO_DEC,			"->DEC"			},
	{ TO_FRAC,			"->FRAC"		},
	{ STORE,			"->"			},
	{ BOXPLOT,			"BoxPlot"		},
	{ RADIANS,			"[radians]"		},
	{ DEGREES,			"[degrees]"		},
	{ INVERSE,			"^-1"			},
	{ SQUARE,			"^2"			},
	{ TRANSPOSE,		"[transpose]"	},
	{ CUBE,				"^3"			},
	{ ROUND,			"round("		},
	{ PXLTEST,			"pxl-Test("		},
	{ AUGMENT,			"augment("		},
	{ ROWSWAP,			"RowSwap("		},
	{ ROWPLUS,			"row+("			},
	{ STARROW,			"*row("			},
	{ STARROWPLUS,		"*row+("		},
	{ MAX,				"max("			},
	{ MIN,				"min("			},
	{ MEDIAN,			"median("		},
	{ RANDM,			"randM("		},
	{ MEAN,				"mean("			},
	{ SOLVE,			"solve("		},
	{ SEQFUNC,			"seq("			},
	{ FNINT,			"fnInt("		},
	{ NDERIV,			"NDeriv("		},
	{ FMIN,				"FMin("			},
	{ FMAX,				"FMax("			},
	{ CUBICREG,			"CubicReg "		},
	{ QUARTREG,			"QuartReg "		},
	{ LOGIC_OR,			" or "			},
	{ LOGIC_XOR,		" xor "			},
	{ LOGIC_AND,		"and"			},
	{ STR_THETA,		"[theta]"		},
	{ PROGRAM,			"prgm"			},
	{ RADIAN,			"Radian"		},
	{ DEGREE,			"Degree"		},
	{ NORMAL,			"Normal"		},
	{ SCI,				"Sci"			},
	{ ENG,				"Eng"			},
	{ FLOAT,			"Float"			},
	{ TEST_LOREQU,		"<="			},
	{ TEST_GOREQU,		">="			},
	{ TEST_NOTEQUAL,	"!="			},
	{ ANSWER,			"Ans"			},
	{ FIX,				"Fix "			},
	{ HORIZ,			"Horiz"			},
	{ FULL,				"Full"			},
	{ FUNC,				"Func"			},
	{ PARAM,			"Param"			},
	{ POLAR,			"Polar"			},
	{ SEQ,				"Seq"			},
	{ INDPNTAUTO,		"IndpntAuto"	},
	{ INDPNTASK,		"IndpntAsk"		},
	{ DEPENDAUTO,		"DependAuto"	},
	{ DEPENDASK,		"DependAsk"		},
	{ BOX,				"[box]"			},
	{ DOT,				"[dot]"			},
	{ TRACE,			"Trace"			},
	{ CLRDRAW,			"ClrDraw"		},
	{ ZSTANDARD,		"ZStandard"		},
	{ ZTRIG,			"ZTrig"			},
	{ ZBOX,				"ZBox"			},
	{ ZOOMIN,			"Zoom In"		},
	{ ZOOMOUT,			"Zoom Out"		},
	{ ZSQUARE,			"ZSquare"		},
	{ ZINTEGER,			"ZInteger"		},
	{ ZPREVIOUS,		"ZPrevious"		},
	{ ZDECIMAL,			"ZDecimal"		},
	{ ZOOMSTAT,			"ZoomStat"		},
	{ ZOOMRCL,			"ZoomRcl"		},
	{ PRINTSCREEN,		"PrintScreen"	},
	{ ZOOMSTO,			"ZoomSto"		},
	{ TEXTFUNC,			"Text("			},
	{ NPR,				"nPr"			},
	{ NCR,				"nCr"			},
	{ FNON,				"FnOn "			},
	{ FNOFF,			"FnOff "		},
	{ STOREPIC,			"StorePic "		},
	{ RECALLPIC,		"RecallPic "	},
	{ STOREGDB,			"StoreGDB "		},
	{ RECALLGDB,		"RecallGDB "	},
	{ LINE,				"Line("			},
	{ VERTICAL,			"Vertical "		},
	{ PTON,				"Pt-On("		},
	{ PTOFF,			"Pt-Off("		},
	{ PTCHANGE,			"Pt-Change("	},
	{ PXLON,			"Pxl-On("		},
	{ PXLOFF,			"Pxl-Off("		},
	{ PXLCHANGE,		"Pxl-Change("	},
	{ SHADE,			"Shade("		},
	{ CIRCLE,			"Circle("		},
	{ HORIZONTAL,		"Horizontal "	},
	{ TANGENT,			"Tangent("		},
	{ DRAWINV,			"DrawInv "		},
	{ DRAWF,			"DrawF "		},
	{ PI,				"[pi]"			},
	{ GETKEY,			"getKey"		},
	{ NEGATIVE,			"[neg]"			},
	{ CONV_INT,			"int("			},
	{ ABS,				"abs("			},
	{ DETERMINANT,		"det("			},
	{ IDENTITY,			"identity("		},
	{ DIM,				"dim("			},
	{ SUM,				"sum("			},
	{ PROD,				"prod("			},
	{ NOT,				"not("			},
	{ IPART,			"iPart("		},
	{ FPART,			"fPart("		},
	{ SQR_ROOT,			"[root]^2"		},
	{ CUBE_ROOT,		"[root]^3"		},
	{ NATLOG,			"ln("			},
	{ ETOPOWER,			"e^"			},
	{ LOGARITHM,		"log("			},
	{ POWER10,			"10^("			},
	{ SINE,				"sin("			},
	{ INVSIN,			"asin("			},
	{ COSINE,			"cos("			},
	{ INVCOSINE,		"acos("			},
	{ TANGENT_TRIG,		"tan("			},
	{ INVTANGENT,		"atan("			},
	{ HYP_SINE,			"sinh("			},
	{ HYP_ISINE,		"asinh("		},
	{ HYP_COSINE,		"cosh("			},
	{ HYP_ICOSINE,		"acosh("		},
	{ HYP_TANGENT,		"tanh("			},
	{ HYP_ITANGENT,		"atanh("		},
	{ LOGIC_ELSE,		"Else"			},
	{ FILL,				"Fill("			},
	{ SORTA,			"SortA("		},
	{ SORTD,			"SortD("		},
	{ LIST,				"[list]"		},
	{ PLOT1,			"Plot1("		},
	{ PLOT2,			"Plot2("		},
	{ PLOT3,			"Plot3("		},
	{ XTHROOT,			"[root]^"		},
	{ VARSTATS_1,		"1-Var Stats "	},
	{ VARSTATS_2,		"2-Var Stats "	},
	{ LINREG1,			"LinReg(a+bx) "	},
	{ EXPREG,			"ExpReg "		},
	{ LNREG,			"LnReg "		},
	{ PWRREG,			"PwrReg "		},
	{ MEDMED,			"Med-Med "		},
	{ QUADREG,			"QuadReg "		},
	{ CLRLIST,			"ClrList "		},
	{ CLRTABLE,			"ClrTable"		},
	{ HISTOGRAM,		"Histogram"		},
	{ XYLINE,			"xyLine"		},
	{ SCATTER,			"Scatter"		},
	{ LINREG2,			"LinReg(ax+b) "	},
	{ RAND,				"rand"			},
};

// two-byte variables
struct TwoByte CalcVars[] = {
	// AsmPrgm (uncompiled)
	{ 0x6CBB,			"AsmPrgm"	},
	{ 0x6DBB,			"AsmPrgm"	}, // this means decompilation works, but compilation won't hit this

	// SysVar
	{ MAT_A,			"[A]"		},
	{ MAT_B,			"[B]"		},
	{ MAT_C,			"[C]"		},
	{ MAT_D,			"[D]"		},
	{ MAT_E,			"[E]"		},
	{ MAT_F,			"[F]"		},
	{ MAT_G,			"[G]"		},
	{ MAT_H,			"[H]"		},
	{ MAT_I,			"[I]"		},
	{ MAT_J,			"[J]"		},
	{ L1,				"L1"		},
	{ L2,				"L2"		},
	{ L3,				"L3"		},
	{ L4,				"L4"		},
	{ L5,				"L5"		},
	{ L6,				"L6"		},
	{ L7,				"L7"		},
	{ L8,				"L8"		},
	{ L9,				"L9"		},
	{ L0,				"L0"		},
	{ Y1,				"Y1"		},
	{ Y2,				"Y2"		},
	{ Y3,				"Y3"		},
	{ Y4,				"Y4"		},
	{ Y5,				"Y5"		},
	{ Y6,				"Y6"		},
	{ Y7,				"Y7"		},
	{ Y8,				"Y8"		},
	{ Y9,				"Y9"		},
	{ Y0,				"Y0"		},
	{ X1T,				"X1T"		},
	{ Y1T,				"Y1T"		},
	{ X2T,				"X2T"		},
	{ Y2T,				"Y2T"		},
	{ X3T,				"X3T"		},
	{ Y3T,				"Y3T"		},
	{ X4T,				"X4T"		},
	{ Y4T,				"Y4T"		},
	{ X5T,				"X5T"		},
	{ Y5T,				"Y5T"		},
	{ X6T,				"X6T"		},
	{ Y6T,				"Y6T"		},
	{ R1,				"R1"		},
	{ R2,				"R2"		},
	{ R3,				"R3"		},
	{ R4,				"R4"		},
	{ R5,				"R5"		},
	{ R6,				"R6"		},
	{ SYSVAR_U,			"[u]"		},
	{ SYSVAR_V,			"[v]"		},
	{ PIC1,				"PIC1"		},
	{ PIC2,				"PIC2"		},
	{ PIC3,				"PIC3"		},
	{ PIC4,				"PIC4"		},
	{ PIC5,				"PIC5"		},
	{ PIC6,				"PIC6"		},
	{ PIC7,				"PIC7"		},
	{ PIC8,				"PIC8"		},
	{ PIC9,				"PIC9"		},
	{ PIC0,				"PIC0"		},
	{ GDB1,				"GDB1"		},
	{ GDB2,				"GDB2"		},
	{ GDB3,				"GDB3"		},
	{ GDB4,				"GDB4"		},
	{ GDB5,				"GDB5"		},
	{ GDB6,				"GDB6"		},
	{ GDB7,				"GDB7"		},
	{ GDB8,				"GDB8"		},
	{ GDB9,				"GDB9"		},
	{ GDB0,				"GDB0"		},
	// finally, StatVars
	{ SX1,				"Sx1"		},
	{ SX2,				"Sx2"		},
	{ SXP,				"Sxp"		},
	{ REGEQ,			"RegEq"		},
	{ STAT_N,			"[n]"		},
	{ MEANX,			"mean(x)"	},
	{ SUMXSQUARED,		"sum(x)^2"	},
	{ SUMX,				"sum(x)"	},
	{ SX,				"Sx"		},
	{ SIGMAX,			"[sigma]x"	},
	{ MINX,				"minX"		},
	{ MAXX,				"maxX"		},
	{ MINY,				"minY"		},
	{ MAXY,				"maxY"		},
	{ MEANY,			"mean(y)"	},
	{ SUMYSQUARED,		"sum(y)^2"	},
	{ SUMY,				"sum(y)"	},
	{ SY,				"Sy"		},
	{ SIGMAY,			"[sigma]y"	},
	{ SUMXY,			"sum(xy)"	},
	{ SYSVAR_R,			"[r]"		},
	{ MED,				"Med"		},
	{ Q1,				"Q1"		},
	{ Q3,				"Q3"		},
	{ SYSVAR_A,			"[a]"		},
	{ SYSVAR_B,			"[b]"		},
	{ SYSVAR_C,			"[c]"		},
	{ SYSVAR_D,			"[d]"		},
	{ SYSVAR_E,			"[stat_e]"	}, // because '[e]' refers to the constant e
	{ X1,				"x1"		},
	{ X2,				"x2"		},
	{ X3,				"x3"		},
	{ Y1_1,				"y1"		},
	{ Y2_1,				"y2"		},
	{ Y3_1,				"y3"		},
	{ SYSVAR_N,			"[n]"		}, // somebody please tell me why there are so many variations on n
	{ SYSVAR_P,			"[p]"		},
	{ SYSVAR_Z,			"[z]"		},
	{ SYSVAR_T,			"[t]"		},
	{ CHISQUARED,		"[chi]^2"	},
	{ FIN,				"[fin]"		},
	{ DF,				"[df]"		},
	{ PHAT,				"[p^]"		},
	{ PHAT1,			"[p^1]"		},
	{ PHAT2,			"[p^2]"		},
	{ MEANX1,			"mean(x1)"	},
	{ N1,				"[n1]"		},
	{ MEANX2,			"mean(x2)"	},
	{ N2,				"[n2]"		},
	{ LOWER,			"[lower]"	},
	{ UPPER,			"[upper]"	},
	{ SYSVAR_S,			"[s]"		},
	{ RSQUARED,			"r^2"		},
	{ CAPRSQUARED,		"R^2"		},
	{ DF2,				"[df]"		}, // somebody was high when they invented the token tables
	{ SS,				"SS"		},
	{ DF3,				"[df]"		}, // see previous comment
	{ SS1,				"SS"		}, // again...
	{ MS1,				"MS"		}, // and again!
	// graph data
	{ ZXSCL,			"ZXscl"		},
	{ ZYSCL,			"ZYscl"		},
	{ XSCL,				"Xscl"		},
	{ YSCL,				"Yscl"		},
	{ ZUNSTART,			"ZUnStart"	},
	{ ZVNSTART,			"ZVnStart"	},
	{ UNSTART,			"UnStart"	},
	{ VNSTART,			"VnStart"	},
	{ UNINVERSE,		"Un-1"		}, // i read a ^-1, but it's actually a -1...
	{ VNINVERSE,		"Vn-1"		}, // same as above
	{ ZXMIN,			"ZXmin"		},
	{ ZXMAX,			"ZXmax"		},
	{ ZYMIN,			"ZYmin"		},
	{ ZYMAX,			"ZYmax"		},
	{ ZTHETAMIN,		"Ztheta_min"},
	{ ZTHETAMAX,		"Ztheta_max"},
	{ ZTMIN,			"ZTmin"		},
	{ ZTMAX,			"ZTmax"		},
	{ XMIN,				"Xmin"		},
	{ XMAX,				"Xmax"		},
	{ YMIN,				"Ymin"		},
	{ YMAX,				"Ymax"		},
	{ TMIN,				"Tmin"		},
	{ TMAX,				"Tmax"		},
	{ THETAMIN,			"theta_min"	},
	{ THETAMAX,			"theta_max"	},
	{ TBLMIN,			"TblMin"	},
	{ ZNMIN,			"ZnMin"		},
	{ NMIN,				"nMin"		},
	{ ZNMAX,			"ZnMax"		},
	{ NMAX,				"nMax"		},
	{ ZNSTART,			"ZnStart"	},
	{ NSTART,			"nStart"	},
	{ DELTATABLE,		"delta_Tbl"	},
	{ ZTSTEP,			"ZTstep"	},
	{ ZTHETASTEP,		"Ztheta_step"},
	{ TSTEP,			"Tstep"		},
	{ THETASTEP,		"theta_step"},
	{ DELTAX,			"delta_X"	},
	{ DELTAY,			"delta_Y"	},
	{ XFACT,			"XFact"		},
	{ YFACT,			"YFact"		},
	{ TBLINPUT,			"TblInput"	},
	// finance app
	{ SYSVAR_CAPN,		"[N]"		}, // this is the N in the Finance app
	{ IPERCENT,			"I%"		},
	{ PV,				"PV"		},
	{ PMT,				"PMT"		},
	{ FV,				"FV"		},
	{ ZXRES,			"ZXres"		},
	{ XRES,				"Xres"		},
	// strings
	{ STR1,				"STR1"		},
	{ STR2,				"STR2"		},
	{ STR3,				"STR3"		},
	{ STR4,				"STR4"		},
	{ STR5,				"STR5"		},
	{ STR6,				"STR6"		},
	{ STR7,				"STR7"		},
	{ STR8,				"STR8"		},
	{ STR9,				"STR9"		},
	{ STR0,				"STR0"		},
	// 7E Variables
	{ SEQ,				"Sequential"	},
	{ SIMUL,			"Simul"			},
	{ POLARGC,			"PolarGC"		},
	{ RECTGC,			"RectGC"		},
	{ COORDON,			"CoordOn"		},
	{ COORDOFF,			"CoordOff"		},
	{ CONNECTED,		"Connected"		},
	{ DOT_7E,			"Dot"			},
	{ AXESON,			"AxesOn"		},
	{ AXESOFF,			"AxesOff"		},
	{ GRIDON,			"GridOn"		},
	{ GRIDOFF,			"GridOff"		},
	{ LABELON,			"LabelOn"		},
	{ LABELOFF,			"LabelOff"		},
	{ WEB,				"Web"			},
	{ TIME,				"Time"			},
	{ UVAXES,			"uvAxes"		},
	{ VWAXES,			"vwAxes"		},
	{ UWAXES,			"uwAxes"		},
	// BB Variables
	{ NPV,				"npv("			},
	{ IRR,				"irr("			},
	{ BAL,				"bal("			},
	{ SUMPRN,			"sum_prn("		},
	{ SUMINT,			"sum_int("		},
	{ TONOM,			"->Nom"			},
	{ TOEFF,			"->Eff"			},
	{ DBD,				"dbd("			},
	{ LCM,				"lcm("			},
	{ GCD,				"gcd("			},
	{ RANDINT,			"RandInt("		},
	{ RANDBIN,			"RandBin("		},
	{ SUB,				"sub("			},
	{ STDDEV,			"StdDev("		},
	{ VARIANCE,			"variance("		},
	{ INSTRING,			"inString("		},
	{ NORMALCDF,		"normalcdf("	},
	{ INVNORM,			"invNorm("		},
	{ TCDF,				"tcdf("			},
	{ XSQUCDF,			"x^2cdf("		},
	{ FCDF,				"fcdf("			},
	{ BINOMPDF,			"binompdf("		},
	{ BINOMCDF,			"binomcdf("		},
	{ POISSONPDF,		"poissonpdf("	},
	{ POISSONCDF,		"poissoncdf("	},
	{ GEOMETPDF,		"geometpdf("	},
	{ GEOMETCDF,		"geometcdf("	},
	{ NORMALPDF,		"normalpdf("	},
	{ TPDF,				"tpdf("			},
	{ XSQUPDF,			"x^2pdf("		},
	{ FPDF,				"fpdf("			},
	{ RANDNORM,			"RandNorm("		},
	{ TVM_PMT,			"tvm_pmt"		},
	{ TVM_I_PERCENT,	"tvm_i%"		},
	{ TVM_PV,			"tvm_PV"		},
	{ TVM_N,			"tvm_N"			},
	{ TVM_FV,			"tvm_FV"		},
	{ CONJ,				"conj("			},
	{ REAL,				"real("			},
	{ IMAG,				"imag("			},
	{ ANGLE,			"angle("		},
	{ CUMSUM,			"cumSum("		},
	{ EXPR,				"expr("			},
	{ LENGTH,			"length("		},
	{ DELTA_LIST,		"delta_List("	},
	{ REF,				"ref("			},
	{ RREF,				"rref("			},
	{ TORECT,			"->Rect"		},
	{ TOPOLAR,			"->Polar"		},
	{ VAR_E,			"[e]"			}, // e by itself is impossible, and dangerous (imagine Disp "Hello"!)
	{ SINREG,			"SinReg "		},
	{ LOGISTIC,			"Logistic "		},
	{ LINREGTTEST,		"LinRegTTest "	},
	{ SHADENORM,		"ShadeNorm("	},
	{ SHADE_T,			"Shade_t("		},
	{ SHADEXSQU,		"Shade_x^2("	},
	{ SHADEF,			"ShadeF("		},
	{ MATRTOLIST,		"Matr->list"	},
	{ LISTTOMATR,		"List->matr"	},
	{ ZTEST,			"Z-Test("		},
	{ TTEST,			"T-Test "		},
	{ TWO_SAMPZTEST,	"2-SampZTest("	},
	{ ONE_PROPZTEST,	"1-PropZTest("	},
	{ TWO_PROPZTEST,	"2-PropZTest("	},
	{ XSQUTEST,			"x^2_test("		},
	{ ZINTERVAL,		"ZInterval"		},
	{ TWO_SAMPZINT,		"2-SampZInt("	},
	{ ONE_PROPZINT,		"1-PropZInt("	},
	{ TWO_PROPZINT,		"2-PropZInt("	},
	{ GRAPHSTYLE,		"GraphStyle("	},
	{ TWO_SAMPTTEST,	"2-SampTTest "	},
	{ TWO_SAMPFTEST,	"2-SampFTest_"	},
	{ TINTERVAL,		"TInterval "	},
	{ TWO_SAMPTINT,		"2-SampTInt "	},
	{ SETUPEDITOR,		"SetUpEditor "	},
	{ PMT_END,			"PMT_End"		},
	{ PMT_BGN,			"PMT_Bgn"		},
	{ REAL_BB,			"Real"			},
	{ REPOWTHETAI,		"re^[theta]i"	},
	{ APLUSBI,			"a+bi"			},
	{ EXPRON,			"ExprOn"		},
	{ EXPROFF,			"ExprOff"		},
	{ CLRALLLISTS,		"ClrAllLists"	},
	{ GETCALC,			"GetCalc("		},
	{ DELVAR,			"DelVar "		},
	{ EQUTOSTRING,		"Equ->String("	},
	{ STRINGTOEQU,		"String->Equ("	},
	{ CLEARENTRIES,		"Clear Entries"	},
	{ SELECT,			"Select("		},
	{ ANOVA,			"ANOVA("		},
	{ MODBOXPLOT,		"ModBoxPlot"	},
	{ NORMPROBPLOT,		"NormProbPlot"	},
};

// Replacements
// Replacements are rules that define special characters that must be replaced with a token.
struct ConvertRule Replacements[] = {
	{ '"', DOUBLEQUOTE },
	{ '\'', APOSTROPHE },
	{ ',', COMMA },
	{ '?', QUESTIONMARK },
	{ ' ', SPACE },
	{ '=', TEST_EQUAL },
	{ '<', TEST_LESSTHAN },
	{ '>', TEST_HIGHTHAN },
	{ '+', ADD },
	{ '-', SUBTRACT },
	{ '/', DIVIDE_SLASH },
	{ '*', MULTIPLY },
	{ '!', FACTORIAL },
	{ ':', COLON },
	{ '\n', HARD_RETURN },
	{ '0', NUM_0 },
	{ '1', NUM_1 },
	{ '2', NUM_2 },
	{ '3', NUM_3 },
	{ '4', NUM_4 },
	{ '5', NUM_5 },
	{ '6', NUM_6 },
	{ '7', NUM_7 },
	{ '8', NUM_8 },
	{ '9', NUM_9 },
	{ '.', PERIOD },
	{ '[', LEFTSBRACK },
	{ ']', RIGHTSBRACK },
	{ '{', LEFTBRACE },
	{ '}', RIGHTBRACE },
	{ '(', LEFTBRACKET },
	{ ')', RIGHTBRACKET },
	{ '&', LOGIC_AND },
	{ '|', LOGIC_OR },
	{ '~', LOGIC_XOR },
	{ '=', TEST_EQUAL },
	{ '<', TEST_LESSTHAN },
	{ '>', TEST_HIGHTHAN },
	{ '^', POWEROF },
};
