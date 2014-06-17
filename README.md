MRFEst
======

MuRay Flux estimator - simple flux estimator, based on Honda flux model and attenuation tree 

**Author:** *Andrey Sheshukov*

Requirements
-------------
Linux
Root

Compilation
------------
type

```
make
```
to compile the code. This should create main executable in the root folder: *MuRaySim*

to clean compilation type 
```
make clean
```

Configuration
-----------
Exposition is configured using special config files (see in **config** folder)
For this application, only DEM and DET options are used. 

I.e. for Teide volcano config file should look somewhat like this:

```
#this part describes DEM file to be used
DEM.File:	../DEM/Teide.xyz
DEM.Format:	YX

#this part describes detector configuration
DET.Pos:	339237.38 3128197.85 3560 #detector poition
DET.Ang:	80 0 -50.0    #detector angle
DET.Size:	2.0 1.0       #detector size in meters, not used
DET.Tmax:	1.0           #maximum track slope
DET.Surf:	0.625e4       #detector surface area in m2
DET.ExpDays:	198       #exposure time in days

# exposure: 22.10.2012-??(08).05.2013=198

#this part describes output histogramming options
HIST.TX_nbin:	400
HIST.TX_lim:	-1 1
HIST.TY_nbin:	400
HIST.TY_lim:	-1 1

HIST.N_iter:	5 #number of rays per histogram bin - the more of them, the more accurate will be the estimation.
```

Running
----------
You can run simulation typing (staying in MRFEst folder)
```
./MuRaySim -cfg=config/Teide.cfg
```
This will produce images 

*plots/Teide.png*

*plots/TeideL.png*

and also a root file with same histograms:

*sim/Teide.root*
