/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_TensorLogCommandFactory_h_
#define _itk_TensorLogCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class TensorLogCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef TensorLogCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static TensorLogCommandFactory* FactoryNew() { return new TensorLogCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(TensorLogCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      TensorLogCommandFactory::Pointer CSFFactory = TensorLogCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    TensorLogCommandFactory();
    ~TensorLogCommandFactory();
    
  private:
    TensorLogCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
