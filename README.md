VNPSP
=====

VNPSP is a PSP runtime for [VNDS](http://digital-haze.net/projects/vnds.html), a Visual Novel engine for Nintendo DS.

Building
--------
Set up a PSP Toolchain on a Linux machine or in a VM using [pspdev/psptoolchain](https://github.com/pspdev/psptoolchain/), then just run `make` in your local copy of this repository.  
Remember to clone submodules too!

Usage
-----
Make a folder in `/GAME` on your memory card, and put the built EBOOT.PBP in it. Then make a folder called `/novels` and put a few VNDS novels in there. Fire VNPSP up and try to read something.  
Note that archived directories (script.zip, background.zip, etc) are NOT currently supported, so you'll have to unzip them first.
