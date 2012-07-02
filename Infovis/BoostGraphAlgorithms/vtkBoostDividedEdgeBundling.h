/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkBoostDividedEdgeBundling.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*-------------------------------------------------------------------------
  Copyright 2008 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------*/
// .NAME vtkBoostDividedEdgeBundling - layout graph edges in directed edge bundles
//
// .SECTION Description

// .SECTION Thanks
// This algorithm was developed in the paper

#ifndef __vtkBoostDividedEdgeBundling_h
#define __vtkBoostDividedEdgeBundling_h

#include "vtkInfovisBoostGraphAlgorithmsModule.h" // For export macro
#include "vtkDirectedGraphAlgorithm.h"

class VTKINFOVISBOOSTGRAPHALGORITHMS_EXPORT vtkBoostDividedEdgeBundling : public vtkDirectedGraphAlgorithm
{
public:
  static vtkBoostDividedEdgeBundling *New();

  vtkTypeMacro(vtkBoostDividedEdgeBundling,vtkDirectedGraphAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

protected:
  vtkBoostDividedEdgeBundling();
  ~vtkBoostDividedEdgeBundling() {}

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
  vtkBoostDividedEdgeBundling(const vtkBoostDividedEdgeBundling&);  // Not implemented.
  void operator=(const vtkBoostDividedEdgeBundling&);  // Not implemented.
};

#endif
