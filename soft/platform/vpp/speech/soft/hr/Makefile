## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/vpp/speech/soft/hr

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS ?=    platform \
                        platform/base/common \
                        platform/vpp/speech/soft/hr \
                        platform/chip/hal \
                        platform/chip/defs
                         

# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS ?= 
                    
## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
include src/make.srcs

AUTO_XCP2H ?= yes



## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
