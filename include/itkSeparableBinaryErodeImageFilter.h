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
#ifndef __itkSeparableBinaryErodeImageFilter_h
#define __itkSeparableBinaryErodeImageFilter_h

#include "itkSeparableBinaryMorphologyImageFilter.h"

namespace itk
{
/** \class SeparableBinaryErodeImageFilter
 * \brief A fast implementation of binary erode for box shaped
 * structuring elements.
 *
 * \ingroup ITKBinaryMorpholgyPerformance
 */
template< class TInputImage, class TOutputImage, class TKernel >
class ITK_EXPORT SeparableBinaryErodeImageFilter:
    public SeparableBinaryMorphologyImageFilter< TInputImage, TOutputImage, TKernel >
{
public:
  /** Standard class typedefs. */
  typedef SeparableBinaryErodeImageFilter                             Self;
  typedef SeparableBinaryMorphologyImageFilter< TInputImage, TOutputImage, TKernel >  Superclass;
  typedef SmartPointer< Self >                                        Pointer;
  typedef SmartPointer< const Self >                                  ConstPointer;

  /** Type macro that defines a name for this class. */
  itkTypeMacro(SeparableBinaryErodeImageFilter, SeparableBinaryMorphologyImageFilter);

  /** Smart pointer typedef support.  */
  typedef typename TInputImage::PixelType  InputPixelType;
  typedef typename TOutputImage::PixelType OutputPixelType;

  typedef typename TOutputImage::RegionType OutputImageRegionType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

protected:
  SeparableBinaryErodeImageFilter();
  // virtual ~SeparableBinaryErodeImageFilter() {} default implementation ok


  virtual void FilterDataArray(OutputPixelType *outs, unsigned int ln, unsigned int radius);


private:
  SeparableBinaryErodeImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);                //purposely not implemented

};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSeparableBinaryErodeImageFilter.hxx"
#endif

#endif
