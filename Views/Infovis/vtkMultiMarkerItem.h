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
#ifndef __vtkMultiMarkerItem_h
#define __vtkMultiMarkerItem_h

#include "vtkViewsInfovisModule.h"
#include "vtkMultiItem.h"

#include "vtkColor.h" // For color types in API
#include "vtkNew.h" // For ivars
#include "vtkVector.h" // For vector types in API

class vtkImageData;

class VTKVIEWSINFOVIS_EXPORT vtkMultiMarkerItem : public vtkMultiItem
{
public:
  static vtkMultiMarkerItem *New();
  vtkTypeMacro(vtkMultiMarkerItem, vtkContextItem);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

protected:
  vtkMultiMarkerItem();
  ~vtkMultiMarkerItem();

  virtual float Size(vtkIdType item);
  virtual vtkColor4ub Color(vtkIdType item);
  virtual vtkVector2f Position(vtkIdType item);
  virtual int Marker(vtkIdType item);

  virtual void RebuildBuffers();
  virtual void PaintBuffers(vtkContext2D *painter);

private:
  vtkMultiMarkerItem(const vtkMultiMarkerItem&); // Not implemented
  void operator=(const vtkMultiMarkerItem&); // Not implemented

  vtkNew<vtkImageData> Sprite;

  class Internals;
  Internals *Internal;
};

#endif
