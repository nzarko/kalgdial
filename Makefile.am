SUBDIRS = $(TOPSUBDIRS)

EXTRACTRC=extractrc
podir=$(top_srcdir)/po
$(top_srcdir)/configure.in: configure.in.in $(top_srcdir)/subdirs
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common configure.in ;

$(top_srcdir)/subdirs:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common subdirs

$(top_srcdir)/acinclude.m4: $(top_srcdir)/admin/acinclude.m4.in $(top_srcdir)/admin/libtool.m4.in
	@cd $(top_srcdir) && cat admin/acinclude.m4.in admin/libtool.m4.in > acinclude.m4

MAINTAINERCLEANFILES = subdirs configure.in acinclude.m4 configure.files 

package-messages:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common package-messages
	$(MAKE) -C po merge

messages:
	$(EXTRACTRC) `find . -name "*.rc" -o -name "*.ui" -o -name "*.kcfg"` > rc.cpp
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name *.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
			$(XGETTEXT) $$LIST -o $(podir)/kalgdial.pot; \
	fi
	rm -f rc.cpp
#$(EXTRACTRC) *.rc */*.rc >> rc.cpp
#$(EXTRACTRC) src/viewloginfobase.ui >> rc.cpp
#$(XGETTEXT) rc.cpp src/*.h src/*.cpp \
#-o $(podir)/kalgdial.pot

EXTRA_DIST = admin COPYING configure.in.in

dist-hook:
	cd $(top_distdir) && perl admin/am_edit -padmin
	cd $(top_distdir) && $(MAKE) -f admin/Makefile.common subdirs
