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
#include "vtkGraphItem.h"

#include "vtkBrush.h"
#include "vtkContext2D.h"
#include "vtkGraph.h"
#include "vtkPen.h"
#include "vtkPoints.h"
#include "vtkVector.h"

vtkStandardNewMacro(vtkGraphItem);

vtkCxxSetObjectMacro(vtkGraphItem, Graph, vtkGraph);
vtkCxxSetObjectMacro(vtkGraphItem, VertexPen, vtkPen);
vtkCxxSetObjectMacro(vtkGraphItem, VertexBrush, vtkBrush);
vtkCxxSetObjectMacro(vtkGraphItem, EdgePen, vtkPen);

vtkGraphItem::vtkGraphItem()
{
  this->Graph = 0;
  this->VertexPen = vtkPen::New();
  this->VertexBrush = vtkBrush::New();
  this->VertexSize = 8.0f;
  this->EdgePen = vtkPen::New();
}

vtkGraphItem::~vtkGraphItem()
{
  if (this->Graph)
    {
    this->Graph->Delete();
    }
  if (this->VertexPen)
    {
    this->VertexPen->Delete();
    }
  if (this->VertexBrush)
    {
    this->VertexBrush->Delete();
    }
  if (this->EdgePen)
    {
    this->EdgePen->Delete();
    }
}

bool vtkGraphItem::Paint(vtkContext2D *painter)
{
  if (this->EdgePen)
    {
    painter->ApplyPen(this->EdgePen);
    }
  for (vtkIdType e = 0; e < this->Graph->GetNumberOfEdges(); ++e)
    {
    this->PaintEdge(painter, e);
    }

  if (this->VertexPen)
    {
    painter->ApplyPen(this->VertexPen);
    }
  if (this->VertexBrush)
    {
    painter->ApplyBrush(this->VertexBrush);
    }
  for (vtkIdType n = 0; n < this->Graph->GetNumberOfVertices(); ++n)
    {
    double *p = this->Graph->GetPoint(n);
    vtkVector2f v(static_cast<float>(p[0]), static_cast<float>(p[1]));
    this->PaintVertex(painter, n, v);
    }

  return true;
}
void vtkGraphItem::PaintEdge(vtkContext2D *painter, vtkIdType edge)
{
  vtkIdType numEdgePoints = this->Graph->GetNumberOfEdgePoints(edge);
  for (vtkIdType m = 0; m < numEdgePoints + 1; ++m)
    {
    double *p1;
    double *p2;
    if (m == 0)
      {
      p1 = this->Graph->GetPoint(this->Graph->GetSourceVertex(edge));
      }
    else
      {
      p1 = this->Graph->GetEdgePoint(edge, m-1);
      }
    if (m == numEdgePoints)
      {
      p2 = this->Graph->GetPoint(this->Graph->GetTargetVertex(edge));
      }
    else
      {
      p2 = this->Graph->GetEdgePoint(edge, m);
      }
    vtkVector2f v1(static_cast<float>(p1[0]), static_cast<float>(p1[1]));
    vtkVector2f v2(static_cast<float>(p2[0]), static_cast<float>(p2[1]));
    float startFraction = 0.0f;
    float endFraction = 1.0f;
    if (numEdgePoints > 0)
      {
      startFraction = static_cast<float>(m) / (numEdgePoints + 1);
      endFraction = static_cast<float>(m+1) / (numEdgePoints + 1);
      }
    this->PaintEdgeSegment(painter, edge, startFraction, endFraction, v1, v2);
    }
}

void vtkGraphItem::PaintEdgeSegment(vtkContext2D *painter, vtkIdType edge, float startFraction, float endFraction, const vtkVector2f& p1, const vtkVector2f& p2)
{
  painter->DrawLine(p1[0], p1[1], p2[0], p2[1]);
}

void vtkGraphItem::PaintVertex(vtkContext2D *painter, vtkIdType vertex, const vtkVector2f& p)
{
  painter->DrawEllipse(p[0], p[1], this->VertexSize/2.0f, this->VertexSize/2.0f);
}
