/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkContextItem.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// .NAME vtkContextTransform - all children of this item are transformed
// by the vtkTransform2D of this item.
//
// .SECTION Description
// This class can be used to transform all child items of this class. The
// default transform is the identity.

#ifndef __vtkContextTransform_h
#define __vtkContextTransform_h

#include "vtkRenderingContext2DModule.h" // For export macro
#include "vtkAbstractContextItem.h"
#include "vtkSmartPointer.h" // Needed for SP ivars.

class vtkTransform2D;

class VTKRENDERINGCONTEXT2D_EXPORT vtkContextTransform : public vtkAbstractContextItem
{
public:
  vtkTypeMacro(vtkContextTransform, vtkAbstractContextItem);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Creates a vtkContextTransform object.
  static vtkContextTransform *New();

  // Description:
  // Perform any updates to the item that may be necessary before rendering.
  // The scene should take care of calling this on all items before their
  // Paint function is invoked.
  virtual void Update();

  // Description:
  // Paint event for the item, called whenever the item needs to be drawn.
  virtual bool Paint(vtkContext2D *painter);

  // Description:
  // Translate the item by the specified amounts dx and dy in the x and y
  // directions.
  virtual void Translate(float dx, float dy);

  // Description:
  // Scale the item by the specified amounts dx and dy in the x and y
  // directions.
  virtual void Scale(float dx, float dy);

  // Description:
  // Rotate the item by the specified angle.
  virtual void Rotate(float angle);

  // Description:
  // Access the vtkTransform2D that controls object transformation.
  virtual vtkTransform2D* GetTransform();

  // Description:
  // Transforms a point to the parent coordinate system.
  virtual vtkVector2f MapToParent(const vtkVector2f& point);

  // Description:
  // Transforms a point from the parent coordinate system.
  virtual vtkVector2f MapFromParent(const vtkVector2f& point);

//BTX
  vtkSetMacro(PanMouseButton, int);
  vtkGetMacro(PanMouseButton, int);

  vtkSetMacro(PanModifier, int);
  vtkGetMacro(PanModifier, int);

  vtkSetMacro(SecondaryPanMouseButton, int);
  vtkGetMacro(SecondaryPanMouseButton, int);

  vtkSetMacro(SecondaryPanModifier, int);
  vtkGetMacro(SecondaryPanModifier, int);

  vtkSetMacro(ZoomMouseButton, int);
  vtkGetMacro(ZoomMouseButton, int);

  vtkSetMacro(ZoomModifier, int);
  vtkGetMacro(ZoomModifier, int);

  vtkSetMacro(SecondaryZoomMouseButton, int);
  vtkGetMacro(SecondaryZoomMouseButton, int);

  vtkSetMacro(SecondaryZoomModifier, int);
  vtkGetMacro(SecondaryZoomModifier, int);

  vtkSetMacro(ZoomOnMouseWheel, bool);
  vtkGetMacro(ZoomOnMouseWheel, bool);
  vtkBooleanMacro(ZoomOnMouseWheel, bool);

  vtkSetMacro(PanYOnMouseWheel, bool);
  vtkGetMacro(PanYOnMouseWheel, bool);
  vtkBooleanMacro(PanYOnMouseWheel, bool);

  // Description:
  // Returns true if the transform is interactive, false otherwise.
  virtual bool Hit(const vtkContextMouseEvent &mouse);

  // Description:
  // Mouse move event. Perform pan or zoom as specified by the mouse bindings.
  virtual bool MouseMoveEvent(const vtkContextMouseEvent &mouse);

  // Description:
  // Mouse wheel event. Perform pan or zoom as specified by mouse bindings.
  virtual bool MouseWheelEvent(const vtkContextMouseEvent &mouse, int delta);
//ETX

//BTX
protected:
  vtkContextTransform();
  ~vtkContextTransform();

  vtkSmartPointer<vtkTransform2D> Transform;

  int PanMouseButton;
  int PanModifier;
  int ZoomMouseButton;
  int ZoomModifier;
  int SecondaryPanMouseButton;
  int SecondaryPanModifier;
  int SecondaryZoomMouseButton;
  int SecondaryZoomModifier;

  bool ZoomOnMouseWheel;
  bool PanYOnMouseWheel;

private:
  vtkContextTransform(const vtkContextTransform &); // Not implemented.
  void operator=(const vtkContextTransform &);   // Not implemented.
//ETX
};

#endif //__vtkContextTransform_h
