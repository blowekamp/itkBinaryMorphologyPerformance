
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
#ifndef __itkSeparableBinaryMorphologyImageFilter_hxx
#define __itkSeparableBinaryMorphologyImageFilter_hxx

#include "itkSeparableBinaryMorphologyImageFilter.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "itkImageAlgorithm.h"
#include "itkProgressReporter.h"

namespace itk
{
template< class TInputImage, class TOutputImage, class TKernel >
SeparableBinaryMorphologyImageFilter< TInputImage, TOutputImage, TKernel >
::SeparableBinaryMorphologyImageFilter()
{
  this->m_BoundaryToForeground = false;
  this->m_Direction = 0;
}

template< class TInputImage, class TOutputImage, class TKernel >
unsigned int
SeparableBinaryMorphologyImageFilter< TInputImage, TOutputImage, TKernel >
::SplitRequestedRegion(unsigned int i, unsigned int num, OutputImageRegionType & splitRegion)
{
  // Get the output pointer
  OutputImageType *outputPtr = this->GetOutput();

  const class TOutputImage::SizeType & requestedRegionSize =
    outputPtr->GetRequestedRegion().GetSize();

  int splitAxis;
  typename TOutputImage::IndexType splitIndex;
  typename TOutputImage::SizeType splitSize;

  // Initialize the splitRegion to the output requested region
  splitRegion = outputPtr->GetRequestedRegion();
  splitIndex = splitRegion.GetIndex();
  splitSize = splitRegion.GetSize();

  // split on the outermost dimension available
  // and avoid the current dimension
  splitAxis = outputPtr->GetImageDimension() - 1;
  while ( requestedRegionSize[splitAxis] == 1 || splitAxis == (int)m_Direction )
    {
    --splitAxis;
    if ( splitAxis < 0 )
      { // cannot split
      itkDebugMacro("  Cannot Split");
      return 1;
      }
    }

  // determine the actual number of pieces that will be generated
  typename TOutputImage::SizeType::SizeValueType range = requestedRegionSize[splitAxis];
  unsigned int valuesPerThread = (unsigned int)vcl_ceil(range / (double)num);
  unsigned int maxThreadIdUsed = (unsigned int)vcl_ceil(range / (double)valuesPerThread) - 1;

  // Split the region
  if ( i < maxThreadIdUsed )
    {
    splitIndex[splitAxis] += i * valuesPerThread;
    splitSize[splitAxis] = valuesPerThread;
    }
  if ( i == maxThreadIdUsed )
    {
    splitIndex[splitAxis] += i * valuesPerThread;
    // last thread needs to process the "rest" dimension being split
    splitSize[splitAxis] = splitSize[splitAxis] - i * valuesPerThread;
    }

  // set the split region ivars
  splitRegion.SetIndex(splitIndex);
  splitRegion.SetSize(splitSize);

  itkDebugMacro("  Split Piece: " << splitRegion);

  return maxThreadIdUsed + 1;
}

template< class TInputImage, class TOutputImage, class TKernel >
void
SeparableBinaryMorphologyImageFilter< TInputImage, TOutputImage, TKernel >
::VerifyPreconditions()
{
  typename TInputImage::ConstPointer inputImage( this->GetInput() );

  const unsigned int imageDimension = inputImage->GetImageDimension();

  if ( this->m_Direction >= imageDimension )
    {
    itkExceptionMacro("Direction selected for filtering is greater than ImageDimension");
    }

  // todo verify that we have a box structuring element
}


//----------------------------------------------------------------------------
template< class TInputImage, class TOutputImage, class TKernel >
void
SeparableBinaryMorphologyImageFilter< TInputImage, TOutputImage, TKernel >
::GenerateData()
{
  // Call a method that can be overriden by a subclass to allocate
  // memory for the filter's outputs
  this->AllocateOutputs();

  // Call a method that can be overridden by a subclass to perform
  // some calculations prior to splitting the main computations into
  // separate threads
  this->BeforeThreadedGenerateData();

  // Set up the multithreaded processing
  typename ImageSource<TOutputImage>::ThreadStruct str;
  str.Filter = this;

  this->GetMultiThreader()->SetNumberOfThreads( this->GetNumberOfThreads() );
  this->GetMultiThreader()->SetSingleMethod(this->ThreaderCallback, &str);

  // multithread the execution in each direction
  for ( unsigned int d = 0; d < TOutputImage::ImageDimension; ++d )
    {
    this->m_Direction = d;
    SizeValueType r = this->m_Kernel.GetRadius( d );

    if ( d == 0 || r > 0 )
      {
      this->GetMultiThreader()->SingleMethodExecute();
      }
    }

  // Call a method that can be overridden by a subclass to perform
  // some calculations after all the threads have completed
  this->AfterThreadedGenerateData();
}

template< class TInputImage, class TOutputImage, class TKernel >
void
SeparableBinaryMorphologyImageFilter< TInputImage, TOutputImage, TKernel >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread, ThreadIdType threadId)
{
  typedef ImageLinearConstIteratorWithIndex< TOutputImage > InputConstIteratorType;
  typedef ImageLinearIteratorWithIndex< TOutputImage >     OutputIteratorType;

  typename TOutputImage::Pointer     outputImage( this->GetOutput() );

  OutputImageRegionType region = outputRegionForThread;

  // the first time through we may need to copy the input image to the output
  if ( this->m_Direction == 0 && !this->GetRunningInPlace() )
    {
    typename TInputImage::ConstPointer inputImage( this->GetInput() );
    ImageAlgorithm::Copy( inputImage.GetPointer(), outputImage.GetPointer(),
                          outputRegionForThread, outputRegionForThread );
    }


  InputConstIteratorType inputIterator(outputImage,  region);
  OutputIteratorType     outputIterator(outputImage, region);

  inputIterator.SetDirection(this->m_Direction);
  outputIterator.SetDirection(this->m_Direction);

  const unsigned int ln = region.GetSize()[this->m_Direction];

  const SizeValueType radius = this->m_Kernel.GetRadius( this->m_Direction );

  OutputPixelType *outs = 0;

  try
    {
    outs = new OutputPixelType[ln];

    inputIterator.GoToBegin();
    outputIterator.GoToBegin();

    const unsigned int numberOfLinesToProcess = outputRegionForThread.GetNumberOfPixels() / outputRegionForThread.GetSize(this->m_Direction) + 1;
    const float currentProgress = float( this->m_Direction ) / TOutputImage::ImageDimension;
    const float progressWeight = 1.0 / TOutputImage::ImageDimension;
    ProgressReporter   progress(this, threadId, numberOfLinesToProcess, 10,
                                currentProgress, progressWeight );

    while ( !inputIterator.IsAtEnd() && !outputIterator.IsAtEnd() )
      {
      unsigned int i = 0;
      while ( !inputIterator.IsAtEndOfLine() )
        {
        outs[i++] = inputIterator.Get();
        ++inputIterator;
        }

      SizeValueType r = radius;
      while( r*2 > 32 )
        {
          this->FilterDataArray(outs, ln, 16 );
          r -= 16;
        }
      this->FilterDataArray(outs, ln, static_cast<unsigned int>( r ) );

      unsigned int j = 0;
      while ( !outputIterator.IsAtEndOfLine() )
        {
        outputIterator.Set( outs[j++] );
        ++outputIterator;
        }

      inputIterator.NextLine();
      outputIterator.NextLine();

      // Although the method name is CompletedPixel(),
      // this is being called after each line is processed
      progress.CompletedPixel();
      }
    }
  catch (...)
    {
    delete[] outs;
    throw;
    }

  delete[] outs;
}




template< class TInputImage, class TOutputImage, class TKernel >
void
SeparableBinaryMorphologyImageFilter< TInputImage, TOutputImage, TKernel >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Foreground Value: "
     << static_cast< typename NumericTraits< InputPixelType >::PrintType >( m_ForegroundValue ) << std::endl;
  os << indent << "Background Value: "
     << static_cast< typename NumericTraits< OutputPixelType >::PrintType >( m_BackgroundValue ) << std::endl;

  os << indent << "BoundaryToForeground: " << m_BoundaryToForeground << std::endl;
  os << indent << "Direction: " << m_Direction << std::endl;
}
} // end namespace itk

#endif
