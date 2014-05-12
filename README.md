ProjIMA
=======

Implementation of [1] and improvements.

### Introduction
Due to the high dynamic of SAR images (because of the strong scatterers), applying a simple total variation (TV) regularization to a SAR image cannot give good results. The aim of [1] is to introduce a TV+L0 decomposition of the scene, in order to have a background regularized with a TV prior and the scatterers with an L0 prior.
In this project we apply the TV+L0 decomposition using two methods:

* Ishikawa's graph on the whole image (as suggested in [1] and described in [2]).
* A block-by-block computation of Ishikawa's graph.

The block-by-block computation aims at solving the high-memory required by the construction of the graph described in [2].
Currently, the project implements Rice, Rayleigh and Gaussian distributions to model the noise. It can be used on both SAR (.imw) and "normal" images.

### Using the project
This project uses CMake as a build system.

* Make sure that CMake is installed on your system.
* Go in the directory of the project and type:
```
cmake . && make
```
* You should have two binaries created in bin/. One of them (Viewer) is a very simple viewer for imw images. The other one, named Denoise works that way:

```
./bin/Denoise -BBV 1 -BS 30 -i inputName -oBV outputNameBV -oS outputNameS -oC outputNameC [-r]
```
This line will apply the decomposition on an image named inputName and will produce 3 images:

* outputNameBV (The background image);
* outputNameS (The scatterers image);
* outputNameC (The complete image, i.e. + outputNameBV + outputNameS).

If you are using a radar image (e.g. image.imw bundled with image.dim), just put *image* (without the extension) as inputName and add the **-r** option.

Other options are available and can be checked by calling Denoise with **-h**. One of them is **-no** which will use the block-by-block computation.

### Bibliography
[1] L. Denis, F. Tupin and X. Rondeau, "Exact discrete minimization for TV+L0 image decomposition models", ICIP 2010, Hong Kong, September 2010.

[2] H. Ishikawa, “Exact optimization for Markov random fields with convex priors,” IEEE TPAMI, vol. 25, no. 10, pp. 1333–1336, 2003.