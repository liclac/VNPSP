VNPSP
=====

VNPSP is a PSP runtime for [VNDS](http://digital-haze.net/projects/vnds.html), a Visual Novel engine for Nintendo DS.

Building
--------
Set up a PSP Toolchain on a Linux machine or in a VM using [pspdev/psptoolchain](https://github.com/pspdev/psptoolchain/), then just run `make` in your local copy of this repository.  
Remember to clone submodules too!

Usage
-----
* Put the built EBOOT.PBP in `ms0:/PSP/GAME/VNPSP/`
* Put a VN or a few in `ms0:/novels/`
* Activate your CFW and fire up VNPSP

It's that easy.

**Note:**  
A lot of VNs come partially zipped, as a directory containing a `background.zip`, a `sound.zip`, a `script.zip`, etc. This is not yet supported, and will fail to load.  
For now, just extract the files - all files in `<novel>/script.zip` to `<novel>/script/`, etc.
