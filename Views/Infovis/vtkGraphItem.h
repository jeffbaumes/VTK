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

#include "vtkViewsInfovisModule.h"
#include "vtkContextItem.h"

class vtkBrush;
class vtkGraph;
class vtkPen;

class VTKVIEWSINFOVIS_EXPORT vtkGraphItem : public vtkContextItem
{
public:
  static vtkGraphItem *New();
  vtkTypeMacro(vtkGraphItem, vtkContextItem);

  vtkGetObjectMacro(Graph, vtkGraph);
  virtual void SetGraph(vtkGraph *graph);

  vtkGetObjectMacro(VertexPen, vtkPen);
  virtual void SetVertexPen(vtkPen *vertexPen);

  vtkGetObjectMacro(VertexBrush, vtkBrush);
  virtual void SetVertexBrush(vtkBrush *vertexBrush);

  vtkGetMacro(VertexSize, float);
  vtkSetMacro(VertexSize, float);

  vtkGetObjectMacro(EdgePen, vtkPen);
  virtual void SetEdgePen(vtkPen *edgePen);

  virtual bool Paint(vtkContext2D *painter);

protected:
  vtkGraphItem();
  ~vtkGraphItem();

  virtual void PaintVertex(vtkContext2D *painter, vtkIdType vertex, const vtkVector2f& p);
  virtual void PaintEdge(vtkContext2D *painter, vtkIdType edge);
  virtual void PaintEdgeSegment(vtkContext2D *painter, vtkIdType edge, float startFraction, float endFraction, const vtkVector2f& p1, const vtkVector2f& p2);

  vtkPen *VertexPen;
  vtkBrush *VertexBrush;
  float VertexSize;
  vtkPen *EdgePen;
  vtkGraph *Graph;

private:
  vtkGraphItem(const vtkGraphItem&); // Not implemented
  void operator=(const vtkGraphItem&); // Not implemented
};
