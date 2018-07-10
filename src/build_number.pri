win32:{
    BUILDDATE = $$system( date /t ) $$system( time /t )
    BUILD_DATE = $$member(BUILDDATE,0)_$$member(BUILDDATE,1)
    BUILDDATE = $$split(BUILDDATE, /)
    BUILDDATE = $$split(BUILDDATE, :)
    BUILDDATE = $$split(BUILDDATE, .)
}else:unix:!macx: {
    #BUILDDATE = $$system( date "+%d.%m.%Y_%H:%M:%S" )
    BUILD_DATE = $$system( date "+%d.%m.%Y_%H:%M:%S" )
    BUILDDATE = $$system( date $$shell_quote("+%d %m %Y %H %M %S") )
}else:{
    message(no build date defined)
}

#BUILD_NUMBER = $$member(BUILDDATE,1)$$member(BUILDDATE,0)$$member(BUILDDATE,3)$$member(BUILDDATE,4)
#BUILD_NUMBER = $$member(BUILDDATE,1)$$member(BUILDDATE,0)$$member(BUILDDATE,3)
#BUILD_NUMBER = $$member(BUILDDATE,0)$$member(BUILDDATE,3)
BUILD_NUMBER = $$member(BUILDDATE,0).$$member(BUILDDATE,3)$$member(BUILDDATE,4)
#BUILD_NUMBER = 695

#message(BUILDDATE: $$BUILDDATE)
#message(BUILD_DATE: $$BUILD_DATE)
#message(BUILD_NUMBER: $$BUILD_NUMBER)

#DEFINES += BUILD_DATE='"$(shell date)"'
DEFINES += BUILD='"$$BUILD_NUMBER"'
