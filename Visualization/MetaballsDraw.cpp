#include "stdafx.h"
#include "MetaballsDraw.h"

#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>


CMetaballsDraw::CMetaballsDraw()
{
}


CMetaballsDraw::~CMetaballsDraw()
{
}

void CMetaballsDraw::DrawFunc()
{
	glm::mat4 mat;
	glm::mat4 trans;
	trans = glm::translate(trans, glm::vec3(0.338f, 1.456f, 10.07f));
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

}
