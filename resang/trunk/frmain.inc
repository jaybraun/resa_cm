###                    FRMAIN.inc
### modification of wes version 3.1 to the VAX 11/780
###
###    started 29-october-81
###
### number prog    module      comments
### =====  ====    ======      =========================================
### #VAX00 rak     force.inc   Changes to the defines to adjust to the
###                            differences in word size between the
###                            VAX 11/780 and the DEC-20
###                            Character strings are stored in longwords
###                            and integers are stored in longwords
###                            that overlay the first half of quadword
###                            in arrays.


###
### mod to version 3.0          dated 07-jan-1981
###
### smn prog    module  comments
### === ====    ======  =================================================
### 002 dew             increased max number of orders to 100 (from 50)
###                     added reporting policy matrix
###


#####   version 3.0  dated- 22dec80


# 

define($DEBUGM,#)              # debug statements for Mainline Force
define($DEBUGI,#)              # debug statements for FINIT.rat
define($DEBUGF,#)              # debug for FDopen and FDclos
define($DEBUGE,#)              # debug statements for FENTER.rat
define($DEBUGB,#)              # debug statements in the FBYE.rat code
define($DEBUGP,#)              # debug statements in the FPRINT.rat code
define($DEBUGEP,#)             # debug statements for FEPLAT.rat
define($DEBUGD,#)              # debug statements for FDELET.rat

include "macros.inc"         

undefine(path)                 # remove path =  from software tools     
undefine(read)                 # remove read =  from software tools     
undefine(write)                # remove write =  from software tools    
undefine(err)                  # remove err =  from software tools      
                                                                        
define($intrealmatch,2)        # Used to make long word integer overlays
                               # a quad word.                           
define($halfquad,1)            # First half of a quad word is used as an
                               # integer.                               
                                                                        
                                                                        
###                                                                     

# define maximum numbers of everything

#define($WORKFILESIZE,80000)
define($WORKFILESIZE,100000)
define($files,53)
define($ParserRecLen,$stk$lastarg)    #  length of physical records     
                                      #    produced by the parser       
define($forces,2000)                  # jb    3/17/89
define($forceslen,12)                 # KHC 6/1/92
define($shore,150)                    # jb 150 vice 85        3/26/91
define($shorelen,5)
define($site,1000)                    # max number of sites     MJF 3/26/92
define($sitelen,5)                    #                         MJF 3/26/92
define($class,1000)                   # max ship classes  # joa YS2012
define($classlen,5)
define($groundunit,100)               #jfr  3/29/91 -- characteristics
define($groundunitlen,5)              #jfr  3/29/91 -- characteristics
define($org,100)                      #JFR  5/30/91 -- ground forces
define($orglen,21)                    #JFR  5/30/91 -- for upward compat.
define($cbi,8)                        #JFR 5/14/91 -8 formations = 8x8 CBI array
define($att,1280)                     #JFR 5/14/91 (64x20)
define($vehicle,100)                 #JFR 5/24/91 - 500 in old JC3S version
define($vehiclelen,5)                 #JFR 5/24/91
define($orders,100)                   #smn002
define($orderslen,$ParserRecLen)
define($plans,100)
define($planslen,$ParserRecLen)
define($plansize,30)
define($search,100)
define($searchlen,$ParserRecLen)
define($searchsize,30)
define($action,100)
define($actlen,$ParserRecLen)
define($actsize,20)
define($weather,26)
define($airlog,3000)        # reh 4/89
define($boatlog,2000)       # joa 5/03 (E2343)
define($sosus,20)
define($sosuslen,20)
define($SWABSregions,100)           # clk 10/4/92  (50-Blue and 50-Orange)
define($SWABS$Orange$offset, 50)   # Orange SWABS region + offset
define($sonev,20)
define($snrenv,4)
define($LIDARregions,10)        # smm & clk 2/7/91 max. LIDAR regions
define($LDR$Default$Region, 99)
define($survsats,10)
define($survsatslen,10)
define($hfdf,20)
define($hfdflen,20)
define($policy,100)                            #smn002
define($policylen,100)
define($air,900)
define($airlen,5)
define($cruise,255)     # joa 8/14
define($cruiselen,5)
define($commbuoy,10)
define($commbuoylen,5)
define($commsuite,30)
define($commsuitelen,5)
define($comjm,50)                       # REH 03/01/88 # 50 vice 30 for jecewsi - JFR 8/3/94
define($comjmlen,5)                     # REH 03/01/88
define($ground,300)                     # clk 8/17/92 (at UFL) 
define($groundlen,5)
define($chaff,20)                        # jb    7/10/90
define($chafflen,20)                     # jb    7/10/90
define($commpair,100)
define($commpairlen,5)
define($paths,40)
define($pathslen1,6)                              #  data for one path 
define($pathslen2,8)                              #  dimension for nodes
                                                  #  making up a path   
define($jammers,120)                    # reh 4/89 # 50 vice 20 for jecewsi - JFR 8/3/94
define($jammerslen,5)
define($fomplan,10)                    # reh 6/91
define($fomplanlen,12)
define($maxFomItems,10)
define($MuRho,1)                        # REH 02/29/88 Used in comm jammer
define($MuRholen,102)                   # REH 02/29/88 
define($UVP,24)                         # REH 02/02/90 Used in UAV model 
define($UVPLen,14)                      # REH 02/02/90
define($emcon,40)
define($emconlen,3)
define($on,1)
define($off,0)
define($navaid,40)
define($navaidlen,5)
define($sensors,1023)                    # reh 3/11/89
define($sensorslen,5)
define($sonar,125)
define($sonarlen,5)
define($sonobuoy,40)
define($sonobuoylen,5)
define($weapons,1023)    # Should always be largest number of all types of objects
define($weaponslen,5)
define($sats,10)
define($satslen,5)
define($emitters,1023)
define($emitterslen,2)
define($members,400)
define($membersize,440)                # this should be 40 more than $members
define($ton,1000)                    # reh 4/89
define($tonlen,5)
define($eph,1024)
define($mdr,1000)         # 6/90    MacroASW model
define($mdrlen,6)         # 6/90
define($Intra,512)        # 8/90    Intra platform EMI model
define($Intralen,4)       # 8/90
define($rothr,8)        # 8 rothr tables possible RJS 8/86, CML 2/90
define($trpdo,50)         # SMM 4/90
define($trpdolen,5)       # SMM 4/90
define($ctrme,50)         # SMM 4/90
define($ctrmelen,5)       # SMM 4/90
define($iffs,40)          # CML 3/91  maximum number of IFFs
define($iffslen,5)        # CML 3/91
define($bpairs,100)       # CML 6/91  maximum number of bistatic pairs
define($MaxSquadrons, 1000)   # clk 4/95 - maximum number of squadron names allowed

# GOUND UNIT FORMATIONS --  JFR 5/16/91
define($onroad$formation,1)           # JFR 5/16/91
define($column$formation,2)           # JFR 5/16/91  
define($line$formation,3)             # JFR 5/16/91
define($wedge$formation,4)            # JFR 5/16/91
define($vee$formation,5)              # JFR 5/16/91
define($circle$formation,6)           # JFR 5/16/91
define($echelonleft$formation,7)      # JFR 5/16/91
define($echelonright$formation,8)     # JFR 5/16/91

# define fields

define(force$file," .frc")
define(force$index,kforce)
define(force$task,   wiforce[$halfquad,1,force$index])
define(force$name,   wrforce[2,force$index])          
define(force$class,  wrforce[3,force$index])          
define(force$lat,    wiforce[$halfquad,4,force$index])
define(force$long,   wiforce[$halfquad,5,force$index])
define(force$course, wiforce[$halfquad,6,force$index])
define(force$speed,  wiforce[$halfquad,7,force$index])
define(force$guide,  wrforce[8,force$index])
define(force$range,  wiforce[$halfquad,9,force$index])
define(force$bearing,wiforce[$halfquad,10,force$index])
define(force$depth,  wiforce[$halfquad,11,force$index])
define(force$indicator, wiforce[$halfquad,12,force$index])        #KHC 6/1/92 - indicate a shore base, ship, or site


##################################################################
# FORCE DEFINES FOR GROUND UNITS IN .ORG FILE ---JFR 5/30/91
#   
define(org$file," .org")       #JFR  -- 5/30/91
define(org$index,korg)         #JFR -- 5/30/91
define(org$task,                    wiorg[$halfquad,1,org$index])  #JFR 6/3/91
define(org$name,                    wrorg[2,org$index])  #JFR 6/3/91
define(org$groundunit$name,         wrorg[3,org$index])            #jfr
define(org$source$zone$number,      wiorg[$halfquad,4,org$index])  #jfr
define(org$source$zone$letter,      wrorg[5,org$index])            #jfr
define(org$source$easting,          wiorg[$halfquad,6,org$index])  #jfr
define(org$source$northing,         wiorg[$halfquad,7,org$index])  #jfr
define(org$source$latitude,         wiorg[$halfquad,8,org$index])  #jfr
define(org$source$longitude,        wiorg[$halfquad,9,org$index])  #jfr
define(org$destination$latitude,    wiorg[$halfquad,10,org$index])  #jfr
define(org$destination$longitude,   wiorg[$halfquad,11,org$index])  #jfr
define(org$destination$zone$number, wiorg[$halfquad,12,org$index])  #jfr
define(org$destination$zone$letter, wrorg[13,org$index])            #jfr
define(org$destination$easting,     wiorg[$halfquad,14,org$index])  #jfr
define(org$destination$northing,    wiorg[$halfquad,15,org$index])  #jfr
define(org$formation,               wiorg[$halfquad,16,org$index])  #jfr
define(org$parent$name,             wrorg[17,org$index])            #jfr
define(org$xoffset,                 wiorg[$halfquad,18,org$index])  #jfr
define(org$yoffset,                 wiorg[$halfquad,19,org$index])  #jfr
define(org$course,                  wiorg[$halfquad,20,org$index])  #jfr
define(org$speed,                   wiorg[$halfquad,21,org$index])  #jfr
######################################################################

define(path$file," .nod")
define(path$index,kpath)
define(node$index,knode)
define(path$name, wrpaths[1,1,path$index])
define(path$nodes,wipaths[$halfquad,2,1,path$index])                    
define(path$ckt,  wrpaths[6,1,path$index])                    
define(path$cat,  wrpaths[1,node$index,path$index])
define(path$class,wrpaths[2,node$index,path$index])
define(path$rho,  wrpaths[3,node$index,path$index])           
define(path$mu,   wrpaths[4,node$index,path$index])           
define(path$pair, wrpaths[5,node$index,path$index])
define(path$roger,wrpaths[6,node$index,path$index])

#$% frequency omission plan 6/91, reh
define(freq_omission$file," .fom")
define(freq_omission$index,kfreq_omission)
define(freq_omission$index2,kfreq2_omission)
define(freq_omission$item_index,kfreq_omission_item)
define(freq_omission$plan_name,wrfom[1,freq_omission$index])
define(freq_omission$plan_name2,wrfom[1,freq_omission$index2])
define(freq_omission$path_name,wrfom[2,freq_omission$index])
define(freq_omission$hifreq,wifom[1,2+freq_omission$item_index,freq_omission$index])                    
define(freq_omission$lowfreq,wifom[2,2+freq_omission$item_index,freq_omission$index])
                                                                        #**-9
define(emitter$file," .emt")
define(emitter$index,kemitr)
define(emitter$name,wremit[1,emitter$index])
define(emitter$plat,wremit[2,emitter$index])
                                                                        #**-2
define(member$file," .mem")
define(member$index,kmembr)
define(name$index,kmemNM)
define(name$index$limit,member$len-2)
define(member$len,wrmembr[member$index])                     
define(member$path,wrmembr[member$index+1])
define(member$name,wrmembr[member$index+name$index+1])
                                                                        #**-3
define(orders$file," .xpo")
define(orders$index,korder)
define(suborders$index,korder)

define(plans$file," .xcp")
define(plans$index,kplans)
define(plans$side,wrplan[1,1,plans$index])
define(plans$name,wrplan[2,1,plans$index])
define(plans$size,wiplan[$halfquad,3,1,plans$index])
define(subplans$data,wrplan[1,subplans$index,plans$index])
define(subplans$index,ksplan)

define(search$file," .xsp")
define(search$index,ksearc)
define(search$side,wrsearc[1,1,search$index])
define(search$name,wrsearc[2,1,search$index])
define(search$size,wisearc[$halfquad,3,1,search$index])
define(subsearch$data,wrsearc[1,subsearch$index,search$index])
define(subsearch$index,kssear)

define(action$file," .xap")
define(act$index,kact)
define(act$side,wract[1,1,act$index])
define(act$name,wract[2,1,act$index])
define(act$size,wiact[$halfquad,3,1,act$index])
define(subact$data,wract[1,subact$index,act$index])
define(subact$index,ksact)

define(emcon$file," .emc")
define(emcon$index,kemcon)
define(emcon$option$index,kemcox)
define(emcon$name,wremcon[1,emcon$index])
define(emcon$side,wremcon[2,emcon$index])
define(emcon$codes,wiemcon[$halfquad,3,emcon$index])
define(emcon$option,istrip(wiemcon[$halfquad,3,emcon$index],
        emcon$option$index,1))
define(emcon$option$on,call_idress($on,wiemcon[$halfquad,3,
       emcon$index],emcon$option$index,1))
define(emcon$option$off,call_idress($off,wiemcon[$halfquad,3,
emcon$index],emcon$option$index,1))

##
## weather data definition

define(wet$file," .wet")
define(wet$pointer,kwet)
define(wet$address,kweta)
define(wet$pointer$first,wet$pointer=wet$base)
define(wet$pointer$next,wet$pointer=wet$pointer+wet$entry)
define(wet$pointer$valid,(wet$pointer>=wet$base&wet$pointer<=wet$last))

define(wet$pointer$to,wet$pointer$first-wet$entry+wet$entry*)
define(wet$pointer$index,((wet$pointer-wet$base)/wet$entry+1))

define(wet$map,call mappnt (wet$pointer,wet$address))

define(wet$base,513)
define(wet$entry,9)
define(wet$slots,$weather)
define(wet$size,arith(wet$entry,*,wet$slots))
define(wet$last,(wet$base+wet$size-1))

define(wet$InUse,wet$category)
define(wet$waveheight,work$file$i[$halfquad,wet$address])
define(wet$wavedirection,work$file$i[$halfquad,wet$address+1])
define(wet$windspeed,work$file$i[$halfquad,wet$address+2])
define(wet$winddirection,work$file$i[$halfquad,wet$address+3])
define(wet$cloudcover,work$file$i[$halfquad,wet$address+4])
define(wet$ceiling,work$file$i[$halfquad,wet$address+5])
define(wet$clouddepth,work$file$i[$halfquad,wet$address+6])
define(wet$visibility,work$file$i[$halfquad,wet$address+7])
define(wet$category,work$file$r[wet$address+8])

define(aml$file," .aml")
define(aml$pointer,kaml)
define(aml$address,kamla)
define(aml$pointer$first,aml$pointer=aml$base)
define(aml$pointer$next,aml$pointer=aml$pointer+aml$entry)
define(aml$pointer$valid,(aml$pointer>=aml$base&aml$pointer<=aml$last))

define(aml$pointer$to,aml$pointer$first-aml$entry+aml$entry*)
define(aml$pointer$index,((aml$pointer-aml$base)/aml$entry+1))

define(aml$map,call mappnt (aml$pointer,aml$address))

define(aml$base,arith(wet$base,+,wet$size))
define(aml$entry,7)
define(aml$slots,$airlog)
define(aml$size,arith(aml$entry,*,aml$slots))
define(aml$last,(aml$base+aml$size-1))

define(aml$InUse,aml$sidenumber)
define(aml$sidenumber,work$file$r[aml$address])
define(aml$type,work$file$r[aml$address+1])
define(aml$basename,work$file$r[aml$address+2])
define(aml$timeflown,work$file$i[$halfquad,aml$address+3])
define(aml$timeup,work$file$i[$halfquad,aml$address+4])
define(aml$sqdindx,work$file$i[$halfquad,aml$address+5])
define(aml$defaultmission,work$file$i[$halfquad,aml$address+6])

# clk 4/95 - Added squadron name information (E1394)
define(sqd$file,    " .sqd")
define(sqd$pointer, ksqd)
define(sqd$address, ksqda)
define(sqd$pointer$first, sqd$pointer=sqd$base)
define(sqd$pointer$next,  sqd$pointer=sqd$pointer+bis$entry)
define(sqd$pointer$valid, (sqd$pointer>=sqd$base&sqd$pointer<=sqd$last))
define(sqd$pointer$to,    sqd$pointer$first-sqd$entry+sqd$entry*)
define(sqd$pointer$index, ((sqd$pointer-sqd$base)/sqd$entry+1))
define(sqd$map,           call mappnt (sqd$pointer,sqd$address))
define(sqd$zap,           {for (isqdxx=0;isqdxx<sqd$entry;isqdxx=isqdxx+1)
                              work$file$r[sqd$address+isqdxx]=0})  

define(sqd$base,arith(aml$base,+,aml$size))
define(sqd$entry,1)
define(sqd$slots,$MaxSquadrons)
define(sqd$size,arith(sqd$entry,*,sqd$slots))
define(sqd$last,(sqd$base+sqd$size-1))
define(sqd$InUse,sqd$SquadronName)
define(sqd$SquadronName, work$file$r[sqd$address])

define(bis$file," .bis")
define(bis$pointer,kbis)
define(bis$address,kbisa)
define(bis$pointer$first,bis$pointer=bis$base)
define(bis$pointer$next,bis$pointer=bis$pointer+bis$entry)
define(bis$pointer$valid,(bis$pointer>=bis$base&bis$pointer<=bis$last))

define(bis$pointer$to,bis$pointer$first-bis$entry+bis$entry*)
define(bis$pointer$index,((bis$pointer-bis$base)/bis$entry+1))

define(bis$map,call mappnt (bis$pointer,bis$address))
define(bis$zap,{for(ibisxx=0;ibisxx<bis$entry;ibisxx=ibisxx+1)
                  work$file$i[$halfquad,bis$address+ibisxx]=0})

define(bis$base,arith(sqd$base,+,sqd$size))
define(bis$entry,2)
define(bis$slots,$bpairs)
define(bis$size,arith(bis$entry,*,bis$slots))
define(bis$last,(bis$base+bis$size-1))

define(BIS$ActiveSource,work$file$r[bis$address])
define(BIS$PassiveReceiver,work$file$r[bis$address+1])
#----------------------------------------
define(sos$file," .sos")
define(sos$pointer,ksos)
define(sos$address,ksosa)
define(sos$pos$index,ksosx1)
define(sos$sub$index,ksosx2)
define(sos$det$index,ksosx3)
define(sos$pointer$first,sos$pointer=sos$base)
define(sos$pointer$next,sos$pointer=sos$pointer+sos$entry)
define(sos$pointer$valid,(sos$pointer>=sos$base&sos$pointer<=sos$last))

define(sos$pointer$to,sos$pointer$first-sos$entry+sos$entry*)
define(sos$pointer$index,((sos$pointer-sos$base)/sos$entry+1))

define(sos$map,call mappnt (sos$pointer,sos$address))
define(sos$zap,{for(isosxx=0;isosxx<sos$entry;isosxx=isosxx+1)
                    work$file$i[$halfquad,sos$address+isosxx] = 0})

define(sos$pos$slots,6)         # number of lat/long pairs
define(sos$pos$entry,2)
define(sos$pos$size,arith(sos$pos$entry,*,sos$pos$slots))

define(sos$det$slots,12)        # number of Pbs (pairs of Pb at 6 speeds)
define(sos$det$entry,1)
define(sos$det$size,arith(sos$det$entry,*,sos$det$slots))

define(sos$sub$slots,96)        # number of class/depth/mode entries
define(sos$sub$entry,arith(3,+,sos$det$size))
define(sos$sub$size,arith(sos$sub$entry,*,sos$sub$slots))

define(sos$base,arith(bis$base,+,bis$size))
define(sos$last,(sos$base+sos$size-1))
define(sos$entry,arith(8,+,arith(sos$pos$size,+,sos$sub$size)))
define(sos$size,arith(sos$entry,*,$sosus))

define(sos$pos$address,(sos$address+ 5 + ((sos$pos$index-1)*sos$pos$entry)))

define(sos$bis$address,(sos$address + 5 + sos$pos$size))

define(sos$sub$address,(sos$bis$address + 3 + (sos$sub$index-1) * sos$sub$entry))
define(sos$sub$det$address,(sos$sub$address + 3 + (sos$det$index-1)* sos$det$entry))

define(sos$InUse,sos$regnumber)
define(sos$regdefined,sos$meanaxislength)

define(sos$regnumber,     work$file$i[$halfquad,sos$address])
define(sos$meanaxislength,work$file$i[$halfquad,sos$address+1])
define(sos$sigmalength,   work$file$i[$halfquad,sos$address+2])
define(sos$boundarydepth, work$file$i[$halfquad,sos$address+3])
define(sos$numberofsets,  work$file$i[$halfquad,sos$address+4])
define(sos$pos$lat,       work$file$i[$halfquad,sos$pos$address])
define(sos$pos$lon,       work$file$i[$halfquad,sos$pos$address+1])
define(sos$bis$regionarea,work$file$r[sos$bis$address])
define(sos$bis$nodedensity,work$file$r[sos$bis$address+1])
define(sos$bis$range,     work$file$r[sos$bis$address+2])
define(sos$sub$class,     work$file$r[sos$sub$address])
define(sos$sub$depth,     work$file$r[sos$sub$address+1]) #shallow/deep 
define(sos$sub$propmode,  work$file$r[sos$sub$address+2])
define(sos$sub$det$prob,  work$file$i[$halfquad,sos$sub$det$address])
#----------------------------------------
define(swb$file," .swb")
define(swb$pointer,kswb)
define(swb$address,kswba)
define(swb$pos$index,kswbx1)
define(swb$sub$index,kswbx2)
define(swb$det$index,kswbx3)
define(swb$pointer$first,swb$pointer=swb$base)
define(swb$pointer$next,swb$pointer=swb$pointer+swb$entry)
define(swb$pointer$valid,(swb$pointer>=swb$base&swb$pointer<=swb$last))

define(swb$pointer$to,swb$pointer$first-swb$entry+swb$entry*)
define(swb$pointer$index,((swb$pointer-swb$base)/swb$entry+1))

define(swb$map,call mappnt (swb$pointer,swb$address))
define(swb$zap,{for(iswbxx=0;iswbxx<swb$entry;iswbxx=iswbxx+1)
                    work$file$i[$halfquad,swb$address+iswbxx] = 0})

define(swb$pos$slots,6)         # number of lat/long pairs
define(swb$pos$entry,2)
define(swb$pos$size,arith(swb$pos$slots,*,swb$pos$entry))

define(swb$det$slots,6)         # number of Pbs (Pb at 6 speeds)
define(swb$det$entry,1)
define(swb$det$size,arith(swb$det$slots,*,swb$det$entry))

define(swb$sub$slots,20)        # number of class entries
define(swb$sub$entry,arith(1,+,swb$det$size))
define(swb$sub$size,arith(swb$sub$slots,*,swb$sub$entry))

define(swb$base,arith(sos$base,+,sos$size))
define(swb$size,arith(swb$entry,*,$SWABSregions))
define(swb$last,(swb$base+swb$size-1))
define(swb$entry,(5 + 2*swb$pos$slots + (1+swb$det$slots) * swb$sub$slots))
define(swb$entry,arith(5,+,arith(swb$pos$size,+,swb$sub$size)))

define(swb$pos$address,(swb$address+3+(swb$pos$index*2)))
define(swb$sub$address,(swb$address+5+swb$pos$slots*2+
                        (swb$sub$index-1)*(1+swb$det$slots)))
define(swb$sub$det$address,(swb$sub$address+swb$det$index))

define(swb$InUse,swb$regnumber)

define(swb$regnumber,     work$file$i[$halfquad,swb$address])
define(swb$locationerror, work$file$i[$halfquad,swb$address+1])
define(swb$sigma,         work$file$i[$halfquad,swb$address+2])
define(swb$numberofsets,  work$file$i[$halfquad,swb$address+3])
define(swb$regdefined,    work$file$i[$halfquad,swb$address+4])
define(swb$pos$lat,       work$file$i[$halfquad,swb$pos$address])
define(swb$pos$lon,       work$file$i[$halfquad,swb$pos$address+1])
define(swb$sub$class,     work$file$r[swb$sub$address])
define(swb$sub$det$prob,  work$file$r[swb$sub$det$address])

define(hfd$file," .hfd")
define(hfd$pointer,khfd)
define(hfd$address,khfda)
define(hfd$index,khfdx)
define(hfd$pointer$first,hfd$pointer=hfd$base)
define(hfd$pointer$next,hfd$pointer=hfd$pointer+hfd$entry)
define(hfd$pointer$valid,(hfd$pointer>=hfd$base&hfd$pointer<=hfd$last))

define(hfd$pointer$to,hfd$pointer$first-hfd$entry+hfd$entry*)
define(hfd$pointer$index,((hfd$pointer-hfd$base)/hfd$entry+1))

define(hfd$map,call mappnt (hfd$pointer,hfd$address))
define(hfd$zap,{for(ihfdxx=0;ihfdxx<hfd$entry;ihfdxx=ihfdxx+1)
                    work$file$i[$halfquad,hfd$address+ihfdxx] = 0})

define(hfd$base,arith(swb$base,+,swb$size))
define(hfd$entry,101)
define(hfd$slots,$hfdf)
define(hfd$size,arith(hfd$entry,*,hfd$slots))
define(hfd$last,(hfd$base+hfd$size-1))

define(hfd$InUse,hfd$basename)
define(hfd$basename,work$file$r[hfd$address])
define(hfd$prob,work$file$i[$halfquad,hfd$address+hfd$index])


define(sst$file," .sst")
define(sst$pointer,ksst)
define(sst$address,kssta)
define(sst$pointer$first,sst$pointer=sst$base)
define(sst$pointer$next,sst$pointer=sst$pointer+sst$entry)
define(sst$pointer$valid,(sst$pointer>=sst$base&sst$pointer<=sst$last))

define(sst$pointer$to,sst$pointer$first-sst$entry+sst$entry*)
define(sst$pointer$index,((sst$pointer-sst$base)/sst$entry+1))

define(sst$map,call mappnt (sst$pointer,sst$address))
define(sst$zap,{for(isstxx=0;isstxx<sst$entry;isstxx=isstxx+1)
                    work$file$i[$halfquad,sst$address+isstxx] = 0})

define(sst$base,arith(hfd$base,+,hfd$size))
#define(sst$entry,9)
define(sst$entry,12)                          # jpk 6/3/86
define(sst$slots,$survsats)
define(sst$size,arith(sst$entry,*,sst$slots))
define(sst$last,(sst$base+sst$size-1))

define(sst$InUse,sst$name)
define(sst$name,work$file$r[sst$address])
define(sst$type,work$file$r[sst$address+1])
define(sst$period,work$file$i[$halfquad,sst$address+2])
define(sst$altitude,work$file$i[$halfquad,sst$address+3])
define(sst$inclination,work$file$i[$halfquad,sst$address+4])
define(sst$xtime,work$file$i[$halfquad,sst$address+5])
define(sst$xlongitude,work$file$i[$halfquad,sst$address+6])
define(sst$dutycycle,work$file$i[$halfquad,sst$address+7])
define(sst$groundstation,work$file$r[sst$address+8])
define(sst$latitude,work$file$i[$halfquad,sst$address+9])
define(sst$longitude,work$file$i[$halfquad,sst$address+10]) # jpk 6/3/86
define(sst$orbit,work$file$i[$halfquad,sst$address+11])     # jpk 6/3/86


define(rpm$file," .rpm")
define(rpm$pointer,krpm)
define(rpm$address,krpma)
define(rpm$pointer$first,rpm$pointer=rpm$base)
define(rpm$pointer$next,rpm$pointer=rpm$pointer+rpm$entry)
define(rpm$pointer$valid,(rpm$pointer>=rpm$base&rpm$pointer<=rpm$last))

define(rpm$pointer$to,rpm$pointer$first-rpm$entry+rpm$entry*)
define(rpm$pointer$index,((rpm$pointer-rpm$base)/rpm$entry+1))

define(rpm$map,call mappnt (rpm$pointer,rpm$address))
define(rpm$zap,{for(irpmxx=0;irpmxx<rpm$entry;irpmxx=irpmxx+1)
                    work$file$i[$halfquad,rpm$address+irpmxx] = 0}) 

define(rpm$base,arith(sst$base,+,sst$size))
define(rpm$entry,12)   # JFR 12 vice 11 --- 12/2/91
define(rpm$slots,$policy)
define(rpm$size,arith(rpm$entry,*,rpm$slots))
define(rpm$last,(rpm$base+rpm$size-1))

define(rpm$side,    work$file$r[rpm$address])
define(rpm$name,    work$file$r[rpm$address+1])
define(rpm$circuit1,work$file$i[$halfquad,rpm$address+2])
define(rpm$circuit2,work$file$i[$halfquad,rpm$address+3])
define(rpm$violate, work$file$r[rpm$address+4])
define(rpm$position,work$file$i[$halfquad,rpm$address+5])
define(rpm$surface, work$file$i[$halfquad,rpm$address+6])
define(rpm$air,     work$file$i[$halfquad,rpm$address+7])
define(rpm$esm,     work$file$i[$halfquad,rpm$address+8])
define(rpm$ground,  work$file$i[$halfquad,rpm$address+9]) # JFR 12/2/91
define(rpm$type,    work$file$i[$halfquad,rpm$address+10])
define(rpm$unit,    work$file$r[rpm$address+11])


define(son$file," .ser")
define(son$pointer,kson)
define(son$address,ksona)
define(son$pos$index,ksonx1)
define(son$pointer$first,son$pointer=son$base)
define(son$pointer$next,son$pointer=son$pointer+son$entry)
define(son$pointer$valid,(son$pointer>=son$base&son$pointer<=son$last))

define(son$pointer$to,son$pointer$first-son$entry+son$entry*)
define(son$pointer$index,((son$pointer-son$base)/son$entry+1))

define(son$map,call mappnt (son$pointer,son$address))
define(son$zap,{for(isonxx=0;isonxx<son$entry;isonxx=isonxx+1)
                    work$file$i[$halfquad,son$address+isonxx] = 0})

define(son$pos$slots,6)         # number of lat/long pairs
define(son$pos$entry,2)
define(son$pos$size,arith(son$pos$slots,*,son$pos$entry))
define(son$pos$address,(son$address+5+(son$pos$index-1)*son$pos$entry))

define(son$base,arith(rpm$base,+,rpm$size))
define(son$last,(son$base+son$size-1))

#@define(son$entry,(2 + 2*son$pos$slots ))
define(son$entry,arith(5,+,son$pos$size))

define(son$size,arith(son$entry,*,$sonev))


define(son$InUse,son$regnumber)
define(son$regdefined,son$pos$lon)

define(son$regnumber,     work$file$i[$halfquad,son$address])
define(son$sonarenv,      work$file$i[$halfquad,son$address+1])
define(son$pos$lat,       work$file$i[$halfquad,son$pos$address])
define(son$pos$lon,       work$file$i[$halfquad,son$pos$address+1])

define(ldr$file," .ler")
define(ldr$pointer,kldr)
define(ldr$address,kldra)
define(ldr$pos$index,kldrx1)
define(ldr$pointer$first,ldr$pointer=ldr$base)
define(ldr$pointer$next,ldr$pointer=ldr$pointer+ldr$entry)
define(ldr$pointer$valid,(ldr$pointer>=ldr$base&ldr$pointer<=ldr$last))

define(ldr$pointer$to,ldr$pointer$first-ldr$entry+ldr$entry*)
define(ldr$pointer$index,((ldr$pointer-ldr$base)/ldr$entry+1))

define(ldr$map,call mappnt (ldr$pointer,ldr$address))
define(ldr$zap,{for(ildrxx=0;ildrxx<ldr$entry;ildrxx=ildrxx+1)
                    work$file$i[$halfquad,ldr$address+ildrxx] = 0})

define(ldr$pos$slots,6)         # number of lat/long pairs
define(ldr$pos$entry,2)
define(ldr$pos$size,arith(ldr$pos$slots,*,ldr$pos$entry))


define(ldr$base,arith(son$base,+,son$size))
define(ldr$last,(ldr$base+ldr$size-1))
define(ldr$entry,arith(2,+,ldr$pos$size))
define(ldr$size,arith(ldr$entry,*,$LIDARregions))

define(ldr$pos$address,(ldr$address+2+(ldr$pos$index-1)*ldr$pos$entry))

define(ldr$InUse,ldr$regnumber)

define(ldr$regnumber,      work$file$i[$halfquad,ldr$address])
define(ldr$kvalue,         work$file$r[ldr$address+1])
define(ldr$pos$lat,        work$file$i[$halfquad,ldr$pos$address])
define(ldr$pos$lon,        work$file$i[$halfquad,ldr$pos$address+1])

define(ton$file," .ton")
define(ton$pointer,kton)
define(ton$address,ktona)
define(ton$pointer$first,ton$pointer=ton$base)
define(ton$pointer$next,ton$pointer=ton$pointer+ton$entry)
define(ton$pointer$valid,(ton$pointer>=ton$base&ton$pointer<=ton$last))

define(ton$pointer$to,ton$pointer$first-ton$entry+ton$entry*)
define(ton$pointer$index,((ton$pointer-ton$base)/ton$entry+1))

define(ton$map,call mappnt (ton$pointer,ton$address))
define(ton$zap,{for(itonxx=0;itonxx<ton$entry;itonxx=itonxx+1)
                    work$file$i[$halfquad,ton$address+itonxx] = 0})

define(ton$base,arith(ldr$base,+,ldr$size))
define(ton$entry,5) # JFR 5/5/93 - dumped tonals
define(ton$slots,$ton)
define(ton$size,arith(ton$entry,*,ton$slots))
define(ton$last,(ton$base+ton$size-1))

define(ton$InUse,ton$name)
define(ton$name,work$file$r[ton$address])
define(ton$classification1,work$file$r[ton$address+1])
define(ton$classification2,work$file$r[ton$address+2]) # JFR 5/5/93
define(ton$classification3,work$file$r[ton$address+3]) # JFR 5/5/93
define(ton$classification4,work$file$r[ton$address+4]) # JFR 5/5/93

define(eph$file," .eph")
define(eph$pointer,keph)
define(eph$address,kepha)
define(eph$pointer$first,eph$pointer=eph$base)
define(eph$pointer$next,eph$pointer=eph$pointer+eph$entry)
define(eph$pointer$valid,(eph$pointer>=eph$base&eph$pointer<=eph$last))

define(eph$pointer$to,eph$pointer$first-eph$entry+eph$entry*)
define(eph$pointer$index,((eph$pointer-eph$base)/eph$entry+1))

define(eph$map,call mappnt (eph$pointer,eph$address))
define(eph$zap,{for(iephxx=0;iephxx<eph$entry;iephxx=iephxx+1)
                    work$file$i[$halfquad,eph$address+iephxx] = 0})

define(eph$base,arith(ton$base,+,ton$size))
define(eph$entry,4)
define(eph$slots,$eph)
define(eph$size,arith(eph$entry,*,eph$slots))
define(eph$last,(eph$base+eph$size-1))

define(eph$InUse,eph$name)
define(eph$name,work$file$r[eph$address])
define(eph$target,work$file$r[eph$address+1])
define(eph$weapon,work$file$r[eph$address+2])
define(eph$percent,work$file$i[$halfquad,eph$address+3])

#############################################################################
#$%
#$% jfr - Combat Index Table  3/29/91
#$%
#############################################################################

define(cbi$file," .cbi")
define(cbi$pointer,kcbi)
define(cbi$address,kcbia)
define(cbi$pointer$first,cbi$pointer=cbi$base)
define(cbi$pointer$next,cbi$pointer=cbi$pointer+cbi$entry)
define(cbi$pointer$valid,(cbi$pointer>=cbi$base&cbi$pointer<=cbi$last))

define(cbi$pointer$to,cbi$pointer$first-cbi$entry+cbi$entry*)
define(cbi$pointer$index,((cbi$pointer-cbi$base)/cbi$entry+1))

define(cbi$map,call mappnt (cbi$pointer,cbi$address))
define(cbi$zap,{for(icbixx=0;icbixx<cbi$entry;icbixx=icbixx+1)
		    work$file$i[$halfquad,cbi$address+icbixx] = 0})

define(cbi$base,arith(eph$base,+,eph$size))
define(cbi$entry,10) #JFR 5/14/91
define(cbi$slots,$cbi)
define(cbi$size,arith(cbi$entry,*,cbi$slots))
define(cbi$last,(cbi$base+cbi$size-1))

define(cbi$InUse,cbi$name)
define(cbi$name,work$file$r[cbi$address])
define(cbi$attacker$posture,work$file$r[cbi$address+1])
define(cbi$defender$onroad,work$file$i[$halfquad,cbi$address+2]) #JFR 5/14/91
define(cbi$defender$column,work$file$i[$halfquad,cbi$address+3]) #JFR 5/14/91
define(cbi$defender$line,work$file$i[$halfquad,cbi$address+4]) #JFR 5/14/91
define(cbi$defender$wedge,work$file$i[$halfquad,cbi$address+5]) #JFR 5/14/91
define(cbi$defender$vee,work$file$i[$halfquad,cbi$address+6]) #JFR 5/14/91
define(cbi$defender$circle,work$file$i[$halfquad,cbi$address+7]) #JFR 5/14/91
define(cbi$defender$echelonleft,work$file$i[$halfquad,cbi$address+8])#JFR 5/14/91
define(cbi$defender$echelonright,work$file$i[$halfquad,cbi$address+9])#JFR 5/14/91

#############################################################################
#$%
#$% jfr - ATT Table  5/14/91
#$%
#############################################################################

define(att$file," .att")
define(att$pointer,katt)
define(att$address,katta)
define(att$pointer$first,att$pointer=att$base)
define(att$pointer$next,att$pointer=att$pointer+att$entry)
define(att$pointer$valid,(att$pointer>=att$base&att$pointer<=att$last))

define(att$pointer$to,att$pointer$first-att$entry+att$entry*)
define(att$pointer$index,((att$pointer-att$base)/att$entry+1))

define(att$map,call mappnt (att$pointer,att$address))
define(att$zap,{for(iattxx=0;iattxx<att$entry;iattxx=iattxx+1)
		    work$file$i[$halfquad,att$address+iattxx] = 0})

define(att$base,arith(cbi$base,+,cbi$size))
define(att$entry,5)  #JFR 5/14/91
define(att$slots,$att)
define(att$size,arith(att$entry,*,att$slots))
define(att$last,(att$base+att$size-1))

define(att$InUse,att$name)
define(att$name,work$file$r[att$address])
define(att$table,work$file$i[$halfquad,att$address+1]) #JFR 5/14/91
define(att$attacker,work$file$r[att$address+2])
define(att$defender,work$file$r[att$address+3])
define(att$coefficient,work$file$i[$halfquad,att$address+4])

# bml$  added 5/8/89 reh
define(bml$file," .bml")
define(bml$pointer,kbml)
define(bml$address,kbmla)
define(bml$pointer$first,bml$pointer=bml$base)
define(bml$pointer$next,bml$pointer=bml$pointer+bml$entry)
define(bml$pointer$valid,(bml$pointer>=bml$base&bml$pointer<=bml$last))

define(bml$pointer$to,bml$pointer$first-bml$entry+bml$entry*)
define(bml$pointer$index,((bml$pointer-bml$base)/bml$entry+1))

define(bml$map,call mappnt (bml$pointer,bml$address))

define(bml$base,arith(att$base,+,att$size)) #JFR 5/14/91
define(bml$entry,3)
define(bml$slots,$boatlog)
define(bml$size,arith(bml$entry,*,bml$slots))
define(bml$last,(bml$base+bml$size-1))

define(bml$InUse,bml$name)
define(bml$name,work$file$r[bml$address])
define(bml$class,work$file$r[bml$address+1])
define(bml$basename,work$file$r[bml$address+2])

# rothr$ added RJS 8/86, CML 2/90
define(rothr$file," .rth")
define(rothr$pointer,krothra)
define(rothr$address,krothra)
define(rothr$pointer$first,rothr$pointer=rothr$base)
define(rothr$pointer$next,rothr$pointer=rothr$pointer+rothr$entry)
define(rothr$pointer$valid,(rothr$pointer>=rothr$base&rothr$pointer<=rothr$last))
define(rothr$pointer$to,rothr$pointer$first-rothr$entry+rothr$entry*)
define(rothr$pointer$index,((rothr$pointer-(rothr$base))/rothr$entry+1))

define(rothr$base,arith(bml$base,+,bml$size))
define(rothr$entry,300)
define(rothr$slots,$rothr)
define(rothr$size,arith(rothr$entry,*,rothr$slots))
define(rothr$last,(rothr$base+rothr$size-1))
define(rothr$InUse,rothr$basename)
define(rothr$basename,work$file$r[rothr$address])
define(rothr$othbr,work$file$i[$halfquad,rothr$address+1])
define(rothr$power,work$file$i[$halfquad,rothr$address+2])
define(rothr$gain,work$file$i[$halfquad,rothr$address+3])
define(rothr$freq,work$file$i[$halfquad,rothr$address+4])
define(rothr$noise,work$file$i[$halfquad,rothr$address+5])
define(rothr$task,work$file$i[$halfquad,rothr$address+6])

# rothr subtable air dir specs

define(airdir$base,rothr$address+7)
define(airdir$pointer,krotaira)
define(airdir$address,krotaira)
define(airdir$pointer$first,airdir$pointer=airdir$base)
define(airdir$pointer$next,airdir$pointer=airdir$pointer+airdir$entry)
define(airdir$pointer$valid,(airdir$pointer>=airdir$base&airdir$pointer<=airdir$last))
define(airdir$entry,16)
define(airdir$slots,6)
define(airdir$size,(airdir$entry*airdir$slots))
define(airdir$last,(airdir$base+airdir$size-1))
define(airdir$pointer$to,airdir$pointer$first-airdir$entry+airdir$entry*)
define(airdir$pointer$index,((airdir$pointer-(airdir$base))/airdir$entry+1))

define(set_airdir_slot,
          for ( ixix = @3; ixix < @4; ixix = ixix + 1)
#         for ( ixix = 0; ixix < airdir$entry; ixix = ixix + 1)
           {
            work$file$i[$halfquad,airdir$address+ixix] = @1[@2]
            @2 = @2 + 1
           }
      )

define(get_airdir_slot,
         for ( ixix = 0; ixix < airdir$entry; ixix = ixix + 1)
           {
            @1[@2] = work$file$i[$halfquad,airdir$address+ixix]
            @2 = @2 + 1
           }
      )

# rothr sub-table surf dir specs

define(surfdir$base,airdir$base+airdir$size)
define(surfdir$pointer,krotsurfa)
define(surfdir$address,krotsurfa)
define(surfdir$pointer$first,surfdir$pointer=surfdir$base)
define(surfdir$pointer$next,surfdir$pointer=surfdir$pointer+surfdir$entry)
define(surfdir$pointer$valid,(surfdir$pointer>=surfdir$base&surfdir$pointer<=surfdir$last))
define(surfdir$entry,16)
define(surfdir$slots,12)
define(surfdir$size,(surfdir$entry*surfdir$slots))
define(surfdir$last,(surfdir$base+surfdir$size-1))
define(surfdir$pointer$to,surfdir$pointer$first-surfdir$entry+surfdir$entry*)
define(surfdir$pointer$index,((surfdir$pointer-(surfdir$base))/surfdir$entry+1))

define(set_surfdir_slot,
         for ( ixix = @3; ixix < @4; ixix = ixix + 1)
#         for ( ixix = 0; ixix < surfdir$entry; ixix = ixix + 1)
           {
            work$file$i[$halfquad,surfdir$address+ixix] = @1[@2]
            @2 = @2 + 1
           }
      )

define(get_surfdir_slot,
         for ( ixix = 0; ixix < surfdir$entry; ixix = ixix + 1)
           {
            @1[@2] = work$file$i[$halfquad,surfdir$address+ixix]
            @2 = @2 + 1
           }
      )

define(ZZZ$base,arith(rothr$base,+,rothr$size))

#############################################################################

define(shore$file," .shr")
define(shore$index,kshore)
define(shore$name,wrshore[1,shore$index])
define(shore$addr,wishore[$halfquad,2,shore$index])
define(shore$used,wishore[$halfquad,3,shore$index])
define(shore$oldaddr,wishore[$halfquad,4,shore$index])
define(shore$type,wrshore[5,shore$index])

#$% #MJF this is the site characteristics pointer file  3/26/92
define(site$file,   " .sit")                            #MJF 3/26/92
define(site$index,  ksite)                              #MJF 3/26/92
define(site$name,   wrsite[1,site$index])               #MJF 3/26/92
define(site$addr,   wisite[$halfquad,2,site$index])     #MJF 3/26/92
define(site$used,   wisite[$halfquad,3,site$index])     #MJF 3/26/92
define(site$oldaddr,wisite[$halfquad,4,site$index])     #MJF 3/26/92
define(site$type,   wrsite[5,site$index])               #MJF 3/26/92

define(class$file," .shp")
define(class$index,kclass)
define(class$name,wrclass[1,class$index])
define(class$addr,wiclass[$halfquad,2,class$index])
define(class$used,wiclass[$halfquad,3,class$index])
define(class$oldaddr,wiclass[$halfquad,4,class$index])
define(class$type,wrclass[5,class$index])

###########################################################################
#$%
#$% #jfr this is the ground unit characteristics pointer file  3/29/91
#$%
###########################################################################

define(groundunit$file,   " .unt")                                    #jfr
define(groundunit$index,  kgroundunit)                                #jfr
define(groundunit$name,   wrgroundunit[1,groundunit$index])           #jfr
define(groundunit$addr,   wigroundunit[$halfquad,2,groundunit$index]) #jfr
define(groundunit$used,   wigroundunit[$halfquad,3,groundunit$index]) #jfr
define(groundunit$oldaddr,wigroundunit[$halfquad,4,groundunit$index]) #jfr
define(groundunit$type,   wrgroundunit[5,groundunit$index])           #jfr

###########################################################################
#$%
#$% #JFR this is the vehicle characteristics pointer file -- 5/24/91
#$%
###########################################################################

define(vehicle$file,   " .veh")                               #JFR 5/24/91
define(vehicle$index,  kvehicle)                              #JFR 5/24/91
define(vehicle$name,   wrvehicle[1,vehicle$index])           #JFR 5/24/91
define(vehicle$addr,   wivehicle[$halfquad,2,vehicle$index]) #JFR 5/24/91
define(vehicle$used,   wivehicle[$halfquad,3,vehicle$index]) #JFR 5/24/91
define(vehicle$oldaddr,wivehicle[$halfquad,4,vehicle$index]) #JFR 5/24/91
define(vehicle$type,   wrvehicle[5,vehicle$index])           #JFR 5/24/91

define(air$file," .air")
define(air$index,kair)
define(air$name,wrair[1,air$index])
define(air$addr,wiair[$halfquad,2,air$index])
define(air$used,wiair[$halfquad,3,air$index])
define(air$oldaddr,wiair[$halfquad,4,air$index])
define(air$type,wrair[5,air$index])

define(cruise$file," .cru")
define(cruise$index,kcruis)
define(cruise$name,wrcruis[1,cruise$index])
define(cruise$addr,wicruis[$halfquad,2,cruise$index])
define(cruise$used,wicruis[$halfquad,3,cruise$index])
define(cruise$oldaddr,wicruis[$halfquad,4,cruise$index])
define(cruise$type,wrcruis[5,cruise$index])

define(commb$file," .cmb")
define(commb$index,kcommb)
define(commb$name,wrcommb[1,commb$index])
define(commb$addr,wicommb[$halfquad,2,commb$index])
define(commb$used,wicommb[$halfquad,3,commb$index])
define(commb$oldaddr,wicommb[$halfquad,4,commb$index])
define(commb$type,wrcommb[5,commb$index])

define(comms$file," .cms")
define(comms$index,kcomms)
define(comms$name,wrcomms[1,comms$index])
define(comms$addr,wicomms[$halfquad,2,comms$index])
define(comms$used,wicomms[$halfquad,3,comms$index])
define(comms$oldaddr,wicomms[$halfquad,4,comms$index])

define(commp$file," .cmp")
define(commp$index,kcommp)
define(commp$name,wrcommp[1,commp$index])
define(commp$addr,wicommp[$halfquad,2,commp$index])
define(commp$used,wicommp[$halfquad,3,commp$index])
define(commp$oldaddr,wicommp[$halfquad,4,commp$index])

define(jammer$file," .jam")
define(jammer$index,kjamme)
define(jammer$name,wrjamme[1,jammer$index])
define(jammer$addr,wijamme[$halfquad,2,jammer$index])
define(jammer$used,wijamme[$halfquad,3,jammer$index])
define(jammer$oldaddr,wijammer[$halfquad,4,jammer$index])


# REH 03/01/88
define(comjm$file," .cjm")
define(comjm$index,kcomjm)
define(comjm$name,wrcomjm[1,comjm$index])
define(comjm$addr,wicomjm[$halfquad,2,comjm$index])
define(comjm$used,wicomjm[$halfquad,3,comjm$index])
define(comjm$oldaddr,wicomjm[$halfquad,4,comjm$index])

# REH 03/01/88
define(ground$file," .gnd")
define(ground$index,kground)
define(ground$name,wrground[1,ground$index])
define(ground$addr,wiground[$halfquad,2,ground$index])
define(ground$used,wiground[$halfquad,3,ground$index])
define(ground$oldaddr,wiground[$halfquad,4,ground$index])

# jb    7/10/90
define(chaff$file," .chf")
define(chaff$index,kchaff)
define(chaff$name,wrchaff[1,chaff$index])
define(chaff$addr,wichaff[$halfquad,2,chaff$index])
define(chaff$used,wichaff[$halfquad,3,chaff$index])
define(chaff$oldaddr,wichaff[$halfquad,4,chaff$index])

# REH 02/29/88
define(MuRho$file," .mrv")
define(MuRho$ptr,kMuRho)
define(MuRho$JS$min$ptr,$MuRholen - 1)
define(MuRho$Last$value$ptr,$MuRholen - 2)
define(MuRho$JS$minimum,wrMuRho[MuRho$JS$min$ptr,1])
define(MuRho$JS$increment,wrMuRho[$MuRholen,1])
define(MuRho$mrv,wrMuRho[MuRho$ptr,1])
define($significant_places,3)
define($EntryNotUsed,-1)
#define(MuRho$addr,wiMuRho[$halfquad,2,MuRho$ptr])
#define(MuRho$used,wiMuRho[$halfquad,3,MuRho$ptr])
#define(MuRho$oldaddr,wiMuRho[$halfquad,4,MuRho$ptr])

#$% added UAV stuff 2/2/90, reh
define(UVP$file," .uvp")           # unmanned air vehicle probabilities
define(UVP$Index,kUVP)
#define($RangeSlot,KRangeSlot)
#define($SetRangeSlot,$RangeSlot=@1)
define(UVP$RangeSlot$Ptr,(UVP$NumRanges*2+1))
define(UVP$TargTypeIndex,wiuvp[1,1,UVP$Index])
define(UVP$Type,wiuvp[2,1,UVP$Index])
define(UVP$NumRanges,wiuvp[1,2,UVP$Index])
define(UVP$Range,wiuvp[1,UVP$RangeSlot$Ptr,UVP$Index])
define(UVP$DetectPb,wiuvp[1,UVP$RangeSlot$Ptr+1,UVP$Index])
define(UVP$RecognitionPb,wiuvp[2,UVP$RangeSlot$Ptr,UVP$Index])
define(UVP$IdentPb,wiuvp[2,UVP$RangeSlot$Ptr+1,UVP$Index])
define(UVP$addr,wiuvp[$halfquad,2,UVP$Index])
define(UVP$used,wiuvp[$halfquad,3,UVP$Index])
define(UVP$oldaddr,wiuvp[$halfquad,4,UVP$Index])

#$% MacroASW 6/90, Median Detection Ranges
define(MDR$file," .mdr")
define(MDR$Index,kmdr)
define(MDR$SonoBuoyName,wrmdr[1,MDR$Index])
define(MDR$SubClass,wrmdr[2,MDR$Index])
define(MDR$GoodENVDetectRange,wrmdr[3,MDR$Index])
define(MDR$FairENVDetectRange,wrmdr[4,MDR$Index])
define(MDR$PoorENVDetectRange,wrmdr[5,MDR$Index])
define(MDR$DefaultENVDetectRange,wrmdr[6,MDR$Index])
define(MDR2$Index,kmdr2)
define(MDR2$SonoBuoyName,wrmdr[1,MDR2$Index])
define(MDR2$SubClass,wrmdr[2,MDR2$Index])
define(MDR2$GoodENVDetectRange,wrmdr[3,MDR2$Index])
define(MDR2$FairENVDetectRange,wrmdr[4,MDR2$Index])
define(MDR2$PoorENVDetectRange,wrmdr[5,MDR2$Index])
define(MDR2$DefaultENVDetectRange,wrmdr[6,MDR2$Index])

#$% Intra platform EMI 8/90
define(INTRA$file," .ipe")
define(INTRA$Index,kINTRA)
define(INTRA$Class,wrINTRA[1,INTRA$Index])
define(INTRA$Victim,wrINTRA[2,INTRA$Index])
define(INTRA$Percent,wrINTRA[3,INTRA$Index])
define(INTRA$Culprit,wrINTRA[4,INTRA$Index])
define(INTRA$Entry,wrINTRA[1,INTRA$Index])
define(NextINTRA$Entry,wrINTRA[1,INTRA$Index+1])

define(navaid$file," .nav")
define(navaid$index,knavad)
define(navaid$name,wrnavad[1,navaid$index])
define(navaid$addr,winavad[$halfquad,2,navaid$index])
define(navaid$used,winavad[$halfquad,3,navaid$index])
define(navaid$oldaddr,winavaid[$halfquad,4,navaid$index])

define(sensor$file," .sen")
define(sensor$index,ksenso)
define(sensor$name,wrsenso[1,sensor$index])
define(sensor$addr,wisenso[$halfquad,2,sensor$index])
define(sensor$used,wisenso[$halfquad,3,sensor$index])
define(sensor$oldaddr,wisensor[$halfquad,4,sensor$index])

define(sat$file," .sat")
define(sat$index,ksat)
define(sat$name,wrsat[1,sat$index])
define(sat$addr,wisat[$halfquad,2,sat$index])
define(sat$used,wisat[$halfquad,3,sat$index])
define(sat$oldaddr,wisat[$halfquad,4,sat$index])
define(sat$type,wrsat[5,sat$index])

define(sonar$file," .snr")
define(sonar$index,ksonar)
define(sonar$name,wrsonar[1,sonar$index])
define(sonar$addr,wisonar[$halfquad,2,sonar$index])
define(sonar$used,wisonar[$halfquad,3,sonar$index])
define(sonar$oldaddr,wisonar[$halfquad,4,sonar$index])

define(sonob$file," .snb")
define(sonob$index,ksonob)
define(sonob$name,wrsonob[1,sonob$index])
define(sonob$addr,wisonob[$halfquad,2,sonob$index])
define(sonob$used,wisonob[$halfquad,3,sonob$index])
define(sonob$oldaddr,wisonob[$halfquad,4,sonob$index])

define(weapon$file," .wep")
define(weapon$index,kweap)
define(weapon$name,wrweap[1,weapon$index])
define(weapon$addr,wiweap[$halfquad,2,weapon$index])
define(weapon$used,wiweap[$halfquad,3,weapon$index])
##define(weapon$oldaddr,wiweapon[$halfquad,4,weapon$index])
define(weapon$oldaddr,wiweap[$halfquad,4,weapon$index])
                # jb  wiweap vice wiweapon??            6/26/89

define(trpdo$file," .trp")
define(trpdo$index,ktrpdo)
define(trpdo$name,wrtrpdo[1,trpdo$index])
define(trpdo$addr,witrpdo[$halfquad,2,trpdo$index])
define(trpdo$used,witrpdo[$halfquad,3,trpdo$index])
define(trpdo$oldaddr,witrpdo[$halfquad,4,trpdo$index]) # SMM 4/90

define(ctrme$file," .ctm")
define(ctrme$index,kctrme)
define(ctrme$name,wrctrme[1,ctrme$index])
define(ctrme$addr,wictrme[$halfquad,2,ctrme$index])
define(ctrme$used,wictrme[$halfquad,3,ctrme$index])
define(ctrme$oldaddr,wictrme[$halfquad,4,ctrme$index]) # SMM 4/90

define(iff$file," .iff")
define(iff$index,kiff)
define(iff$name,wriff[1,iff$index])
define(iff$addr,wiiff[$halfquad,2,iff$index])
define(iff$used,wiiff[$halfquad,3,iff$index])
define(iff$oldaddr,wiiff[$halfquad,4,iff$index]) # CML 3/91

define   (LitIntEquiv,
                literal       wr@1($@2len,$@2)
                integer       wi@1($IntRealMatch,$@2len,$@2)
                equivalence  (wr@1,wi@1)
         )

define (ForceImplicit,)                                        # MJC 6/9/2K (E2145)

define (ForceCommon,
    ForceImplicit                           # Default Implicit # MJC 6/9/2K (E2145)
    work$file$Common                        # Something like the Blackboard
%C---- Max address used in WRwork is % ZZZ$Base % (out of % $WORKFILESIZE %) ---- %

    LitIntEquiv    (force,forces)
    LitIntEquiv    (org,org)
    LitIntEquiv    (emit,emitters)
    LitIntEquiv    (order,orders)
    LitIntEquiv    (shore,shore)
    LitIntEquiv    (site,site)                  #MJF 3/26/92
    LitIntEquiv    (class,class)
    LitIntEquiv    (groundunit,groundunit)      #jfr  3/29/91
    LitIntEquiv    (vehicle,vehicle)            #jfr  5/24/91
    LitIntEquiv    (air,air)
    LitIntEquiv    (cruis,cruise)
    LitIntEquiv    (commb,commbuoy)
    LitIntEquiv    (comms,commsuite)
    LitIntEquiv    (commp,commpair)
    LitIntEquiv    (comjm,comjm)
    LitIntEquiv    (ctrme,ctrme)            # SMM 4/90
    LitIntEquiv    (ground,ground)
    LitIntEquiv    (chaff,chaff)            # jb    7/10/90
    LitIntEquiv    (uvp,uvp)                # reh 2/2/90
    LitIntEquiv    (mdr,mdr)                # 6/90
    LitIntEquiv    (Intra,Intra)            # 8/90
    LitIntEquiv    (emcon,emcon)
    LitIntEquiv    (jamme,jammers)
    LitIntEquiv    (MuRho,MuRho)            # REH 02/29/88
    LitIntEquiv    (navad,navaid)
    LitIntEquiv    (senso,sensors)
    LitIntEquiv    (sat,sats)
    LitIntEquiv    (sonar,sonar)
    LitIntEquiv    (sonob,sonobuoy)
    LitIntEquiv    (trpdo,trpdo)            # SMM 4/90
    LitIntEquiv    (weap,weapons)
    LitIntEquiv    (iff,iffs)               # CML 3/91
    
    literal       rrec[1500]
    integer       irec[$IntRealMatch,1500]
    equivalence   (rrec,irec)

    literal       wrmembr($membersize)
    integer       wimembr($IntRealMatch,$membersize)
    equivalence  (wrmembr,wimembr)

    literal       wrplan($planslen,$plansize,$plans)
    integer       wiplan($IntRealMatch,$planslen,$plansize,$plans)
    equivalence  (wrplan,wiplan)

    literal       wrpaths($pathslen1,$pathslen2,$paths)
    integer       wipaths($IntRealMatch,$pathslen1,$pathslen2,$paths)
    equivalence  (wrpaths,wipaths)

    literal       wrfom($fomplanLen,$fomplan)
    integer       wifom($IntRealMatch,$fomplanLen,$fomplan)
    equivalence  (wrfom,wifom)

    literal       wrsearc($searchlen,$searchsize,$search)
    integer       wisearc($IntRealMatch,$searchlen,$searchsize,$search)
    equivalence  (wrsearc,wisearc)

    literal       wract($actlen,$actsize,$action)
    integer       wiact($IntRealMatch,$actlen,$actsize,$action)
    equivalence  (wract,wiact)

    literal       ftbl[$files]
    integer       lenbf
    integer       itbl[2,$files]
    character     linebf[132]
    vaxcharacter  mdate*17      # jb now needs 17 bytes  1/3/00

    common /CBLOCK_Force/_
                wrforce,        # Literals (Lit-Int Equiv)
                wrorg,
                wremit,
                wrorder,
                wrshore,
                wrsite,
                wrclass,
                wrgroundunit,
                wrvehicle,
                wrair,
                wrcruis,
                wrcommb,
                wrcomms,
                wrcommp,
                wrcomjm,
                wrctrme,
                wrground,
                wrchaff,
                wruvp,
                wrmdr,
                wrIntra,
                wremcon,
                wrjamme,
                wrMuRho,
                wrnavad,
                wrsenso,
                wrsat,
                wrsonar,
                wrsonob,
                wrtrpdo,
                wrweap,
                wriff,
                rrec,       # Literal
                wrmembr,    # More 
                wrplan,
                wrpaths,
                wrfom,
                wrsearc,
                wract,
                ftbl,
                lenbf,      # Integer
                itbl,
                linebf,     # Character
                mdate       # VMSChar
    )



define (top$page,; call fwritf)
define (start$line,{lenbf=0;call fmovx($space,132);lenbf=1})
define (space$over,; lenbf=lenbf+)
define (skip$lines,; call fwritr )
define (start$head,; call fmovt)
define (mv$n,; call fmovib)
define (mv$s,; call fmovh)
define (mv$sn,; call fmovhn)
define (mv$v,; call fmovv)
define (write$line,; call fwrit (linebf,lenbf))

define ($yes,1)
define ($no,0)
define ($read,0)
define ($write,1)

define($pages,10)
define($minbeyond,6)

define(map$work$file,call mapwrk($pages,$minbeyond))
define(unmap$work$file,call mapwrk(-1,0))

define(work$file$common,
       integer    work$file$i[$intrealmatch,$WORKFILESIZE]
       literal    work$file$r[$WORKFILESIZE]

       equivalence (work$file$i,work$file$r)

       common /ccwork/work$file$r
      )

define(work$file$i,wiwork)
define(work$file$r,wrwork)

define(work$area,wiwork[1])             # not used
define(work$min,wiwork[2])              # not used
define(work$page,wiwork[3])             # not used
define(build$name,wrwork[4])
define(scenario$name,wrwork[5])
define(errcnt,wiwork[$halfquad,6])
define(typrun,wrwork[7])
define(typinp,wiwork[8])                # not used
define(fcnt,wiwork[9])                  # not used
define(fuse,wiwork[$halfquad,10])          
define(nforce,wiwork[$halfquad,11])
define(npaths,wiwork[$halfquad,12])
define(nemit,wiwork[$halfquad,13])
define(nmembr,wiwork[$halfquad,14])
define(norder,wiwork[$halfquad,15])
define(nplans,wiwork[$halfquad,16])
define(nsearc,wiwork[$halfquad,17])
define(nshore,wiwork[$halfquad,18])
define(nclass,wiwork[$halfquad,19])
define(nair,wiwork[$halfquad,20])
define(ncruis,wiwork[$halfquad,21])
define(ncommb,wiwork[$halfquad,22])
define(ncomms,wiwork[$halfquad,23])
define(ncommp,wiwork[$halfquad,24])
define(nemcon,wiwork[$halfquad,25])
define(njamme,wiwork[$halfquad,26])
define(nnavad,wiwork[$halfquad,27])
define(nsenso,wiwork[$halfquad,28])
define(nsat,wiwork[$halfquad,29])
define(nsonar,wiwork[$halfquad,30])
define(nsonob,wiwork[$halfquad,31])
define(nweap,wiwork[$halfquad,32])
define(nweath,wiwork[$halfquad,33])
define(naml,wiwork[$halfquad,34])
define(nsos,wiwork[$halfquad,35])       # SMM 4/9/91
define(nswb,wiwork[$halfquad,36])
define(nhfdf,wiwork[$halfquad,37])
define(nssts,wiwork[$halfquad,38])
define(nrpm,wiwork[$halfquad,39])
define(nson,wiwork[$halfquad,40])
define(nact,wiwork[$halfquad,41])
define(nton,wiwork[$halfquad,42])
define(neph,wiwork[$halfquad,43])
define(nMuRho,wiwork[$halfquad,44])     # REH 02/29/88
define(ncomjm,wiwork[$halfquad,45])     # REH 03/01/88
define(nbml,wiwork[$halfquad,46])       # REH 5/9/89
define(nground,wiwork[$halfquad,47])    # REH 11/89
define(nuvp,wiwork[$halfquad,48])       # REH 2/2/90
define(nrothr,wiwork[$halfquad,49])     # RJS 8/86, CML 2/8/90
define(ntrpdo,wiwork[$halfquad,50])     # SMM 4/90
define(nctrme,wiwork[$halfquad,51])     # SMM 4/90
define(nmdr,wiwork[$halfquad,52])       # reh 6/90
define(nchaff,wiwork[$halfquad,53])     # jb 7/6/90
define(nintra,wiwork[$halfquad,54])     # reh 6/90
define(nldr,wiwork[$halfquad,55])       # SMM/CLK 2/91
define(niff,wiwork[$halfquad,56])       # CML 3/91
define(ngroundunit,wiwork[$halfquad,57])#jfr  3/29/91
define(ncbi,wiwork[$halfquad,58])       #jfr  3/29/91
define(natt,wiwork[$halfquad,59])       #jfr  5/14/91
define(nvehicle,wiwork[$halfquad,60])   #JFR 5/24/91
define(norg,wiwork[$halfquad,61])       #JFR 5/30/91
define(nfom,wiwork[$halfquad,62])       # reh 6/91
define(nbis,wiwork[$halfquad,63])       # cml 6/91
define(nsite,wiwork[$halfquad,64])      # MJF 3/26/92
define(nsquadrons, wiwork[$halfquad, 65])    # clk 4/95 (E1394)
