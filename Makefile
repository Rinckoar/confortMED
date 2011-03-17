#2011-03-07  Juan Felipe Restrepo <jf.restrepo.rinckoar@gmail.com>
# Makeflie for CCS Pic C Compiler

PROJECT= confortMED
VERSION= v0.0.2
AUTHOR= Juan Felipe Restrepo <jf.restrepo.rinckoar@gmail.com>

DEVICE=18F452

CC=ccsc
CCPATH = $(HOME)/.wine/drive_c/Program\ Files/PICC
CONFIG_CC= wine $(CCPATH)/$(CC)

LOCAL_PATH=$(HOME)/prog_home/$(PROJECT)
LOCAL_LIB=$(LOCAL_PATH)/lib
LOCAL_INCLUDE=$(LOCAL_PATH);$(LOCAL_PATH)/src

CCFLAGS= +STDOUT +EA -P +DF +LSlst +O8hex +M -J -A +FH I+="$(LOCAL_PATH);$(LOCAL_INCLUDE)"

SRC=*.c 

all:
	$(CONFIG_CC) $(CCFLAGS) $(SRC)
	mv *.cof *.err *.esym *.hex *.lst *.sym $(LOCAL_LIB)



clean:
	@echo "[Makefile] Cleaning..."
	@rm -fv $(LOCAL_LIB)/*.cof
	@rm -fv $(LOCAL_LIB)/*.err 
	@rm -fv $(LOCAL_LIB)/*.esym 
	@rm -fv $(LOCAL_LIB)/*.hex 
	@rm -fv $(LOCAL_LIB)/*.lst 
	@rm -fv $(LOCAL_LIB)/*.sym 

info:
	@echo "[Makefile] Info:"
	@echo "  Project:   $(PROJECT)"
	@echo "  Version:   $(VERSION)" 
	@echo "  Author:    $(AUTHOR)"
	@echo "  Device:    $(DEVICE)"











