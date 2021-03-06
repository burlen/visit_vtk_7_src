/*****************************************************************************
*
* Copyright (c) 2000 - 2015, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

// ************************************************************************* //
//                        avtShiftCenteringFilter.C                          //
// ************************************************************************* //

#include <avtShiftCenteringFilter.h>

#include <math.h>

#include <vtkCellData.h>
#include <vtkCellDataToPointData.h>
#include <vtkDataSet.h>
#include <vtkExecutive.h>
#include <vtkPointData.h>
#include <vtkPointDataToCellData.h>
#include <vtkPolyData.h>
#include <vtkRectilinearGrid.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>

#include <DebugStream.h>

#include <string>
#include <vector>


// ****************************************************************************
//  Method: avtShiftCenteringFilter constructor
//
//  Arguments:
//      target      The target centering.
//
//  Programmer: Kathleen Bonnell 
//  Creation:   April 19, 2001.
//
//  Modifications:
//
//    Hank Childs, Fri Feb  8 18:33:39 PST 2002
//    Removed all references to PC atts.
//
//    Hank Childs, Tue Sep 10 15:00:49 PDT 2002
//    Removed data members for shifting centering.
//
//    Sean Ahern, Wed Sep 10 13:13:18 EDT 2008
//    For ease of reading code, I forced the argument to be
//    avtCentering, not integer.
//
// ****************************************************************************

avtShiftCenteringFilter::avtShiftCenteringFilter(avtCentering target)
{
    centeringTarget = target;
}


// ****************************************************************************
//  Method: avtShiftCenteringFilter destructor
//
//  Purpose:
//      Defines the destructor.  Note: this should not be inlined in the header
//      because it causes problems for certain compilers.
//
//  Programmer: Hank Childs
//  Creation:   February 5, 2004
//
// ****************************************************************************

avtShiftCenteringFilter::~avtShiftCenteringFilter()
{
    ;
}


// ****************************************************************************
//  Method: avtShiftCenteringFilter::ExecuteData
//
//  Purpose:
//      Shifts the centering of the data from point to cell or vice-versa if
//      necessary.
//
//  Arguments:
//      inDR       The input data representation.
//
//  Returns:       The output data representation. 
//
//  Programmer: Kathleen Bonnell 
//  Creation:   April 19, 2001 
//
//  Modifications:
//
//    Hank Childs, Thu Apr 26 14:51:39 PDT 2001
//    Memory leak.
//
//    Kathleen Bonnell, Wed Jun 13 11:46:02 PDT 2001
//    Ensure that ghost-cell information is passed on.
//
//    Hank Childs, Wed Jan  9 14:07:42 PST 2002
//    Remove unnecessary code in favor of MakeObject.
//
//    Hank Childs, Fri Jan 25 10:39:07 PST 2002
//    Fix memory leak.
//
//    Hank Childs, Fri Feb  8 18:33:39 PST 2002
//    Remove unnecessary references to PC atts.
//
//    Hank Childs, Tue Sep 10 15:00:49 PDT 200
//    Made filters that were data members become automatic variables.
//
//    Kathleen Bonnell, Fri Dec 13 14:07:15 PST 2002 
//    Use NewInstance instead of MakeObject, to match new vtk api. 
//
//    Kathleen Bonnell, Thu Mar 13 10:23:37 PST 2003   
//    Ensure that avtOriginalCellNumbers remains a cell-data array.
//
//    Hank Childs, Fri Aug 27 16:02:58 PDT 2004
//    Rename ghost data array.  Also add support for ghost nodes.
//
//    Kathleen Bonnell, Wed May 17 15:08:39 PDT 2006 
//    Can no longer use SetOutput on a vtk filter, must grab the filters' 
//    executive and SetOuputData. 
//
//    Hank Childs, Thu Aug  7 09:39:40 PDT 2008
//    Add support for non-floating point arrays.
//
//    Cyrus Harrison, Mon Aug 11 13:48:25 PDT 2008
//    Fixed indexing problem with non-floating point array support. 
//    Switched to use array name and exclude special avt arrays:
//    avtGhostZones, avtGhostNode, avtOriginalCellNumber from int-float-int
//    conversion.
//
//    Sean Ahern, Wed Sep 10 13:13:18 EDT 2008
//    For ease of reading code, I forced the target to be of type
//    avtCentering, not integer.
//
//    Kathleen Biagas, Thu Sep 6 15:18:27 MST 2012
//    Preserve data type.
//
//    Eric Brugger, Tue Jul 22 08:01:18 PDT 2014
//    Modified the class to work with avtDataRepresentation.
//
// ****************************************************************************

avtDataRepresentation *
avtShiftCenteringFilter::ExecuteData(avtDataRepresentation *inDR)
{
    //
    // Get the VTK data set.
    //
    vtkDataSet *inDS = inDR->GetDataVTK();

    vtkDataSet *newDS = (vtkDataSet *) inDS->NewInstance();
    newDS->ShallowCopy(inDS);
    vtkDataSet *outDS = newDS;

    if (centeringTarget == AVT_NODECENT)
    {
        size_t nArray = inDS->GetCellData()->GetNumberOfArrays();
        std::vector<std::string> arraysToSwap;
        
        for (size_t i = 0 ; i < nArray ; i++)
        {
            vtkDataArray *arr = inDS->GetCellData()->GetArray(i);
            int dt = arr->GetDataType();
            if (dt == VTK_UNSIGNED_CHAR || dt == VTK_INT ||
                dt == VTK_UNSIGNED_INT)
            {
                std::string arr_name(arr->GetName());
                if(arr_name != "avtGhostZones" && 
                   arr_name != "avtGhostNodes" && 
                   arr_name != "avtOriginalCellNumbers")
                    arraysToSwap.push_back(arr_name);
            }
        }

        vtkDataSet *dsToShift = inDS;
        if (arraysToSwap.size() > 0)
        {
            dsToShift = (vtkDataSet *) inDS->NewInstance();
            dsToShift->ShallowCopy(inDS);
            for (size_t k = 0 ; k < arraysToSwap.size() ; k++) 
            {
                int index = arraysToSwap.size()-k-1;
                vtkDataArray *arr = inDS->GetCellData()->GetArray(arraysToSwap[index].c_str());
                vtkDataArray *fa = arr->NewInstance();
                int ntups  = arr->GetNumberOfTuples();
                int ncomps = arr->GetNumberOfComponents();
                fa->SetNumberOfComponents(ncomps);
                fa->SetNumberOfTuples(ntups);
                for (size_t i = 0 ; i < (size_t)ntups ; i++)
                    for (size_t j = 0 ; j < (size_t)ncomps ; j++)
                        fa->SetComponent(i, j, arr->GetComponent(i, j));
                fa->SetName(arr->GetName());
                dsToShift->GetCellData()->RemoveArray(arr->GetName());
                dsToShift->GetCellData()->AddArray(fa);
                fa->Delete();
            }
        }

        //
        //  User requested node-centered but our data is zone-centered,
        //  create the point data from cell data.
        //
        vtkCellDataToPointData *cd2pd = vtkCellDataToPointData::New();
        cd2pd->SetInputData(dsToShift);
        cd2pd->GetExecutive()->SetOutputData(0, outDS);
        cd2pd->Update();
        cd2pd->Delete();

        // We want to preserve knowledge of ghost zones
        vtkDataArray *ghosts = inDS->GetCellData()->GetArray("avtGhostZones");
        if (ghosts)
        {
            outDS->GetCellData()->AddArray(ghosts);
            // Only want ghost cell-data, not ghost point-data.
            outDS->GetPointData()->RemoveArray("avtGhostZones");
        }
        if (inDS->GetPointData()->GetArray("avtGhostNodes") != NULL)
        {
            outDS->GetPointData()->AddArray(
                              inDS->GetPointData()->GetArray("avtGhostNodes"));
        }
        // We want to preserve knowledge of original cells 
        vtkDataArray *origCells = 
                       inDS->GetCellData()->GetArray("avtOriginalCellNumbers");
        if (origCells)
        {
            outDS->GetCellData()->AddArray(origCells);
            // Only want origCells cell-data, not origCells point-data.
            outDS->GetPointData()->RemoveArray("avtOriginalCellNumbers");
        }

        // Convert the former int arrays back to int.
        if (arraysToSwap.size() > 0)
        {
            for (size_t k = 0 ; k < arraysToSwap.size() ; k++)
            {
                vtkDataArray *arr_in  = inDS->GetCellData()
                                            ->GetArray(arraysToSwap[k].c_str());
                vtkDataArray *new_arr = vtkDataArray::CreateDataArray(
                                                         arr_in->GetDataType());
                vtkDataArray *arr_out = outDS->GetPointData()
                                            ->GetArray(arraysToSwap[k].c_str());
                
                size_t ntups  = arr_out->GetNumberOfTuples();
                size_t ncomps = arr_out->GetNumberOfComponents();
                new_arr->SetNumberOfComponents(ncomps);
                new_arr->SetNumberOfTuples(ntups);
                for (size_t i = 0 ; i < ntups ; i++)
                    for (size_t j = 0 ; j < ncomps ; j++)
                        new_arr->SetComponent(i, j, 
                                           arr_out->GetComponent(i, j)+0.001);
                new_arr->SetName(arr_out->GetName());
                bool isActiveScalar = 
                                 (inDS->GetCellData()->GetScalars() == arr_in);
                bool isActiveVector = 
                                 (inDS->GetCellData()->GetVectors() == arr_in);
                outDS->GetPointData()->RemoveArray(arr_out->GetName());
                outDS->GetPointData()->AddArray(new_arr);
                if (isActiveScalar)
                    outDS->GetPointData()->SetActiveScalars(new_arr->GetName());
                if (isActiveVector)
                    outDS->GetPointData()->SetActiveVectors(new_arr->GetName());
                new_arr->Delete();
            }
            dsToShift->Delete();
        }
    }
    else if (centeringTarget == AVT_ZONECENT)
    {
        // Detect if there are any integer type arrays and make them be floats for
        // recenting.
        size_t nArray = inDS->GetPointData()->GetNumberOfArrays();
        std::vector<std::string> arraysToSwap;
        
        for (size_t i = 0 ; i < nArray ; i++)
        {
            vtkDataArray *arr = inDS->GetPointData()->GetArray(i);
            int dt = arr->GetDataType();
            if (dt == VTK_UNSIGNED_CHAR || dt == VTK_INT ||
                dt == VTK_UNSIGNED_INT)
            {
                std::string arr_name(arr->GetName());
                if(arr_name != "avtGhostZones" && 
                   arr_name != "avtGhostNodes" )
                    arraysToSwap.push_back(arr_name);
            }
        }

        vtkDataSet *dsToShift = inDS;
        if (arraysToSwap.size() > 0)
        {
            dsToShift = (vtkDataSet *) inDS->NewInstance();
            dsToShift->ShallowCopy(inDS);
            for (size_t k = 0 ; k < arraysToSwap.size() ; k++)
            {
                vtkDataArray *arr = inDS->GetPointData()->GetArray(arraysToSwap[k].c_str());
                vtkDataArray *fa = arr->NewInstance();
                size_t ntups  = arr->GetNumberOfTuples();
                size_t ncomps = arr->GetNumberOfComponents();
                fa->SetNumberOfComponents(ncomps);
                fa->SetNumberOfTuples(ntups);
                for (size_t i = 0 ; i < ntups ; i++)
                    for (size_t j = 0 ; j < ncomps ; j++)
                        fa->SetComponent(i, j, arr->GetComponent(i, j));
                fa->SetName(arr->GetName());
                dsToShift->GetPointData()->RemoveArray(arr->GetName());
                dsToShift->GetPointData()->AddArray(fa);
                fa->Delete();
            }
        }

        //
        //  User requested zone-centered but our data is node-centered,
        //  create the cell data from point data.
        //
        vtkPointDataToCellData *pd2cd = vtkPointDataToCellData::New();
     
        pd2cd->SetInputData(dsToShift);
        pd2cd->GetExecutive()->SetOutputData(0, outDS);
        pd2cd->Update();
        pd2cd->Delete();

        // We want to preserve knowledge of ghost zones
        vtkDataArray *ghosts = inDS->GetCellData()->GetArray("avtGhostZones");
        if (ghosts)
        {
            outDS->GetCellData()->AddArray(ghosts);
        }
        vtkDataArray *gn = inDS->GetPointData()->GetArray("avtGhostNodes");
        if (gn != NULL)
        {
            outDS->GetPointData()->AddArray(gn);
            outDS->GetCellData()->RemoveArray("avtGhostNodes");
        }
        

        // Convert the former int arrays back to int.
        if (arraysToSwap.size() > 0)
        {
            for (size_t k = 0 ; k < arraysToSwap.size() ; k++)
            {
                vtkDataArray *arr_in  = inDS->GetPointData()
                                            ->GetArray(arraysToSwap[k].c_str());
                vtkDataArray *new_arr = vtkDataArray::CreateDataArray(
                                                         arr_in->GetDataType());

                vtkDataArray *arr_out = outDS->GetCellData()
                                            ->GetArray(arraysToSwap[k].c_str());
                size_t ntups  = arr_out->GetNumberOfTuples();
                size_t ncomps = arr_out->GetNumberOfComponents();
                new_arr->SetNumberOfComponents(ncomps);
                new_arr->SetNumberOfTuples(ntups);
                for (size_t i = 0 ; i < ntups ; i++)
                    for (size_t j = 0 ; j < ncomps ; j++)
                        new_arr->SetComponent(i, j, 
                                             arr_out->GetComponent(i, j)+0.001);
                new_arr->SetName(arr_out->GetName());
                bool isActiveScalar = 
                                 (inDS->GetPointData()->GetScalars() == arr_in);
                bool isActiveVector = 
                                 (inDS->GetPointData()->GetVectors() == arr_in);
                outDS->GetCellData()->RemoveArray(arr_out->GetName());
                outDS->GetCellData()->AddArray(new_arr);
                if (isActiveScalar)
                    outDS->GetCellData()->SetActiveScalars(new_arr->GetName());
                if (isActiveVector)
                    outDS->GetCellData()->SetActiveVectors(new_arr->GetName());
                new_arr->Delete();
            }
            dsToShift->Delete();
        }
    }
    else
    {
        //
        //  We don't need to do anything to our data. 
        //
        outDS = inDS;
    }

    avtDataRepresentation *outDR = new avtDataRepresentation(outDS,
        inDR->GetDomain(), inDR->GetLabel());

    newDS->Delete();

    return outDR;
}


// ****************************************************************************
//  Method: avtShiftCenteringFilter::UpdateDataObjectInfo
//
//  Purpose:
//      Tells its meta-data object what kind of centering it has.
//
//  Programmer: Hank Childs
//  Creation:   February 27, 2002
//
//  Modifications:
//
//    Hank Childs, Thu Feb 26 08:15:12 PST 2004
//    Account for multiple variables.
//
//    Sean Ahern, Wed Sep 10 13:13:18 EDT 2008
//    For ease of reading code, I forced the target to be of type
//    avtCentering, not integer.
//
// ****************************************************************************

void
avtShiftCenteringFilter::UpdateDataObjectInfo(void)
{
    avtDataAttributes &in_atts = GetInput()->GetInfo().GetAttributes();
    avtDataAttributes &out_atts = GetOutput()->GetInfo().GetAttributes();
    if (!in_atts.ValidActiveVariable())
    {
        // We don't have enough information to figure out which variable
        // we are shifting the centering of.
        return;
    }

    if (centeringTarget == AVT_NODECENT || centeringTarget == AVT_ZONECENT)
        out_atts.SetCentering(centeringTarget);
}


// ****************************************************************************
//  Method: avtShiftCenteringFilter::ModifyContract
//
//  Purpose:
//      Tell the database that we will need ghost zones.
//
//  Programmer: Hank Childs
//  Creation:   August 11, 2004
//
// ****************************************************************************

avtContract_p
avtShiftCenteringFilter::ModifyContract(avtContract_p in_spec)
{
    avtContract_p spec = new avtContract(in_spec);
    spec->GetDataRequest()->SetDesiredGhostDataType(GHOST_ZONE_DATA);
    return spec;
}


