Author: Calvin Lobo
Date: September 2017


== Overview ==


This project primarily consist of the following sub-projects:

'''sensor_network''' - Networking and message transfer of sensor data including Radar, GPS, odometry, etc.

'''common_utils''' - Constants, definitions and utility functions.

'''processing_lib''' - Signal processing algorithms and image creation.

'''3rdparty''' - Additional libraries required by this repository.




== Goal ==

Create one common API that can unify all applications moving forward. Software should be built to be cross-platform
(Windows and Linux and DVL500) as well as callable from both python and C++.


== Dependencies ==

The goal is to not re-create the wheel and use mature and well documented open-source software projects to acheive our
goals. Some of these projects include

ROS (Robot Operating System) - Network message passing

OpenCV - Image processing / creation and machine learning

Boost Python - C++ Python bindings


== Design Philosophy ==

In order to maximize the productivity and leverage the expertise of different departments, the code is carefully
compartmentalized into a defined structure that aims to reduce the overlap of department specific knowledge. The different
compartments (or layers) of the software are described below:


Basic C Functions (C, Algorithm Logic, Primitive Layer)
-------------------------------------------
The signal processing software stored in processing_lib/src/processing should be entirely written in basic C with minimal dependencies.
This allows the science group to more easily write new algorithms without having to rely on experience with higher level custom data structures.
These algorithms should use basic raw arrays, and built-in data types (float, boolean, int, char*).


High Level Wrappers (C++ / Python, Accessibility Layer)
--------------------------------------------------------
To expose the algorithm logic (primitive layout) in a more user-accessible way we create a C++ wrapper layer. These C++ wrappers have the advantage of
leveraging higher-level data structures in C++ such as classes, vectors, maps and strings. The wrapper functions can (and should)
accept higher-level data structures as arguments, unwrap them to their primitive C form and then pass them to the basic C functions.
Having this layer written in C++ provides fast performance as well as the ability to expose the functions to Python.


Graphics and Visualization (C++/ Python, Graphics Layer)
---------------------------------------------------------

In order to better analyze and display information to the user, data must be placed in a visual form. This include plots such as
the cross section, trace plot, depth slices and map view. Data visualization often have specific requirements that are often
not easily satisfied by external plotting libraries. For this reason it is best to create our own custom plots using a low level
graphics drawing library. OpenCV provides basic drawing functionality as well as keyboard / mouse input handling. In addition to this
OpenCV was designed to handle computer vision and machine learning tasks which may be useful in future applications. OpenCV provides
both C++ and python APIs. This layer should be implemented in C++ but exposed to Python via the Accessibility Layer.


Recipes and Procedures (C++ / Python, Procedure Layer)
-------------------------------------------------------

Procedures or recipes are sequences of steps that are followed in order to achieve a specific outcome for the user.
This includes applications such as depth slicing, auto-velocity calibratiions and pavement density calculations.
Procedures sequence algorithms from the Accessibility Layer in order to create a easily packaged solution to a problem.
These procedures can be easily and rapidly prototyped in Python and then finalized in a C++ library.


Networking and Message Transfer (C++ / Python, Network Layer)
--------------------------------------------------------------

The networking layer is in charge of receiving data from a variety of sensors, packaging them into pre-defined and standardized
messages and then broadcasting them to the network. "Network" does not need to imply that message transfer is done between different PCs,
but software must be designed to be generic enough to handle interprocess and inter-PC communication. In addition to this, the network
layer should leverage a publisher-subscriber model so that each "node" can be developed independently given the constraint of what
data it is expecting. The ROS (Robot Operating System) open source project provides a powerful C++ and Python API for developing
publisher-subscriber networks. Additionality it comes with tools for analyzing node networks and recording and playing back messages
in simulated time. An example of a publisher-subscriber node network could be the following:

Radar node (publisher) ------------->   Controller (publisher + subscriber) ----> Application (publisher + subscriber) --> File (subscriber)

Odometer node (publisher) -------------------------^


Common Utilities, Constants and Definitions (C++ / Python, Utilties)
-------------------------------------------------------------------------

During software development at every layer there will be certain commonly used functions, constants and definitions.
In order to reduce code duplication, these functions, constants and definitions should be kept in a single library.
src/common_utils should contain software such as depth time conversions, array manipulation routines, typedefs and constants.
Constants can be kept in a C++ header file (.h) but will need to be compiled into a separate library in order for Python
to import them.


== Setup ==


Clone the multisensor_framework repository

Install the dependencies (ROS, Boost, OpenCV as well as set up the custom environment) with install_dependencies.bash

Source the env.sh script to set the environment variables used throughout the build process with Make. (You can source this in your .bashrc file so it happens everytime you open a shell)

The install dependencies script should place the '''sandbox''' utility in your /usr/local/bin directory. If not you can do this by going into the sandbox directory and typing the following command:

'''ln -s $SANDBOX_WS/sandbox /usr/local/bin/sandbox'''

The sandbox utility provides convenient shortcuts to common tasks such as building all or a specifc library, running scripts in the sandbox directory and cleaning compiled files.

Type sandbox --help for more details.


== Writing Test Scripts ==

Testing is usually done in the sandbox directory. You can create python scripts in this directory and have them import
the compiled C/C++ library from sandbox/pylib.

Of course, you need to build libraries first with '''sandbox build''' and then place the built libraries in pylib.
If you call '''sandbox run <path to python script in sandbox directory>''' it will automatically copy the latest built
libraries from the other directories and then run your script.


== Auto-documentation ==

It is crucial that everything added to this library be document usings the Doxygen commenting standards.
Doxygen provides a way of autogenerating web based documentation for the code by looking at specific comments around
the declaration of files, functions and classes/structs.

You must install doxygen:

apt-get install doxygen

You can generate the documentation for multisensor_framework by running the update_docs.sh script.

See (http://www.stack.nl/~dimitri/doxygen/) for more information of comment styling.

