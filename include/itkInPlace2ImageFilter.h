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
/*=========================================================================
 *
 *  Portions of this file are subject to the VTK Toolkit Version 3 copyright.
 *
 *  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
 *
 *  For complete copyright, license and disclaimer of warranty information
 *  please refer to the NOTICE file at the top of the ITK source tree.
 *
 *=========================================================================*/
#ifndef __itkInPlace2ImageFilter_h
#define __itkInPlace2ImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkIsSame.h"

namespace itk
{
/** \class InPlace2ImageFilter
 * This is a copy InPlaceImagefilter with the addition of a third
 * template parameter to specify its parent. It is intended to replace
 * the current implmentation. This enable the addition of the InPlace
 * feature into other ITK class hierarchies. The third template
 * parameter, TSuperClass, is optional, and defaults to the original
 * superclass, for ease of use and compoatibility.
 *
 * \ingroup ImageFilters
 * \ingroup ITKCommon
 */
template< class TInputImage, class TOutputImage = TInputImage,
          class TSuperClass = ImageToImageFilter< TInputImage, TOutputImage >  >
class ITK_EXPORT InPlace2ImageFilter:
    public TSuperClass
{
public:
  /** Standard class typedefs. */
  typedef InPlace2ImageFilter                             Self;
  typedef TSuperClass                                     Superclass;
  typedef SmartPointer< Self >                            Pointer;
  typedef SmartPointer< const Self >                      ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(InPlace2ImageFilter, TSuperClass);

  /** Superclass typedefs. */
  typedef typename Superclass::OutputImageType       OutputImageType;
  typedef typename Superclass::OutputImagePointer    OutputImagePointer;
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;
  typedef typename Superclass::OutputImagePixelType  OutputImagePixelType;

  /** Some convenient typedefs. */
  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** In place operation can be turned on and off. Asking for
   * in-place operation, i.e. calling SetInplace(true) or InplaceOn(),
   * will be effective only if CanRunInPlace2 also returns true.
   * By default CanRunInPlace2 checks whether the input and output
   * image type match. */
  itkSetMacro(InPlace, bool);
  itkGetConstMacro(InPlace, bool);
  itkBooleanMacro(InPlace);

  /** Can the filter run in place? To do so, the filter's first input
   * and output must have the same dimension and pixel type. This
   * method can be used in conjunction with the InPlace ivar to
   * determine whether a particular use of the filter is really
   * running in place. Some filters may be able to optimize their
   * operation if the InPlace is true and CanRunInPlace is true.
   * CanRunInPlace may also be overridded by InPlaceImageFilter
   * subclasses to fine tune its behavior. */
  virtual bool CanRunInPlace() const;

protected:
  InPlace2ImageFilter();
  ~InPlace2ImageFilter();

  virtual void PrintSelf(std::ostream & os, Indent indent) const;

  /** The GenerateData method normally allocates the buffers for all
   * of the outputs of a filter. Since InPlace2ImageFilter's can use an
   * overwritten version of the input for its output, the output
   * buffer should not be allocated. When possible, we graft the input
   * to the filter to the output.  If an InPlace2Filter has multiple
   * outputs, then it would need to override this method to graft one
   * of its outputs and allocate the remaining. If a filter is
   * threaded (i.e. it provides an implementation of
   * ThreadedGenerateData()), this method is called automatically. If
   * an InPlace2Filter is not threaded (i.e. it provides an
   * implementation of GenerateData()), then this method (or
   * equivalent) must be called in GenerateData(). */
  virtual void AllocateOutputs()
  {
    this->InternalAllocateOutputs(IsSame<TInputImage, TOutputImage>());
  }

  /** InPlace2ImageFilter may transfer ownership of the input bulk data
   * to the output object.  Once the output object owns the bulk data
   * (done in AllocateOutputs()), the input object must release its
   * hold on the bulk data.  ProcessObject::ReleaseInputs() only
   * releases the input bulk data when the user has set the
   * ReleaseDataFlag.  InPlace2ImageFilter::ReleaseInputs() also
   * releases the input that it has overwritten.
   *
   * \sa ProcessObject::ReleaseInputs() */
  virtual void ReleaseInputs();

  /** This methods should only be called during the GenerateData phase
   *  of the pipeline. This method return true if the input image's
   *  bulk data is the same as the output image's data.
   */
  itkGetConstMacro(RunningInPlace,bool);

private:
  InPlace2ImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);     //purposely not implemented

  // the type are different we can't run in place
  void InternalAllocateOutputs( const FalseType& )
  {
    this->m_RunningInPlace = false;
    this->Superclass::AllocateOutputs();
  }

  void InternalAllocateOutputs( const TrueType& );

  bool m_InPlace; // enable the possibility of in-place
  bool m_RunningInPlace;

};
} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_InPlace2ImageFilter(_, EXPORT, TypeX, TypeY)                  \
  namespace itk                                                                    \
  {                                                                                \
  _( 2 ( class EXPORT InPlace2ImageFilter< ITK_TEMPLATE_2 TypeX > ) )              \
  namespace Templates                                                              \
  {                                                                                \
  typedef InPlace2ImageFilter< ITK_TEMPLATE_2 TypeX > InPlace2ImageFilter##TypeY;  \
  }                                                                                \
  }

#if ITK_TEMPLATE_EXPLICIT
#include "Templates/itkInPlace2ImageFilter+-.h"
#endif

#if ITK_TEMPLATE_TXX
#include "itkInPlace2ImageFilter.hxx"
#endif

#endif
