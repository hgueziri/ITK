/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkQuickSelectTest1.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkListSample.h"
#include "itkStatisticsAlgorithm.h"

int itkQuickSelectTest1(int argc, char * argv [] )
{
  if( argc < 2 )
    {
    std::cout << "Missing arguments" << std::endl;
    std::cerr << "Usage: ";
    std::cerr << argv[0] << " inputDataFile" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Statistics Algorithm Test \n \n";
  bool pass = true;

  typedef float MeasurementType;
  const unsigned int Dimension = 1;
  const unsigned int testDimension = 0;

  typedef itk::FixedArray< MeasurementType, Dimension > MeasurementVectorType;

  typedef itk::Statistics::ListSample< MeasurementVectorType > SampleType;
  typedef itk::Statistics::Subsample< SampleType > SubsampleType;

  SampleType::Pointer sample = SampleType::New();

  SubsampleType::Pointer subsample1 = SubsampleType::New();
  SubsampleType::Pointer subsample2 = SubsampleType::New();

  subsample1->SetSample( sample );
  subsample2->SetSample( sample );

  // Sort all the values in subsample1
  itk::Statistics::HeapSort< SubsampleType >( subsample1, testDimension, 0, subsample1->Size());

  //
  // Test the Quick Select Algorithm by asking k-th elements in subsample2 and
  // comparing them to the enetries in the sorted subsample1.
  //
  for( unsigned int kth = 0; kth < subsample2->Size(); kth++)
    {
    MeasurementType kthValue2 = itk::Statistics::QuickSelect< SubsampleType >(
      subsample2, testDimension, 0, subsample2->Size(), kth );

    MeasurementType kthValue1 =
      subsample1->GetMeasurementVectorByIndex( kth )[testDimension];

    if( vnl_math_abs( kthValue1 - kthValue2 ) > vnl_math::eps )
      {
      std::cerr << "Comparison failed for component kth= " << kth << std::endl;
      pass = false;
      }
    }

  if( !pass )
    {
    std::cerr << "Test FAILED " << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Test passed." << std::endl;
  return EXIT_SUCCESS;
}

