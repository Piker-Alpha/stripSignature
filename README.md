
csrutil enable --without kext

kextcache should then output this:

com.apple.kextcache[NNN]: kext-dev-mode allowing invalid signature -67062 0xFFFFFFFFFFFEFA0A for kext Unsupported.kext
com.apple.kextcache[NNN]: kext-dev-mode allowing invalid signature -67062 0xFFFFFFFFFFFEFA0A for kext Private.kext
com.apple.kextcache[NNN]: kext-dev-mode allowing invalid signature -67062 0xFFFFFFFFFFFEFA0A for kext Mach.kext
com.apple.kextcache[NNN]: kext-dev-mode allowing invalid signature -67062 0xFFFFFFFFFFFEFA0A for kext MACFramework.kext
com.apple.kextcache[NNN]: kext-dev-mode allowing invalid signature -67062 0xFFFFFFFFFFFEFA0A for kext Libkern.kext
com.apple.kextcache[NNN]: kext-dev-mode allowing invalid signature -67062 0xFFFFFFFFFFFEFA0A for kext IOSystemManagement.kext
com.apple.kextcache[NNN]: kext-dev-mode allowing invalid signature -67062 0xFFFFFFFFFFFEFA0A for kext IONVRAMFamily.kext
com.apple.kextcache[NNN]: kext-dev-mode allowing invalid signature -67062 0xFFFFFFFFFFFEFA0A for kext IOKit.kext
com.apple.kextcache[NNN]: kext-dev-mode allowing invalid signature -67062 0xFFFFFFFFFFFEFA0A for kext BSDKernel.kext
com.apple.kextcache[NNN]: kext-dev-mode allowing invalid signature -67062 0xFFFFFFFFFFFEFA0A for kext ApplePlatformFamily.kext
com.apple.kextcache[NNN]: kext-dev-mode allowing invalid signature -67062 0xFFFFFFFFFFFEFA0A for kext AppleNMI.kext
com.apple.kextcache[NNN]: kext-dev-mode allowing invalid signature -67062 0xFFFFFFFFFFFEFA0A for kext System.kext

The prelinkedkernel is nicely rebuilt, with our unsigned copy of System.kext in it, as we can see in system.log:

com.apple.kextcache[NNN]: Created old kernelcache copy "/System/Library/Caches/com.apple.kext.caches/Startup/kernelcache"
com.apple.kextcache[NNN]: Created prelinked kernel "/System/Library/PrelinkedKernels/prelinkedkernel"
com.apple.kextcache[NNN]: Created prelinked kernel using "/System/Library/Kernels/kernel"

Oh. And this is the only non-fatal warning I could find in system.log:

com.apple.kextd[NN]: validateKextsAlertDict null array

I think that this should be a fatal error.

You can even run this without issues:

csrutil enable

That is… until the prelinkedkernel is to be updated, because then kextcache rejects the unsigned copy of System.kext