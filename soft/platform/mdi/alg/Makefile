## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##

include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := platform/mdi/alg

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS := \
						platform/mdi/alg/wma_decoder \
						platform/mdi/alg/audiojpeg_dec_voc \
						platform/mdi/alg/audiojpeg_enc_voc \
						platform/mdi/alg/avi_enc \
						platform/mdi/alg/preview \
						platform/mdi/alg/common \
						
ifneq "$(strip ${TARGET_VIDEO_PLAYER_SUPPORT_OFF})" "1" 
LOCAL_MODULE_DEPENDS += platform/mdi/alg/avi_dec \
						platform/mdi/alg/h263_dec \
						platform/mdi/alg/mpeg4_dec \
						platform/mdi/alg/isomedia_demux

ifeq "${TARGET_MEDIA_RM_MODEL}" "1" 						
LOCAL_MODULE_DEPENDS +=platform/mdi/alg/rm_dec \
						platform/mdi/alg/ra8lbr_dec
endif

endif
						


# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
#						platform/mdi \


ifeq "${ANALOG_TV_SUPPORT}" "1"
LOCAL_MODULE_DEPENDS += platform/mdi/alg/analogtv
endif
					

# Set this to a non-null string to signal a toplevel module, like 
# phy but not like phy/kitchensink. This defines the behavior of 
# make deliv
IS_TOP_LEVEL := yes


# This can be used to define some preprocessor variables to be used in 
# the current module, but also exported to all dependencies.
# This is especially useful in an ENTRY_POINT modules
# Ex. : LOCAL_EXPORT_FLAGS += OS_USED DEBUG will result in 
# -DOS_USED -DDEBUG being passed on each subsequent compile command.
LOCAL_EXPORT_FLAG += 




## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
# Assembly / C code
S_SRC :=
C_SRC := 

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk




