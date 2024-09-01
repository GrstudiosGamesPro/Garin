#include "../includes/GMaterial.h"

void GMaterial::defines()
{
    GVAR(VertexPath, "assets/shaders/mesh_vertex.glsl", std::string);
    GVAR(FragmentPath, "assets/shaders/mesh_fragment.glsl", std::string);
    GVAR(SlabPath, "assets/shaders/Standard.slab", std::string);

    // GVAR(VertexPathGbuffer, "assets/shaders/g_buffer_vertex.glsl", std::string);
    // GVAR(FragmentPathGbuffer, "assets/shaders/g_buffer_fragment.glsl", std::string);

    // GVAR(VertexPathDeferred, "assets/shaders/deferred_shading_vertex.glsl", std::string);
    // GVAR(FragmentPathDeferred, "assets/shaders/deferred_shading_fragment.glsl", std::string);
}

void GMaterial::init()
{
    CustomShader shader = CustomShaderParser::ParseShaderFile(FileManager::get_project_path() + GETVAR(SlabPath, std::string));
    shader.SaveToVariables();
    // std::string vertexpath = FileManager::get_project_path() + GETVAR(VertexPath, std::string);
    // std::string fragpath = FileManager::get_project_path() + GETVAR(FragmentPath, std::string);
    //p_shader = new Shader(vertexpath.c_str(), fragpath.c_str());

    p_shader = new Shader(shader.VERTEX, shader.FRAGMENT);

    std::cout << "Compiled VERTEX: \n" << shader.VERTEX << std::endl << std::endl; 
    std::cout << "Compiled FRAGMENT: \n" << shader.FRAGMENT << std::endl << std::endl; 

    // p_gbuffer_shader = new Shader(GETVAR(VertexPathGbuffer, std::string).c_str(), GETVAR(FragmentPathGbuffer, std::string).c_str());
    // p_deferred_shader = new Shader(GETVAR(VertexPathDeferred, std::string).c_str(), GETVAR(FragmentPathDeferred, std::string).c_str());

    // p_deferred_shader->use();
    // p_deferred_shader->setInt("gPosition", 0);
    // p_deferred_shader->setInt("gNormal", 1);
    // p_deferred_shader->setInt("gAlbedoSpec", 2);
}

void GMaterial::update()
{
}

void GMaterial::clean()
{
    // if (p_shader)
    // {
    //     delete p_shader;
    //     p_shader = nullptr;
    // }

    // if (p_deferred_shader)
    // {
    //     delete p_deferred_shader;
    //     p_deferred_shader = nullptr;
    // }

    // if (p_gbuffer_shader)
    // {
    //     delete p_gbuffer_shader;
    //     p_gbuffer_shader = nullptr;
    // }
}