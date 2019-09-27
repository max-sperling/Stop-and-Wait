#pragma once
/************************/
/* Author: Max Sperling */
/************************/

#include <memory>

class IView;
typedef std::shared_ptr<IView> IViewPtr;
class IViewListener;