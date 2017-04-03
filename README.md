# Multiple Object Tracking
Created a basic paint application in VC++ that allowed the user to draw on a canvas using only gestures. Our application can track the position of the marker and applied curve generation algorithms to generate smooth drawings. The application also allows the user to zoom in/out by pinch in/out gestures using markers. We used OPENCV library for image processing. 

I this project we designed our own algorithms for zooming in and zooming out gestures. We also designed a curve generatin algorithm that rendered smooth curves. Various techniques are used for object detection, which are documented in the code itself.

## Getting Started

Install git (http://git-scm.com/) onto your system. Then run a clone:

```
https://github.com/ShivBhosale/MultipleObjectTracking.git
```

This will download the Application Source Code.

### Prerequisites

What things you need to use the application and how to install them

1) Visual Studio 10

2) OpenCV

```
https://sourceforge.net/projects/opencvlibrary/

```
### Installing

-create a new project in visual studio and add the given files to the project and you are ready to go


## Running the tests

- Initially select the HSV slider and calibrate the paintbrush color. It is recomended to use bright colored pointers for better drawing
  enable the caliberation in "mulObjTrack.cpp" by changing the boolean on line 499:
  
  ```
  bool calibrationMode = true;
  ```
- Disable the caliberation (same line) to begin using the paint application by:

 ```
  bool calibrationMode = false;
  ```

## Built With

* VC++
* OpenCV

## Authors

* **Shivjeet Bhosale** - (https://github.com/ShivBhosale)

## Acknowledgments

* My team members who were as "jugaadu" as me
* Kyle Hunslow's YouTube tutorials on Image Processing
* PurpleBooth for this Readme.md template
