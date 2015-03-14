SUBUNIT = CUCS_SSV

TOPDIR = -.-.--
!This line must be empty!
! NB: No white-spacing between include directories due to LINT
TARGET_INCLUDES = [.SRC],[-.INC],[-.-.-.-.-.-.-.SRC.COMMON.RCP.INC],GLOBAL_INCLUDES,geo_includes
CXXFLAGS = /INCLUDE_DIRECTORY=([.SRC], [-.INC],GLOBAL_INCLUDES)

! Warning  - the following assumes cpp is not used as part of the file basename
TARGETS = $(SUBST .cpp,,$(SUBST .CPP,,$(WILDCARD [.SRC]*.CPP)))

CXXOBJECTS_DEBUG   = $(ADDSUFFIX .OBJ, $(ADDPREFIX [.OBJ.DEBUG], $(TARGETS)))
CXXOBJECTS_RELEASE = $(ADDSUFFIX .OBJ, $(ADDPREFIX [.OBJ.RELEASE], $(TARGETS)))
LINTOBJECTS        = $(ADDSUFFIX .LXML, $(ADDPREFIX [.OBJ.RELEASE], $(TARGETS)))

DEFAULT :
 ! You must specify target - CLEAN, RELEASE_BUILD, DEBUG_BUILD or TEST_BUILD.
 !
 ! These virtual targets (i.e. targets which do not map into filenames)
 ! all have a "_BUILD" string appended, as Microsoft Visual Studio
 ! /Compaq Enterprise Toolkit insists on creating these directories, 
 ! meaning that e.g. the virtual target "DEBUG" would map to an actual
 ! directory, which is always up-to-date, which would cause MMS to
 ! refuse to perform the build. Now you know.
 !
 ! The CXXFLAGS macro is not inherited - set MY_CXXFLAGS using the
 ! MMS/MACRO=() feature if needed.  
 ! 
 ! Important note: Remember to use MMS/EXTENDED_SYNTAX
 

RELEASE_BUILD :
 MMS $(MMSQUALIFIERS) /DESCRIPTION="[$(TOPDIR).host]gloBALS.MMS + DESCRIP.MMS" -
					  /MACRO=("BUILD=RELEASE", -
							  "CXXOBJECTS=CXXOBJECTS_RELEASE", -
							  "CXXFLAGS=$(CXXFLAGS)/OPTIMIZE", -
                              "CXXLINKFLAGS=$(CXXLINKFLAGS)", -
							  "MOMMA=$@") $(SUBUNIT)_RELEASE.EXE
 

DEBUG_BUILD : 
 MMS $(MMSQUALIFIERS) /DESCRIPTION="[$(TOPDIR).host]gloBALS.MMS + DESCRIP.MMS" -
					  /MACRO=("BUILD=DEBUG", -
							  "CXXOBJECTS=CXXOBJECTS_DEBUG", -
							  "CXXFLAGS=$(CXXFLAGS)/DEBUG/NOOPTIMIZE", -
                              "CXXLINKFLAGS=$(CXXLINKFLAGS)/DEBUG", -
							  "MOMMA=$@") $(SUBUNIT)_DEBUG.EXE

! *** Run tests	
TEST_BUILD :
 !void

! *** Lint build
LINT_BUILD :
 MMS $(MMSQUALIFIERS) /DESCRIPTION="[$(TOPDIR).host]gloBALS.MMS + DESCRIP.MMS" -
                              /MACRO=("BUILD=RELEASE", "TARGET_INCLUDES=$(TARGET_INCLUDES)") [.OBJ.RELEASE]$(SUBUNIT)_FILES.LXML


! *** Clean
CLEAN :
 MMS $(MMSQUALIFIERS) /DESCRIPTION="[$(TOPDIR).host]gloBALS.MMS" BASICCLEAN
! SET DEF [.TEST]
! MMS $(MMSQUALIFIERS) CLEAN 
! SET DEF [-]

! *** Lint
[.OBJ.$(BUILD)]$(SUBUNIT)_FILES.LXML : $(LINTOBJECTS), DESCRIP.MMS, [$(TOPDIR).host]gloBALS.MMS
 DIR [.OBJ.$(BUILD)]*.LXML/OUT=$>
 if RAN$user .eqs. "DEV.BUILD" THEN -
  copy /ftp/log 'user$devtop'.cucs.cucs_ssv.obj.release]*.lxml bambam"hjp hjp"::"./temp/CUCS_SSV/"   
 

! *** Build	
$(SUBUNIT)_$(BUILD).EXE : $($(CXXOBJECTS)) []DESCRIP_$(BUILD).OPT
 ON WARNING THEN user$com:deletefiles "$(SUBUNIT)_$(BUILD).EXE"
 CXXLINK $(CXXLINKFLAGS) /EXECUTABLE=$> []DESCRIP_$(BUILD).OPT/OPTION, $(CXXLINK_SPECIAL)
 if f$search("$(SUBUNIT)_$(BUILD).EXE") .nes. "" then $(PUR) $*.*

[.OBJ.$(BUILD)]MAIN.OBJ							: [.SRC]MAIN.CPP

[.OBJ.$(BUILD)]CUCS_SSV.OBJ						: [.SRC]CUCS_SSV.CPP -
												  [.SRC]CUCS_SSV.H

[.OBJ.$(BUILD)]CUCS_SSVCONFIGURATION.OBJ		: [.SRC]CUCS_SSVCONFIGURATION.CPP -
												  [.SRC]CUCS_SSVCONFIGURATION.H

[.OBJ.$(BUILD)]HANDLESSVFILE.OBJ				: [.SRC]HANDLESSVFILE.CPP -
												  [.SRC]HANDLESSVFILE.H
	