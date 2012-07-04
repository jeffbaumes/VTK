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
#include "vtkMultiMarkerItem.h"

#include "vtkContext2D.h"
#include "vtkImageData.h"
#include "vtkMarkerUtilities.h"
#include "vtkPen.h"

#include <vector>

vtkStandardNewMacro(vtkMultiMarkerItem);

struct vtkMultiMarkerItem::Internals {
  std::vector<float> Sizes;
  std::vector<vtkVector2f> Positions;
  std::vector<vtkColor4ub> Colors;
  std::vector<int> Markers;
};

vtkMultiMarkerItem::vtkMultiMarkerItem()
{
  this->Internal = new Internals();
}

vtkMultiMarkerItem::~vtkMultiMarkerItem()
{
  delete this->Internal;
}

vtkColor4ub vtkMultiMarkerItem::Color(vtkIdType item)
{
  return vtkColor4ub(0, 0, 0, 255);
}

vtkVector2f vtkMultiMarkerItem::Position(vtkIdType item)
{
  return vtkVector2f(0.0f, 0.0f);
}

float vtkMultiMarkerItem::Size(vtkIdType item)
{
  return 100.0f;
}

int vtkMultiMarkerItem::Marker(vtkIdType item)
{
  return vtkMarkerUtilities::CIRCLE;
}

void vtkMultiMarkerItem::RebuildBuffers()
{
  vtkIdType numItems = this->NumberOfItems();
  this->Internal->Positions = std::vector<vtkVector2f>(numItems);
  this->Internal->Colors = std::vector<vtkColor4ub>(numItems);
  this->Internal->Sizes = std::vector<float>(numItems);
  this->Internal->Markers = std::vector<int>(numItems);
  vtkMarkerUtilities::GenerateMarker(this->Sprite.GetPointer(), this->Marker(0), this->Size(0));
  for (vtkIdType line = 0; line < numItems; ++line)
    {
    this->Internal->Positions[line] = this->Position(line);
    this->Internal->Colors[line] = this->Color(line);
    this->Internal->Sizes[line] = this->Size(line);
    this->Internal->Markers[line] = this->Marker(line);
    }
}

void vtkMultiMarkerItem::PaintBuffers(vtkContext2D *painter)
{
  if (this->Internal->Positions.empty())
    {
    return;
    }
  painter->GetPen()->SetWidth(this->Internal->Sizes[0]);
  painter->DrawPointSprites(this->Sprite.GetPointer(),
                            this->Internal->Positions[0].GetData(),
                            this->Internal->Positions.size(),
                            this->Internal->Colors[0].GetData(), 4);
}

void vtkMultiMarkerItem::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
