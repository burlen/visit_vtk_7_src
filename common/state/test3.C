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

#include <MaterialAttributes.h>
#include <ColorAttributeList.h>
#include <ColorAttribute.h>
#include <Observer.h>
#include <BufferConnection.h>

#include <TestUtil.h>

#define VERBOSE
#define N_TESTS 1

// Prototypes
bool Run_Test1(bool verbose, int *subtest, int *nsubtests);

// ****************************************************************************
// Function: main
//
// Purpose:
//   The main function for this test program. It executes the test
//   cases.
//
// Programmer: Brad Whitlock
// Creation:   Mon Dec 11 12:01:58 PDT 2000
//
// Modifications:
//   
// ****************************************************************************

int
main(int argc, char *argv[])
{
    int      subtest, nsubtests;
    bool     test[N_TESTS];
    TestUtil util(argc, argv, "Tests the ColorAttributeList class.");

    // Test the Plot class.
    test[0] = Run_Test1(util.verbose, &subtest, &nsubtests);
    util.PrintTestResult(1, subtest, nsubtests, test[0]);

    return util.PassFail(test, N_TESTS);
}

ostream &operator <<(ostream &os, const ColorAttribute &ca)
{
    os << "{" << ca.Red() << ", " << ca.Green() << ", " << ca.Blue()
       << ", " << ca.Alpha() << "}";
    return os;
}

ostream &operator <<(ostream &os, const ColorAttributeList &cal)
{
    os << "{";
    for(int i = 0; i < cal.GetNumColorAttributes(); ++i)
    {
        os << cal[i];
        if(i < cal.GetNumColorAttributes() - 1)
           os << ", ";
    }
    os << "}";

    return os;
}

// ****************************************************************************
// Function: Run_Test1
//
// Purpose:
//   Tests operations for the ColorAttributeList class.
//
// Arguments:
//   verbose   : Whether or not to do lots of output.
//   subtest   : A return code to indicate the last subtest to be
//               executed.
//   nsubtests : The number of subtests.
//
// Notes:      
//
// Programmer: Brad Whitlock
// Creation:   Mon Dec 11 12:01:58 PDT 2000
//
// Modifications:
//   
// ****************************************************************************

bool
Run_Test1(bool verbose, int *subtest, int *nsubtests)
{
    // We have 5 subtests
    *nsubtests = 5;

#ifdef VERBOSE
    if(verbose)
    {
        cout << "=================================================" << endl;
        cout << "Running Test 1" << endl;
        cout << "=================================================" << endl;
    }
#endif

    // Create a list of ColorAttribute.
    ColorAttributeList cal;

    // Add some ColorAttribute to it.
    ColorAttribute c1(255, 0, 0), c2(255,255,0), c3(0, 255, 0), c4(0,0,255);
    if(verbose)
       cout << "Before 1st addition: " << cal << endl;
    cal.AddColorAttribute(c1);
    if(verbose)
       cout << "After 1st addition: " << cal << endl;
    cal.AddColorAttribute(c2);
    if(verbose)
       cout << "After 2nd addition: " << cal << endl;
    cal.AddColorAttribute(c3);
    if(verbose)
       cout << "After 3rd addition: " << cal << endl;
    cal.AddColorAttribute(c4);
    if(verbose)
       cout << "After 4th addition: " << cal << endl << endl;
    if(cal.GetNumColorAttributes() != 4)
        return false;

    // Test the copy constructor.
    *subtest = 2;
    ColorAttributeList cal2(cal);
    if(verbose)
    {
        cout << "Original:" << cal << endl;
        cout << "Copy:" << cal2 << endl << endl;
    }
    if(cal != cal2)
        return false;

    // Test ClearColorAttribute.
    *subtest = 3;
    if(verbose)
        cout << "Before clear:" << cal2 << endl;
    cal2.ClearColorAttributes();
    if(verbose)
        cout << "After clear:" << cal2 << endl << endl;
    if(cal2.GetNumColorAttributes() != 0)
         return false;

    // Test RemoveColorAttribute.
    *subtest = 4;
    ColorAttributeList calRem(cal);
    if(verbose)
        cout << "Before removal:" << calRem << endl;
    bool retval = true;
    for(int i = 0; i < 4 && !retval; ++i)
    {
        calRem.RemoveColorAttribute(0);
        if(verbose)
            cout << calRem << endl;
        switch(i)
        {
        case 0:
            retval = (calRem[0] == c2);
            break;
        case 1:
            retval = (calRem[0] == c3);
            break;
        case 2:
            retval = (calRem[0] == c4);
            break;
        case 3:
            retval = true;
            break;
        }
    }
    if(verbose)
        cout << "After all removals: " << calRem << endl << endl;
    if(!retval)
        return false;

    // Try writing the ColorAttribute to a list and reading them back.
    *subtest = 5;
    BufferConnection buf;
    cal.Write(buf);
    ColorAttributeList reader;
    reader.Read(buf);
    if(verbose)
    {
        cout << "Writer: " << cal << endl;
        cout << "Reader: " << reader << endl;
    }
    if(!(cal == reader))
        return false;

    return true;
}
