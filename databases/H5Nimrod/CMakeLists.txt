# DO NOT EDIT THIS FILE! THIS FILE IS AUTOMATICALLY GENERATED BY xml2cmake
PROJECT(H5Nimrod_database)

INCLUDE(${VISIT_SOURCE_DIR}/CMake/PluginMacros.cmake)

SET(COMMON_SOURCES
H5NimrodPluginInfo.C
H5NimrodCommonPluginInfo.C
)

SET(LIBI_SOURCES 
H5NimrodPluginInfo.C
)

SET(LIBM_SOURCES
H5NimrodMDServerPluginInfo.C
${COMMON_SOURCES}
avtH5NimrodFileFormat.C
H5utils.C
)

SET(LIBE_SOURCES
H5NimrodEnginePluginInfo.C
${COMMON_SOURCES}
avtH5NimrodFileFormat.C
H5utils.C
)

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}
${HDF5_INCLUDE_DIR}
${VISIT_COMMON_INCLUDES}
${VISIT_INCLUDE_DIR}/avt/DBAtts/MetaData
${VISIT_INCLUDE_DIR}/avt/DBAtts/SIL
${VISIT_INCLUDE_DIR}/avt/Database/Database
${VISIT_INCLUDE_DIR}/avt/Database/Formats
${VISIT_INCLUDE_DIR}/avt/Database/Ghost
${VISIT_INCLUDE_DIR}/avt/FileWriter
${VISIT_INCLUDE_DIR}/avt/Filters
${VISIT_INCLUDE_DIR}/avt/MIR/Base
${VISIT_INCLUDE_DIR}/avt/MIR/Tet
${VISIT_INCLUDE_DIR}/avt/MIR/Zoo
${VISIT_INCLUDE_DIR}/avt/Math
${VISIT_INCLUDE_DIR}/avt/Pipeline/AbstractFilters
${VISIT_INCLUDE_DIR}/avt/Pipeline/Data
${VISIT_INCLUDE_DIR}/avt/Pipeline/Pipeline
${VISIT_INCLUDE_DIR}/avt/Pipeline/Sinks
${VISIT_INCLUDE_DIR}/avt/Pipeline/Sources
${VISIT_INCLUDE_DIR}/avt/VisWindow/VisWindow
${VISIT_INCLUDE_DIR}/visit_vtk/full
${VISIT_INCLUDE_DIR}/visit_vtk/lightweight
${EAVL_INCLUDE_DIR} 
${VTKM_INCLUDE_DIR} 
${VTK_INCLUDE_DIRS} 
)


LINK_DIRECTORIES(${VISIT_LIBRARY_DIR} ${EAVL_LIBRARY_DIR} ${VTKM_LIBRARY_DIR} ${VTK_LIBRARY_DIRS} ${HDF5_LIBRARY_DIR} )

ADD_LIBRARY(IH5NimrodDatabase ${LIBI_SOURCES})
TARGET_LINK_LIBRARIES(IH5NimrodDatabase visitcommon)
SET(INSTALLTARGETS IH5NimrodDatabase)

IF(NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)
    ADD_LIBRARY(MH5NimrodDatabase ${LIBM_SOURCES}    )
    TARGET_LINK_LIBRARIES(MH5NimrodDatabase visitcommon avtdbatts avtdatabase_ser ${HDF5_LIB} ${ZLIB_LIB} )
    ADD_TARGET_DEFINITIONS(MH5NimrodDatabase MDSERVER)
    SET(INSTALLTARGETS ${INSTALLTARGETS} MH5NimrodDatabase)
ENDIF(NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)

ADD_LIBRARY(EH5NimrodDatabase_ser ${LIBE_SOURCES})
TARGET_LINK_LIBRARIES(EH5NimrodDatabase_ser visitcommon avtdatabase_ser avtpipeline_ser ${HDF5_LIB} ${ZLIB_LIB} )
ADD_TARGET_DEFINITIONS(EH5NimrodDatabase_ser ENGINE)
SET(INSTALLTARGETS ${INSTALLTARGETS} EH5NimrodDatabase_ser)

IF(VISIT_PARALLEL)
    ADD_PARALLEL_LIBRARY(EH5NimrodDatabase_par ${LIBE_SOURCES})
    TARGET_LINK_LIBRARIES(EH5NimrodDatabase_par visitcommon avtdatabase_par avtpipeline_par ${HDF5_LIB} ${ZLIB_LIB} )
    ADD_TARGET_DEFINITIONS(EH5NimrodDatabase_par ENGINE)
    SET(INSTALLTARGETS ${INSTALLTARGETS} EH5NimrodDatabase_par)
ENDIF(VISIT_PARALLEL)

VISIT_INSTALL_DATABASE_PLUGINS(${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_RTOD(databases ${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_FOLDER(databases H5Nimrod ${INSTALLTARGETS})

