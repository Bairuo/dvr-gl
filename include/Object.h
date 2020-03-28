#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <boost/shared_ptr.hpp>

class Transform;
class Renderer;
class Shader;
class GraphNode;

class Object
{
public:
    int id;
    bool dirty = false;     // If true, object need to be updated in Spatial Data Structure
    GraphNode *graphNode = nullptr;

    // Components
    // Can change to array
    // RigidBody *rigidbody[MAXSIZE]
    boost::shared_ptr<Transform> transform;
    boost::shared_ptr<Renderer> renderer;

    Object(int id, Transform *transform, Renderer *renderer);
    ~Object();

    void update();

    void render();

    void render(Shader &shader);

    void setParent(boost::shared_ptr<Object> obj);

    void addSubObject(boost::shared_ptr<Object> obj);

    // ...
};

extern std::map<int, boost::shared_ptr<Object> > Objects;

boost::shared_ptr<Object> generateObject(
            Transform *transform,
            Renderer *renderer = nullptr);

#endif // OBJECT_H
