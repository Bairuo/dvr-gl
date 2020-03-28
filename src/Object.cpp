#include "Object.h"
#include "Renderer.h"
#include "Transform.h"
#include "Shader.h"
#include "GraphNode.h"

int ObjectID = 0;

std::map<int, boost::shared_ptr<Object> > Objects;

Object::Object(int id, Transform *transform, Renderer *renderer)
    :id(id)
{
    this->transform.reset(transform);
    this->renderer.reset(renderer);
}

Object::~Object()
{

}

void Object::update()
{
    if(transform.get() != nullptr)
    {
        transform->update();
    }

    // other components
    // ...
}

void Object::render(Shader &shader)
{
    if(renderer.get() != nullptr)
    {
        renderer->render(shader);
    }
}

void Object::render()
{
    if(renderer.get() != nullptr)
    {
        renderer->render();
    }
}

void Object::setParent(boost::shared_ptr<Object> obj)
{
    graphNode->setParentObject(obj.get());
}

void Object::addSubObject(boost::shared_ptr<Object> obj)
{
    obj->graphNode->setParentObject(this);
}

boost::shared_ptr<Object> generateObject(Transform *transform, Renderer *renderer)
{
    boost::shared_ptr<Object> newObj(new Object(ObjectID, transform, renderer));

    if(transform != nullptr)
    {
        newObj->transform->obj = newObj.get();
    }

    if(renderer != nullptr)
    {
        newObj->renderer->obj = newObj.get();
    }

    Objects.insert(std::pair<int, boost::shared_ptr<Object> >(ObjectID, newObj));
    SceneGraph.addSubObject(newObj.get());

    ObjectID++;

    return newObj;
}
