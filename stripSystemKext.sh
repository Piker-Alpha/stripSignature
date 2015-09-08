#!/bin/sh
#
# Script (stripSystemKext.sh) to strip CodeSignatures from System.kext
#
# Version 0.2 - Copyright (c) 2015 by Pike <PikeRAlpha@yahoo.com>
#
# Readme......: https://github.com/Piker-Alpha/stripSystemKext.sh/blob/master/README.md
#
# Bug reports.: https://github.com/Piker-Alpha/stripSystemKext.sh/issues
#
#			    Please provide clear steps to reproduce the bug. Thank you!
#

path="$(pwd)"
buildVersion="$(sw_vers -buildVersion)"

cd /System/Library/Extensions

if [[ -f System_kext.$buildVersion ]];
  then
    cp -R System.kext System_kext.$buildVersion
fi

cd System.kext

#
# TODO: Traverse System.kext
#

if [[ -f _CodeSignature ]];
  then
    rm -R _CodeSignature
fi

cd PlugIns

if [[ -f AppleNMI.kext/_CodeSignature ]];
  then
    rm -R AppleNMI.kext/_CodeSignature
fi

if [[ -f ApplePlatformFamily.kext/_CodeSignature ]];
  then
    rm -R ApplePlatformFamily.kext/_CodeSignature
fi

if [[ -f BSDKernel.kext/_CodeSignature ]];
  then
    rm -R BSDKernel.kext/_CodeSignature
fi

if [[ -f IOKit.kext/_CodeSignature ]];
  then
    rm -R IOKit.kext/_CodeSignature
fi

if [[ -f IONVRAMFamily.kext/_CodeSignature ]];
  then
    rm -R IONVRAMFamily.kext/_CodeSignature
fi

if [[ -f IOSystemManagement.kext/_CodeSignature ]];
  then
    rm -R IOSystemManagement.kext/_CodeSignature
fi

if [[ -f Libkern.kext/_CodeSignature ]];
  then
    rm -R Libkern.kext/_CodeSignature
fi

if [[ -f MACFramework.kext/_CodeSignature ]];
  then
    rm -R MACFramework.kext/_CodeSignature
fi

if [[ -f Mach.kext/_CodeSignature ]];
  then
    rm -R Mach.kext/_CodeSignature
fi

if [[ -f Private.kext/_CodeSignature ]];
  then
    rm -R Private.kext/_CodeSignature
fi

if [[ -f Unsupported.kext/_CodeSignature ]];
  then
    rm -R Unsupported.kext/_CodeSignature
fi

#
# The following commands will output: "ERROR: No CodeSignature found!"
# for binaries without a/already stripped of Code Signature block.
#
"${path}/stripSignature" Mach.kext/Mach Mach.kext/Mach
"${path}/stripSignature" IOKit.kext/IOKit IOKit.kext/IOKit
"${path}/stripSignature" Libkern.kext/Libkern Libkern.kext/Libkern
"${path}/stripSignature" Private.kext/Private Private.kext/Private
"${path}/stripSignature" Unsupported.kext/Unsupported Unsupported.kext/Unsupported
"${path}/stripSignature" BSDKernel.kext/BSDKernel BSDKernel.kext/BSDKernel
"${path}/stripSignature" MACFramework.kext/MACFramework MACFramework.kext/MACFramework
