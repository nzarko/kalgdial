# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# these are the headers for your project
noinst_HEADERS = kalgdial.h dockwidget.h kalgdialview.h pppstats.h utils.h \
		kalgpppconfig.h loginfo.h loginfoview.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

KDE_ICON = AUTO

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kalgdial

# the application source, library search path, and link libraries
kalgdial_SOURCES = viewloginfobase.ui main.cpp kalgdialview.cpp kalgdial.cpp dockwidget.cpp \
		pppstats.cpp utils.cpp loginfo.cpp loginfoview.cpp
kalgdial_LDFLAGS = $(KDE_RPATH) $(all_libraries)
kalgdial_LDADD = $(LIB_KDEUI) -lkio

# this is where the desktop file will go 
shelldesktopdir   = $(kde_appsdir)/Utilities
shelldesktop_DATA = kalgdial.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir   = $(kde_datadir)/kalgdial
shellrc_DATA = kalgdialui.rc

