###
### mod to version 3.0          dated 15-oct-1982
###

###
### mod to version 3.0          dated 9-mar-1981
###
### smn prog    module  comments
### === ====    ======  =================================================
### 002 rjw             added to collect in one file a number of macros
###                     used for various STATUS routines
###
###       
#####   version 3.0     dated 22-dec-1980
##
##      Note:  Has been revised for stationary satellites JPK 7/86
##

# 

define ($Max$Lines,400)
define ($ST$Line$Length,80)
define ($ST$Line$words,arith($ST$Line$Length,/,4))

define ($Omron,1)
define ($AnnArbor,2)
define ($TeleVideo,3)
define ($Ramtek,4)

define ($$MaxKeys,5)

#define (STATUScommon,Common /A/_ # (see Common /A/_ , below.. 
define (STATUScommon,               # jb  E2141         1/19/00

LITERAL        nmvue                  ## ascii string for current view
LITERAL lkey,                   ## primary key of item at top of current page
        lkey2                  ## secondary key of item at top of current page
LITERAL nkey,                   ## primary key of item at top of next page
        nkey2                  ## secondary key of item at top of next page
LITERAL ikey,                   ## current primary key
        ikey2                  ## current secondary key

REAL       igmin                  ## saved value of game$minute

Integer  IPAGE[$ST$Line$words,$Max$Lines]    ## page buffer
#        IPAGE1[$ST$Line$words,$Max$Lines],   ## page buffer
#        IPAGE42[$ST$Line$words,$Max$Lines]   ## page buffer
Integer LIMIT,                  ## max number of lines on page
        LINCNT,                 ## number of lines present
        lin1cnt,                ## number of stationary lines present
        lin42cnt,               ## number of orbiting lines present
        ibyte,                  ## running count of line length
        linlen[$Max$Lines],     ## number of characters in each line
        lin1len[$Max$Lines],    ## number of characters in each 1 line
        lin42len[$Max$Lines],   ## number of characters in each 42 line
        KPAGE,                  ## page counter
        IVIEW,                  ## view of current board
        LVIEW,                  ## view of previous board
        MVIEW,                  ## view signed on as
        iside,                  ## side for current board
    mside,                  ## side signed on as      1/8/91 trs
        inmbr                  ## view number for current board
Integer        ismin                  ## saved value of status$minute
Integer nblue,                  ## number of blue views in game
        norng,                  ## number of orange views in game
        lsect                  ## section of board at top of current page
Integer nsect                  ## section of board at top of next page
Integer  isect                  ## current section of board
Integer iovflo,                 ## page overflow flag
        linsav,                 ## pointer to last displayable line
        lin1sav,                ## pointer to last stationry sat line jpk 7/8/86
        lin42sav,               ## pointer to last orbiting sat line jpk 7/8/86
        ibrdrq,                 ## requested board
        isidrq,                 ## requested side
        inbrrq,                 ## requested number
        icntrq,                 ## continue-request flag
        iBOARD,                 ## CURRENT BOARD
        isect2,                 ## jpk 11/4/87
        lsect2,                 ## jpk 11/4/87
        nsect2,                 ## jpk 11/4/87
        rx_count,               ## receiver counter for the unit
        asb_refresh             ## refresh indicator

    Common /status_A/_
        nmvue, lkey, lkey2, nkey, nkey2, ikey, ikey2, # jb  LITERALS 1/19/00

        igmin,                                        # jb  REAL     1/19/00

        IPAGE,      # IPAGE1,IPAGE42,               # jb  INTEGERS: 1/19/00
        LIMIT,LINCNT,lin1cnt,lin42cnt,ibyte,linlen,lin1len,lin42len,
        KPAGE,IVIEW,LVIEW,MVIEW,iside,mside,inmbr,ismin,
        nblue,norng,lsect,nsect,isect,iovflo,
        linsav,lin1sav,lin42sav,ibrdrq,isidrq,inbrrq,icntrq,iBOARD,
        isect2,lsect2,nsect2,rx_count,asb_refresh

#        Literal   nmvue,
#                  ikey,
#                  ikey2,
#                  lkey,
#                  lkey2,
#                  nkey,
#                  nkey2

#      Integer    rx_count,
#                 asb_refresh
        
#       Real       igmin
    )

define (OmronCommon,
        Integer Lpage[$ST$Line$words,$Max$Lines],Llen[$Max$Lines],llcnt,iterm
        Logical Telnet,Hrdcpy,Refrsh,First,Clear

        Common /OmronC/ Lpage,Llen,llcnt,iterm,
                        Telnet,Hrdcpy,Refrsh,First,Clear
       )

define (InventoryCommon,
        literal sam_names(10)           # SMM 4/98 (E1506)
        integer number_of_sams          # SMM 4/98 (E1506)

        Integer nbreq, iquant[100]             # clk 7/27/94 (E1315)
        Literal nameq[100],namac[100]     # The size of nbrrdy cased iquant and
        Integer nbrrdy[40,8]              # nbreq to be stomped on, so the order
        Equivalence (namac,nameq)         # was rearanged.  Nbrrdy can start at
        Equivalence (nbrrdy,nameq[41])    # nameq[41] since only 40 AC types are
                                          # permitted.
        Common/InvC/sam_names, number_of_sams, nbreq, iquant, nameq
       )

define ($column,ibyte=-1+)
define ($over,ibyte=ibyte+)
define ($prepare$next$line,{
                    lincnt=lincnt+1
                    linlen[lincnt] = 0
                    call lib$movc5 (1,BLANK,BLANK,$ST$Line$Length,ipage[1,lincnt])
                    #for(ixcl=1;ixcl<=20;ixcl=ixcl+1)ipage[ixcl,lincnt]=" "
                   })
define ($prepare$next$1line,{                      # jpk 7/8/86
                    lin1cnt=lin1cnt+1
                    lin1len[lin1cnt] = 0
                    call lib$movc5 (1,BLANK,BLANK,40,ipage[1,lin1cnt])
                    #for(ixcl=1;ixcl<=10;ixcl=ixcl+1)ipage[ixcl,lin1cnt]=" "
                   })
define ($prepare$next$42line,{                      # jpk 7/8/86
                    lin42cnt=lin42cnt+1
                    lin42len[lin42cnt] = 0
                    call lib$movc5 (1,BLANK,BLANK,44,ipage[10,lin42cnt])
                    #for(ixcl=10;ixcl<=20;ixcl=ixcl+1)ipage[ixcl,lin42cnt]=" "
                   })
define ($save$line$count,linsav=lincnt)
define ($save$1line$count,lin1sav=lin1cnt)            # jpk 7/8/86
define ($save$42line$count,lin42sav=lin42cnt)         # jpk 7/8/86

define ($match$line$length, {                         # jpk 7/8/86
            if (lin1cnt>lin42cnt) iline=lin1cnt
            else                  iline=lin42cnt
            for (lincnt=6;lincnt<=iline;lincnt=lincnt+1)
               {
                if (lin42len[lincnt]>0) linlen[lincnt]=lin42len[lincnt]
                else                    linlen[lincnt]=lin1len[lincnt]
               }
            linlen[lincnt] = 0
        for(ixcl=1;ixcl<=$ST$Line$words;ixcl=ixcl+1)ipage[ixcl,lincnt]=$BLANKI4
        })
define ($match$1line$count,lin1cnt=lincnt)            # jpk 7/8/86
define ($match$42line$count,lin42cnt=lincnt)          # jpk 7/8/86
define ($clear$restof$screen, {                       # jpk 7/9/86
              if (lin1cnt>lin42cnt) 
                 {     
                  jline = lin1cnt-lin42cnt
                  for (i=1;i<=(jline);i=i+1) $prepare$next$42line
                 }
              else if (lin42cnt>lin1cnt) 
                 {
                  jline = lin42cnt-lin1cnt
                  for (i=1;i<=(jline);i=i+1) $prepare$next$1line
                 }})

define ( $Clear$Screen$Bottom,  {       # jb <<<>>>             2/19/87
        jline = LIMIT - LinCnt
        if (jline > 0 )
         {
          for( i=1; i<=(jline); i=i+1 ) 
           {
            $prepare$next$line
           }
         }                      })

define ($test$next$line,{if(lincnt>=limit)iovflo=$yes})
define ($test$next$1line,{if(lin1cnt>=limit)iovflo=$yes})   # jpk 7/8/86
define ($test$next$42line,{if(lin42cnt>=limit)iovflo=$yes}) # jpk 7/8/86
define ($exit$on$overflow,{if(iovflo!=$no)return})
define ($top$of$page,lincnt=0)
define ($SET$overflow,iovflo=$yes)
define ($clear$overflow,iovflo=$no)
define ($beyond$column,ibyte>)
define ($current$column,ibyte)
define ($restore$saved$line$count,lincnt=linsav)
define ($increment$keys,call litinc($low$order$key) )
define ($save$keys,{
                    nsect=isect
                    nsect2=isect2
                    nkey =ikey
                    nkey2=ikey2
                   })
define ($update$keys,{
                      $save$keys
                      $increment$keys
                     })
define ($at$line,lincnt==)
define ($at$top,(lincnt==2))
define ($highest$ascii,'zzzzzzzz')
define ($lowest$ascii,"2020202020202020"X)

define ($Blue$Designator,$letB)
define ($Orange$Designator,$letO)
define ($Sosus$Designator,$letZ)
define ($Hfdf$Designator,$letH)
define ($SWABS$Designator, $letT)



#______________________________________________________________________
# OUTH Macros

define ($OutV,{
    call smovV(@1,ipage[1,lincnt],ibyte)
    linlen[lincnt] = ibyte
    })

define ($OutVt,{
    call smovV(@1,ipage[1,lincnt],ibyte)
    linlen[lincnt] = ibyte
    })

define ($OutH,{
    call smovh(@1,@2,ipage[1,lincnt],ibyte)
    linlen[lincnt] = ibyte
    })

define ($OutH1,{
    call smovh(@1,@2,ipage[1,lin1cnt],ibyte)
    lin1len[lin1cnt] = ibyte
    })

define ($OutV1,{                         # jb E2141 (added)     1/19/00
    call smovV(@1,ipage[1,lin1cnt],ibyte)
    lin1len[lin1cnt] = ibyte
    })

define ($OutH42,{
    call smovh(@1,@2,ipage[1,lin42cnt],ibyte)
    lin42len[lin42cnt] = ibyte
    })

define ($OutV42,{                        # jb E2141 (added)     1/19/00
    call smovV(@1,ipage[1,lin42cnt],ibyte)
    lin42len[lin42cnt] = ibyte
    })

define($OutHN,{
    call smovhn(@1,@2,ipage[1,lincnt],ibyte)
    linlen[lincnt] = ibyte
    })

define($OutVN,{                          # jb E2141 (added)     1/19/00
    call smovVn(@1,ipage[1,lincnt],ibyte)
    linlen[lincnt] = ibyte
    })

define($OutI,{
    call smovi(@1,ipage[1,lincnt],ibyte)
    linlen[lincnt] = ibyte
    })

define($OutF,{
    call smovf(@1,@2,ipage[1,lincnt],ibyte)
    linlen[lincnt] = ibyte
    })

define ($OutF_Abrev, {      # joa 3/96 (E1467)
    call smovf_abrev (@1, ipage[1,lincnt], ibyte)
    linlen[lincnt] = ibyte
    })

define($OutI1,{
    call smovi(@1,ipage[1,lin1cnt],ibyte)
    lin1len[lin1cnt] = ibyte
    })

define($OutI42,{
    call smovi(@1,ipage[1,lin42cnt],ibyte)
    lin42len[lin42cnt] = ibyte
    })

define($OutIB,{
    call smovib(@1,@2,ipage[1,lincnt],ibyte)
    linlen[lincnt] = ibyte
    })

define($OutIB1,{
    call smovib(@1,@2,ipage[1,lin1cnt],ibyte)
    lin1len[lin1cnt] = ibyte
    })

define($OutIB42,{
    call smovib(@1,@2,ipage[1,lin42cnt],ibyte)
    lin42len[lin42cnt] = ibyte
    })

define($OutIZ,{
    call smoviz(@1,@2,ipage[1,lincnt],ibyte)
    linlen[lincnt] = ibyte
    })

define($OutIZ1,{
    call smoviz(@1,@2,ipage[1,lin1cnt],ibyte)
    lin1len[lin1cnt] = ibyte
    })

define($OutIZ42,{
    call smoviz(@1,@2,ipage[1,lin42cnt],ibyte)
    lin42len[lin42cnt] = ibyte
    })

define($OutLL,{
    call smovll(@1,@2,ipage[1,lincnt],ibyte)
    linlen[lincnt] = ibyte
    })

define($OutLLS,{
    call smovlls(@1,@2,ipage[1,lincnt],ibyte)
    linlen[lincnt] = ibyte
    })

define($OutLL1,{
    call smovll(@1,@2,ipage[1,lin1cnt],ibyte)
    lin1len[lin1cnt] = ibyte
    })

define($OutLL42,{
    call smovll(@1,@2,ipage[1,lin42cnt],ibyte)
    lin42len[lin42cnt] = ibyte
    })

define($Out,{
    call smov(@1,ipage[1,lincnt],ibyte)
    linlen[lincnt] = ibyte
    })

#______________________________________________________________________
# SMOV Macros

# Commented out for ALPHA Execution (No Assembly code wanted)
#define (smovH,xsmovh(%%ref(@1),@2,@3,@4))
#define (smovHN,xsmovhN(%%ref(@1),@2,@3,@4))
#define (smovi,xsmovi(@1,@2,@3))
#define (smovib,xsmovib(@1,@2,@3,@4))
#define (smoviz,xsmoviz(@1,@2,@3,@4))
#define (smov,xsmov(%%ref(@1),@2,@3))
