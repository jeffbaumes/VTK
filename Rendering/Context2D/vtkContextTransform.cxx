/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkContextTransform.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "vtkContextTransform.h"
#include "vtkObjectFactory.h"
#include "vtkContextScenePrivate.h"
#include "vtkContext2D.h"
#include "vtkContextMouseEvent.h"
#include "vtkTransform2D.h"
#include "vtkVector.h"

vtkStandardNewMacro(vtkContextTransform);

//-----------------------------------------------------------------------------
vtkContextTransform::vtkContextTransform()
{
  this->Transform = vtkSmartPointer<vtkTransform2D>::New();
  this->PanMouseButton = vtkContextMouseEvent::LEFT_BUTTON;
  this->PanModifier = vtkContextMouseEvent::NO_MODIFIER;
  this->ZoomMouseButton = vtkContextMouseEvent::RIGHT_BUTTON;
  this->ZoomModifier = vtkContextMouseEvent::NO_MODIFIER;
  this->SecondaryPanMouseButton = vtkContextMouseEvent::NO_BUTTON;
  this->SecondaryPanModifier = vtkContextMouseEvent::NO_MODIFIER;
  this->SecondaryZoomMouseButton = vtkContextMouseEvent::LEFT_BUTTON;
  this->SecondaryZoomModifier = vtkContextMouseEvent::SHIFT_MODIFIER;

  this->ZoomOnMouseWheel = true;
  this->PanYOnMouseWheel = false;
}

//-----------------------------------------------------------------------------
vtkContextTransform::~vtkContextTransform()
{
}

//-----------------------------------------------------------------------------
bool vtkContextTransform::Paint(vtkContext2D *painter)
{
  painter->PushMatrix();
  painter->AppendTransform(this->Transform);
  bool result = this->PaintChildren(painter);
  painter->PopMatrix();
  return result;
}

//-----------------------------------------------------------------------------
void vtkContextTransform::Update()
{
}

//-----------------------------------------------------------------------------
void vtkContextTransform::Translate(float dx, float dy)
{
  float d[] = { dx, dy };
  this->Transform->Translate(d);
}

//-----------------------------------------------------------------------------
void vtkContextTransform::Scale(float dx, float dy)
{
  float d[] = { dx, dy };
  this->Transform->Scale(d);
}

//-----------------------------------------------------------------------------
void vtkContextTransform::Rotate(float angle)
{
  this->Transform->Rotate(double(angle));
}

//-----------------------------------------------------------------------------
vtkTransform2D* vtkContextTransform::GetTransform()
{
  return this->Transform;
}

//-----------------------------------------------------------------------------
vtkVector2f vtkContextTransform::MapToParent(const vtkVector2f& point)
{
  vtkVector2f p;
  this->Transform->TransformPoints(point.GetData(), p.GetData(), 1);
  return p;
}

//-----------------------------------------------------------------------------
vtkVector2f vtkContextTransform::MapFromParent(const vtkVector2f& point)
{
  vtkVector2f p;
  this->Transform->InverseTransformPoints(point.GetData(), p.GetData(), 1);
  return p;
}

//-----------------------------------------------------------------------------
bool vtkContextTransform::Hit(const vtkContextMouseEvent &mouse)
{
}

//-----------------------------------------------------------------------------
bool vtkContextTransform::MouseMoveEvent(const vtkContextMouseEvent &mouse)
{

}

//-----------------------------------------------------------------------------
bool vtkContextTransform::MouseWheelEvent(const vtkContextMouseEvent &mouse, int delta)
{

}

//-----------------------------------------------------------------------------
void vtkContextTransform::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
