#!/bin/bash
# ----------------------------------------------------------------------------
__debug__=0; __help__=0
__usage__=0
__clean__=0
# ----------------------------------------------------------------------------
[ "$1" = '--gui'   ] && { __gui__=1; shift;   }
[ "$1" = '-h'      ] && { __help__=1; shift;  }
[ "$1" = '--help'  ] && { __help__=1; shift;  }
[ "$1" = '-d'      ] && { __debug__=1; ECHO=echo;shift; }
[ "$1" = '--debug' ] && { __debug__=1; ECHO=echo;shift; }
[ "$1" = '--usage' ] && { __usage__=1; shift; }
[ "$1" = '--clean' ] && { __clean__=1; shift; }
# ----------------------------------------------------------------------------

##############################################################################
##############################################################################
##############################################################################

# vi:set nu nowrap:

# $Id:$
# AppTech’s LiveCam technology.
#      Author: Ivan carlos da Silva Lopes
#        Mail: lopesivan (dot) ufrj (at) gmail (dot) com
#        Site: http://www.apptechcorp.com
#       Phone: +1 561 801 7985
#    Language: Shell Script
#        File: mkimage1.sh
#        Date: Sun 01 Apr 2012 08:01:31 PM BRT
# Description:
#
#

createImage()
{
	W=$1
	H=$2
	newImage=$3
	COLOR=red
	COLOR=$4
	convert -depth 32 -size ${W}x${H} \
		xc:"$COLOR" -colorspace RGB     \
		$newImage
}

# HD
# 16:9
W=1280
H=800

output=img1.png
createImage $W $H ${output} '#436EEE'

output=img2.png
createImage $W $H ${output} '#FFBBFF'
# ----------------------------------------------------------------------------
exit 0
