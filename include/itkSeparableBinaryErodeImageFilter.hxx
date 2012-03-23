
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
#ifndef __itkSeparableBinaryErodeImageFilter_hxx
#define __itkSeparableBinaryErodeImageFilter_hxx

#include "itkSeparableBinaryErodeImageFilter.h"

namespace itk
{
template< class TInputImage, class TOutputImage, class TKernel >
SeparableBinaryErodeImageFilter< TInputImage, TOutputImage, TKernel >
::SeparableBinaryErodeImageFilter()
{
  this->m_BoundaryToForeground = true;
}

template< class TInputImage, class TOutputImage, class TKernel >
void
SeparableBinaryErodeImageFilter< TInputImage, TOutputImage, TKernel >
::FilterDataArray(OutputPixelType *outs, unsigned int ln, unsigned int radius)
  {
    assert( radius =< 16 );
    const OutputPixelType foreground = this->m_ForegroundValue;
    const OutputPixelType background = this->m_BackgroundValue;
    const size_t          usedBits = sizeof(uint32_t)*8-2*radius;
    const uint32_t        new_bit = uint32_t(1)<<usedBits;
    const uint32_t        mask = ~uint32_t(0)<<usedBits;

    uint32_t     bits = 0;
    unsigned int i = 0;

    // initial boundary condition boundary condition
    if ( this->m_BoundaryToForeground )
      {
      bits = mask;
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

      // if any of the bits are not set, change foreground to background
      if ( ( outs[i] != foreground || bits^mask ) && outs[i-radius] == foreground )
        {
        outs[i-radius] = background;
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

      // if any of the bits are not set, change foreground to background
      if ( (!this->m_BoundaryToForeground ||  bits^mask ) && outs[i-radius] == foreground )
        {
        outs[i-radius] = background;
        }

      bits <<= 1;

      if ( this->m_BoundaryToForeground  )
        {
        bits = bits | new_bit;
        }

      }

  }

} // end namespace itk

#endif
