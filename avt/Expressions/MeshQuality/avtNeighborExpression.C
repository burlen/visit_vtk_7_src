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
//                       avtNeighborExpression.C                             //
// ************************************************************************* //

#include <avtNeighborExpression.h>

#include <math.h>
#include <float.h>

#include <vtkCellArray.h>
#include <vtkDataArray.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkPointLocator.h>
#include <vtkPolyData.h>
#include <vtkVertex.h>

#include <avtExtents.h>
#include <DebugStream.h>
#include <ImproperUseException.h>

const char * avtNeighborExpression::variableName = "neighbor";

// ****************************************************************************
//  Method: avtNeighborExpression constructor
//
//  Programmer: Akira Haddox
//  Creation:   June 27, 2002
//
// ****************************************************************************

avtNeighborExpression::avtNeighborExpression()
{

}


// ****************************************************************************
//  Method: avtNeighborExpression destructor
//
//  Programmer: Akira Haddox
//  Creation:   June 27, 2002
//
// ****************************************************************************

avtNeighborExpression::~avtNeighborExpression()
{
}

// ****************************************************************************
//  Method: avtNeighborExpression::ExecuteData
//
//  Purpose:
//      Does the actual VTK code to modify the dataset.
//
//  Arguments:
//      in_dr     The input data representation.
//
//  Returns:      The output data representation.
//
//  Programmer:   Akira Haddox
//  Creation:     June 27, 2002
//
//  Modifications:
//
//    Hank Childs, Thu Aug 26 13:47:30 PDT 2010
//    Change extents names.
//
//    Eric Brugger, Wed Aug 20 16:19:07 PDT 2014
//    Modified the class to work with avtDataRepresentation.
//
// ****************************************************************************
 
avtDataRepresentation *
avtNeighborExpression::ExecuteData(avtDataRepresentation *in_dr)
{
    //
    // Get the VTK data set.
    //
    vtkDataSet *in_ds = in_dr->GetDataVTK();

    // Let's get the points from the input dataset.
    vtkPoints *pts = NULL;
    switch (in_ds->GetDataObjectType())
    {
      // Easily done, just grab them.
      case VTK_UNSTRUCTURED_GRID:
      case VTK_POLY_DATA:
      case VTK_STRUCTURED_GRID:
        pts = ((vtkPointSet *) in_ds)->GetPoints();
        break;
 
      // If they're any other type of grid, this filter shouldn't be used
      default:
        EXCEPTION0(ImproperUseException);
    }
 
    vtkIdType nPoints = pts->GetNumberOfPoints();

    // A neighbor filter would require the existance of neighbors
    if (nPoints < 2)
        EXCEPTION0(ImproperUseException);

    // Now that we've done all of our checks for improper use,
    // let's allocate for our needs
    vtkPolyData *results = vtkPolyData::New();
    vtkCellArray *verts = vtkCellArray::New();
    vtkDataArray *data = CreateArrayFromMesh(in_ds);

    results->SetPoints(pts);

    data->SetNumberOfComponents(1);
    data->SetNumberOfTuples(nPoints);

    double bounds[6];
    in_ds->GetBounds(bounds);

    // Create the point locator
    vtkPointLocator *ptLoc = vtkPointLocator::New();
    ptLoc->SetDataSet(in_ds);
    ptLoc->BuildLocator();

    for (vtkIdType id = 0; id < nPoints; id++)
    {
        // Build the vertex list
        vtkVertex *v = vtkVertex::New();
        v->Initialize( 1, &id, pts);
        verts->InsertNextCell(v);
        v->Delete();

        // And at the same time, set the distance data
        double coords[3];
        pts->GetPoint(id, coords);

        // Find the closest 2 points, since the closest is itself of course.
        vtkIdList *closeId = vtkIdList::New();
        ptLoc->FindClosestNPoints(2, coords, closeId);

        double nearCoords[3];
        pts->GetPoint(closeId->GetId(1), nearCoords);

        double distance = (coords[0]-nearCoords[0])*(coords[0]-nearCoords[0]) +
                         (coords[1]-nearCoords[1])*(coords[1]-nearCoords[1]) +  
                         (coords[2]-nearCoords[2])*(coords[2]-nearCoords[2]);
        distance=sqrt(distance);

        data->SetTuple1(id, distance);
        closeId->Delete();
    }
 
    data->SetName("neighbor");
    results->GetPointData()->AddArray(data);
    results->GetPointData()->SetActiveScalars("neighbor");

    results->SetVerts(verts);


    //
    // Make our best attempt at maintaining our extents.
    //
    double exts[2];
    double range[2];
    data->GetRange(range, 0);
    exts[0] = range[0];
    exts[1] = range[1];

    GetOutput()->GetInfo().GetAttributes().GetOriginalDataExtents()->Merge(exts);

    data->Delete();
    verts->Delete();
    pts->Delete();

    avtDataRepresentation *out_dr = new avtDataRepresentation(results,
        in_dr->GetDomain(), in_dr->GetLabel());

    results->Delete();

    return out_dr;    
}

// ****************************************************************************
//  Method: avtNeighborExpression::UpdateDataObjectInfo
//
//  Purpose:
//      Tells our output that we now have a variable, and that our mesh has
//      drastically changed.
//
//  Programmer: Akira Haddox
//  Creation:   June 27, 2002
//
//  Modifications:
//
//    Hank Childs, Fri Feb 20 15:51:54 PST 2004
//    Have base class do more of the work in setting up output.
//
// ****************************************************************************
 
void
avtNeighborExpression::UpdateDataObjectInfo(void)
{
    avtSingleInputExpressionFilter::UpdateDataObjectInfo();

    // And we're now dealing with vertexes
    avtDataAttributes &outAtts = GetOutput()->GetInfo().GetAttributes();
    outAtts.SetTopologicalDimension(0);
    GetOutput()->GetInfo().GetValidity().InvalidateZones();
}
