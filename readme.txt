CONTENTS OF THIS FILE
---------------------
   
 * Authors
 * Introduction
 * Requirements
 * Configuration
 * Execute Code
 * Folder Structure


AUTHORS
-------

This project is created by:
	* Anders Buron Christiansen, s103685, M.Sc. DTU Compute
	* Themi Tsiotas von Pfaler,  s113603, M.Sc. DTU Compute


INTRODUCTION
------------

The project contains the source code for the implementation of the topic Relatives Similarity Scores (RSS) for the course 02238 Biometric Systems, June 2016, at the Technical University of Denmark (DTU).

The project is made as an Eclipse Project that uses OpenCV and its Local Binary Pattern algorithm to perform a face recognition between a set of probes and a training set. 


Anders Buron Christiansen\\
    \affaddr{Technical University\\ of Denmark}\\
    \affaddr{M.Sc. DTU Compute}\\
    \affaddr{s103685}\\
    \affaddr{Team Member}


REQUIREMENTS
------------

These libraries are necessary to install:

* OpenCV - Version 2.4.13 
	Website: (http://opencv.org/downloads.html)
	Easy to install with Homebrew (http://brew.sh/)

* Eclipse for C/C++ Developers, version Mars.2 Release (4.5.2) 
	Website: (https://eclipse.org/downloads/packages/eclipse-ide-cc-developers/mars2)


CONFIGURATION
-------------

When the project has been opened with Eclipse, the project needs to be configured to work correctly:

1. Right click on the project and select 'Properties' from the menu.

2. In the left side navigate to 'C/C++ Build' -> 'Settings'.

3. Navigate to 'GCC C++ Compiler' -> 'Includes'.

4. Under 'Include paths (-l)' add the path to the installed opencv.

5. Navigate to 'MacOS X C++ Linker' -> 'Libraries'.

6. Add the following libraries:
	opencv_legacy
	opencv_gpu
	opencv_flann
	opencv_features2d
	opencv_contrib
	opencv_calib3d
	opencv_core
	opencv_imgproc
	opencv_videostab
	opencv_video
	opencv_ts
	opencv_superres
	opencv_stitching
	opencv_photo
	opencv_ocl
	opencv_objdetect
	opencv_nonfree
	opencv_ml
	opencv_highgui

7. Under 'Library search path' add the path to the installed opencv's lib folder.

8. Press 'Apply'.

You are now done with the configurations.


EXECUTE CODE
------------

Now the code can be executed with the following arguments:

	input/facerec_own.txt input/facerec_test.txt

This will generate a csv file with the similarity scores for the two inputs that will be stored in the folder 'output'.


FOLDER STRUCTURE
----------------
Each folder will be briefly explained here:

	* src : Contains the source code for implementation of OpenCV's LBP algoritm

	* output : The generate csv file with the similarity scores will be stored here.

	* input : The csv files that contains the references to all the pictures, IDs and labels are here.

	* faces : The folder with all the gathered pictures of relatives and public figures. The content is further divided into: 
		- celebs : The pictures of the colllected celebrites.

		- s# : Contains the training set.

		- test : Contains the testing set (aka. the probes).

	* att_faces : the AT&T public face database.