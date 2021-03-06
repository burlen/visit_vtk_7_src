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

#include <ClientMethod.h>
#include <DataNode.h>

// ****************************************************************************
// Method: ClientMethod::ClientMethod
//
// Purpose: 
//   Init utility for the ClientMethod class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void ClientMethod::Init()
{

    ClientMethod::SelectAll();
}

// ****************************************************************************
// Method: ClientMethod::ClientMethod
//
// Purpose: 
//   Copy utility for the ClientMethod class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void ClientMethod::Copy(const ClientMethod &obj)
{
    methodName = obj.methodName;
    intArgs = obj.intArgs;
    doubleArgs = obj.doubleArgs;
    stringArgs = obj.stringArgs;

    ClientMethod::SelectAll();
}

// Type map format string
const char *ClientMethod::TypeMapFormatString = CLIENTMETHOD_TMFS;
const AttributeGroup::private_tmfs_t ClientMethod::TmfsStruct = {CLIENTMETHOD_TMFS};


// ****************************************************************************
// Method: ClientMethod::ClientMethod
//
// Purpose: 
//   Default constructor for the ClientMethod class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ClientMethod::ClientMethod() : 
    AttributeSubject(ClientMethod::TypeMapFormatString)
{
    ClientMethod::Init();
}

// ****************************************************************************
// Method: ClientMethod::ClientMethod
//
// Purpose: 
//   Constructor for the derived classes of ClientMethod class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ClientMethod::ClientMethod(private_tmfs_t tmfs) : 
    AttributeSubject(tmfs.tmfs)
{
    ClientMethod::Init();
}

// ****************************************************************************
// Method: ClientMethod::ClientMethod
//
// Purpose: 
//   Copy constructor for the ClientMethod class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ClientMethod::ClientMethod(const ClientMethod &obj) : 
    AttributeSubject(ClientMethod::TypeMapFormatString)
{
    ClientMethod::Copy(obj);
}

// ****************************************************************************
// Method: ClientMethod::ClientMethod
//
// Purpose: 
//   Copy constructor for derived classes of the ClientMethod class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ClientMethod::ClientMethod(const ClientMethod &obj, private_tmfs_t tmfs) : 
    AttributeSubject(tmfs.tmfs)
{
    ClientMethod::Copy(obj);
}

// ****************************************************************************
// Method: ClientMethod::~ClientMethod
//
// Purpose: 
//   Destructor for the ClientMethod class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ClientMethod::~ClientMethod()
{
    // nothing here
}

// ****************************************************************************
// Method: ClientMethod::operator = 
//
// Purpose: 
//   Assignment operator for the ClientMethod class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ClientMethod& 
ClientMethod::operator = (const ClientMethod &obj)
{
    if (this == &obj) return *this;

    ClientMethod::Copy(obj);

    return *this;
}

// ****************************************************************************
// Method: ClientMethod::operator == 
//
// Purpose: 
//   Comparison operator == for the ClientMethod class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
ClientMethod::operator == (const ClientMethod &obj) const
{
    // Create the return value
    return ((methodName == obj.methodName) &&
            (intArgs == obj.intArgs) &&
            (doubleArgs == obj.doubleArgs) &&
            (stringArgs == obj.stringArgs));
}

// ****************************************************************************
// Method: ClientMethod::operator != 
//
// Purpose: 
//   Comparison operator != for the ClientMethod class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
ClientMethod::operator != (const ClientMethod &obj) const
{
    return !(this->operator == (obj));
}

// ****************************************************************************
// Method: ClientMethod::TypeName
//
// Purpose: 
//   Type name method for the ClientMethod class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

const std::string
ClientMethod::TypeName() const
{
    return "ClientMethod";
}

// ****************************************************************************
// Method: ClientMethod::CopyAttributes
//
// Purpose: 
//   CopyAttributes method for the ClientMethod class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
ClientMethod::CopyAttributes(const AttributeGroup *atts)
{
    if(TypeName() != atts->TypeName())
        return false;

    // Call assignment operator.
    const ClientMethod *tmp = (const ClientMethod *)atts;
    *this = *tmp;

    return true;
}

// ****************************************************************************
// Method: ClientMethod::CreateCompatible
//
// Purpose: 
//   CreateCompatible method for the ClientMethod class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

AttributeSubject *
ClientMethod::CreateCompatible(const std::string &tname) const
{
    AttributeSubject *retval = 0;
    if(TypeName() == tname)
        retval = new ClientMethod(*this);
    // Other cases could go here too. 

    return retval;
}

// ****************************************************************************
// Method: ClientMethod::NewInstance
//
// Purpose: 
//   NewInstance method for the ClientMethod class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

AttributeSubject *
ClientMethod::NewInstance(bool copy) const
{
    AttributeSubject *retval = 0;
    if(copy)
        retval = new ClientMethod(*this);
    else
        retval = new ClientMethod;

    return retval;
}

// ****************************************************************************
// Method: ClientMethod::SelectAll
//
// Purpose: 
//   Selects all attributes.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void
ClientMethod::SelectAll()
{
    Select(ID_methodName, (void *)&methodName);
    Select(ID_intArgs,    (void *)&intArgs);
    Select(ID_doubleArgs, (void *)&doubleArgs);
    Select(ID_stringArgs, (void *)&stringArgs);
}

///////////////////////////////////////////////////////////////////////////////
// Set property methods
///////////////////////////////////////////////////////////////////////////////

void
ClientMethod::SetMethodName(const std::string &methodName_)
{
    methodName = methodName_;
    Select(ID_methodName, (void *)&methodName);
}

void
ClientMethod::SetIntArgs(const intVector &intArgs_)
{
    intArgs = intArgs_;
    Select(ID_intArgs, (void *)&intArgs);
}

void
ClientMethod::SetDoubleArgs(const doubleVector &doubleArgs_)
{
    doubleArgs = doubleArgs_;
    Select(ID_doubleArgs, (void *)&doubleArgs);
}

void
ClientMethod::SetStringArgs(const stringVector &stringArgs_)
{
    stringArgs = stringArgs_;
    Select(ID_stringArgs, (void *)&stringArgs);
}

///////////////////////////////////////////////////////////////////////////////
// Get property methods
///////////////////////////////////////////////////////////////////////////////

const std::string &
ClientMethod::GetMethodName() const
{
    return methodName;
}

std::string &
ClientMethod::GetMethodName()
{
    return methodName;
}

const intVector &
ClientMethod::GetIntArgs() const
{
    return intArgs;
}

intVector &
ClientMethod::GetIntArgs()
{
    return intArgs;
}

const doubleVector &
ClientMethod::GetDoubleArgs() const
{
    return doubleArgs;
}

doubleVector &
ClientMethod::GetDoubleArgs()
{
    return doubleArgs;
}

const stringVector &
ClientMethod::GetStringArgs() const
{
    return stringArgs;
}

stringVector &
ClientMethod::GetStringArgs()
{
    return stringArgs;
}

///////////////////////////////////////////////////////////////////////////////
// Select property methods
///////////////////////////////////////////////////////////////////////////////

void
ClientMethod::SelectMethodName()
{
    Select(ID_methodName, (void *)&methodName);
}

void
ClientMethod::SelectIntArgs()
{
    Select(ID_intArgs, (void *)&intArgs);
}

void
ClientMethod::SelectDoubleArgs()
{
    Select(ID_doubleArgs, (void *)&doubleArgs);
}

void
ClientMethod::SelectStringArgs()
{
    Select(ID_stringArgs, (void *)&stringArgs);
}

///////////////////////////////////////////////////////////////////////////////
// Keyframing methods
///////////////////////////////////////////////////////////////////////////////

// ****************************************************************************
// Method: ClientMethod::GetFieldName
//
// Purpose: 
//   This method returns the name of a field given its index.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

std::string
ClientMethod::GetFieldName(int index) const
{
    switch (index)
    {
    case ID_methodName: return "methodName";
    case ID_intArgs:    return "intArgs";
    case ID_doubleArgs: return "doubleArgs";
    case ID_stringArgs: return "stringArgs";
    default:  return "invalid index";
    }
}

// ****************************************************************************
// Method: ClientMethod::GetFieldType
//
// Purpose: 
//   This method returns the type of a field given its index.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

AttributeGroup::FieldType
ClientMethod::GetFieldType(int index) const
{
    switch (index)
    {
    case ID_methodName: return FieldType_string;
    case ID_intArgs:    return FieldType_intVector;
    case ID_doubleArgs: return FieldType_doubleVector;
    case ID_stringArgs: return FieldType_stringVector;
    default:  return FieldType_unknown;
    }
}

// ****************************************************************************
// Method: ClientMethod::GetFieldTypeName
//
// Purpose: 
//   This method returns the name of a field type given its index.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

std::string
ClientMethod::GetFieldTypeName(int index) const
{
    switch (index)
    {
    case ID_methodName: return "string";
    case ID_intArgs:    return "intVector";
    case ID_doubleArgs: return "doubleVector";
    case ID_stringArgs: return "stringVector";
    default:  return "invalid index";
    }
}

// ****************************************************************************
// Method: ClientMethod::FieldsEqual
//
// Purpose: 
//   This method compares two fields and return true if they are equal.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
ClientMethod::FieldsEqual(int index_, const AttributeGroup *rhs) const
{
    const ClientMethod &obj = *((const ClientMethod*)rhs);
    bool retval = false;
    switch (index_)
    {
    case ID_methodName:
        {  // new scope
        retval = (methodName == obj.methodName);
        }
        break;
    case ID_intArgs:
        {  // new scope
        retval = (intArgs == obj.intArgs);
        }
        break;
    case ID_doubleArgs:
        {  // new scope
        retval = (doubleArgs == obj.doubleArgs);
        }
        break;
    case ID_stringArgs:
        {  // new scope
        retval = (stringArgs == obj.stringArgs);
        }
        break;
    default: retval = false;
    }

    return retval;
}

///////////////////////////////////////////////////////////////////////////////
// User-defined methods.
///////////////////////////////////////////////////////////////////////////////

void
ClientMethod::ClearArgs()
{
    intArgs.clear();
    doubleArgs.clear();
    stringArgs.clear();
    SelectIntArgs();
    SelectDoubleArgs();
    SelectStringArgs();
}

void
ClientMethod::AddArgument(int val)
{
    intArgs.push_back(val);
    SelectIntArgs();
}

void
ClientMethod::AddArgument(double val)
{
    doubleArgs.push_back(val);
    SelectDoubleArgs();
}

void
ClientMethod::AddArgument(const std::string &val)
{
    stringArgs.push_back(val);
    SelectStringArgs();
}

