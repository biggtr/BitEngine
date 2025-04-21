#include <string>
namespace BitEngine
{
struct ShaderSources
{
    std::string VertexShaderSource;
    std::string FragmentShaderSource;
};

class Shader
{
private:
    unsigned int m_ID;
    
public:
    Shader(std::string path);

    void Bind();

    void UnBind();

private: 
    unsigned int CompileShader(unsigned int shaderType,const std::string& shaderSource);
    ShaderSources ParseShader(const std::string path);


};

}
