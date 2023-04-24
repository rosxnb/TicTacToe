#include "view_delegate.hpp"
#include "utility.hpp"

ViewDelegate::ViewDelegate(MTL::Device* pDevice)
{
    std::unique_ptr<std::string> pShaderSrc = Utility::read_file("shader/program.metal");
    _pRenderer = new Renderer( pDevice, std::move(pShaderSrc) );
}

ViewDelegate::~ViewDelegate()
{ delete _pRenderer; }

void ViewDelegate::drawInMTKView(MTK::View* pView)
{
    _pRenderer->draw(pView);
}
