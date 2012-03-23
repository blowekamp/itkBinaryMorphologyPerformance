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
#ifndef __itkSeparableBinaryMorphologyImageFilter_h
#define __itkSeparableBinaryMorphologyImageFilter_h

#include "itkBinaryMorphologyBaseImageFilter.h"
#include "itkInPlace2ImageFilter.h"
#include "itkNumericTraits.h"

namespace itk
{
/** \class SeparableBinaryMorphologyImageFilter
 * \brief A base class for separable binary morphology filters
 *
 * This a virual base class which implements the stardard binary
 * morphology interface, for a specilized algorithms and high
 * performance algorithm which can run on an image independently in
 * axis oriented arrays.
 *
 * \author Bradley Lowekamp
 * \sa itkBinaryMorphologyBaseImageFilter
 * \ingroup ITKBinaryMorpholgyPerformance
 */
template< class TInputImage, class TOutputImage, class TKernel >
class ITK_EXPORT SeparableBinaryMorphologyImageFilter:
    public InPlace2ImageFilter< TInputImage, TOutputImage, BinaryMorphologyBaseImageFilter< TInputImage, TOutputImage, TKernel > >
{
public:
  /** Standard class typedefs. */
  typedef SeparableBinaryMorphologyImageFilter                Self;
  typedef InPlace2ImageFilter< TInputImage, TOutputImage,
    BinaryMorphologyBaseImageFilter< TInputImage, TOutputImage, TKernel > > Superclass;
  typedef SmartPointer< Self >                                Pointer;
  typedef SmartPointer< const Self >                          ConstPointer;

  /** Type macro that defines a name for this class. */
  itkTypeMacro(SeparableBinaryMorphologyImageFilter, InPlace2ImageFilter);

  /** Smart pointer typedef support.  */
  typedef typename TInputImage::Pointer      InputImagePointer;
  typedef typename TInputImage::ConstPointer InputImageConstPointer;

  typedef typename TInputImage::PixelType  InputPixelType;
  typedef typename TOutputImage::PixelType OutputPixelType;

  typedef typename TOutputImage::RegionType OutputImageRegionType;

  /** Type of the input image */
  typedef TInputImage InputImageType;

  /** Type of the output image */
  typedef TOutputImage OutputImageType;

  /** Kernel typedef. */
  typedef TKernel KernelType;

  /** Set kernel (structuring element). */
  void SetKernel(const KernelType & kernel)
  {
    // todo check that we have a box structuring element
    this->Superclass::SetKernel( kernel );
  }

protected:
  SeparableBinaryMorphologyImageFilter();
  // virtual ~SeparableBinaryMorphologyImageFilter() {} default implementation OK
  void PrintSelf(std::ostream & os, Indent indent) const;

  virtual void VerifyPreconditions();

  virtual void GenerateData();

  virtual void ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread, ThreadIdType threadId);

  virtual unsigned int SplitRequestedRegion(unsigned int i, unsigned int num, OutputImageRegionType & splitRegion);

  /** The function to do the real morphology algorithm on a per-line
   * basis. It is to be run inplace where the "outs" parameter is both
   * the input and the output. ln is the size of the array. */
  virtual void FilterDataArray(OutputPixelType *outs, unsigned int ln, unsigned int radius) = 0;

private:
  SeparableBinaryMorphologyImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);                //purposely not implemented

  /** Direction in which the filter is currently being applied,
   *  which should be in the range [0,ImageDimension-1]. */
  unsigned int m_Direction;

};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSeparableBinaryMorphologyImageFilter.hxx"
#endif

#endif
