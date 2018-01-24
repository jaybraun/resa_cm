#ifdef WIN32
#pragma warning( disable : 4100 4101 4102 4013 4131 4189 4244 4706)   /*  un refed parameter  */
#pragma warning( disable : 4001 )   /*  single line comment */
#pragma warning( disable : 4505 )   /*  unrefed function    */
#endif

#ifdef I64
#define IN_T long
#else
#define IN_T int
#endif



#include <stdio.h>
#include <string.h>
# define U(x) x
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# define YYLMAX 200
# define output(c) putc(c,yyout)
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
# define ECHO fprintf(yyout, "%s",yytext)
# define REJECT { nstr = yyreject(); goto yyfussy;}
static IN_T yyleng=0; static  char yytext[YYLMAX];
static IN_T yymorfg=0;
static char yysbuf[YYLMAX]; static char *yysptr=yysbuf;
static IN_T yytchar=0;
static FILE *yyin ={0};
//static FILE *yyout ={0};
static IN_T yylineno;
struct yysvf { 
	struct yywork *yystoff;
	struct yysvf *yyother;
	IN_T *yystops;};
static struct yysvf *yyestate;
#define dreturn return
#define  printf3(a,b,c)
#define xprintf3(a,b,c) printf(a,b,c)
#define  printf2(a,b)
#define xprintf2(a,b)   printf(a,b)
#if 1
#undef output
#undef input
#undef unput
# define output(c)

# define xinput() (((yytchar=yysptr>yysbuf?U(*--yysptr):args_cv(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
# define xunput(c) {yytchar= (IN_T)(c);if(yytchar==((IN_T)'\n'))yylineno--;*yysptr++=((unsigned char)yytchar);}
#endif
//static char *yylval;
static int yywrap();
static int yylook();
static int yyback();

# define YYNEWLINE 10
static int yylex(){
IN_T nstr; 
while((nstr = yylook()) >= 0)
//yyfussy: 
switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:
        {     printf2("HYPHEN\n", yytext );
                        xunput('!')
                        xunput('!')
                        dreturn(5);
                  }
break;
case 2:
 {   yytext[yyleng-1] = '\0';
                      printf2("stuff1{%s}\n", yytext );
                      xunput('-')
                      dreturn(2);

                  }
break;
case 3:
       { printf2("qqstuff{%s}\n", yytext );
                            dreturn(4);
                  }
break;
case 4:
       { printf2("qstuff{%s}\n", yytext );
                      dreturn(3);

                  }
break;
case 5:
          { printf2("stuff2{%s}\n", yytext );
                      dreturn(2);

                  }
break;
case 6:
              {   printf2("COMMA\n", yytext );
                        dreturn(1);
                  }
break;
case 7:
             {   printf2("NL\n", yytext );
                        dreturn(0);
                  }
break;
case 8:
         {   printf2("whitespace{%s}\n", yytext );  /* eat up whitespace */     }
break;
case 9:
        {   printf2("comments  {%s}\n", yytext );  /* eat up comments   */     }
break;
case 10:
                {   printf2( "Unrecognized character: {%s}\n", yytext ); }
break;
case -1:
break;
default:
/*fprintf(yyout,"bad switch yylook %d",nstr);*/;
} return(0); }
/* end of yylex */
static int yywrap() {return 1;}
static char alspbuf[1024];
static char *alspbufp;
/*
 *
 *             args_cv_get  :  Get a token - This will return two values; The
 *                             function return value is a token type as follows:
 *
 *
 *
 *                             NL     0     - a newline
 *                             COMMA  1     - comma
 *                             STRING 2     - simple string, not quoted
 *                             SQUOTE 3     - string in single quotes
 *                             DQUOTE 4     - string in  double quotes
 *                             HYPHEN 5     - a "-"  as a token
 *
 *
 *
 *
 *
 *                          str    -   the output string; a copy of the token
 *                                     that was found. If quoted, the quotes
 *                                     are included at the beginning and end
 *
 */
   IN_T args_cv_get (str) char *str;
{
        IN_T i;

        i = yylex();
        strcpy(str,yytext);
        return i;


}
/*
 *
 *             args_cv_init :  Supply an input string to be processed, i.e.
 *                             to be broken up into tokens. This string should
 *                             end with a '\n' (newline) character.
 *
 *
 *
 *
 *                          str    -   the input string to be processed
 *
 *
 */
void args_cv_init(str) char *str;
{
//        IN_T c;

        yysptr=yysbuf;          /* reset put back buffer to empty */
        strcpy(alspbuf,str);
        alspbufp = alspbuf;
/*      printf("reset\n");      */
}
static IN_T args_cv(in)  FILE *in;
{
        IN_T c;

        c = *alspbufp++;
/*      printf("get '%c'\n",c);   */
        if (c == '\0') {
                c = '\n';
                alspbufp--;
        }
        return c;
}
#if 1
static void unput(c) IN_T c;

{
        xunput(c);
/*      printf("pbk '%c'\n",c);   */

}
static IN_T input()
{

        IN_T c;
        c = xinput();
/*      printf("inp '%c'\n",c);   */
        return c;
}
#endif

#define NL     0
#define COMMA  1
#define STRING 2
#define SQUOTE 3
#define DQUOTE 4
#define HYPHEN 5
/*
 *    tokens:  Take an input null terminated string and return a count of
 *             tokens (as the function return value) and an array of
 *             pointers to null terminated strings (the tokens). If the last
 *             token is a hyphen ("-"), the count is negated.
 *
 *
 *
 *
 *             str      -       input string to break into pieces
 *             av       -       input pointer to array of char pointers
 *                              (this must be large enough)
 *             out      -       array of char where the pieces are placed
 *                              (this must be large enough)
 *
 *
 *
 *    grammar rules:
 *
 *            A token is either a word without blanks or it is a string
 *            in quotes (either single or double quotes). The tokens may
 *            be separated by either spaces or commas. the only difference
 *            betweeen spaces and commas is that commas can be used to indicate
 *            null arguments. A comment may appear beginning with a "!" and
 *            runs to the end of the input. If the ! is in a quoted string, then
 *            it is not taken as the comment initiator.
 *
 *            token1 'token 2' "token!3",,"token 5"!this is a comment
 *
 *
 *            would result in 5 for the return value and an array of 5 strings
 *
 *
 *             0 {token1}
 *             1 {token 2}
 *             2 {token!3}
 *             3 {}
 *             4 {token 5}
 *
 *            note that the quotes are stripped. The {} is used here to show
 *            the beginning and end of the strings.
 *
 *            A trailing comma is ignored; multiple trailing commas are not -
 *            only the last trailing comma is ignored. To indicate a null arg
 *            at the end of a line, one must use a pair of quotes.
 *
 *            A - (hyphen) is permitted to indicate that the line is continued
 *            This results in a negative count for the number of arguments.
 *
 *            The idea behind this is that if you get a negative arg count
 *            then you should continue reading input and concatonating arg
 *            vectors. See routine cat_tokens which does this.
 *
 *            A hyphen is like a comment, all chars following the hypen are
 *            ignored whether or not there is a !comment following.
 *
 */

int tokens(char *str, char **av, char *out) {

//       IN_T n = 0;
       IN_T i,c=0,l,a=0,hflag=0;
       char str2[1000],*p;

       args_cv_init(str);
       p = out;
       for(;;) {
           i = args_cv_get(str2);
           /* printf("Type=%d {%s}\n",i,str2);  */
           if (i == COMMA) {
                c++;
              if (c > 1) {
                   strcpy(p,"");
                   av[a++] = p;
                   p = p+1;
              }
           } else if ((i == DQUOTE) || (i == SQUOTE)) {
                c = 0;
                l = strlen(str2);
                str2[l-1] = '\0';
                strcpy(p,&str2[1]);
                av[a++] = p;
                p = p+l;
           } else if (i == STRING) {
                c = 0;
                l = strlen(str2);
                strcpy(p,str2);
                av[a++] = p;
                p = p+l+1;
           } else if (i == HYPHEN) {
                hflag = 1;
           } else if (i == NL) {
#if 0
                if (c > 0) {
                   strcpy(p,"");
                   av[a++] = p;
                   p = p+1;
                }
#endif
                break;
           }

       }
       if (hflag) {
         a = -a;
       }
       return a;
}
/*
 *
 *      cat_tokens - allows concatonation of argc/argv type count/vectors
 *                   to be built up by passing in the prior arg count when
 *                   it was negative.
 *
 *
 *                   str - new string to be scanned
 *                   pn  - number from last time, if neg, a concat occurs
 *                         if 0 or pos, then starts over
 *
 *                   av  - output argv vector
 *                   out - scratch string, note that on a concat, the prior
 *                         av vector and the number -pn are used to determine
 *                         where available scratch space begins
 */
IN_T cat_tokens(str,pn,av,out) char *str; IN_T pn; char **av; char *out;
{
        IN_T m,l,n; char *newout;

/*      printf("last n = %d\n",pn);  */

        n = pn;
        if (n >= 0) {
           m  = tokens(str,av,out);
        } else {
           n = -n;
           l = strlen(av[n-1]);
           newout = av[n-1] + l + 2;
           m  = tokens(str,&av[n],newout) ;
           /* printf("m = %d\n",m);     */
           if (m == 0) {
               m = pn;        /* a null line, just continue */
           } else if (m < 0) {
               m =  m -n;
               /*   printf("m1= %d\n",m); */
           } else {
               m = m + n;
               /*   printf("m2= %d\n",m); */
           }
        }
        return m;



}
static IN_T yyvstop[] ={
0,

5,
10,
-2,
0,

8,
10,
0,

7,
0,

10,
0,

10,
0,

10,
0,

6,
10,
0,

1,
5,
10,
-2,
0,

5,
-2,
0,

2,
5,
-2,
0,

8,
0,

9,
0,

3,
0,

4,
0,

1,
2,
5,
-2,
0,
0};
# define YYTYPE char
static struct yywork { YYTYPE verify, advance; } yycrank[] ={
{0,0},	{0,0},	{1,3},	{0,0},	
{0,0},	{0,0},	{3,11},	{0,0},	
{0,0},	{0,0},	{1,4},	{1,5},	
{14,0},	{15,0},	{3,0},	{3,0},	
{17,0},	{0,0},	{4,13},	{6,14},	
{0,0},	{7,15},	{0,0},	{8,17},	
{0,0},	{0,0},	{0,0},	{6,14},	
{6,0},	{7,15},	{7,0},	{8,17},	
{8,0},	{0,0},	{1,6},	{1,7},	
{2,6},	{3,0},	{3,0},	{3,0},	
{1,8},	{4,13},	{10,0},	{10,0},	
{3,0},	{1,9},	{1,10},	{2,9},	
{2,10},	{3,0},	{3,12},	{0,0},	
{6,14},	{0,0},	{7,16},	{0,0},	
{8,17},	{6,14},	{0,0},	{7,15},	
{0,0},	{8,18},	{11,0},	{11,0},	
{0,0},	{10,0},	{10,0},	{10,0},	
{0,0},	{0,0},	{12,0},	{12,0},	
{10,0},	{0,0},	{0,0},	{0,0},	
{0,0},	{10,0},	{10,19},{19,0},	
{19,0},	{0,0},	{0,0},	{0,0},	
{0,0},	{11,0},	{11,0},	{11,0},	
{0,0},	{0,0},	{0,0},	{0,0},	
{11,0},	{12,0},	{12,0},	{12,0},	
{0,0},	{11,0},	{11,12},{0,0},	
{12,0},	{0,0},	{19,0},	{19,0},	
{19,0},	{12,0},	{12,12},{0,0},	
{0,0},	{19,0},	{0,0},	{0,0},	
{0,0},	{0,0},	{19,0},	{19,19},	
{0,0}};
static struct yysvf yysvec[] ={
{0,	0,	0},
{yycrank+-1,	0,		0},
{yycrank+-3,	yysvec+1,	0},
{yycrank+-5,	0,		yyvstop+1},
{yycrank+9,	0,		yyvstop+5},
{yycrank+0,	0,		yyvstop+8},
{yycrank+-18,	0,		yyvstop+10},
{yycrank+-20,	0,		yyvstop+12},
{yycrank+-22,	0,		yyvstop+14},
{yycrank+0,	0,		yyvstop+16},
{yycrank+-33,	yysvec+3,	yyvstop+19},
{yycrank+-53,	yysvec+3,	yyvstop+24},
{yycrank+-61,	yysvec+3,	yyvstop+27},
{yycrank+0,	yysvec+4,	yyvstop+31},
{yycrank+-2,	yysvec+6,	yyvstop+33},
{yycrank+-3,	yysvec+7,	0},
{yycrank+0,	0,		yyvstop+35},
{yycrank+-6,	yysvec+8,	0},
{yycrank+0,	0,		yyvstop+37},
{yycrank+-70,	yysvec+3,	yyvstop+39},
{0,	0,	0}};
static struct yywork *yytop = yycrank+115;
static struct yysvf *yybgin = yysvec+1;
static char yymatch[] ={
00  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,011 ,012 ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
011 ,011 ,'"' ,01  ,01  ,01  ,01  ,047 ,
01  ,01  ,01  ,01  ,011 ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
0};
static char yyextra[] ={
0,0,1,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
# define YYU(x) x
# define NLSTATE yyprevious=YYNEWLINE
static struct yysvf *yylstate [YYLMAX]={0}, **yylsp={0}, **yyolsp={0};
static IN_T *yyfnd=0;
static IN_T yyprevious = YYNEWLINE;
static int yylook(){
	struct yysvf *yystate, **lsp;
	struct yywork *yyt;
	struct yysvf *yyz;
	IN_T yych;
	struct yywork *yyr;
# ifdef LEXDEBUG
	IN_T debug;
# endif
	char *yylastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	if (!yymorfg)
		yylastch = yytext;
	else {
		yymorfg=0;
		yylastch = yytext+yyleng;
		}
	for(;;){
		lsp = yylstate;
		yyestate = yystate = yybgin;
		if (yyprevious==YYNEWLINE) yystate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(yyout,"state %d\n",yystate-yysvec-1);
# endif
			yyt = yystate->yystoff;
			if(yyt == yycrank){		/* may not be any transitions */
				yyz = yystate->yyother;
				if(yyz == 0)break;
				if(yyz->yystoff == yycrank)break;
				}
			*yylastch++ = yych = input();
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"char ");
				allprint(yych);
				putchar('\n');
				}
# endif
			yyr = yyt;
			if ( (IN_T)yyt > (IN_T)yycrank){
				yyt = yyr + yych;
				if (yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput((IN_T)*--yylastch);break;}

						 yystate = yyt->advance+yysvec;
						 *lsp++ =  yystate;
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((IN_T)yyt < (IN_T)yycrank) {		/* r < yycrank */
				yyt = yyr = yycrank+(yycrank-yyt);
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"compressed state\n");
# endif
				yyt = yyt + yych;
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput((IN_T)*--yylastch);break;}

					 yystate = yyt->advance+yysvec;
					 *lsp++ =  yystate;
					goto contin;
					}
				yyt = yyr + YYU(yymatch[yych]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"try fall back character ");
					allprint(YYU(yymatch[yych]));
					putchar('\n');
					}
# endif
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transition */
						{unput((IN_T)*--yylastch);break;}
					yystate = yyt->advance+yysvec;
					*lsp++ =  yystate;

					goto contin;
					}
				}
			if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != yycrank){
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput((IN_T)*--yylastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"state %d char ",yystate-yysvec-1);
				allprint(yych);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
			allprint(yych);
			putchar('\n');
			}
# endif
		while (lsp-- > yylstate){
			*yylastch-- = 0;
			if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0){
				yyolsp = lsp;
				if(yyextra[*yyfnd]){		/* must backup */
					while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate){
						lsp--;
						unput((IN_T)*yylastch--);
						}
					}
				yyprevious = YYU(*yylastch);
				yylsp = lsp;
				yyleng = yylastch-yytext+1;
                                yytext[yyleng] = 0;
# ifdef LEXDEBUG2
                                if(deb2&& (!inmacro)) {
                                   if(slookup(defines,yytext)==0) {
                                      if(yytext[0] != '"')eprintf("%s",yytext);
                                   }
                                }
# endif
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"\nmatch ");
					sprint(yytext);
					fprintf(yyout," action %d\n",*yyfnd);
					}
# endif
				return(*yyfnd++);
				}
			unput((IN_T)*yylastch);
			}
		if (yytext[0] == 0  /* && feof(yyin) */)
			{
			yysptr=yysbuf;
			return(0);
			}
		yyprevious = yytext[0] = input();
		if (yyprevious>0)
			output(yyprevious);
		yylastch=yytext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
static int yyback(p, m)
	IN_T *p;
{
if (p==0) return(0);
while (*p)
	{
	if (*p++ == m)
		return(1);
	}
return(0);
}

#ifdef LEX_LIB
	/* the following are only used in the lex library */
static yyinput(){
	return(input());
	}
static void yyoutput(c)
  IN_T c; {
	output(c);
	}
static void yyunput(c)
   IN_T c; {
	unput(c);
	}
#endif
