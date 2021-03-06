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

#ifndef CLIENTINFORMATION_H
#define CLIENTINFORMATION_H
#include <state_exports.h>
#include <string>
#include <AttributeSubject.h>


// ****************************************************************************
// Class: ClientInformation
//
// Purpose:
//    This class contains the attributes that describe the client
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

class STATE_API ClientInformation : public AttributeSubject
{
public:
    // These constructors are for objects of this class
    ClientInformation();
    ClientInformation(const ClientInformation &obj);
protected:
    // These constructors are for objects derived from this class
    ClientInformation(private_tmfs_t tmfs);
    ClientInformation(const ClientInformation &obj, private_tmfs_t tmfs);
public:
    virtual ~ClientInformation();

    virtual ClientInformation& operator = (const ClientInformation &obj);
    virtual bool operator == (const ClientInformation &obj) const;
    virtual bool operator != (const ClientInformation &obj) const;
private:
    void Init();
    void Copy(const ClientInformation &obj);
public:

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();
    void SelectClientName();
    void SelectMethodNames();
    void SelectMethodPrototypes();

    // Property setting methods
    void SetClientName(const std::string &clientName_);
    void SetMethodNames(const stringVector &methodNames_);
    void SetMethodPrototypes(const stringVector &methodPrototypes_);

    // Property getting methods
    const std::string  &GetClientName() const;
          std::string  &GetClientName();
    const stringVector &GetMethodNames() const;
          stringVector &GetMethodNames();
    const stringVector &GetMethodPrototypes() const;
          stringVector &GetMethodPrototypes();


    // Keyframing methods
    virtual std::string               GetFieldName(int index) const;
    virtual AttributeGroup::FieldType GetFieldType(int index) const;
    virtual std::string               GetFieldTypeName(int index) const;
    virtual bool                      FieldsEqual(int index, const AttributeGroup *rhs) const;

    // User-defined methods
    void DeclareMethod(const std::string &name, const std::string &proto);
    int GetMethodIndex(const std::string &name) const;
    std::string GetMethod(int i) const;
    std::string GetMethodPrototype(int i) const;
    void ClearMethods();

    // IDs that can be used to identify fields in case statements
    enum {
        ID_clientName = 0,
        ID_methodNames,
        ID_methodPrototypes,
        ID__LAST
    };

private:
    std::string  clientName;
    stringVector methodNames;
    stringVector methodPrototypes;

    // Static class format string for type map.
    static const char *TypeMapFormatString;
    static const private_tmfs_t TmfsStruct;
};
#define CLIENTINFORMATION_TMFS "ss*s*"

#endif
