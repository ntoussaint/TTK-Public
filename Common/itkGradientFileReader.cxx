/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: svn+ssh://ntoussaint@scm.gforge.inria.fr/svn/ttk/trunk/Common/itkAffineTensorTransform.h $
  Language:  C++
  Date:      $Date: 2010-06-07 12:39:13 +0100 (Mon, 07 Jun 2010) $
  Version:   $Revision: 68 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGradientFileReader_cxx
#define __itkGradientFileReader_cxx

#include "itkGradientFileReader.h"

#include <itksys/SystemTools.hxx>
#include <itksys/ios/sstream>
#include <sstream>
#include <fstream>

namespace itk
{


std::string trim(std::string const& source, char const* delims = " \t\r\n")
{
  std::string result(source);
  std::string::size_type index = result.find_last_not_of(delims);
  if(index != std::string::npos)
  {
    result.erase(++index);
  }
  
  index = result.find_first_not_of(delims);
  if(index != std::string::npos)
  {
    result.erase(0, index);
  }
  else
  {
    result.erase();
  }
  return result;
}


  
/** Constructor */
GradientFileReader
::GradientFileReader()
{
  m_FileName = "";
}

/** Destructor */
GradientFileReader
::~GradientFileReader()
{
}

/** Update the Reader */
void GradientFileReader
::Update()
{  
  std::ifstream in;
  in.open ( m_FileName.c_str(), std::ios::in | std::ios::binary );
  if( in.fail() )
    {
    in.close();
    itkExceptionMacro ( "The file could not be opened for read access "
                        << std::endl << "Filename: \"" << m_FileName << "\"" );
    }

  OStringStream InData;

  // in.get ( InData );
  std::filebuf *pbuf;
  pbuf=in.rdbuf();

  // get file size using buffer's members
  int size=static_cast<int>(pbuf->pubseekoff (0,std::ios::end,std::ios::in));
  pbuf->pubseekpos (0,std::ios::in);

  // allocate memory to contain file data
  char* buffer=new char[size+1];

  // get file data  
  pbuf->sgetn (buffer,size); 
  buffer[size]='\0';
  itkDebugMacro ( "Read file transform Data" );
  InData << buffer;

  delete[] buffer;
  std::string data = InData.str();
  in.close();

  // Read line by line
  std::string::size_type position = 0;
  
  // check for line end convention
  std::string line_end("\n");
  if(data.find('\n') == std::string::npos)
  {
    if(data.find('\r') == std::string::npos)
    {
      itkExceptionMacro ( "No line ending character found, not a valid ITK Transform TXT file" );
    }
    line_end = "\r";
  }

  std::string::size_type firstend = data.find ( line_end, position );
  std::string firstline = trim ( data.substr ( position, firstend - position ) );
  std::string fileextension = itksys::SystemTools::GetFilenameLastExtension(m_FileName);
  
  if (firstline != "#Insight GradientList File V1.0")
  {
    if (fileextension == ".bvec")
      this->ReadBvecFile (data);
    else
      this->ReadSimpleTextFile (data);
    return;
  }
  else
  {
    this->ReadInsightStyleFile (data);
  }
  
}
  
  
/** Update the Reader */
void GradientFileReader
::ReadBvecFile(std::string data)
{

  // check for line end convention
  std::string line_end("\n");
  // Read line by line
  std::string::size_type position = 0;

  unsigned int line_id = -1;
  
  
  while ( position < data.size() )
  {
    // Find the next string
    std::string::size_type end = data.find ( line_end, position );
    std::string line = trim ( data.substr ( position, end - position ) );
    itkDebugMacro ("Found line: \"" << line << "\"" );
    
    if ( line.length() == 0 )
    {
      continue;
    }
    if ( line[0] == '#' || std::string::npos == line.find_first_not_of ( " \t" ) )
    {
      // Skip lines beginning with #, or blank lines
      continue;
    }

    line_id++;
    if (line_id > 2)
    {
      /// weird, we can't have that
      continue;
    }
    
    itksys_ios::istringstream parse ( line );
    std::vector<ScalarType> values;

    // Read the line
    std::string::size_type valueposition = position;
    unsigned int column_id = 0;
    std::string RestOfLine = line;
    
    while ( valueposition < end )
    {
      std::string::size_type valueend = RestOfLine.find (" ");
      std::string Value = trim ( line.substr ( valueposition, valueend) );
      RestOfLine = trim ( line.substr ( valueend+1, end) );
      ScalarType value;
      parse >> value;
      if (m_GradientList.size() > column_id)
	m_GradientList[column_id][line_id] = value;
      else
      {
	VectorType gradient;
	gradient[line_id] = value;
	m_GradientList.push_back (gradient);
      }
      valueposition = valueend + 1;
      column_id++;
    }
    
    position = end+1;
  }  
}
  

/** Update the Reader */
void GradientFileReader
::ReadSimpleTextFile(std::string data)
{

  // check for line end convention
  std::string line_end("\n");
  // Read line by line
  std::string::size_type position = 0;
  while ( position < data.size() )
  {
    // Find the next string
    std::string::size_type end = data.find ( line_end, position );
    std::string line = trim ( data.substr ( position, end - position ) );
    position = end+1;
    itkDebugMacro ("Found line: \"" << line << "\"" );
    
    if ( line.length() == 0 )
    {
      continue;
    }
    if ( line[0] == '#' || std::string::npos == line.find_first_not_of ( " \t" ) )
    {
      // Skip lines beginning with #, or blank lines
      continue;
    }
    if (line.length() < 5)
    {      
      // Skip the line showing the Nb of gradients as it is automatic now
      continue;
    }
    
    itksys_ios::istringstream parse ( line );    

    VectorType gradient;
    ScalarType value;
    for (unsigned int i=0; i<3; i++)
    {
      parse >> value;
      gradient[i] = value;
    }

    m_GradientList.push_back (gradient);
  }
}
  


/** Update the Reader */
void GradientFileReader
::ReadInsightStyleFile(std::string data)
{

  // check for line end convention
  std::string line_end("\n");
  // Read line by line
  std::string::size_type position = 0;
  
  while ( position < data.size() )
  {
    // Find the next string
    std::string::size_type end = data.find ( line_end, position );
    std::string line = trim ( data.substr ( position, end - position ) );
    position = end+1;
    itkDebugMacro ("Found line: \"" << line << "\"" );
    
    if ( line.length() == 0 )
    {
      continue;
    }
    if ( line[0] == '#' || std::string::npos == line.find_first_not_of ( " \t" ) )
    {
      // Skip lines beginning with #, or blank lines
      continue;
    }
    
    // Get the name
    end = line.find ( ":" );
    if ( end == std::string::npos )
    {
      // Throw an error
      itkExceptionMacro ( "Tags must be delimited by :" );
    }
    std::string Name = trim ( line.substr ( 0, end ) );
    std::string Value = trim ( line.substr ( end + 1, line.length() ) );
    
    // Push back 
    itkDebugMacro ( "Name: \"" << Name << "\"" );
    itkDebugMacro ( "Value: \"" << Value << "\"" );
    itksys_ios::istringstream parse ( Value );

    int vector_id = std::atoi (Name.c_str());
    if (vector_id < 0 || vector_id > 512)
    {
      // we encountered a weired line
      itkExceptionMacro ( "Unrecognized tag : " << Name.c_str() );
    }

    VectorType gradient;
    ScalarType value;
    for (unsigned int i=0; i<3; i++)
    {
      parse >> value;
      gradient[i] = value;
    }

    m_GradientList.push_back (gradient);
  }
}

} // namespace itk

#endif