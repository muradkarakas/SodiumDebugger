/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 2 "lemon.commands.yy"
 
#include "pch.h"

#include <stdio.h>
#include <assert.h>

#include "..\SodiumExtension\mkSessionDataStructures.h"

#include "commandsDataStructures.h"
#include "commandParser.imp.h"
#include "commands.h"
#include "consoleThread.h"
#include "consoleLoger.h"
#include "lemon.commands.h"
#line 43 "lemon.commands.c"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    commandParseTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is commandParseTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    commandParseARG_SDECL     A static variable declaration for the %extra_argument
**    commandParseARG_PDECL     A parameter declaration for the %extra_argument
**    commandParseARG_STORE     Code to store %extra_argument into yypParser
**    commandParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 117
#define YYACTIONTYPE unsigned short int
#define commandParseTOKENTYPE  TokenCommand 
typedef union {
  int yyinit;
  commandParseTOKENTYPE yy0;
  TokenCommand yy125;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define commandParseARG_SDECL  TokenCommand *Command ;
#define commandParseARG_PDECL , TokenCommand *Command 
#define commandParseARG_FETCH  TokenCommand *Command  = yypParser->Command 
#define commandParseARG_STORE yypParser->Command  = Command 
#define YYNSTATE             107
#define YYNRULE              96
#define YY_MAX_SHIFT         106
#define YY_MIN_SHIFTREDUCE   194
#define YY_MAX_SHIFTREDUCE   289
#define YY_MIN_REDUCE        290
#define YY_MAX_REDUCE        385
#define YY_ERROR_ACTION      386
#define YY_ACCEPT_ACTION     387
#define YY_NO_ACTION         388
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE
**
**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as either:
**
**    (A)   N = yy_action[ yy_shift_ofst[S] + X ]
**    (B)   N = yy_default[S]
**
** The (A) formula is preferred.  The B formula is used instead if:
**    (1)  The yy_shift_ofst[S]+X value is out of range, or
**    (2)  yy_lookahead[yy_shift_ofst[S]+X] is not equal to X, or
**    (3)  yy_shift_ofst[S] equal YY_SHIFT_USE_DFLT.
** (Implementation note: YY_SHIFT_USE_DFLT is chosen so that
** YY_SHIFT_USE_DFLT+X will be out of range for all possible lookaheads X.
** Hence only tests (1) and (2) need to be evaluated.)
**
** The formulas above are for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (296)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   387,    1,   92,  194,  194,  194,  194,  194,  194,  194,
 /*    10 */   194,   90,  194,  194,  194,  194,  194,  194,  194,  194,
 /*    20 */   194,  194,  194,  194,  194,  194,  194,  194,  105,  252,
 /*    30 */   104,  287,  103,  102,   71,   93,   91,  281,   32,   98,
 /*    40 */    97,  226,   96,   73,   94,   85,    2,    2,   90,  256,
 /*    50 */   256,  256,  256,  256,  256,  256,  256,   90,  256,  256,
 /*    60 */   256,  256,  256,  256,  256,  256,  256,  256,  256,  256,
 /*    70 */   256,  256,  256,  256,   28,   32,  197,   31,   87,   33,
 /*    80 */    29,  244,  245,  246,  247,   35,   89,   36,  106,  253,
 /*    90 */   284,  287,   37,   38,   90,  249,  250,   84,  243,   82,
 /*   100 */    59,   80,   79,   78,   77,   76,   75,   39,   41,   16,
 /*   110 */   285,   42,   14,  282,   31,   69,   33,   67,   66,   43,
 /*   120 */    40,   44,   35,   63,   36,   61,   60,   30,   58,   37,
 /*   130 */    38,   45,   34,  350,   28,   57,  284,  287,   87,   46,
 /*   140 */    29,  251,   47,  248,   39,   41,   64,   48,   42,  288,
 /*   150 */   242,  287,   49,   50,   51,  241,   43,   84,   44,   82,
 /*   160 */   287,   80,   79,   78,   77,   76,   75,  238,   45,   16,
 /*   170 */    52,   53,   14,   54,  234,   69,   46,   67,   66,   47,
 /*   180 */    40,   55,   56,   63,   48,   61,   60,   30,  232,   49,
 /*   190 */    50,   51,   34,  235,  236,   57,  231,  287,  229,  227,
 /*   200 */   224,  221,  220,  287,  219,  287,  218,   52,   53,  217,
 /*   210 */    54,  216,  287,  213,  210,  200,  198,  287,   55,   56,
 /*   220 */   195,    3,  100,   99,    4,    5,    6,    7,    8,  287,
 /*   230 */   239,   62,    9,   10,  233,   65,  287,   95,   11,   12,
 /*   240 */    13,   68,  225,   15,  230,   17,  228,   18,   74,  214,
 /*   250 */   287,   19,   20,   21,   22,   23,   24,  211,  287,   25,
 /*   260 */   287,  287,  287,  287,  287,   81,  287,   83,  287,   26,
 /*   270 */   199,  287,   86,  287,  196,  287,  287,  287,  287,   27,
 /*   280 */   351,   70,  287,   72,  222,  209,  208,  207,  206,  205,
 /*   290 */   204,   88,  101,  203,  202,  201,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*    10 */    77,   78,   79,   80,   81,   82,   83,   84,   85,   86,
 /*    20 */    87,   88,   89,   90,   91,   92,   93,   94,    8,    2,
 /*    30 */    10,   64,   12,   13,  105,  106,   16,   41,   78,   19,
 /*    40 */    20,   38,   22,  103,  104,   99,  113,  114,  115,   70,
 /*    50 */    71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
 /*    60 */    81,   82,   83,   84,   85,   86,   87,   88,   89,   90,
 /*    70 */    91,   92,   93,   94,    1,  115,    4,   78,    5,   78,
 /*    80 */     7,   55,   56,   57,   58,   78,   95,   78,   97,    2,
 /*    90 */    63,   64,   78,   78,  115,   60,   61,   24,    2,   26,
 /*   100 */   111,   28,   29,   30,   31,   32,   33,   78,   78,   36,
 /*   110 */   115,   78,   39,  114,  115,   42,  115,   44,   45,   78,
 /*   120 */    47,   78,  115,   50,  115,   52,   53,   54,  112,  115,
 /*   130 */   115,   78,   59,    0,    1,   62,   63,   64,    5,   78,
 /*   140 */     7,    2,   78,    2,  115,  115,  109,   78,  115,   65,
 /*   150 */     2,   64,   78,   78,   78,    2,  115,   24,  115,   26,
 /*   160 */    64,   28,   29,   30,   31,   32,   33,    2,  115,   36,
 /*   170 */    78,   78,   39,   78,    2,   42,  115,   44,   45,  115,
 /*   180 */    47,   78,   78,   50,  115,   52,   53,   54,    2,  115,
 /*   190 */   115,  115,   59,   48,   49,   62,    2,   64,    2,    2,
 /*   200 */     2,    2,    2,   64,    2,   64,    2,  115,  115,    2,
 /*   210 */   115,    2,   64,    2,    2,    2,    2,   64,  115,  115,
 /*   220 */     2,   96,   17,   18,   96,   96,   96,   96,   96,   64,
 /*   230 */    51,  110,   96,   96,   46,  108,   64,   34,   96,   96,
 /*   240 */    96,  107,  104,   96,   43,   96,  106,   96,  102,   27,
 /*   250 */    64,   96,   96,   96,   96,   96,   96,   25,   64,   96,
 /*   260 */    64,   64,   64,   64,   64,  101,   64,  100,   64,   96,
 /*   270 */     6,   64,   98,   64,   97,   64,   64,   64,   64,   96,
 /*   280 */     0,   40,   64,   37,   35,   23,   21,   15,   11,   15,
 /*   290 */    15,    3,   14,   11,   11,    9,
};
#define YY_SHIFT_USE_DFLT (296)
#define YY_SHIFT_COUNT    (106)
#define YY_SHIFT_MIN      (-33)
#define YY_SHIFT_MAX      (288)
static const short yy_shift_ofst[] = {
 /*     0 */    73,  133,   27,  -33,  -33,  -33,  -33,  -33,  -33,  -33,
 /*    10 */   -33,  -33,  -33,  -33,   -4,  -33,    3,  -33,  -33,  -33,
 /*    20 */   -33,  -33,  -33,  -33,  -33,  -33,  -33,  -33,   72,   20,
 /*    30 */    26,   87,  139,  141,   35,   96,  148,  153,  165,  172,
 /*    40 */   145,  186,  194,  196,  197,  198,  199,  200,  202,  204,
 /*    50 */   207,  209,  211,  212,  213,  214,  218,   84,   84,   84,
 /*    60 */    84,   84,   84,  179,   84,   84,  188,   84,   84,  201,
 /*    70 */    -4,   84,    3,   84,   84,  203,   84,   84,   84,   84,
 /*    80 */    84,   84,  222,   84,  232,   84,   84,  264,   72,   84,
 /*    90 */   -33,  205,  280,  241,  246,  249,  262,  265,  272,  277,
 /*   100 */   274,  275,  278,  282,  283,  286,  288,
};
#define YY_REDUCE_USE_DFLT (-72)
#define YY_REDUCE_COUNT (90)
#define YY_REDUCE_MIN   (-71)
#define YY_REDUCE_MAX   (183)
static const short yy_reduce_ofst[] = {
 /*     0 */   -67,  -21,   -1,  -40,    1,    7,    9,   14,   15,   29,
 /*    10 */    30,   33,   41,   43,  -71,   53,  -60,   61,   64,   69,
 /*    20 */    74,   75,   76,   92,   93,   95,  103,  104,   -9,  -54,
 /*    30 */   -11,   -5,   -5,   -5,   16,   -5,   -5,   -5,   -5,   -5,
 /*    40 */    37,   -5,   -5,   -5,   -5,   -5,   -5,   -5,   -5,   -5,
 /*    50 */    -5,   -5,   -5,   -5,   -5,   -5,   -5,  125,  128,  129,
 /*    60 */   130,  131,  132,  121,  136,  137,  127,  142,  143,  134,
 /*    70 */   140,  144,  138,  147,  149,  146,  151,  155,  156,  157,
 /*    80 */   158,  159,  164,  160,  167,  163,  173,  174,  177,  183,
 /*    90 */    -5,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   386,  386,  386,  386,  386,  386,  386,  386,  386,  386,
 /*    10 */   386,  386,  386,  386,  386,  386,  386,  386,  386,  386,
 /*    20 */   386,  386,  386,  386,  386,  386,  386,  386,  386,  386,
 /*    30 */   386,  386,  386,  386,  386,  386,  386,  386,  386,  386,
 /*    40 */   333,  386,  386,  386,  386,  386,  386,  386,  386,  386,
 /*    50 */   386,  386,  386,  386,  386,  386,  386,  385,  385,  385,
 /*    60 */   385,  385,  385,  336,  385,  385,  386,  385,  385,  386,
 /*    70 */   386,  385,  386,  385,  385,  319,  385,  385,  385,  385,
 /*    80 */   385,  385,  311,  385,  308,  385,  385,  386,  386,  385,
 /*    90 */   360,  386,  386,  386,  386,  386,  386,  386,  386,  386,
 /*   100 */   386,  386,  386,  386,  386,  386,  386,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  commandParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void commandParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "CMD_CLEAR_ALL_BREAKPOINTS",  "CMD_END_OF_FILE",  "CMD_CLEARALLBREAKPOINT_PARAM_FULLFILEPATH",
  "CMD_CLEARALLBREAKPOINT_PARAM_DOUBLEQUOTE",  "CMD_CATCH",     "CMD_CATCH_THROW",  "CMD_SET",     
  "CMD_SET_CONFIRM",  "CMD_SET_OFF",   "CMD_SET_WIDTH",  "CMD_SET_NUMERIC",
  "CMD_SET_HEIGHT",  "CMD_SET_BREAKPOINT",  "CMD_SET_PENDING",  "CMD_SET_ON",  
  "CMD_SET_PRINT",  "CMD_SET_ASM_DEMANGLE",  "CMD_SET_ELEMENTS",  "CMD_SET_UNWINDONSIGNAL",
  "CMD_SET_DISASSEMBLY_FLAVOR",  "CMD_SET_ATT",   "CMD_SET_PROMPT",  "CMD_SET_PROMPT_STRING",
  "CMD_OUTPUT",    "CMD_OUTPUT_VARIABLE_NAME",  "CMD_WHATIS",    "CMD_WHATIS_VARIABLE_NAME",
  "CMD_QUIT",      "CMD_DUMMY",     "CMD_NEXT",      "CMD_FINISH",  
  "CMD_STEP",      "CMD_DELETE",    "CMD_DELETE_PARAM_BREAKPOINTS",  "CMD_DELETE_PARAM_NUMERIC",
  "CMD_BREAK",     "CMD_BREAK_PARAM_FULLFILEPATH",  "CMD_BREAK_PARAM_DOUBLEQUOTE",  "CMD_TBREAK",  
  "CMD_TBREAK_PARAM_FULLFILEPATH",  "CMD_TBREAK_PARAM_DOUBLEQUOTE",  "CMD_SOURCE",    "CMD_SOURCE_PARAM_FULLFILEPATH",
  "CMD_CONTINUE",  "CMD_FRAME",     "CMD_FRAME_PARAM_FRAMENO",  "CMD_BACKTRACE",
  "CMD_BACKTRACE_PARAM_FRAMECOUNT",  "CMD_BACKTRACE_PARAM_OTHERS",  "CMD_ATTACH",    "CMD_ATTACH_PARAM_SESSION_ID",
  "CMD_DETACH",    "CMD_KILL",      "CMD_INFO",      "CMD_INFO_PARAM_LOCALS",
  "CMD_INFO_PARAM_ARGS",  "CMD_INFO_PARAM_GLOBALS",  "CMD_INFO_PARAM_FRAME",  "CMD_SHOW",    
  "CMD_SHOW_PARAM_SESSION_COUNT",  "CMD_SHOW_PARAM_VERSION",  "CMD_EXIT",      "CMD_ANY_CHARACTER",
  "CMD_EXECUTE",   "CMD_SEMI_COLON",  "error",         "start",       
  "validcommands",  "invalidcommands",  "command",       "info",        
  "kill",          "exit",          "quit",          "show",        
  "attach",        "detach",        "cmdexecute",    "backtrace",   
  "frame",         "continue",      "source",        "break",       
  "tbreak",        "delete",        "next",          "step",        
  "finish",        "dummy",         "whatis",        "output",      
  "set",           "catch",         "clearallbreakpoints",  "clearallbreakpointsparam",
  "semicolon",     "clearallbreakpointsparamdoublequote",  "catchparams",   "setparams",   
  "outputparams",  "whatisparams",  "deleteparams",  "breakparams", 
  "breakdoublequote",  "tbreakparams",  "tbreakdoublequote",  "sourceparams",
  "frameparams",   "backtraceparams",  "attachparams",  "infoparams",  
  "showparams",    "invalidchars",  "invalidchar",   "cmdexecutechar",
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "validcommands ::= command",
 /*   1 */ "clearallbreakpoints ::= CMD_CLEAR_ALL_BREAKPOINTS clearallbreakpointsparam semicolon cmdexecute CMD_END_OF_FILE",
 /*   2 */ "clearallbreakpointsparam ::= clearallbreakpointsparamdoublequote CMD_CLEARALLBREAKPOINT_PARAM_FULLFILEPATH clearallbreakpointsparamdoublequote",
 /*   3 */ "clearallbreakpointsparamdoublequote ::= CMD_CLEARALLBREAKPOINT_PARAM_DOUBLEQUOTE",
 /*   4 */ "catch ::= CMD_CATCH catchparams semicolon cmdexecute CMD_END_OF_FILE",
 /*   5 */ "catchparams ::= CMD_CATCH_THROW",
 /*   6 */ "set ::= CMD_SET setparams semicolon cmdexecute CMD_END_OF_FILE",
 /*   7 */ "setparams ::= CMD_SET_CONFIRM CMD_SET_OFF",
 /*   8 */ "setparams ::= CMD_SET_WIDTH CMD_SET_NUMERIC",
 /*   9 */ "setparams ::= CMD_SET_HEIGHT CMD_SET_NUMERIC",
 /*  10 */ "setparams ::= CMD_SET_BREAKPOINT CMD_SET_PENDING CMD_SET_ON",
 /*  11 */ "setparams ::= CMD_SET_PRINT CMD_SET_ASM_DEMANGLE CMD_SET_ON",
 /*  12 */ "setparams ::= CMD_SET_PRINT CMD_SET_ELEMENTS CMD_SET_NUMERIC",
 /*  13 */ "setparams ::= CMD_SET_UNWINDONSIGNAL CMD_SET_ON",
 /*  14 */ "setparams ::= CMD_SET_DISASSEMBLY_FLAVOR CMD_SET_ATT",
 /*  15 */ "setparams ::= CMD_SET_PROMPT CMD_SET_PROMPT_STRING",
 /*  16 */ "output ::= CMD_OUTPUT outputparams semicolon cmdexecute CMD_END_OF_FILE",
 /*  17 */ "outputparams ::= CMD_OUTPUT_VARIABLE_NAME",
 /*  18 */ "outputparams ::=",
 /*  19 */ "whatis ::= CMD_WHATIS whatisparams semicolon cmdexecute CMD_END_OF_FILE",
 /*  20 */ "whatisparams ::= CMD_WHATIS_VARIABLE_NAME",
 /*  21 */ "whatisparams ::=",
 /*  22 */ "quit ::= CMD_QUIT semicolon cmdexecute CMD_END_OF_FILE",
 /*  23 */ "dummy ::= CMD_DUMMY semicolon cmdexecute CMD_END_OF_FILE",
 /*  24 */ "next ::= CMD_NEXT semicolon cmdexecute CMD_END_OF_FILE",
 /*  25 */ "finish ::= CMD_FINISH semicolon cmdexecute CMD_END_OF_FILE",
 /*  26 */ "step ::= CMD_STEP semicolon cmdexecute CMD_END_OF_FILE",
 /*  27 */ "delete ::= CMD_DELETE deleteparams semicolon cmdexecute CMD_END_OF_FILE",
 /*  28 */ "deleteparams ::= CMD_DELETE_PARAM_BREAKPOINTS CMD_DELETE_PARAM_NUMERIC",
 /*  29 */ "deleteparams ::=",
 /*  30 */ "break ::= CMD_BREAK breakparams semicolon cmdexecute CMD_END_OF_FILE",
 /*  31 */ "breakparams ::= breakdoublequote CMD_BREAK_PARAM_FULLFILEPATH breakdoublequote",
 /*  32 */ "breakdoublequote ::= CMD_BREAK_PARAM_DOUBLEQUOTE",
 /*  33 */ "tbreak ::= CMD_TBREAK tbreakparams semicolon cmdexecute CMD_END_OF_FILE",
 /*  34 */ "tbreakparams ::= tbreakdoublequote CMD_TBREAK_PARAM_FULLFILEPATH tbreakdoublequote",
 /*  35 */ "source ::= CMD_SOURCE sourceparams semicolon cmdexecute CMD_END_OF_FILE",
 /*  36 */ "sourceparams ::= CMD_SOURCE_PARAM_FULLFILEPATH",
 /*  37 */ "continue ::= CMD_CONTINUE semicolon cmdexecute CMD_END_OF_FILE",
 /*  38 */ "frame ::= CMD_FRAME frameparams semicolon cmdexecute CMD_END_OF_FILE",
 /*  39 */ "frameparams ::= CMD_FRAME_PARAM_FRAMENO",
 /*  40 */ "backtrace ::= CMD_BACKTRACE backtraceparams semicolon cmdexecute CMD_END_OF_FILE",
 /*  41 */ "backtraceparams ::= CMD_BACKTRACE_PARAM_FRAMECOUNT",
 /*  42 */ "backtraceparams ::= CMD_BACKTRACE_PARAM_OTHERS",
 /*  43 */ "backtraceparams ::=",
 /*  44 */ "attach ::= CMD_ATTACH attachparams semicolon cmdexecute CMD_END_OF_FILE",
 /*  45 */ "attachparams ::= CMD_ATTACH_PARAM_SESSION_ID",
 /*  46 */ "attachparams ::=",
 /*  47 */ "detach ::= CMD_DETACH semicolon cmdexecute CMD_END_OF_FILE",
 /*  48 */ "kill ::= CMD_KILL semicolon cmdexecute CMD_END_OF_FILE",
 /*  49 */ "info ::= CMD_INFO infoparams semicolon cmdexecute CMD_END_OF_FILE",
 /*  50 */ "infoparams ::= CMD_INFO_PARAM_LOCALS",
 /*  51 */ "infoparams ::= CMD_INFO_PARAM_ARGS",
 /*  52 */ "infoparams ::= CMD_INFO_PARAM_GLOBALS",
 /*  53 */ "infoparams ::= CMD_INFO_PARAM_FRAME",
 /*  54 */ "show ::= CMD_SHOW showparams semicolon cmdexecute CMD_END_OF_FILE",
 /*  55 */ "showparams ::= CMD_SHOW_PARAM_SESSION_COUNT",
 /*  56 */ "showparams ::= CMD_SHOW_PARAM_VERSION",
 /*  57 */ "exit ::= CMD_EXIT semicolon cmdexecute CMD_END_OF_FILE",
 /*  58 */ "invalidcommands ::= invalidchars CMD_END_OF_FILE",
 /*  59 */ "invalidcommands ::= invalidchars cmdexecute CMD_END_OF_FILE",
 /*  60 */ "start ::= validcommands",
 /*  61 */ "start ::= invalidcommands",
 /*  62 */ "validcommands ::= validcommands command",
 /*  63 */ "command ::= info",
 /*  64 */ "command ::= kill",
 /*  65 */ "command ::= exit",
 /*  66 */ "command ::= quit",
 /*  67 */ "command ::= show",
 /*  68 */ "command ::= attach",
 /*  69 */ "command ::= detach",
 /*  70 */ "command ::= cmdexecute",
 /*  71 */ "command ::= backtrace",
 /*  72 */ "command ::= frame",
 /*  73 */ "command ::= continue",
 /*  74 */ "command ::= source",
 /*  75 */ "command ::= break",
 /*  76 */ "command ::= tbreak",
 /*  77 */ "command ::= delete",
 /*  78 */ "command ::= next",
 /*  79 */ "command ::= step",
 /*  80 */ "command ::= finish",
 /*  81 */ "command ::= dummy",
 /*  82 */ "command ::= whatis",
 /*  83 */ "command ::= output",
 /*  84 */ "command ::= set",
 /*  85 */ "command ::= catch",
 /*  86 */ "command ::= clearallbreakpoints",
 /*  87 */ "tbreakdoublequote ::= CMD_TBREAK_PARAM_DOUBLEQUOTE",
 /*  88 */ "invalidchars ::= invalidchars invalidchar",
 /*  89 */ "invalidchars ::= invalidchar",
 /*  90 */ "invalidchar ::= CMD_ANY_CHARACTER",
 /*  91 */ "cmdexecute ::= cmdexecute cmdexecutechar",
 /*  92 */ "cmdexecute ::= cmdexecutechar",
 /*  93 */ "cmdexecutechar ::= CMD_EXECUTE",
 /*  94 */ "semicolon ::= CMD_SEMI_COLON",
 /*  95 */ "semicolon ::=",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.  Return the number
** of errors.  Return 0 on success.
*/
static int yyGrowStack(yyParser *p){
  int newSize;
  int idx;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  idx = p->yytos ? (int)(p->yytos - p->yystack) : 0;
  if( p->yystack==&p->yystk0 ){
    pNew = malloc(newSize*sizeof(pNew[0]));
    if( pNew ) pNew[0] = p->yystk0;
  }else{
    pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  }
  if( pNew ){
    p->yystack = pNew;
    p->yytos = &p->yystack[idx];
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows from %d to %d entries.\n",
              yyTracePrompt, p->yystksz, newSize);
    }
#endif
    p->yystksz = newSize;
  }
  return pNew==0; 
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to commandParseAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to commandParse and commandParseFree.
*/
void *commandParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ){
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yytos = NULL;
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    if( yyGrowStack(pParser) ){
      pParser->yystack = &pParser->yystk0;
      pParser->yystksz = 1;
    }
#endif
#ifndef YYNOERRORRECOVERY
    pParser->yyerrcnt = -1;
#endif
    pParser->yytos = pParser->yystack;
    pParser->yystack[0].stateno = 0;
    pParser->yystack[0].major = 0;
  }
  return pParser;
}

/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  commandParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yytos!=0 );
  assert( pParser->yytos > pParser->yystack );
  yytos = pParser->yytos--;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void commandParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
#ifndef YYPARSEFREENEVERNULL
  if( pParser==0 ) return;
#endif
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int commandParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static unsigned int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yytos->stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
        iLookAhead = iFallback;
        continue;
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD && iLookAhead>0
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead],
               yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   commandParseARG_FETCH;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   commandParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  commandParseTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yytos++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
    yypParser->yyhwm++;
    assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack) );
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yytos>=&yypParser->yystack[YYSTACKDEPTH] ){
    yypParser->yytos--;
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yypParser->yytos--;
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 68, 1 },
  { 94, 5 },
  { 95, 3 },
  { 97, 1 },
  { 93, 5 },
  { 98, 1 },
  { 92, 5 },
  { 99, 2 },
  { 99, 2 },
  { 99, 2 },
  { 99, 3 },
  { 99, 3 },
  { 99, 3 },
  { 99, 2 },
  { 99, 2 },
  { 99, 2 },
  { 91, 5 },
  { 100, 1 },
  { 100, 0 },
  { 90, 5 },
  { 101, 1 },
  { 101, 0 },
  { 74, 4 },
  { 89, 4 },
  { 86, 4 },
  { 88, 4 },
  { 87, 4 },
  { 85, 5 },
  { 102, 2 },
  { 102, 0 },
  { 83, 5 },
  { 103, 3 },
  { 104, 1 },
  { 84, 5 },
  { 105, 3 },
  { 82, 5 },
  { 107, 1 },
  { 81, 4 },
  { 80, 5 },
  { 108, 1 },
  { 79, 5 },
  { 109, 1 },
  { 109, 1 },
  { 109, 0 },
  { 76, 5 },
  { 110, 1 },
  { 110, 0 },
  { 77, 4 },
  { 72, 4 },
  { 71, 5 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 75, 5 },
  { 112, 1 },
  { 112, 1 },
  { 73, 4 },
  { 69, 2 },
  { 69, 3 },
  { 67, 1 },
  { 67, 1 },
  { 68, 2 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 106, 1 },
  { 113, 2 },
  { 113, 1 },
  { 114, 1 },
  { 78, 2 },
  { 78, 1 },
  { 115, 1 },
  { 96, 1 },
  { 96, 0 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno        /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  commandParseARG_FETCH;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[-yysize].stateno);
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
      yypParser->yyhwm++;
      assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack));
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yytos>=&yypParser->yystack[YYSTACKDEPTH-1] ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        return;
      }
      yymsp = yypParser->yytos;
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
      case 0: /* validcommands ::= command */
#line 39 "lemon.commands.yy"
{
	BOOL success = cmdImp(Command); 
	if (success) {
		// command is executed successfully
		//DLOG("\nDone\n\n", NULL);
	}
}
#line 1061 "lemon.commands.c"
        break;
      case 1: /* clearallbreakpoints ::= CMD_CLEAR_ALL_BREAKPOINTS clearallbreakpointsparam semicolon cmdexecute CMD_END_OF_FILE */
#line 80 "lemon.commands.yy"
{
	Command->tokenCode = CMD_CLEAR_ALL_BREAKPOINTS;
}
#line 1068 "lemon.commands.c"
        break;
      case 2: /* clearallbreakpointsparam ::= clearallbreakpointsparamdoublequote CMD_CLEARALLBREAKPOINT_PARAM_FULLFILEPATH clearallbreakpointsparamdoublequote */
#line 84 "lemon.commands.yy"
{
	char* fullFilePath = yymsp[-1].minor.yy0.tokenStr;
	strncpy_s(Command->params->p.clearallbreakpoint_cmd_params.fullFilePath, MAX_PATH, fullFilePath, yymsp[-1].minor.yy0.tokenStrLength);
}
#line 1076 "lemon.commands.c"
        break;
      case 3: /* clearallbreakpointsparamdoublequote ::= CMD_CLEARALLBREAKPOINT_PARAM_DOUBLEQUOTE */
#line 89 "lemon.commands.yy"
{

}
#line 1083 "lemon.commands.c"
        break;
      case 4: /* catch ::= CMD_CATCH catchparams semicolon cmdexecute CMD_END_OF_FILE */
#line 97 "lemon.commands.yy"
{
	Command->tokenCode = CMD_CATCH;
}
#line 1090 "lemon.commands.c"
        break;
      case 5: /* catchparams ::= CMD_CATCH_THROW */
#line 101 "lemon.commands.yy"
{
	Command->params->p.catch_cmd_params.throw = TRUE;
}
#line 1097 "lemon.commands.c"
        break;
      case 6: /* set ::= CMD_SET setparams semicolon cmdexecute CMD_END_OF_FILE */
#line 110 "lemon.commands.yy"
{
	Command->tokenCode = CMD_SET;
}
#line 1104 "lemon.commands.c"
        break;
      case 7: /* setparams ::= CMD_SET_CONFIRM CMD_SET_OFF */
#line 114 "lemon.commands.yy"
{
	Command->params->p.set_cmd_params.confirm_off = TRUE;
}
#line 1111 "lemon.commands.c"
        break;
      case 8: /* setparams ::= CMD_SET_WIDTH CMD_SET_NUMERIC */
#line 118 "lemon.commands.yy"
{
	char *width = yymsp[0].minor.yy0.tokenStr;
	Command->params->p.set_cmd_params.width = atoi(width);
}
#line 1119 "lemon.commands.c"
        break;
      case 9: /* setparams ::= CMD_SET_HEIGHT CMD_SET_NUMERIC */
#line 123 "lemon.commands.yy"
{
	char *height = yymsp[0].minor.yy0.tokenStr;
	Command->params->p.set_cmd_params.width = atoi(height);
}
#line 1127 "lemon.commands.c"
        break;
      case 10: /* setparams ::= CMD_SET_BREAKPOINT CMD_SET_PENDING CMD_SET_ON */
#line 128 "lemon.commands.yy"
{
	Command->params->p.set_cmd_params.breakpoint_pending_on = TRUE;
}
#line 1134 "lemon.commands.c"
        break;
      case 11: /* setparams ::= CMD_SET_PRINT CMD_SET_ASM_DEMANGLE CMD_SET_ON */
#line 132 "lemon.commands.yy"
{
	Command->params->p.set_cmd_params.print_asm_demangle_on = TRUE;
}
#line 1141 "lemon.commands.c"
        break;
      case 12: /* setparams ::= CMD_SET_PRINT CMD_SET_ELEMENTS CMD_SET_NUMERIC */
#line 136 "lemon.commands.yy"
{
	char *element = yymsp[0].minor.yy0.tokenStr;
	Command->params->p.set_cmd_params.print_element = atoi(element);
}
#line 1149 "lemon.commands.c"
        break;
      case 13: /* setparams ::= CMD_SET_UNWINDONSIGNAL CMD_SET_ON */
#line 141 "lemon.commands.yy"
{
	Command->params->p.set_cmd_params.unwindonsignal_on = TRUE;
}
#line 1156 "lemon.commands.c"
        break;
      case 14: /* setparams ::= CMD_SET_DISASSEMBLY_FLAVOR CMD_SET_ATT */
#line 145 "lemon.commands.yy"
{
	Command->params->p.set_cmd_params.disassembly_flavor_att = TRUE;
}
#line 1163 "lemon.commands.c"
        break;
      case 15: /* setparams ::= CMD_SET_PROMPT CMD_SET_PROMPT_STRING */
#line 149 "lemon.commands.yy"
{
	char *prompt = yymsp[0].minor.yy0.tokenStr;
	strcpy_s(Command->params->p.set_cmd_params.prompt, MAX_PROMPT_LENGTH, prompt);
}
#line 1171 "lemon.commands.c"
        break;
      case 16: /* output ::= CMD_OUTPUT outputparams semicolon cmdexecute CMD_END_OF_FILE */
#line 159 "lemon.commands.yy"
{
	Command->tokenCode = CMD_OUTPUT;
}
#line 1178 "lemon.commands.c"
        break;
      case 17: /* outputparams ::= CMD_OUTPUT_VARIABLE_NAME */
#line 163 "lemon.commands.yy"
{
	char *variableName = yymsp[0].minor.yy0.tokenStr;
	strncpy_s(Command->params->p.output_cmd_params.variableName, MAX_VARIABLE_NAME_LENGTH, variableName, MAX_VARIABLE_NAME_LENGTH);
	Command->params->p.output_cmd_params.variableName[MAX_VARIABLE_NAME_LENGTH] = '\0';
}
#line 1187 "lemon.commands.c"
        break;
      case 18: /* outputparams ::= */
#line 169 "lemon.commands.yy"
{
	Command->params->p.output_cmd_params.variableName[0] = '\0';
}
#line 1194 "lemon.commands.c"
        break;
      case 19: /* whatis ::= CMD_WHATIS whatisparams semicolon cmdexecute CMD_END_OF_FILE */
#line 177 "lemon.commands.yy"
{
	Command->tokenCode = CMD_WHATIS;
}
#line 1201 "lemon.commands.c"
        break;
      case 20: /* whatisparams ::= CMD_WHATIS_VARIABLE_NAME */
#line 181 "lemon.commands.yy"
{
	char *variableName = yymsp[0].minor.yy0.tokenStr;
	strncpy_s(Command->params->p.whatis_cmd_params.variableName, MAX_VARIABLE_NAME_LENGTH, variableName, MAX_VARIABLE_NAME_LENGTH);
	Command->params->p.whatis_cmd_params.variableName[MAX_VARIABLE_NAME_LENGTH] = '\0';
}
#line 1210 "lemon.commands.c"
        break;
      case 21: /* whatisparams ::= */
#line 187 "lemon.commands.yy"
{
	Command->params->p.whatis_cmd_params.variableName[0] = '\0';
}
#line 1217 "lemon.commands.c"
        break;
      case 22: /* quit ::= CMD_QUIT semicolon cmdexecute CMD_END_OF_FILE */
#line 195 "lemon.commands.yy"
{
	Command->tokenCode = CMD_QUIT;
}
#line 1224 "lemon.commands.c"
        break;
      case 23: /* dummy ::= CMD_DUMMY semicolon cmdexecute CMD_END_OF_FILE */
#line 203 "lemon.commands.yy"
{
	Command->tokenCode = CMD_DUMMY;
}
#line 1231 "lemon.commands.c"
        break;
      case 24: /* next ::= CMD_NEXT semicolon cmdexecute CMD_END_OF_FILE */
#line 211 "lemon.commands.yy"
{
	Command->tokenCode = CMD_NEXT;
}
#line 1238 "lemon.commands.c"
        break;
      case 25: /* finish ::= CMD_FINISH semicolon cmdexecute CMD_END_OF_FILE */
#line 219 "lemon.commands.yy"
{
	Command->tokenCode = CMD_FINISH;
}
#line 1245 "lemon.commands.c"
        break;
      case 26: /* step ::= CMD_STEP semicolon cmdexecute CMD_END_OF_FILE */
#line 228 "lemon.commands.yy"
{
	Command->tokenCode = CMD_STEP;
}
#line 1252 "lemon.commands.c"
        break;
      case 27: /* delete ::= CMD_DELETE deleteparams semicolon cmdexecute CMD_END_OF_FILE */
#line 236 "lemon.commands.yy"
{
	Command->tokenCode = CMD_DELETE;
}
#line 1259 "lemon.commands.c"
        break;
      case 28: /* deleteparams ::= CMD_DELETE_PARAM_BREAKPOINTS CMD_DELETE_PARAM_NUMERIC */
#line 240 "lemon.commands.yy"
{
	char *breakpointID = yymsp[0].minor.yy0.tokenStr;
	Command->params->p.delete_cmd_params.paramType = CMD_DELETE_PARAM_TYPE_BREAKPOINTS;
	Command->params->p.delete_cmd_params.breakpointID = atoi(breakpointID);
}
#line 1268 "lemon.commands.c"
        break;
      case 29: /* deleteparams ::= */
#line 246 "lemon.commands.yy"
{
	Command->params->p.delete_cmd_params.paramType = CMD_DELETE_PARAM_TYPE_NOTSET;
}
#line 1275 "lemon.commands.c"
        break;
      case 30: /* break ::= CMD_BREAK breakparams semicolon cmdexecute CMD_END_OF_FILE */
#line 254 "lemon.commands.yy"
{
	Command->tokenCode = CMD_BREAK;
}
#line 1282 "lemon.commands.c"
        break;
      case 31: /* breakparams ::= breakdoublequote CMD_BREAK_PARAM_FULLFILEPATH breakdoublequote */
#line 258 "lemon.commands.yy"
{	
	char *fullFilePath = yymsp[-1].minor.yy0.tokenStr;

	char *colonPos = strrchr(fullFilePath, ':');
	if (colonPos) {
		*colonPos = (char) '\0';
		colonPos++;
	}
	strncpy_s(Command->params->p.break_cmd_params.fullFilePath, MAX_PATH, fullFilePath, yymsp[-1].minor.yy0.tokenStrLength);
	//Command->params->p.break_cmd_params.fullFilePath[SESSION_ID_LENGTH] = '\0';
	Command->params->p.break_cmd_params.lineNumber = atoi(colonPos);
}
#line 1298 "lemon.commands.c"
        break;
      case 32: /* breakdoublequote ::= CMD_BREAK_PARAM_DOUBLEQUOTE */
#line 271 "lemon.commands.yy"
{
	int test = 1;
	int a  = test * 2;
}
#line 1306 "lemon.commands.c"
        break;
      case 33: /* tbreak ::= CMD_TBREAK tbreakparams semicolon cmdexecute CMD_END_OF_FILE */
#line 281 "lemon.commands.yy"
{
	Command->tokenCode = CMD_TBREAK;
}
#line 1313 "lemon.commands.c"
        break;
      case 34: /* tbreakparams ::= tbreakdoublequote CMD_TBREAK_PARAM_FULLFILEPATH tbreakdoublequote */
#line 285 "lemon.commands.yy"
{	
	char *fullFilePath = yymsp[-1].minor.yy0.tokenStr;

	char *colonPos = strrchr(fullFilePath, ':');
	if (colonPos) {
		*colonPos = (char) '\0';
		colonPos++;
	}
	strncpy_s(Command->params->p.tbreak_cmd_params.fullFilePath, MAX_PATH, fullFilePath, yymsp[-1].minor.yy0.tokenStrLength);
	Command->params->p.tbreak_cmd_params.lineNumber = atoi(colonPos);
}
#line 1328 "lemon.commands.c"
        break;
      case 35: /* source ::= CMD_SOURCE sourceparams semicolon cmdexecute CMD_END_OF_FILE */
#line 303 "lemon.commands.yy"
{
	Command->tokenCode = CMD_SOURCE;
}
#line 1335 "lemon.commands.c"
        break;
      case 36: /* sourceparams ::= CMD_SOURCE_PARAM_FULLFILEPATH */
#line 307 "lemon.commands.yy"
{	
	char *fullFilePath = yymsp[0].minor.yy0.tokenStr;
	strncpy_s(Command->params->p.source_cmd_params.fullFilePath, MAX_PATH, fullFilePath, yymsp[0].minor.yy0.tokenStrLength);
	Command->params->p.source_cmd_params.fullFilePath[SESSION_ID_LENGTH] = '\0';
}
#line 1344 "lemon.commands.c"
        break;
      case 37: /* continue ::= CMD_CONTINUE semicolon cmdexecute CMD_END_OF_FILE */
#line 318 "lemon.commands.yy"
{
	Command->tokenCode = CMD_CONTINUE;
}
#line 1351 "lemon.commands.c"
        break;
      case 38: /* frame ::= CMD_FRAME frameparams semicolon cmdexecute CMD_END_OF_FILE */
#line 326 "lemon.commands.yy"
{
	Command->tokenCode = CMD_FRAME;
}
#line 1358 "lemon.commands.c"
        break;
      case 39: /* frameparams ::= CMD_FRAME_PARAM_FRAMENO */
#line 330 "lemon.commands.yy"
{	
	Command->params->p.frame_cmd_params.frameNo = atoi(yymsp[0].minor.yy0.tokenStr);
}
#line 1365 "lemon.commands.c"
        break;
      case 40: /* backtrace ::= CMD_BACKTRACE backtraceparams semicolon cmdexecute CMD_END_OF_FILE */
#line 338 "lemon.commands.yy"
{
	Command->tokenCode = CMD_BACKTRACE;
}
#line 1372 "lemon.commands.c"
        break;
      case 41: /* backtraceparams ::= CMD_BACKTRACE_PARAM_FRAMECOUNT */
#line 342 "lemon.commands.yy"
{	
	Command->params->p.backtrace_cmd_params.frameCount = atoi(yymsp[0].minor.yy0.tokenStr);
}
#line 1379 "lemon.commands.c"
        break;
      case 42: /* backtraceparams ::= CMD_BACKTRACE_PARAM_OTHERS */
#line 346 "lemon.commands.yy"
{
	char *otherParamsStr = yymsp[0].minor.yy0.tokenStr;
	if (strcmp(otherParamsStr, "full") == 0) {
		Command->params->p.backtrace_cmd_params.full = TRUE;
	}
}
#line 1389 "lemon.commands.c"
        break;
      case 43: /* backtraceparams ::= */
#line 353 "lemon.commands.yy"
{
	Command->params->p.backtrace_cmd_params.frameCount = -1;
	Command->params->p.backtrace_cmd_params.full = FALSE;
}
#line 1397 "lemon.commands.c"
        break;
      case 44: /* attach ::= CMD_ATTACH attachparams semicolon cmdexecute CMD_END_OF_FILE */
#line 362 "lemon.commands.yy"
{
	Command->tokenCode = CMD_ATTACH;
}
#line 1404 "lemon.commands.c"
        break;
      case 45: /* attachparams ::= CMD_ATTACH_PARAM_SESSION_ID */
#line 366 "lemon.commands.yy"
{
	char *lSessionId = yymsp[0].minor.yy0.tokenStr;
	strncpy_s(Command->params->p.attach_cmd_params.sessionId, SESSION_ID_LENGTH+2, lSessionId, SESSION_ID_LENGTH);
	Command->params->p.attach_cmd_params.sessionId[SESSION_ID_LENGTH] = '\0';
}
#line 1413 "lemon.commands.c"
        break;
      case 46: /* attachparams ::= */
#line 372 "lemon.commands.yy"
{
	Command->params->p.attach_cmd_params.sessionId[0] = '\0';
}
#line 1420 "lemon.commands.c"
        break;
      case 47: /* detach ::= CMD_DETACH semicolon cmdexecute CMD_END_OF_FILE */
#line 381 "lemon.commands.yy"
{
	Command->tokenCode = CMD_DETACH;
}
#line 1427 "lemon.commands.c"
        break;
      case 48: /* kill ::= CMD_KILL semicolon cmdexecute CMD_END_OF_FILE */
#line 389 "lemon.commands.yy"
{
	Command->tokenCode = CMD_KILL;
}
#line 1434 "lemon.commands.c"
        break;
      case 49: /* info ::= CMD_INFO infoparams semicolon cmdexecute CMD_END_OF_FILE */
#line 397 "lemon.commands.yy"
{
	Command->tokenCode = CMD_INFO;
}
#line 1441 "lemon.commands.c"
        break;
      case 50: /* infoparams ::= CMD_INFO_PARAM_LOCALS */
#line 401 "lemon.commands.yy"
{
	Command->params->p.info_cmd_params.paramType = CMD_INFO_PARAM_TYPE_LOCALS;
}
#line 1448 "lemon.commands.c"
        break;
      case 51: /* infoparams ::= CMD_INFO_PARAM_ARGS */
#line 405 "lemon.commands.yy"
{
	Command->params->p.info_cmd_params.paramType = CMD_INFO_PARAM_TYPE_ARGS;
}
#line 1455 "lemon.commands.c"
        break;
      case 52: /* infoparams ::= CMD_INFO_PARAM_GLOBALS */
#line 409 "lemon.commands.yy"
{
	Command->params->p.info_cmd_params.paramType = CMD_INFO_PARAM_TYPE_GLOBALS;
}
#line 1462 "lemon.commands.c"
        break;
      case 53: /* infoparams ::= CMD_INFO_PARAM_FRAME */
#line 413 "lemon.commands.yy"
{
	Command->params->p.info_cmd_params.paramType = CMD_INFO_PARAM_TYPE_FRAME;
}
#line 1469 "lemon.commands.c"
        break;
      case 54: /* show ::= CMD_SHOW showparams semicolon cmdexecute CMD_END_OF_FILE */
#line 422 "lemon.commands.yy"
{
	Command->tokenCode = CMD_SHOW;
}
#line 1476 "lemon.commands.c"
        break;
      case 55: /* showparams ::= CMD_SHOW_PARAM_SESSION_COUNT */
#line 426 "lemon.commands.yy"
{
	Command->params->p.show_cmd_params.sessionCountValue = SHOW_CMD_PARAM_SESSION_COUNT_VALUE_SET;
}
#line 1483 "lemon.commands.c"
        break;
      case 56: /* showparams ::= CMD_SHOW_PARAM_VERSION */
#line 430 "lemon.commands.yy"
{
	Command->params->p.show_cmd_params.version = TRUE;
}
#line 1490 "lemon.commands.c"
        break;
      case 57: /* exit ::= CMD_EXIT semicolon cmdexecute CMD_END_OF_FILE */
#line 439 "lemon.commands.yy"
{
	Command->tokenCode = CMD_EXIT;
}
#line 1497 "lemon.commands.c"
        break;
      case 58: /* invalidcommands ::= invalidchars CMD_END_OF_FILE */
#line 448 "lemon.commands.yy"
{
	writeSyntaxError(yymsp[-1].minor.yy125);
}
#line 1504 "lemon.commands.c"
        break;
      case 59: /* invalidcommands ::= invalidchars cmdexecute CMD_END_OF_FILE */
#line 452 "lemon.commands.yy"
{
	writeSyntaxError(yymsp[-2].minor.yy125);
}
#line 1511 "lemon.commands.c"
        break;
      default:
      /* (60) start ::= validcommands */ yytestcase(yyruleno==60);
      /* (61) start ::= invalidcommands */ yytestcase(yyruleno==61);
      /* (62) validcommands ::= validcommands command */ yytestcase(yyruleno==62);
      /* (63) command ::= info (OPTIMIZED OUT) */ assert(yyruleno!=63);
      /* (64) command ::= kill (OPTIMIZED OUT) */ assert(yyruleno!=64);
      /* (65) command ::= exit (OPTIMIZED OUT) */ assert(yyruleno!=65);
      /* (66) command ::= quit (OPTIMIZED OUT) */ assert(yyruleno!=66);
      /* (67) command ::= show (OPTIMIZED OUT) */ assert(yyruleno!=67);
      /* (68) command ::= attach (OPTIMIZED OUT) */ assert(yyruleno!=68);
      /* (69) command ::= detach (OPTIMIZED OUT) */ assert(yyruleno!=69);
      /* (70) command ::= cmdexecute */ yytestcase(yyruleno==70);
      /* (71) command ::= backtrace (OPTIMIZED OUT) */ assert(yyruleno!=71);
      /* (72) command ::= frame (OPTIMIZED OUT) */ assert(yyruleno!=72);
      /* (73) command ::= continue (OPTIMIZED OUT) */ assert(yyruleno!=73);
      /* (74) command ::= source (OPTIMIZED OUT) */ assert(yyruleno!=74);
      /* (75) command ::= break (OPTIMIZED OUT) */ assert(yyruleno!=75);
      /* (76) command ::= tbreak (OPTIMIZED OUT) */ assert(yyruleno!=76);
      /* (77) command ::= delete (OPTIMIZED OUT) */ assert(yyruleno!=77);
      /* (78) command ::= next (OPTIMIZED OUT) */ assert(yyruleno!=78);
      /* (79) command ::= step (OPTIMIZED OUT) */ assert(yyruleno!=79);
      /* (80) command ::= finish (OPTIMIZED OUT) */ assert(yyruleno!=80);
      /* (81) command ::= dummy (OPTIMIZED OUT) */ assert(yyruleno!=81);
      /* (82) command ::= whatis (OPTIMIZED OUT) */ assert(yyruleno!=82);
      /* (83) command ::= output (OPTIMIZED OUT) */ assert(yyruleno!=83);
      /* (84) command ::= set (OPTIMIZED OUT) */ assert(yyruleno!=84);
      /* (85) command ::= catch (OPTIMIZED OUT) */ assert(yyruleno!=85);
      /* (86) command ::= clearallbreakpoints (OPTIMIZED OUT) */ assert(yyruleno!=86);
      /* (87) tbreakdoublequote ::= CMD_TBREAK_PARAM_DOUBLEQUOTE */ yytestcase(yyruleno==87);
      /* (88) invalidchars ::= invalidchars invalidchar */ yytestcase(yyruleno==88);
      /* (89) invalidchars ::= invalidchar (OPTIMIZED OUT) */ assert(yyruleno!=89);
      /* (90) invalidchar ::= CMD_ANY_CHARACTER */ yytestcase(yyruleno==90);
      /* (91) cmdexecute ::= cmdexecute cmdexecutechar */ yytestcase(yyruleno==91);
      /* (92) cmdexecute ::= cmdexecutechar (OPTIMIZED OUT) */ assert(yyruleno!=92);
      /* (93) cmdexecutechar ::= CMD_EXECUTE */ yytestcase(yyruleno==93);
      /* (94) semicolon ::= CMD_SEMI_COLON */ yytestcase(yyruleno==94);
      /* (95) semicolon ::= */ yytestcase(yyruleno==95);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact <= YY_MAX_SHIFTREDUCE ){
    if( yyact>YY_MAX_SHIFT ){
      yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
    }
    yymsp -= yysize-1;
    yypParser->yytos = yymsp;
    yymsp->stateno = (YYACTIONTYPE)yyact;
    yymsp->major = (YYCODETYPE)yygoto;
    yyTraceShift(yypParser, yyact);
  }else{
    assert( yyact == YY_ACCEPT_ACTION );
    yypParser->yytos -= yysize;
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  commandParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  commandParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  commandParseTOKENTYPE yyminor         /* The minor type of the error token */
){
  commandParseARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 26 "lemon.commands.yy"

	Command->errorCode = COMMAND_ERROR_CODE_SYNTAX_ERROR;
#line 1609 "lemon.commands.c"
/************ End %syntax_error code ******************************************/
  commandParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  commandParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  commandParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "commandParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void commandParse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  commandParseTOKENTYPE yyminor       /* The value for the token */
  commandParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  unsigned int yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  yypParser = (yyParser*)yyp;
  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  commandParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos >= yypParser->yystack
            && yymx != YYERRORSYMBOL
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yytos < yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yytos>yypParser->yystack );
#ifndef NDEBUG
  if( yyTraceFILE ){
    yyStackEntry *i;
    char cDiv = '[';
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=&yypParser->yystack[1]; i<=yypParser->yytos; i++){
      fprintf(yyTraceFILE,"%c%s", cDiv, yyTokenName[i->major]);
      cDiv = ' ';
    }
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
