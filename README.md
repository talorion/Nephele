# what & why?

A general purpose data aquisition, data processing, and automation application.

Martin Breitenlechner and Lukas Fischer developed a PTR-MS for cloud9 this is the application controlling the device.


# requirements

## hardware

1) [TFA-Dostmann AirControl Mini CO2 Messgerät](http://www.amazon.de/dp/B00TH3OW4Q) -- for the co2sensor_system

2) [Tofwerk TOF](http://www.tofwerk.com/) -- for tofdaqdll_system, twtooldll_system, twtps_system

3) TCP I/O Box build by Martin Breitenlechner and Lukas Fischer at [UIBK Institute for Ion Physics and Applied Physics](http://www.uibk.ac.at/ionen-angewandte-physik/)


## software

1) [QT5](http://www.qt.io/)

2) [QCustomPlot](https://github.com/dbzhang800/QCustomPlot) for data plotting

3) TofDaq_1.97_API  for tofdaqdll_system, twtooldll_system, twtps_system

4) [GNU OCTAVE](https://gnu.org/software/octave/) for octave_system

5) [libusb](http://www.libusb.org/) for the co2sensor_system



# credits

based on code by [Lukas Fischer](https://github.com/lukasfischer83)
based on code by [henryk ploetz](https://hackaday.io/project/5301-reverse-engineering-a-low-cost-usb-co-monitor/log/17909-all-your-base-are-belong-to-us)

based on an article by [Nick Prühs](http://www.heise.de/developer/artikel/Component-Based-Entity-Systems-in-Spielen-2262126.html)
based on an article by [Adam Martin](http://t-machine.org/index.php/2007/09/03/entity-systems-are-the-future-of-mmog-development-part-1/)

# license

[CC by-nc-sa](https://creativecommons.org/licenses/by-nc-sa/4.0/)
