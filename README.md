## Optical Flow tracking to Detect head movements;

This code allows to track wether someone is shaking or nodding his head. The respective action will be printed in the left-top corner of the screen. 

As input the system uses webcam input and analyses this in real-time. To decide wether a somebody is shaking or nodding, we make use of an Optical Flow method. The method will add key-points to a detected face (for this we need the HAAR Classifiers for face detection again, more details on that can be found here: [https://github.com/Fritskee/Face-Detection-in-OpenFrameworks-OpenCV/wiki](My repository on face detection with HAAR Classifiers) ) Based on the direction that the key-points move in, we can decide wether somebody either shakes, or nodes his/her head. 

## Setup:
Make sure that the paths to the HAAR Classifiers are set to the correct local path on your system. This must be done in ofApp.cpp 
