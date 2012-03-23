/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkBinaryMorphologyBaseImageFilter_hxx
#define __itkBinaryMorphologyBaseImageFilter_hxx

#include "itkBinaryMorphologyBaseImageFilter.h"

namespace itk
{
template< class TInputImage, class TOutputImage, class TKernel >
BinaryMorphologyBaseImageFilter< TInputImage, TOutputImage, TKernel >
::BinaryMorphologyBaseImageFilter()
{
  this->m_ForegroundValue = NumericTraits< InputPixelType >::max();
  this->m_BackgroundValue = NumericTraits< OutputPixelType >::NonpositiveMin();
  this->m_BoundaryToForeground = false;
}


template< class TInputImage, class TOutputImage, class TKernel >
void
BinaryMorphologyBaseImageFilter< TInputImage, TOutputImage, TKernel >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Foreground Value: "
     << static_cast< typename NumericTraits< InputPixelType >::PrintType >( m_ForegroundValue ) << std::endl;
  os << indent << "Background Value: "
     << static_cast< typename NumericTraits< OutputPixelType >::PrintType >( m_BackgroundValue ) << std::endl;

  os << indent << "BoundaryToForeground: " << m_BoundaryToForeground << std::endl;
}
} // end namespace itk

#endif
