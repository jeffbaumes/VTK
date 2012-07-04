/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestDiagram.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef __vtkMultiLineItem_h
#define __vtkMultiLineItem_h

#include "vtkViewsInfovisModule.h"
#include "vtkMultiItem.h"

#include "vtkVector.h" // For vector types in API
#include "vtkColor.h" // For color types in API

class VTKVIEWSINFOVIS_EXPORT vtkMultiLineItem : public vtkMultiItem
{
public:
  static vtkMultiLineItem *New();
  vtkTypeMacro(vtkMultiLineItem, vtkContextItem);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

protected:
  vtkMultiLineItem();
  ~vtkMultiLineItem();

  virtual vtkIdType NumberOfPoints(vtkIdType line);
  virtual float Width(vtkIdType line, vtkIdType point);
  virtual vtkColor4ub Color(vtkIdType line, vtkIdType point);
  virtual vtkVector2f Position(vtkIdType line, vtkIdType point);

  virtual void RebuildBuffers();
  virtual void PaintBuffers(vtkContext2D *painter);

private:
  vtkMultiLineItem(const vtkMultiLineItem&); // Not implemented
  void operator=(const vtkMultiLineItem&); // Not implemented

  struct Internals;
  Internals *Internal;
};

#endif
