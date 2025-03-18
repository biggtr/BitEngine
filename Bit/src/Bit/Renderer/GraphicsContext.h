#include <GLFW/glfw3.h>
namespace BitEngine
{

class GraphicsContext 
{
  
private:
    GLFWwindow* m_Window;
     
public:
    GraphicsContext(){};
    void Create(GLFWwindow* window);

};
}
