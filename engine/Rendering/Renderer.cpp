#include "Renderer.h"

#include <new>

Renderer::Renderer()
{
	renderStack.reserve(512);
	base = renderStack.data();
	end = base;
}

Renderer::~Renderer()
{
}

void Renderer::pushDraw(Mesh* mesh)
{
	//Check if head is pointing to the end of the vector
	if (end == renderStack.end()._Ptr)
	{
		//Do something to resize the renderStack (likely reallocate)
	}
	new(end) RenderCall{mesh};
	end++;
}

void Renderer::render()
{
	RenderCall* current = base;

	//Carry out rendering of the current renderCall


	//Move onto the next RenderCall

	current++;
}

//Reset the render stack
void Renderer::reset()
{
	end = base;
}
