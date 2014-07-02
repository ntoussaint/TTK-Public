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
#include "itkTensorConverterCommand.h"

#include <itkImage.h>
#include <itkTensor.h>
#include <itkTensorImageIO.h>

#include <iostream>

namespace itk
{

  TensorConverterCommand::TensorConverterCommand()
  {
    m_ShortDescription = "Convert a tensor image into another format";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<file in> <file out> [UseFSLStyle (boolean, default = 0)]\n\n";
    m_LongDescription += m_ShortDescription;
  }

  TensorConverterCommand::~TensorConverterCommand()
  {}

  int TensorConverterCommand::Execute (int narg, const char* arg[])
  {
    
    if( narg < 3 )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }

    const char* input = arg[1];
    const char* output = arg[2];

    unsigned int usefsl = narg > 3 ? std::atoi (arg[3]) : 0;
    
    // write the output
    typedef itk::TensorImageIO<double, 3, 3> IOType;
    
    IOType::Pointer myReader = IOType::New();
    myReader->SetFileName(input);
    
    try
    {
      myReader->Read();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    IOType::Pointer myWriter = IOType::New();
    myWriter->SetFileName(output);
    myWriter->SetUseFSLStyle (usefsl);
    myWriter->SetInput( myReader->GetOutput() );
    
    std::cout << "Writing: " << output << std::flush;
    try
    {
      myWriter->Write();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;
    
    
    
    return 0;
  }
  
}
