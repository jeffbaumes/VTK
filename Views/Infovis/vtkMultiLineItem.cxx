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
#include "vtkMultiLineItem.h"

#include "vtkContext2D.h"
#include "vtkPen.h"

#include <vector>

vtkStandardNewMacro(vtkMultiLineItem);

struct vtkMultiLineItem::Internals {
  std::vector<std::vector<vtkVector2f> > Positions;
  std::vector<std::vector<vtkColor4ub> > Colors;
  std::vector<float> Widths;
};

vtkMultiLineItem::vtkMultiLineItem()
{
  this->Internal = new Internals();
}

vtkMultiLineItem::~vtkMultiLineItem()
{
  delete this->Internal;
}

vtkIdType vtkMultiLineItem::NumberOfPoints(vtkIdType line)
{
  return 2;
}

vtkColor4ub vtkMultiLineItem::Color(vtkIdType line, vtkIdType point)
{
  return vtkColor4ub(0, 0, 0, 255);
}

vtkVector2f vtkMultiLineItem::Position(vtkIdType line, vtkIdType point)
{
  return vtkVector2f(0.0f, 0.0f);
}

float vtkMultiLineItem::Width(vtkIdType line, vtkIdType point)
{
  return 0.0f;
}

void vtkMultiLineItem::RebuildBuffers()
{
  vtkIdType numItems = this->NumberOfItems();
  this->Internal->Positions = std::vector<std::vector<vtkVector2f> >(numItems);
  this->Internal->Colors = std::vector<std::vector<vtkColor4ub> >(numItems);
  this->Internal->Widths = std::vector<float>(numItems);
  for (vtkIdType line = 0; line < numItems; ++line)
    {
    vtkIdType numPoints = this->NumberOfPoints(line);
    this->Internal->Positions[line] = std::vector<vtkVector2f>(numPoints);
    this->Internal->Colors[line] = std::vector<vtkColor4ub>(numPoints);
    this->Internal->Widths[line] = this->Width(line, 0);
    for (vtkIdType point = 0; point < numPoints; ++point)
      {
      this->Internal->Positions[line][point] = this->Position(line, point);
      this->Internal->Colors[line][point] = this->Color(line, point);
      }
    }
}

void vtkMultiLineItem::PaintBuffers(vtkContext2D *painter)
{
  vtkIdType numItems = this->Internal->Positions.size();
  for (vtkIdType line = 0; line < numItems; ++line)
    {
    if (this->Internal->Positions[line].empty())
      {
      continue;
      }
    painter->GetPen()->SetWidth(this->Internal->Widths[line]);
    painter->DrawPoly(this->Internal->Positions[line][0].GetData(),
                      this->Internal->Positions[line].size(),
                      this->Internal->Colors[line][0].GetData(), 4);
    }
}

void vtkMultiLineItem::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
