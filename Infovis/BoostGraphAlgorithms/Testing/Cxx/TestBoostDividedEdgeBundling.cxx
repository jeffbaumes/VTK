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

#include "vtkBoostDividedEdgeBundling.h"
#include "vtkDataSetAttributes.h"
#include "vtkFloatArray.h"
#include "vtkGraphItem.h"
#include "vtkMutableDirectedGraph.h"
#include "vtkNew.h"
#include "vtkPoints.h"
#include "vtkStringArray.h"
#include "vtkXMLTreeReader.h"
#include "vtkViewTheme.h"

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkContext2D.h"
#include "vtkContextItem.h"
#include "vtkContextActor.h"
#include "vtkContextScene.h"
#include "vtkPen.h"
#include "vtkBrush.h"
#include "vtkTextProperty.h"
#include "vtkStdString.h"
#include "vtkNew.h"

#include "vtkRegressionTestImage.h"

//----------------------------------------------------------------------------
void BuildSampleGraph(vtkMutableDirectedGraph* graph)
{
  vtkNew<vtkPoints> points;

  graph->AddVertex();
  points->InsertNextPoint(-500, 0, 0);
  graph->AddVertex();
  points->InsertNextPoint(-500, 100, 0);
  graph->AddVertex();
  points->InsertNextPoint(-500, 200, 0);
  graph->AddVertex();
  points->InsertNextPoint(-500, 300, 0);
  graph->AddVertex();
  points->InsertNextPoint(500, 0, 0);
  graph->AddVertex();
  points->InsertNextPoint(500, 100, 0);
  graph->AddVertex();
  points->InsertNextPoint(500, 200, 0);
  graph->AddVertex();
  points->InsertNextPoint(500, 300, 0);
  graph->SetPoints(points.GetPointer());

  graph->AddEdge(0, 4);
  graph->AddEdge(0, 5);
  graph->AddEdge(1, 4);
  graph->AddEdge(1, 5);
  graph->AddEdge(2, 6);
  graph->AddEdge(2, 7);
  graph->AddEdge(3, 6);
  graph->AddEdge(3, 7);

  graph->AddEdge(4, 0);
  graph->AddEdge(5, 0);
  graph->AddEdge(6, 0);
}

//----------------------------------------------------------------------------
void BuildGraphMLGraph(vtkMutableDirectedGraph* graph, std::string file)
{
  vtkNew<vtkXMLTreeReader> reader;
  reader->SetFileName(file.c_str());
  reader->ReadCharDataOn();
  reader->Update();
  vtkTree *tree = reader->GetOutput();
  vtkStringArray *keyArr = vtkStringArray::SafeDownCast(
    tree->GetVertexData()->GetAbstractArray("key"));
  vtkStringArray *sourceArr = vtkStringArray::SafeDownCast(
    tree->GetVertexData()->GetAbstractArray("source"));
  vtkStringArray *targetArr = vtkStringArray::SafeDownCast(
    tree->GetVertexData()->GetAbstractArray("target"));
  vtkStringArray *contentArr = vtkStringArray::SafeDownCast(
    tree->GetVertexData()->GetAbstractArray(".chardata"));
  double x = 0.0;
  double y = 0.0;
  vtkIdType source = 0;
  vtkIdType target = 0;
  vtkNew<vtkPoints> points;
  graph->SetPoints(points.GetPointer());
  for (vtkIdType i = 0; i < tree->GetNumberOfVertices(); ++i)
    {
    vtkStdString k = keyArr->GetValue(i);
    if (k == "x")
      {
      x = vtkVariant(contentArr->GetValue(i)).ToDouble();
      }
    if (k == "y")
      {
      y = vtkVariant(contentArr->GetValue(i)).ToDouble();
      graph->AddVertex();
      points->InsertNextPoint(x, y, 0.0);
      }
    vtkStdString s = sourceArr->GetValue(i);
    if (s != "")
      {
      source = vtkVariant(s).ToInt();
      }
    vtkStdString t = targetArr->GetValue(i);
    if (t != "")
      {
      target = vtkVariant(t).ToInt();
      graph->AddEdge(source, target);
      }
    }
}

//----------------------------------------------------------------------------
class vtkBundledGraphItem : public vtkGraphItem
{
public:
  static vtkBundledGraphItem *New();
  vtkTypeMacro(vtkBundledGraphItem, vtkGraphItem);
  virtual void PaintEdgeSegment(
      vtkContext2D *painter,
      vtkIdType edge,
      float startFraction,
      float endFraction,
      const vtkVector2f& p1,
      const vtkVector2f& p2 );
};

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkBundledGraphItem);

//----------------------------------------------------------------------------
void vtkBundledGraphItem::PaintEdgeSegment(
    vtkContext2D *painter,
    vtkIdType edge,
    float startFraction,
    float endFraction,
    const vtkVector2f& p1,
    const vtkVector2f& p2 )
{
  float fraction = (startFraction + endFraction) / 2.0f;
  painter->GetPen()->SetColor(fraction*255, 0, 255 - fraction*255);
  this->Superclass::PaintEdgeSegment(painter, edge,
      startFraction, endFraction, p1, p2);
}

//----------------------------------------------------------------------------
int TestBoostDividedEdgeBundling(int, char*[])
{
  vtkNew<vtkMutableDirectedGraph> graph;
  vtkNew<vtkBoostDividedEdgeBundling> bundle;

  //BuildSampleGraph(graph.GetPointer());
  BuildGraphMLGraph(graph.GetPointer(), "/Users/jeff/dev/vtk/airlines_flipped.graphml");

  bundle->SetInputData(graph.GetPointer());
  bundle->Update();

  vtkDirectedGraph *output = bundle->GetOutput();
  vtkFloatArray *edgeCompat = vtkFloatArray::SafeDownCast(
    output->GetEdgeData()->GetAbstractArray("compatibility"));
  for (vtkIdType e = 0; e < output->GetNumberOfEdges(); ++e)
    {
    std::cerr << e << ": " << edgeCompat->GetValue(e) << std::endl;
    }

  // Set up a 2D chart actor, APIDiagram object andn add them to the renderer
  vtkNew<vtkContextActor> actor;
  vtkNew<vtkBundledGraphItem> item;
  item->GetEdgePen()->SetOpacity(255/20);
  item->GetEdgePen()->SetWidth(4);
  item->SetGraph(output);
  actor->GetScene()->AddItem(item.GetPointer());
  vtkNew<vtkRenderer> renderer;
  renderer->SetBackground(1.0, 1.0, 1.0);
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->SetSize(1040, 1040);
  renderWindow->AddRenderer(renderer.GetPointer());
  renderer->AddActor(actor.GetPointer());

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(renderWindow.GetPointer());
  renderWindow->SetMultiSamples(0);
  interactor->Initialize();
  interactor->Start();
  return EXIT_SUCCESS;

  return 0;
}

