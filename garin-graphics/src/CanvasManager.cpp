#include <CanvasManager.h>

void CanvasManager::init_ui()
{
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f};

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::cout << "SHADER PATH: " << ("assets/shaders/ui_vertex_shader.glsl") << std::endl;

    std::string vertex = "assets/shaders/ui_vertex_shader.glsl";
    std::string fragment = "assets/shaders/ui_fragment_shader.glsl";

    std::string slab_ui = "assets/shaders/ui.slab";

    CustomShader shader = CustomShaderParser::ParseShaderFile(slab_ui);
    shader.SaveToVariables();
    // std::string vertexpath = FileManager::get_project_path() + GETVAR(VertexPath, std::string);
    // std::string fragpath = FileManager::get_project_path() + GETVAR(FragmentPath, std::string);
    // p_shader = new Shader(vertexpath.c_str(), fragpath.c_str());

    shaderpr = new Shader(shader.VERTEX, shader.FRAGMENT);

    std::cout << "** Compiled UI VERTEX: \n"
              << shader.VERTEX << std::endl
              << std::endl;
    std::cout << "** Compiled UI FRAGMENT: \n"
              << shader.FRAGMENT << std::endl
              << std::endl;
}

void CanvasManager::render_ui()
{
    shaderpr->use();
    shaderpr->setMat4("view", SceneManager::GetOpenScene()->main_camera->GetView());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}