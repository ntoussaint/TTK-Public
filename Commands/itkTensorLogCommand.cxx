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
#include "itkTensorLogCommand.h"

#include <itkImage.h>
#include <itkTensor.h>
#include <itkTensorImageIO.h>

#include <itkLogTensorImageFilter.h>
#include <iostream>

namespace itk
{

  TensorLogCommand::TensorLogCommand()
  {
    m_ShortDescription = "Log a tensor image";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<file in> <file out> [UseFSLStyle (boolean, default = 0)]\n\n";
    m_LongDescription += m_ShortDescription;
  }

  TensorLogCommand::~TensorLogCommand()
  {}

  int TensorLogCommand::Execute (int narg, const char* arg[])
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
    typedef itk::TensorImageIO<double, 3, 3> IOType;
    typedef IOType::TensorImageType TensorImageType;
    
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

    typedef itk::LogTensorImageFilter<TensorImageType, TensorImageType> LogType;
    LogType::Pointer myLog = LogType::New();
    myLog->SetInput (myReader->GetOutput());
    
    IOType::Pointer myWriter = IOType::New();
    myWriter->SetFileName(output);
    myWriter->SetUseFSLStyle (usefsl);
    myWriter->SetInput( myLog->GetOutput() );
    
    std::cout << "Writing: " << output << std::flush;
    try
    {
      myLog->Update();
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
