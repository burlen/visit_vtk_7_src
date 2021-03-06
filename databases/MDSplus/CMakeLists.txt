# DO NOT EDIT THIS FILE! THIS FILE IS AUTOMATICALLY GENERATED BY xml2cmake
PROJECT(MDSplus_database)

INCLUDE(${VISIT_SOURCE_DIR}/CMake/PluginMacros.cmake)

SET(COMMON_SOURCES
MDSplusPluginInfo.C
MDSplusCommonPluginInfo.C
)

SET(LIBI_SOURCES 
MDSplusPluginInfo.C
)

SET(LIBM_SOURCES
MDSplusMDServerPluginInfo.C
${COMMON_SOURCES}
avtMDSplusFileFormat.C
avtMDSplusOptions.C
mdsPlusAPI.C
)

SET(LIBE_SOURCES
MDSplusEnginePluginInfo.C
${COMMON_SOURCES}
avtMDSplusFileFormat.C
avtMDSplusOptions.C
mdsPlusAPI.C
)

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}
${MDSPLUS_INCLUDE_DIR}
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


LINK_DIRECTORIES(${VISIT_LIBRARY_DIR} ${EAVL_LIBRARY_DIR} ${VTKM_LIBRARY_DIR} ${VTK_LIBRARY_DIRS} ${MDSPLUS_LIBRARY_DIR} )

ADD_LIBRARY(IMDSplusDatabase ${LIBI_SOURCES})
TARGET_LINK_LIBRARIES(IMDSplusDatabase visitcommon)
SET(INSTALLTARGETS IMDSplusDatabase)

IF(NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)
    ADD_LIBRARY(MMDSplusDatabase ${LIBM_SOURCES}    )
    TARGET_LINK_LIBRARIES(MMDSplusDatabase visitcommon avtdbatts avtdatabase_ser ${MDSPLUS_LIB} )
    ADD_TARGET_DEFINITIONS(MMDSplusDatabase MDSERVER)
    SET(INSTALLTARGETS ${INSTALLTARGETS} MMDSplusDatabase)
ENDIF(NOT VISIT_ENGINE_ONLY AND NOT VISIT_DBIO_ONLY)

ADD_LIBRARY(EMDSplusDatabase_ser ${LIBE_SOURCES})
TARGET_LINK_LIBRARIES(EMDSplusDatabase_ser visitcommon avtdatabase_ser avtpipeline_ser ${MDSPLUS_LIB} )
ADD_TARGET_DEFINITIONS(EMDSplusDatabase_ser ENGINE)
SET(INSTALLTARGETS ${INSTALLTARGETS} EMDSplusDatabase_ser)

IF(VISIT_PARALLEL)
    ADD_PARALLEL_LIBRARY(EMDSplusDatabase_par ${LIBE_SOURCES})
    TARGET_LINK_LIBRARIES(EMDSplusDatabase_par visitcommon avtdatabase_par avtpipeline_par ${MDSPLUS_LIB} )
    ADD_TARGET_DEFINITIONS(EMDSplusDatabase_par ENGINE)
    SET(INSTALLTARGETS ${INSTALLTARGETS} EMDSplusDatabase_par)
ENDIF(VISIT_PARALLEL)

VISIT_INSTALL_DATABASE_PLUGINS(${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_RTOD(databases ${INSTALLTARGETS})
VISIT_PLUGIN_TARGET_FOLDER(databases MDSplus ${INSTALLTARGETS})

