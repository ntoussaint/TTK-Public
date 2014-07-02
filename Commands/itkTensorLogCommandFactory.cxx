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
#include "itkTensorLogCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkTensorLogCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  TensorLogCommandFactory::TensorLogCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkTensorLogCommand",
			   "Convert a tensor image into a log tensor image",
			   1,
			   CreateObjectFunction<TensorLogCommand>::New());
  }
  
  TensorLogCommandFactory::~TensorLogCommandFactory()
  {
  }
  
  const char* 
  TensorLogCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  TensorLogCommandFactory::GetDescription(void) const
  {
    return "Convert a tensor image into a log tensor image";
  }
  
} // end namespace itk
