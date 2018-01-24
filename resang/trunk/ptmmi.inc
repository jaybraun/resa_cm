#		PTMMI.INC

undefine read
undefine write
undefine end
undefine err
undefine bar

define ($off,		zero)
define ($bold,		one)
define ($underline,	four)
define ($blink,		five)
define ($reverse,	seven)

define ($yes,		1)
define ($no,		0)
define ($eof,		11) 	  	# EOF encountered on input
define ($playbuflen,	MBX$MsgSize)  	# max length of player cmd

define ($data,		2)   # flag to indicate user data to Stuffdisk
define ($dynmenuseg,	115) # segment number of start of dynamic menu pages
define ($inputblkseg,	95)  # segment number of start of "<" blks in menu

define ($header,	1)   # parameter to mean skip an entry in dynamic menu
define ($maxbackup,	15)  # maximum number of backups possible
define ($maxmenus,	300) # maximum number of menus that can be defined 
define ($maxdynslots,	600) # maximum number of entries in dynamic menu
define ($null,		0)   # flag to indicate no data, no header to Stuffdisk

define ($top_menu,	11)  # top level menu number
define ($launch_submenu,77)  # flight plan menu number
define ($pim_submenu,	87)  # proceed PIM sub-menu number

    			     # don't use menu numbers > 90!!

define ($disk,    1)  		# logical unit for menu file
define ($megdsk,  2) 		# logical unit for megatek disk file 
define ($printer, 3)  		# logical unit for printer file
define ($megmap,  4)		# logical unit for megatek file for map to save
define ($dynfile, 7)		# logical unit for dynamic menu file

define (OCTALS,character esc  /8%033/,  semi  /8%073/,
	pound /8%043/,	lpar  /8%050/,	rpar  /8%051/,	
	upl   /8%154/,	lowr  /8%152/,	upr   /8%153/,
	lowl  /8%155/,	cross /8%156/,	leftt /8%164/,
	rightt/8%165/,	bott  /8%166/,	topt  /8%167/, bar   /8%170/,
	zero  /8%060/, one   /8%061/,	two   /8%062/,	three /8%063/,
	four  /8%064/,	five  /8%065/,	six   /8%066/,	seven /8%067/,
	eight /8%070/,	nine  /8%071/,  bell  /8%007/,
    	null  /8%000/, carrot/8%136/, linefeed /8%012/,car_ret/8%015/)

#	start DYNCOM **********************************************************

define (DYNCOM,

    	integer   bitmap[2],binary_interval[2],EF_1,EF_2,EF_3,
    		  idiskbuf[127],mystation

    	INTEGER*2 bufentry,diskblk,dyncode,i2diskbuf[254],
    		  myside,myview,playchan,relblk

    	character ldiskbuf[508],platform[8]

    	vaxcharacter cbuffer*$playbuflen

    	equivalence (idiskbuf,i2diskbuf,ldiskbuf)
    	equivalence (bitmap[1],idiskbuf[3])

    COMMON /DYN/ binary_interval,EF_1,EF_2,EF_3,idiskbuf,	# i*4
    		 mystation,					# i*4

    		 bufentry,diskblk,dyncode,			# i*2
    		 myside,myview,playchan,relblk,			# i*2

    		 platform,					# Char

    		 cbuffer					# VaxC

    	)
#	end DYNCOM ************************************************************




#	start MENCOM **********************************************************

define (MENCOM,		#  
    	OCTALS

    	integer   mystation

    	INTEGER*2 bkplay[$maxbackup],blue[$maxbackup,50],
    		  green[$maxbackup,50],icalid[160],
    		  inpseg[20],mentbl[100],
    		  bkptr,cr,current_page,do_dynamic,dspmenu,
    		  dynchan,dyncode,dynmenunum,dynmenu_ptr,
    		  inpsegptr,lastpage,lasthead_ptr,
    		  menumode,menunum,myside,myview,playchan,
    		  yellow_time,plotmenu,cursor_flag,latblk,trace

    	character dynmenu[10,$maxdynslots],menu[18,100],platform[8],
    		  addressee_type,lat_long[17]

    	vaxcharacter cbuffer*$playbuflen	# used in write_mailbox

    COMMON /MMI/mystation,					# i*4

    		bkplay,blue,green,icalid,			# i*2
    		inpseg,mentbl,					# i*2
    		bkptr,cr,current_page,do_dynamic,dspmenu,	# i*2
    		dynchan,dyncode,dynmenunum,dynmenu_ptr,		# i*2
    		inpsegptr,lastpage,lasthead_ptr,		# i*2
    		menumode,menunum,myside,myview,playchan,	# i*2
    		yellow_time,plotmenu,cursor_flag,		# i*2
    		latblk,trace,					# i*2

    		dynmenu,menu,platform,				# Char
    		addressee_type,lat_long,			# Char
		esc,semi,					# Char
    		pound,lpar,rpar,upl,lowr,			# Char
    		upr,lowl,cross,leftt,rightt,bott,		# Char
    		topt,bar,zero,one,two,three,four,		# Char
		five,six,seven,eight,nine,bell,			# Char
    		null,carrot,linefeed,car_ret,			# Char

    		cbuffer						# VaxC

    	)

#	end MENCOM ***********************************************************

