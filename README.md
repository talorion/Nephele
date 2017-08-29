# what & why?

A general purpose data aquisition, data processing, and automation application.

Martin Breitenlechner and Lukas Fischer developed a 3D PTR-MS "PTR3" for cloud9 this is the application controlling the device.


# requirements

## hardware

1) [Tofwerk TOF](http://www.tofwerk.com/) -- for tofdaqdll_system, twtooldll_system, twtps_system

2) TCP I/O Box build by Martin Breitenlechner and Lukas Fischer at [UIBK Institute for Ion Physics and Applied Physics](http://www.uibk.ac.at/ionen-angewandte-physik/)


## software

1) [QT5](http://www.qt.io/)

2) [QCustomPlot](http://www.qcustomplot.com/) for data plotting (included in github repository)

3) TofDaq_API(http://www.soft.tofwerk.com/) for tofdaqdll_system, twtooldll_system, twtps_system

## compilation
1) Install QT with QTScript option enabled and msvc2015-64bit
2) get Visual C++ 2015 Build Tools http://landinghub.visualstudio.com/visual-cpp-build-tools
3) get Windows SDK and install only Debugging Tools for Windows
4) (optional) download an install git (https://git-scm.com/download/win)
5) get sources from github
6) download TofDaq API from http://www.soft.tofwerk.com and copy the include subfolder to .\ext\TofDaq\include subfolder of source
7) patch TofDaq.dll to name the anonymous structs, use _<originalNameOfStruct)


## fonts

1) [Cantarell] (https://www.fontsquirrel.com/fonts/cantarell)

2) [OpenSans] (https://fonts.google.com/specimen/Open+Sans)

3) [orbitron] (https://www.fontsquirrel.com/fonts/orbitron)

# credits

based on code by [Lukas Fischer](https://github.com/lukasfischer83)

# license

[GNU General Public License](https://www.gnu.org/licenses/gpl.txt)

