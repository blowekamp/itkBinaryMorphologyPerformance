
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
#ifndef __itkSeparableBinaryDilateImageFilter_hxx
#define __itkSeparableBinaryDilateImageFilter_hxx

#include "itkSeparableBinaryDilateImageFilter.h"

namespace itk
{
template< class TInputImage, class TOutputImage, class TKernel >
SeparableBinaryDilateImageFilter< TInputImage, TOutputImage, TKernel >
::SeparableBinaryDilateImageFilter()
{
  this->m_ForegroundValue = NumericTraits< InputPixelType >::max();
  this->m_BackgroundValue = NumericTraits< OutputPixelType >::NonpositiveMin();
  this->m_BoundaryToForeground = false;
}

template< class TInputImage, class TOutputImage, class TKernel >
void
SeparableBinaryDilateImageFilter< TInputImage, TOutputImage, TKernel >
::FilterDataArray(OutputPixelType *outs, unsigned int ln, unsigned int radius)
  {
    assert( radius =< 16 );
    const OutputPixelType foreground = this->m_ForegroundValue;
    const size_t          usedBits = sizeof(uint32_t)*8-2*radius;
    const uint32_t        new_bit = uint32_t(1)<<usedBits;

    uint32_t     bits = 0;
    unsigned int i = 0;

    // initial boundary condition boundary condition
    if ( this->m_BoundaryToForeground )
      {
      bits = ~uint32_t(0)<<usedBits;
      }

    // handle edge before center of radius in on the image line
    for ( ; i < radius && i < ln; ++i )
      {

      bits <<= 1;

      if ( outs[i] == foreground )
        {
        bits = bits | new_bit;
        }
      }

    for ( ; i < ln; ++i )
      {

      // if any bits are set, change to forground
      if ( bits || outs[i] == foreground )
        {
        outs[i-radius] = foreground;
        }

      bits <<= 1;

      if ( outs[i] == foreground )
        {
        bits = bits | new_bit;
        }
      }

    // handle edge where the radius if off the image line
    for( ; i < ln+radius; ++i )
      {

      // if any bits are set, change to forground
      if ( bits || this->m_BoundaryToForeground )
        {
        outs[i-radius] = foreground;
        }

      // Skip adding newbits, as we are just going to check boundary
      // condtion as above.

      bits <<= 1;

      }

  }

} // end namespace itk

#endif
