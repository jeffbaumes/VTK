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
#include "vtkMultiItem.h"

#include "vtkBrush.h"
#include "vtkContext2D.h"
#include "vtkGraph.h"
#include "vtkPen.h"
#include "vtkPoints.h"
#include "vtkTransform2D.h"
#include "vtkVector.h"

vtkStandardNewMacro(vtkMultiItem);

vtkMultiItem::vtkMultiItem()
{
}

vtkMultiItem::~vtkMultiItem()
{
}

vtkIdType vtkMultiItem::NumberOfItems()
{
  return 0;
}

bool vtkMultiItem::IsDirty()
{
  return true;
}

bool vtkMultiItem::Paint(vtkContext2D *painter)
{
  if (this->IsDirty())
    {
    this->RebuildBuffers();
    }
  this->PaintBuffers(painter);
  return true;
}

void vtkMultiItem::RebuildBuffers()
{
}

void vtkMultiItem::PaintBuffers(vtkContext2D *painter)
{
}

void vtkMultiItem::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
