#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-PIC32_USB_Starter_Board_PIC32MX460F512L.mk)" "nbproject/Makefile-local-PIC32_USB_Starter_Board_PIC32MX460F512L.mk"
include nbproject/Makefile-local-PIC32_USB_Starter_Board_PIC32MX460F512L.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=PIC32_USB_Starter_Board_PIC32MX460F512L
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=usb/src/usb.c usb/src/usb_hid.c hci.c l2cap.c sdp.c wiimote.c wm_crypto.c wm_reports.c wm_eeprom.c usb_descriptors.c main.c hardware.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/usb/src/usb.o ${OBJECTDIR}/usb/src/usb_hid.o ${OBJECTDIR}/hci.o ${OBJECTDIR}/l2cap.o ${OBJECTDIR}/sdp.o ${OBJECTDIR}/wiimote.o ${OBJECTDIR}/wm_crypto.o ${OBJECTDIR}/wm_reports.o ${OBJECTDIR}/wm_eeprom.o ${OBJECTDIR}/usb_descriptors.o ${OBJECTDIR}/main.o ${OBJECTDIR}/hardware.o
POSSIBLE_DEPFILES=${OBJECTDIR}/usb/src/usb.o.d ${OBJECTDIR}/usb/src/usb_hid.o.d ${OBJECTDIR}/hci.o.d ${OBJECTDIR}/l2cap.o.d ${OBJECTDIR}/sdp.o.d ${OBJECTDIR}/wiimote.o.d ${OBJECTDIR}/wm_crypto.o.d ${OBJECTDIR}/wm_reports.o.d ${OBJECTDIR}/wm_eeprom.o.d ${OBJECTDIR}/usb_descriptors.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/hardware.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/usb/src/usb.o ${OBJECTDIR}/usb/src/usb_hid.o ${OBJECTDIR}/hci.o ${OBJECTDIR}/l2cap.o ${OBJECTDIR}/sdp.o ${OBJECTDIR}/wiimote.o ${OBJECTDIR}/wm_crypto.o ${OBJECTDIR}/wm_reports.o ${OBJECTDIR}/wm_eeprom.o ${OBJECTDIR}/usb_descriptors.o ${OBJECTDIR}/main.o ${OBJECTDIR}/hardware.o

# Source Files
SOURCEFILES=usb/src/usb.c usb/src/usb_hid.c hci.c l2cap.c sdp.c wiimote.c wm_crypto.c wm_reports.c wm_eeprom.c usb_descriptors.c main.c hardware.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-PIC32_USB_Starter_Board_PIC32MX460F512L.mk dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/usb/src/usb.o: usb/src/usb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb.o 
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/usb/src/usb.o.d" -o ${OBJECTDIR}/usb/src/usb.o usb/src/usb.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/usb/src/usb_hid.o: usb/src/usb_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_hid.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_hid.o 
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/usb/src/usb_hid.o.d" -o ${OBJECTDIR}/usb/src/usb_hid.o usb/src/usb_hid.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/hci.o: hci.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hci.o.d 
	@${RM} ${OBJECTDIR}/hci.o 
	@${FIXDEPS} "${OBJECTDIR}/hci.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/hci.o.d" -o ${OBJECTDIR}/hci.o hci.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/l2cap.o: l2cap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/l2cap.o.d 
	@${RM} ${OBJECTDIR}/l2cap.o 
	@${FIXDEPS} "${OBJECTDIR}/l2cap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/l2cap.o.d" -o ${OBJECTDIR}/l2cap.o l2cap.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/sdp.o: sdp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/sdp.o.d 
	@${RM} ${OBJECTDIR}/sdp.o 
	@${FIXDEPS} "${OBJECTDIR}/sdp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/sdp.o.d" -o ${OBJECTDIR}/sdp.o sdp.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/wiimote.o: wiimote.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/wiimote.o.d 
	@${RM} ${OBJECTDIR}/wiimote.o 
	@${FIXDEPS} "${OBJECTDIR}/wiimote.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/wiimote.o.d" -o ${OBJECTDIR}/wiimote.o wiimote.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/wm_crypto.o: wm_crypto.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/wm_crypto.o.d 
	@${RM} ${OBJECTDIR}/wm_crypto.o 
	@${FIXDEPS} "${OBJECTDIR}/wm_crypto.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/wm_crypto.o.d" -o ${OBJECTDIR}/wm_crypto.o wm_crypto.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/wm_reports.o: wm_reports.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/wm_reports.o.d 
	@${RM} ${OBJECTDIR}/wm_reports.o 
	@${FIXDEPS} "${OBJECTDIR}/wm_reports.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/wm_reports.o.d" -o ${OBJECTDIR}/wm_reports.o wm_reports.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/wm_eeprom.o: wm_eeprom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/wm_eeprom.o.d 
	@${RM} ${OBJECTDIR}/wm_eeprom.o 
	@${FIXDEPS} "${OBJECTDIR}/wm_eeprom.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/wm_eeprom.o.d" -o ${OBJECTDIR}/wm_eeprom.o wm_eeprom.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/usb_descriptors.o: usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/usb_descriptors.o.d" -o ${OBJECTDIR}/usb_descriptors.o usb_descriptors.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/hardware.o: hardware.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hardware.o.d 
	@${RM} ${OBJECTDIR}/hardware.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/hardware.o.d" -o ${OBJECTDIR}/hardware.o hardware.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/usb/src/usb.o: usb/src/usb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb.o 
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/usb/src/usb.o.d" -o ${OBJECTDIR}/usb/src/usb.o usb/src/usb.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/usb/src/usb_hid.o: usb/src/usb_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/usb/src" 
	@${RM} ${OBJECTDIR}/usb/src/usb_hid.o.d 
	@${RM} ${OBJECTDIR}/usb/src/usb_hid.o 
	@${FIXDEPS} "${OBJECTDIR}/usb/src/usb_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/usb/src/usb_hid.o.d" -o ${OBJECTDIR}/usb/src/usb_hid.o usb/src/usb_hid.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/hci.o: hci.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hci.o.d 
	@${RM} ${OBJECTDIR}/hci.o 
	@${FIXDEPS} "${OBJECTDIR}/hci.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/hci.o.d" -o ${OBJECTDIR}/hci.o hci.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/l2cap.o: l2cap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/l2cap.o.d 
	@${RM} ${OBJECTDIR}/l2cap.o 
	@${FIXDEPS} "${OBJECTDIR}/l2cap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/l2cap.o.d" -o ${OBJECTDIR}/l2cap.o l2cap.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/sdp.o: sdp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/sdp.o.d 
	@${RM} ${OBJECTDIR}/sdp.o 
	@${FIXDEPS} "${OBJECTDIR}/sdp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/sdp.o.d" -o ${OBJECTDIR}/sdp.o sdp.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/wiimote.o: wiimote.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/wiimote.o.d 
	@${RM} ${OBJECTDIR}/wiimote.o 
	@${FIXDEPS} "${OBJECTDIR}/wiimote.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/wiimote.o.d" -o ${OBJECTDIR}/wiimote.o wiimote.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/wm_crypto.o: wm_crypto.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/wm_crypto.o.d 
	@${RM} ${OBJECTDIR}/wm_crypto.o 
	@${FIXDEPS} "${OBJECTDIR}/wm_crypto.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/wm_crypto.o.d" -o ${OBJECTDIR}/wm_crypto.o wm_crypto.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/wm_reports.o: wm_reports.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/wm_reports.o.d 
	@${RM} ${OBJECTDIR}/wm_reports.o 
	@${FIXDEPS} "${OBJECTDIR}/wm_reports.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/wm_reports.o.d" -o ${OBJECTDIR}/wm_reports.o wm_reports.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/wm_eeprom.o: wm_eeprom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/wm_eeprom.o.d 
	@${RM} ${OBJECTDIR}/wm_eeprom.o 
	@${FIXDEPS} "${OBJECTDIR}/wm_eeprom.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/wm_eeprom.o.d" -o ${OBJECTDIR}/wm_eeprom.o wm_eeprom.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/usb_descriptors.o: usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/usb_descriptors.o.d" -o ${OBJECTDIR}/usb_descriptors.o usb_descriptors.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/hardware.o: hardware.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hardware.o.d 
	@${RM} ${OBJECTDIR}/hardware.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"usb/include" -MMD -MF "${OBJECTDIR}/hardware.o.d" -o ${OBJECTDIR}/hardware.o hardware.c    -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--no-code-in-dinit,--no-dinit-in-serial-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_PIC32_USB_Starter_Board_PIC32MX460F512L=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/PIC32_USB_Starter_Board_PIC32MX460F512L
	${RM} -r dist/PIC32_USB_Starter_Board_PIC32MX460F512L

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
