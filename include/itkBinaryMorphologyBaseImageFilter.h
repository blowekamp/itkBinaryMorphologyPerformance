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
#ifndef __itkBinaryMorphologyBaseImageFilter_h
#define __itkBinaryMorphologyBaseImageFilter_h

#include "itkKernelImageFilter.h"
#include "itkNumericTraits.h"

namespace itk
{
/** \class BinaryMorphologyBaseImageFilter
 * \brief A generic base class for binary morphology filters
 *
 * This class in intended to provide a stardard interface to setting
 * the foregournd, background and boundary condition.
 *
 * \author Bradley Lowekamp
 * \sa itkBinaryMorphologyImageFilter
 * \ingroup ITKBinaryMorpholgyPerformance
 */
template< class TInputImage, class TOutputImage, class TKernel >
class ITK_EXPORT BinaryMorphologyBaseImageFilter:
    public KernelImageFilter< TInputImage, TOutputImage, TKernel >
{
public:
  /** Standard class typedefs. */
  typedef BinaryMorphologyBaseImageFilter                Self;
  typedef KernelImageFilter< TInputImage, TOutputImage, TKernel > Superclass;
  typedef SmartPointer< Self >                                Pointer;
  typedef SmartPointer< const Self >                          ConstPointer;

  /** Type macro that defines a name for this class. */
  itkTypeMacro(BinaryMorphologyBaseImageFilter, KernelImageFilter);

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

  /** Set the value in the image to consider as "foreground". Defaults to
   * maximum value of PixelType. Subclasses may alias this to
   * DilateValue or ErodeValue. */
  itkSetMacro(ForegroundValue, InputPixelType);

  /** Get the value in the image considered as "foreground". Defaults to
   * maximum value of PixelType. */
  itkGetConstMacro(ForegroundValue, InputPixelType);

  /** Set the value used as "background". Any pixel value which is
   * not DilateValue is considered background. BackgroundValue is used
   * to fill the removed pixels.
   */
  itkSetMacro(BackgroundValue, OutputPixelType);

  /** Get the value used as "background". Any pixel value which is
   * not DilateValue is considered background. BackgroundValue is used
   * to fill the removed pixels.
   */
  itkGetConstMacro(BackgroundValue, OutputPixelType);

  /** Get/Set the borders as foreground (true) or background (false).
   */
  itkSetMacro(BoundaryToForeground, bool);
  itkGetConstReferenceMacro(BoundaryToForeground, bool);
  itkBooleanMacro(BoundaryToForeground);

protected:
  BinaryMorphologyBaseImageFilter();
  // virtual ~BinaryMorphologyBaseImageFilter() {} default implementation OK
  void PrintSelf(std::ostream & os, Indent indent) const;

  /** Pixel value to dilate */
  InputPixelType m_ForegroundValue;

  /** Pixel value for background */
  OutputPixelType m_BackgroundValue;

  /** True if the bounary should be considered foreground */
  bool m_BoundaryToForeground;

private:
  BinaryMorphologyBaseImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);                //purposely not implemented
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryMorphologyBaseImageFilter.hxx"
#endif

#endif
