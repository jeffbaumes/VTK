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
#ifndef __vtkMultiItem_h
#define __vtkMultiItem_h

#include "vtkViewsInfovisModule.h"
#include "vtkContextItem.h"

class VTKVIEWSINFOVIS_EXPORT vtkMultiItem : public vtkContextItem
{
public:
  static vtkMultiItem *New();
  vtkTypeMacro(vtkMultiItem, vtkContextItem);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

protected:
  vtkMultiItem();
  ~vtkMultiItem();

  virtual bool IsDirty();

  virtual vtkIdType NumberOfItems();

  virtual bool Paint(vtkContext2D *painter);

  virtual void RebuildBuffers();

  virtual void PaintBuffers(vtkContext2D *painter);

private:
  vtkMultiItem(const vtkMultiItem&); // Not implemented
  void operator=(const vtkMultiItem&); // Not implemented
};

#endif
