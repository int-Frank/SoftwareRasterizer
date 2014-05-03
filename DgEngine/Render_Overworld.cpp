#include "Overworld.h"
#include "MasterPList.h"
#include "Rasterizer.h"
#include "WindowManager.h"

//--------------------------------------------------------------------------------
//		Render
//--------------------------------------------------------------------------------
void Overworld::Render()
{
	Viewport* vp = ViewportHandler::GetViewport(0);
	if (vp == NULL)
		return;

	fps.Draw(*vp);

	if (helpBox.IsActive())
		vp->DrawMessageBox(helpBox);

}	//End: Overworld::Render()