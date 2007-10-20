Name: fastjar
Version: 0.93
Release: 1mdk
Source: http://prdownloads.sourceforge.net/fastjar/fastjar-%{version}.tgz
License: GPL
BuildRoot: %{_tmppath}/%{name}-%{version}-root
Group: System/Archiving
Summary: FastJar is a C copy of Sun's JDK's jar command.
%description
FastJar is an attempt at creating a feature-for-feature copy of Sun's JDK's
'jar' command.  Sun's jar (or Blackdown's for that matter) is written entirely
in Java which makes it dog slow.  Since FastJar is written in C, it can create
the same .jar file as Sun's tool in a fraction of the time.  On my system,
Sun's jar takes 50 seconds to create a 10MB jar file, while FastJar only takes
a little over a second.
%prep
%setup -q
%build
# Hijacked from /usr/lib/rpm/macros, but without the target_arch because that
# sometimes throws old versions of autoconf
CFLAGS="${CFLAGS:-%optflags}" ; export CFLAGS ; \
CXXFLAGS="${CXXFLAGS:-%optflags}" ; export CXXFLAGS ; \
FFLAGS="${FFLAGS:-%optflags}" ; export FFLAGS ; \
./configure \
    --prefix=%{_prefix} \
    --exec-prefix=%{_exec_prefix} \
    --bindir=%{_bindir} \
    --sbindir=%{_sbindir} \
    --sysconfdir=%{_sysconfdir} \
    --datadir=%{_datadir} \
    --includedir=%{_includedir} \
    --libdir=%{_libdir} \
    --libexecdir=%{_libexecdir} \
    --localstatedir=%{_localstatedir} \
    --sharedstatedir=%{_sharedstatedir} \
    --mandir=%{_mandir} \
    --infodir=%{_infodir}
make
%install
make DESTDIR=${RPM_BUILD_ROOT} install
%clean
if test "/" != "${RPM_BUILD_ROOT}"; then rm -rf ${RPM_BUILD_ROOT}; fi
%files
%defattr(-,root,root)
%doc AUTHORS CHANGES COPYING INSTALL LICENSE NEWS README
%{_bindir}

 	  	 
