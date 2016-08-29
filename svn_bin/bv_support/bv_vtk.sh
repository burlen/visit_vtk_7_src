function bv_vtk_initialize
{
    export DO_VTK="yes"
    export ON_VTK="on"
    export FORCE_VTK="no"
    export USE_SYSTEM_VTK="no"
    add_extra_commandline_args "vtk" "system-vtk" 0 "Using system VTK (exp)"
    add_extra_commandline_args "vtk" "alt-vtk-dir" 1 "Use alternate VTK (exp)"
}

function bv_vtk_enable
{
    DO_VTK="yes"
    ON_VTK="on"
    FORCE_VTK="yes"
}

function bv_vtk_disable
{
    DO_VTK="no"
    ON_VTK="off"
    FORCE_VTK="no"
}

function bv_vtk_system_vtk
{
    TEST=`which vtk-config`
    [ $? != 0 ] && error "System vtk-config not found, cannot configure vtk"

    bv_vtk_enable
    USE_SYSTEM_VTK="yes"
    SYSTEM_VTK_DIR="$1"
    info "Using System VTK: $SYSTEM_VTK_DIR"
}

function bv_vtk_alt_vtk_dir
{
    bv_vtk_enable
    USE_SYSTEM_VTK="yes"
    SYSTEM_VTK_DIR="$1"
    info "Using Alternate VTK: $SYSTEM_VTK_DIR"
}

function bv_vtk_depends_on
{
    depends_on="cmake"

    if [[ "$DO_PYTHON" == "yes" ]]; then
        depends_on="${depends_on} python"
    fi

    if [[ "$DO_R" == "yes" ]]; then
        depends_on="${depends_on} R"
    fi

    # Only depend on Qt if we're not doing server-only builds.
    if [[ "$DO_DBIO_ONLY" != "yes" ]]; then
        if [[ "$DO_ENGINE_ONLY" != "yes" ]]; then
            if [[ "$DO_SERVER_COMPONENTS_ONLY" != "yes" ]]; then 
                depends_on="${depends_on} qt"
            fi
        fi
    fi

    echo ${depends_on}
}

function bv_vtk_force
{
    if [[ "$FORCE_VTK" == "yes" ]]; then
        return 0;
    fi
    return 1;
}

function bv_vtk_info
{
    export VTK_FILE=${VTK_FILE:-"VTK-dbae27c.tar.gz"}
    export VTK_VERSION=${VTK_VERSION:-"7.1.0"}
    export VTK_SHORT_VERSION=${VTK_SHORT_VERSION:-"7.1"}
    export VTK_COMPATIBILITY_VERSION=${VTK_SHORT_VERSION}
    export VTK_BUILD_DIR=${VTK_BUILD_DIR:-"VTK-dbae27c"}
    export VTK_INSTALL_DIR=${VTK_INSTALL_DIR:-"vtk"}
    export VTK_MD5_CHECKSUM=""
    export VTK_SHA256_CHECKSUM=""
}

function bv_vtk_print
{
    printf "%s%s\n" "VTK_FILE=" "${VTK_FILE}"
    printf "%s%s\n" "VTK_VERSION=" "${VTK_VERSION}"
    printf "%s%s\n" "VTK_BUILD_DIR=" "${VTK_BUILD_DIR}"
}

function bv_vtk_print_usage
{
    printf "%-15s %s [%s]\n" "--vtk" "Build VTK" "built by default unless --no-thirdparty flag is used"
    printf "%-15s %s [%s]\n" "--system-vtk" "Use the system installed VTK"
    printf "%-15s %s [%s]\n" "--alt-vtk-dir" "Use VTK from an alternative directory"
}

function bv_vtk_host_profile
{
    echo >> $HOSTCONF
    echo "##" >> $HOSTCONF
    echo "## VTK" >> $HOSTCONF
    echo "##" >> $HOSTCONF

    echo "SETUP_APP_VERSION(VTK $VTK_VERSION)" >> $HOSTCONF
    if [[ "$USE_SYSTEM_VTK" == "yes" ]]; then
        echo "VISIT_OPTION_DEFAULT(VISIT_VTK_DIR $SYSTEM_VTK_DIR)" >> $HOSTCONF
    else
        echo "VISIT_OPTION_DEFAULT(VISIT_VTK_DIR \${VISITHOME}/${VTK_INSTALL_DIR}/\${VTK_VERSION}/\${VISITARCH})" >> $HOSTCONF
    fi
}

function bv_vtk_initialize_vars
{
    info "initalizing vtk vars"
    if [[ $DO_R == "yes" ]]; then
        VTK_INSTALL_DIR="vtk-r"
    fi
}

function bv_vtk_ensure
{
    if [[ "$DO_VTK" == "yes" && "$USE_SYSTEM_VTK" == "no" ]] ; then
        ensure_built_or_ready $VTK_INSTALL_DIR $VTK_VERSION $VTK_BUILD_DIR $VTK_FILE $VTK_URL
        if [[ $? != 0 ]] ; then
            return 1
        fi
    fi
}

function bv_vtk_dry_run
{
    if [[ "$DO_VTK" == "yes" ]] ; then
        echo "Dry run option not set for vtk"
    fi
}

# *************************************************************************** #
#                            Function 6, build_vtk                            #
# *************************************************************************** #


function apply_vtk_710_glew_patch
{
  # path vtk's glew to use our own dlGetProcAddressMesa 

   patch -p0 << \EOF
*** ThirdParty/glew/vtkglew/src/glew.orig.c	Fri Jun 10 12:34:46 2016
--- ThirdParty/glew/vtkglew/src/glew.c	Fri Jun 10 12:43:08 2016
***************
*** 36,42 ****
  #  include <GL/wglew.h>
  #elif defined(VTK_USE_OFFSCREEN_EGL)
  #  include <EGL/egl.h>
! #elif !defined(GLEW_OSMESA) && !defined(__ANDROID__) && !defined(__native_client__) && !defined(__HAIKU__) && (!defined(__APPLE__) || defined(GLEW_APPLE_GLX))
  #  include <GL/glxew.h>
  #endif
  
--- 36,42 ----
  #  include <GL/wglew.h>
  #elif defined(VTK_USE_OFFSCREEN_EGL)
  #  include <EGL/egl.h>
! #elif !defined(__ANDROID__) && !defined(__native_client__) && !defined(__HAIKU__) && (!defined(__APPLE__) || defined(GLEW_APPLE_GLX))
  #  include <GL/glxew.h>
  #endif
  
***************
*** 110,130 ****
  }
  #endif /* __sgi || __sun || GLEW_APPLE_GLX */
  
! #if defined(GLEW_OSMESA)
  #include <dlfcn.h>
  #include <stdio.h>
  #include <stdlib.h>
  
  void* dlGetProcAddressMesa (const GLubyte* name)
  {
    static void* h = NULL;
!   static void* gpa;
  
!   if (h == NULL)
    {
      if ((h = dlopen(NULL, RTLD_LAZY | RTLD_LOCAL)) == NULL)
        return NULL;
!     gpa = dlsym(h, "OSMesaGetProcAddress");
    }
  
    if (gpa != NULL)
--- 110,142 ----
  }
  #endif /* __sgi || __sun || GLEW_APPLE_GLX */
  
! #define VISIT_GLEW_OSMESA
! #if defined(VISIT_GLEW_OSMESA)
  #include <dlfcn.h>
  #include <stdio.h>
  #include <stdlib.h>
+ int using_osmesa = 0;
  
  void* dlGetProcAddressMesa (const GLubyte* name)
  {
    static void* h = NULL;
!   static void* gpa = NULL;
  
!   if (h == NULL && gpa == NULL)
    {
      if ((h = dlopen(NULL, RTLD_LAZY | RTLD_LOCAL)) == NULL)
        return NULL;
! 
!     gpa = dlsym(h, "glXGetProcAddress");
! 
!     if(gpa == NULL)
!     {
!       gpa = dlsym(h, "OSMesaGetProcAddress");
!       if (gpa != NULL)
!       {
!           using_osmesa = 1;
!       }
!     }
    }
  
    if (gpa != NULL)
***************
*** 207,213 ****
  #  define glewGetProcAddress(name) NULL /* TODO */
  #elif defined(__native_client__)
  #  define glewGetProcAddress(name) NULL /* TODO */
! #elif defined(GLEW_OSMESA)
  #  define glewGetProcAddress(name) dlGetProcAddressMesa(name)
  #elif defined(VTK_USE_OFFSCREEN_EGL)
  #  define glewGetProcAddress(name) eglGetProcAddress(name)
--- 219,225 ----
  #  define glewGetProcAddress(name) NULL /* TODO */
  #elif defined(__native_client__)
  #  define glewGetProcAddress(name) NULL /* TODO */
! #elif defined(VISIT_GLEW_OSMESA)
  #  define glewGetProcAddress(name) dlGetProcAddressMesa(name)
  #elif defined(VTK_USE_OFFSCREEN_EGL)
  #  define glewGetProcAddress(name) eglGetProcAddress(name)
***************
*** 13167,13173 ****
    return GLEW_OK;
  }
  
! #elif !defined(GLEW_OSMESA) && !defined(__ANDROID__) && !defined(__native_client__) && !defined(__HAIKU__) && (!defined(__APPLE__) && !defined(VTK_USE_OFFSCREEN_EGL) || defined(GLEW_APPLE_GLX))
  
  PFNGLXGETCURRENTDISPLAYPROC __glewXGetCurrentDisplay = NULL;
  
--- 13179,13185 ----
    return GLEW_OK;
  }
  
! #elif !defined(__ANDROID__) && !defined(__native_client__) && !defined(__HAIKU__) && (!defined(__APPLE__) && !defined(VTK_USE_OFFSCREEN_EGL) || defined(GLEW_APPLE_GLX))
  
  PFNGLXGETCURRENTDISPLAYPROC __glewXGetCurrentDisplay = NULL;
  
***************
*** 14408,14416 ****
    GLenum r;
    r = glewContextInit();
    if ( r != 0 ) return r;
! #if defined(GLEW_OSMESA)
!   return r;
! #elif defined(_WIN32)
    return wglewContextInit();
  #elif !defined(__ANDROID__) && !defined(__native_client__) && !defined(__HAIKU__) && (!defined(__APPLE__) && ! defined(VTK_USE_OFFSCREEN_EGL) || defined(GLEW_APPLE_GLX))
    return glxewContextInit();
--- 14420,14430 ----
    GLenum r;
    r = glewContextInit();
    if ( r != 0 ) return r;
! #if defined(VISIT_GLEW_OSMESA)
!   if(using_osmesa == 1)
!      return r;
! #endif
! #if defined(_WIN32)
    return wglewContextInit();
  #elif !defined(__ANDROID__) && !defined(__native_client__) && !defined(__HAIKU__) && (!defined(__APPLE__) && ! defined(VTK_USE_OFFSCREEN_EGL) || defined(GLEW_APPLE_GLX))
    return glxewContextInit();

EOF

    if [[ $? != 0 ]] ; then
      warn "vtk710 patch vtk glew #1 failed."
      return 1
    fi
   patch -p0 << \EOF
*** ThirdParty/glew/vtkglew/CMakeLists.txt.orig	Fri Jun 10 12:34:46 2016
--- ThirdParty/glew/vtkglew/CMakeLists.txt	Fri Jun 10 12:43:08 2016
***************
*** 22,30 ****
  
  vtk_add_library(vtkglew ${common_SRCS})
  vtk_opengl_link(vtkglew)
! if(VTK_USE_OSMESA AND UNIX AND NOT APPLE)
    target_link_libraries(vtkglew LINK_PRIVATE ${CMAKE_DL_LIBS})
! endif()
  
  if(NOT VTK_INSTALL_NO_DEVELOPMENT)
    install(DIRECTORY ${VTKGLEW_SOURCE_DIR}/include
--- 22,30 ----
  
  vtk_add_library(vtkglew ${common_SRCS})
  vtk_opengl_link(vtkglew)
! #if(VTK_USE_OSMESA AND UNIX AND NOT APPLE)
    target_link_libraries(vtkglew LINK_PRIVATE ${CMAKE_DL_LIBS})
! #endif()
  
  if(NOT VTK_INSTALL_NO_DEVELOPMENT)
    install(DIRECTORY ${VTKGLEW_SOURCE_DIR}/include

EOF

    if [[ $? != 0 ]] ; then
      warn "vtk710 patch vtk glew #2 failed."
      return 1
    fi
    return 0;
}


function apply_vtk_patch
{  
    if [[ ${VTK_VERSION} == 7.1.0 ]] ; then
        if [[ "$DO_MESA" == "yes" && "$OPSYS" == "Linux" ]] ; then
            apply_vtk_710_glew_patch
            if [[ $? != 0 ]] ; then
                return 1
            fi
        fi
    fi

    return 0
}

function build_vtk
{
    #
    # CMake is the build system for VTK.  Call another script that will build
    # that program.
    #
    CMAKE_INSTALL=${CMAKE_INSTALL:-"$VISITDIR/cmake/${CMAKE_VERSION}/$VISITARCH/bin"}
    if [[ -e ${CMAKE_INSTALL}/cmake ]] ; then
        info "VTK: CMake found"
    else
        build_cmake
        if [[ $? != 0 ]] ; then
            warn "Unable to build cmake.  Giving up"
            return 1
        fi
    fi

    #
    # We need python to build the vtk python bindings.
    #
    PYTHON_INSTALL="${VISIT_PYTHON_DIR}/bin"
    if [[ "$DO_PYTHON" == "yes" ]] ; then
        if [[ -e ${PYTHON_INSTALL}/python ]] ; then
            info "VTK: Python found"
        else
            if [[ "$DO_DBIO_ONLY" != "yes" ]]; then
                build_python
                if [[ $? != 0 ]] ; then
                    warn "Unable to build python. Giving up"
                    return 1
                fi
            fi
        fi
    fi

    #
    # We need Qt to build the vtk Qt support. Only do Qt if we're not doing
    # a server-only build.
    #
    if [[ "$DO_DBIO_ONLY" != "yes" ]]; then
        if [[ "$DO_ENGINE_ONLY" != "yes" ]]; then
            if [[ "$DO_SERVER_COMPONENTS_ONLY" != "yes" ]]; then
                if [[ -e ${QT_BIN_DIR}/qmake ]] ; then
                    info "VTK: Qt found"
                else
                    build_qt
                    if [[ $? != 0 ]] ; then
                        warn "Unable to build Qt. Giving up"
                        return 1
                    fi
                fi
            fi
        fi
    fi

    # Extract the sources
    if [[ -d $VTK_BUILD_DIR ]] ; then
        if [[ ! -f $VTK_FILE ]] ; then
            warn "The directory VTK exists, deleting before uncompressing"
            rm -Rf $VTK_BUILD_DIR
            ensure_built_or_ready $VTK_INSTALL_DIR    $VTK_VERSION    $VTK_BUILD_DIR    $VTK_FILE
        fi
    fi

    #
    # Prepare the build dir using src file.
    #
    prepare_build_dir $VTK_BUILD_DIR $VTK_FILE
    untarred_vtk=$?
    # 0, already exists, 1 untarred src, 2 error

    if [[ $untarred_vtk == -1 ]] ; then
        warn "Unable to prepare VTK build directory. Giving Up!"
        return 1
    fi

    #
    # Apply patches
    #
    info "Patching VTK . . ."
    cd $VTK_BUILD_DIR || error "Can't cd to VTK build dir."
    apply_vtk_patch
    if [[ $? != 0 ]] ; then
        if [[ $untarred_vtk == 1 ]] ; then
            warn "Giving up on VTK build because the patch failed."
            return 1
        else
            warn "Patch failed, but continuing.  I believe that this script\n" \
                 "tried to apply a patch to an existing directory that had\n" \
                 "already been patched ... that is, the patch is\n" \
                 "failing harmlessly on a second application."
        fi
    fi

    # move back up to the start dir 
    cd "$START_DIR"

    #
    # Configure VTK
    #
    info "Configuring VTK . . ."

    # Make a build directory for an out-of-source build.. Change the
    # VISIT_BUILD_DIR variable to represent the out-of-source build directory.
    VTK_SRC_DIR=$VTK_BUILD_DIR
    VTK_BUILD_DIR="${VTK_SRC_DIR}-build"
    if [[ ! -d $VTK_BUILD_DIR ]] ; then
        echo "Making build directory $VTK_BUILD_DIR"
        mkdir $VTK_BUILD_DIR
    fi

    #
    # Remove the CMakeCache.txt files ... existing files sometimes prevent
    # fields from getting overwritten properly.
    #
    rm -Rf ${VTK_BUILD_DIR}/CMakeCache.txt ${VTK_BUILD_DIR}/*/CMakeCache.txt

    #
    # Setup paths and libs for python for the VTK build.
    #
    if [[ "$OPSYS" == "Darwin" ]]; then
        if [[ "${VISIT_PYTHON_DIR}/lib" != "/usr/lib" ]]; then
            export DYLD_LIBRARY_PATH="${VISIT_PYTHON_DIR}/lib/:$DYLD_LIBRARY_PATH"
        fi
    else
        export LD_LIBRARY_PATH="${VISIT_PYTHON_DIR}/lib/:$LD_LIBRARY_PATH"
    fi

    export VTK_PY_LIBS="-lpthread"
    if [[ "$OPSYS" == "Linux" ]]; then
        export VTK_PY_LIBS="$VTK_PY_LIBS -ldl -lutil -lm"
    fi

    vopts=""
    vtk_build_mode="${VISIT_BUILD_MODE}"
    vtk_inst_path="${VISITDIR}/${VTK_INSTALL_DIR}/${VTK_VERSION}/${VISITARCH}"
    vtk_debug_leaks="false"

    # Some linker flags.
    lf=""
    if test "${OPSYS}" = "Darwin" ; then
        lf="-Wl,-headerpad_max_install_names"
        lf="${lf},-compatibility_version,${VTK_COMPATIBILITY_VERSION}"
        lf="${lf},-current_version,${VTK_VERSION}"
    fi

    # Add some extra arguments to the VTK cmake command line via the 
    # VTK_EXTRA_OPTIONS environment variable.
    if test -n "$VTK_EXTRA_OPTIONS" ; then
        vopts="${vopts} $VTK_EXTRA_OPTIONS"
    fi

    # normal stuff
    vopts="${vopts} -DCMAKE_BUILD_TYPE:STRING=${vtk_build_mode}"
    vopts="${vopts} -DCMAKE_INSTALL_PREFIX:PATH=${vtk_inst_path}"
    if test "x${DO_STATIC_BUILD}" = "xyes" ; then
        vopts="${vopts} -DBUILD_SHARED_LIBS:BOOL=OFF"
    else
        vopts="${vopts} -DBUILD_SHARED_LIBS:BOOL=ON"
    fi
    vopts="${vopts} -DVTK_DEBUG_LEAKS:BOOL=${vtk_debug_leaks}"
    vopts="${vopts} -DVTK_LEGACY_REMOVE:BOOL=true"
    vopts="${vopts} -DBUILD_TESTING:BOOL=false"
    vopts="${vopts} -DBUILD_DOCUMENTATION:BOOL=false"
    vopts="${vopts} -DCMAKE_C_COMPILER:STRING=${C_COMPILER}"
    vopts="${vopts} -DCMAKE_CXX_COMPILER:STRING=${CXX_COMPILER}"
    vopts="${vopts} -DCMAKE_C_FLAGS:STRING=\"${C_OPT_FLAGS}\""
    vopts="${vopts} -DCMAKE_CXX_FLAGS:STRING=\"${CXX_OPT_FLAGS}\""
    vopts="${vopts} -DCMAKE_EXE_LINKER_FLAGS:STRING=${lf}"
    vopts="${vopts} -DCMAKE_MODULE_LINKER_FLAGS:STRING=${lf}"
    vopts="${vopts} -DCMAKE_SHARED_LINKER_FLAGS:STRING=${lf}"
    vopts="${vopts} -DVTK_REPORT_OPENGL_ERRORS:BOOL=true"
    if test "${OPSYS}" = "Darwin" ; then
        vopts="${vopts} -DVTK_USE_COCOA:BOOL=ON"
        vopts="${vopts} -DCMAKE_INSTALL_NAME_DIR:PATH=${vtk_inst_path}/lib"
        if test "${MACOSX_DEPLOYMENT_TARGET}" = "10.10"; then
            # If building on 10.10 (Yosemite) check if we are building with Xcode 7 ...
            XCODE_VER=$(xcodebuild -version | head -n 1 | awk '{print $2}')
            if test ${XCODE_VER%.*} == 7; then
                # Workaround for Xcode 7 not having a 10.10 SDK: Prevent CMake from linking to 10.11 SDK
                # by using Frameworks installed in root directory.
                echo "Xcode 7 on MacOS 10.10 detected: Enabling CMake workaround"
                vopts="${vopts} -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=\"\" -DCMAKE_OSX_SYSROOT:STRING=/"
            fi
        fi
    fi

    # allow VisIt to override any of vtk's classes
    vopts="${vopts} -DVTK_ALL_NEW_OBJECT_FACTORY:BOOL=true"

    # OpenGL2 backend
    vopts="${vopts} -DVTK_RENDERING_BACKEND:STRING=OpenGL2"

    # Turn off module groups
    vopts="${vopts} -DVTK_Group_Imaging:BOOL=false"
    vopts="${vopts} -DVTK_Group_MPI:BOOL=false"
    vopts="${vopts} -DVTK_Group_Qt:BOOL=false"
    vopts="${vopts} -DVTK_Group_Rendering:BOOL=false"
    vopts="${vopts} -DVTK_Group_StandAlone:BOOL=false"
    vopts="${vopts} -DVTK_Group_Tk:BOOL=false"
    vopts="${vopts} -DVTK_Group_Views:BOOL=false"

    # Turn on individual modules. dependent modules are turned on automatically
    vopts="${vopts} -DModule_vtkCommonCore:BOOL=true"
    vopts="${vopts} -DModule_vtkFiltersFlowPaths:BOOL=true"
    vopts="${vopts} -DModule_vtkFiltersHybrid:BOOL=true"
    vopts="${vopts} -DModule_vtkFiltersModeling:BOOL=true"
    vopts="${vopts} -DModule_vtkGeovisCore:BOOL=true"
    vopts="${vopts} -DModule_vtkIOEnSight:BOOL=true"
    vopts="${vopts} -DModule_vtkIOGeometry:BOOL=true"
    vopts="${vopts} -DModule_vtkIOLegacy:BOOL=true"
    vopts="${vopts} -DModule_vtkIOPLY:BOOL=true"
    vopts="${vopts} -DModule_vtkIOXML:BOOL=true"
    vopts="${vopts} -DModule_vtkInteractionStyle:BOOL=true"
    vopts="${vopts} -DModule_vtkRenderingAnnotation:BOOL=true"
    vopts="${vopts} -DModule_vtkRenderingFreeType:BOOL=true"
    vopts="${vopts} -DModule_vtkRenderingOpenGL2:BOOL=true"
    vopts="${vopts} -DModule_vtklibxml2:BOOL=true"

    # Tell VTK where to locate qmake if we're building graphical support. We
    # do not add graphical support for server-only builds.
    if [[ "$DO_DBIO_ONLY" != "yes" ]]; then
        if [[ "$DO_ENGINE_ONLY" != "yes" ]]; then
            if [[ "$DO_SERVER_COMPONENTS_ONLY" != "yes" ]]; then
                vopts="${vopts} -DModule_vtkGUISupportQtOpenGL:BOOL=true"
                vopts="${vopts} -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_BIN_DIR}/qmake"
                if [[ ${IS_QT5} == "yes" ]]; then
                    vopts="${vopts} -DVTK_QT_VERSION=5"
                    vopts="${vopts} -DCMAKE_PREFIX_PATH=${QT_INSTALL_DIR}/lib/cmake"
                fi
            fi
        fi
    fi

    # Add python wrapping
    if [[ "$DO_DBIO_ONLY" != "yes" ]]; then
        # python... but static libs and python filters are incompatible.
        if [[ "$DO_STATIC_BUILD" != "yes" ]]; then
            py="${PYTHON_COMMAND}"
            pyinc="${PYTHON_INCLUDE_DIR}"
            pylib="${PYTHON_LIBRARY}"

            vopts="${vopts} -DVTK_WRAP_PYTHON:BOOL=true"
            vopts="${vopts} -DPYTHON_EXECUTABLE:FILEPATH=${py}"
            vopts="${vopts} -DPYTHON_EXTRA_LIBS:STRING=${VTK_PY_LIBS}"
            vopts="${vopts} -DPYTHON_INCLUDE_DIR:PATH=${pyinc}"
            vopts="${vopts} -DPYTHON_LIBRARY:FILEPATH=${pylib}"
            #            vopts="${vopts} -DPYTHON_UTIL_LIBRARY:FILEPATH="
        else
            warn "Forgetting python filters because we are doing a static build."
        fi
    fi

    # Add R support
    if test "$DO_R" = "yes" ; then
        vopts="${vopts} -DModule_vtkFiltersStatisticsGnuR:BOOL=true"
        vopts="${vopts} -DR_COMMAND:PATH=${R_INSTALL_DIR}/bin/R"
        vopts="${vopts} -DVTK_R_HOME:PATH=${R_INSTALL_DIR}/lib/R"
        vopts="${vopts} -DR_INCLUDE_DIR:PATH=${R_INSTALL_DIR}/lib/R/include"
        vopts="${vopts} -DR_LIBRARY_BASE:PATH=${R_INSTALL_DIR}/lib/R/lib/libR.${SO_EXT}"
        vopts="${vopts} -DR_LIBRARY_LAPACK:PATH=${R_INSTALL_DIR}/lib/R/lib/libRlapack.${SO_EXT}"
        vopts="${vopts} -DR_LIBRARY_BLAS:PATH=${R_INSTALL_DIR}/lib/R/lib/libRblas.${SO_EXT}"
    fi

    CMAKE_BIN="${CMAKE_INSTALL}/cmake"
    cd ${VTK_BUILD_DIR}

    #
    # Several platforms have had problems with the VTK cmake configure command
    # issued simply via "issue_command".  This was first discovered on 
    # BGQ and then showed up in random cases for both OSX and Linux machines. 
    # Brad resolved this on BGQ  with a simple work around - we write a simple 
    # script that we invoke with bash which calls cmake with all of the properly
    # arguments. We are now using this strategy for all platforms.
    #

    if test -e bv_run_cmake.sh ; then
        rm -f bv_run_cmake.sh
    fi
    echo "\"${CMAKE_BIN}\"" ${vopts} ../${VTK_SRC_DIR} > bv_run_cmake.sh
    cat bv_run_cmake.sh
    issue_command bash bv_run_cmake.sh || error "VTK configuration failed."


    #
    # Now build VTK.
    #
    info "Building VTK . . . (~20 minutes)"
    env DYLD_LIBRARY_PATH=`pwd`/bin $MAKE $MAKE_OPT_FLAGS || \
        error "VTK did not build correctly.  Giving up."

    info "Installing VTK . . . "
    $MAKE install || error "VTK did not install correctly."

    # Filter out an include that references the user's VTK build directory
    configdir="${vtk_inst_path}/lib/cmake/vtk-${VTK_SHORT_VERSION}"
    cat ${configdir}/VTKConfig.cmake | grep -v "vtkTestingMacros" > ${configdir}/VTKConfig.cmake.new
    mv ${configdir}/VTKConfig.cmake.new ${configdir}/VTKConfig.cmake

    chmod -R ug+w,a+rX ${VISITDIR}/${VTK_INSTALL_DIR}
    if [[ "$DO_GROUP" == "yes" ]] ; then
        chgrp -R ${GROUP} "$VISITDIR/${VTK_INSTALL_DIR}"
    fi
    cd "$START_DIR"
    info "Done with VTK"
    return 0
}

function bv_vtk_is_enabled
{
    if [[ $DO_VTK == "yes" ]]; then
        return 1
    fi
    return 0
}

function bv_vtk_is_installed
{
    if [[ "$USE_SYSTEM_VTK" == "yes" ]]; then
        return 1
    fi

    check_if_installed "$VTK_INSTALL_DIR" $VTK_VERSION
    if [[ $? == 0 ]] ; then
        return 1
    fi
    return 0
}

function bv_vtk_build
{
    #
    # Build VTK
    #
    cd "$START_DIR"
    if [[ "$DO_VTK" == "yes" && "$USE_SYSTEM_VTK" == "no" ]] ; then
        check_if_installed $VTK_INSTALL_DIR $VTK_VERSION
        if [[ $? == 0 ]] ; then
            info "Skipping VTK build.  VTK is already installed."
        else
            info "Building VTK (~20 minutes)"
            build_vtk
            if [[ $? != 0 ]] ; then
                error "Unable to build or install VTK.  Bailing out."
            fi
        fi
        info "Done building VTK"
    fi
}
