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
#include "vtkMultiLineItem.h"
#include "vtkMultiMarkerItem.h"
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
#include "vtkContextInteractorStyle.h"
#include "vtkContextItem.h"
#include "vtkContextActor.h"
#include "vtkContextScene.h"
#include "vtkContextTransform.h"
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
class vtkBundledEdgeItem : public vtkMultiLineItem
{
public:
  static vtkBundledEdgeItem *New();
  vtkTypeMacro(vtkBundledEdgeItem, vtkMultiLineItem);

  virtual void SetGraph(vtkGraph *graph);
  vtkGetObjectMacro(Graph, vtkGraph);

protected:
  vtkBundledEdgeItem();
  ~vtkBundledEdgeItem();

  virtual bool IsDirty();

  virtual vtkIdType NumberOfItems();

  virtual vtkIdType NumberOfPoints(vtkIdType line);

  virtual vtkColor4ub Color(vtkIdType line, vtkIdType point);

  virtual vtkVector2f Position(vtkIdType line, vtkIdType point);

  virtual float Width(vtkIdType line, vtkIdType point);

  vtkGraph *Graph;
  unsigned long GraphBuildTime;
};

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkBundledEdgeItem);
vtkCxxSetObjectMacro(vtkBundledEdgeItem, Graph, vtkGraph);

//----------------------------------------------------------------------------
vtkBundledEdgeItem::vtkBundledEdgeItem()
{
  this->Graph = 0;
  this->GraphBuildTime = 0;
}

//----------------------------------------------------------------------------
vtkBundledEdgeItem::~vtkBundledEdgeItem()
{
  if (this->Graph)
    {
    this->Graph->Delete();
    }
}

//----------------------------------------------------------------------------
bool vtkBundledEdgeItem::IsDirty()
{
  if (!this->Graph)
    {
    return false;
    }
  if (this->Graph->GetMTime() > this->GraphBuildTime)
    {
    this->GraphBuildTime = this->Graph->GetMTime();
    return true;
    }
  return false;
}

//----------------------------------------------------------------------------
vtkIdType vtkBundledEdgeItem::NumberOfItems()
{
  if (!this->Graph)
    {
    return 0;
    }
  return this->Graph->GetNumberOfEdges();
}

//----------------------------------------------------------------------------
vtkIdType vtkBundledEdgeItem::NumberOfPoints(vtkIdType line)
{
  if (!this->Graph)
    {
    return 0;
    }
  return this->Graph->GetNumberOfEdgePoints(line) + 2;
}

//----------------------------------------------------------------------------
vtkColor4ub vtkBundledEdgeItem::Color(vtkIdType line, vtkIdType point)
{
  float fraction = static_cast<float>(point) / (this->NumberOfPoints(line) - 1);
  return vtkColor4ub(fraction*255, 0, 255 - fraction*255, 255/20);
}

//----------------------------------------------------------------------------
vtkVector2f vtkBundledEdgeItem::Position(vtkIdType line, vtkIdType point)
{
  double *p;
  if (point == 0)
    {
    p = this->Graph->GetPoint(this->Graph->GetSourceVertex(line));
    }
  else if (point == this->NumberOfPoints(line) - 1)
    {
    p = this->Graph->GetPoint(this->Graph->GetTargetVertex(line));
    }
  else
    {
    p = this->Graph->GetEdgePoint(line, point - 1);
    }
  return vtkVector2f(static_cast<float>(p[0]), static_cast<float>(p[1]));
}

//----------------------------------------------------------------------------
float vtkBundledEdgeItem::Width(vtkIdType line, vtkIdType point)
{
  return 4.0f;
}

//----------------------------------------------------------------------------
class vtkVertexItem : public vtkMultiMarkerItem
{
public:
  static vtkVertexItem *New();
  vtkTypeMacro(vtkVertexItem, vtkMultiMarkerItem);

  virtual void SetGraph(vtkGraph *graph);
  vtkGetObjectMacro(Graph, vtkGraph);

protected:
  vtkVertexItem();
  ~vtkVertexItem();

  virtual bool IsDirty();

  virtual vtkIdType NumberOfItems();

  virtual vtkVector2f Position(vtkIdType line);

  vtkGraph *Graph;
  unsigned long GraphBuildTime;
};

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkVertexItem);
vtkCxxSetObjectMacro(vtkVertexItem, Graph, vtkGraph);

//----------------------------------------------------------------------------
vtkVertexItem::vtkVertexItem()
{
  this->Graph = 0;
  this->GraphBuildTime = 0;
}

//----------------------------------------------------------------------------
vtkVertexItem::~vtkVertexItem()
{
  if (this->Graph)
    {
    this->Graph->Delete();
    }
}

//----------------------------------------------------------------------------
bool vtkVertexItem::IsDirty()
{
  if (!this->Graph)
    {
    return false;
    }
  if (this->Graph->GetMTime() > this->GraphBuildTime)
    {
    this->GraphBuildTime = this->Graph->GetMTime();
    return true;
    }
  return false;
}

//----------------------------------------------------------------------------
vtkIdType vtkVertexItem::NumberOfItems()
{
  if (!this->Graph)
    {
    return 0;
    }
  return this->Graph->GetNumberOfVertices();
}

//----------------------------------------------------------------------------
vtkVector2f vtkVertexItem::Position(vtkIdType item)
{
  double *p = this->Graph->GetPoint(item);
  return vtkVector2f(static_cast<float>(p[0]), static_cast<float>(p[1]));
}

//----------------------------------------------------------------------------
int TestBoostDividedEdgeBundling(int, char*[])
{
  vtkNew<vtkMutableDirectedGraph> graph;
  vtkNew<vtkBoostDividedEdgeBundling> bundle;

  BuildSampleGraph(graph.GetPointer());
  //BuildGraphMLGraph(graph.GetPointer(), "/Users/jeff/dev/airlines_flipped.graphml");

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

  vtkNew<vtkBundledEdgeItem> edgeItem;
  edgeItem->SetGraph(output);

  vtkNew<vtkVertexItem> vertexItem;
  vertexItem->SetGraph(output);

  vtkNew<vtkContextTransform> trans;
  trans->SetInteractive(true);
  trans->AddItem(edgeItem.GetPointer());
  trans->AddItem(vertexItem.GetPointer());
  actor->GetScene()->AddItem(trans.GetPointer());

  vtkNew<vtkRenderer> renderer;
  renderer->SetBackground(1.0, 1.0, 1.0);

  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->SetSize(1040, 1040);
  renderWindow->AddRenderer(renderer.GetPointer());
  renderer->AddActor(actor.GetPointer());

  vtkNew<vtkContextInteractorStyle> interactorStyle;
  interactorStyle->SetScene(actor->GetScene());

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetInteractorStyle(interactorStyle.GetPointer());
  interactor->SetRenderWindow(renderWindow.GetPointer());
  renderWindow->SetMultiSamples(0);
  interactor->Initialize();
  interactor->Start();

  return EXIT_SUCCESS;

  return 0;
}

