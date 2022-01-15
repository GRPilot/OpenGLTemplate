#include "incs.hpp"
#include "logutilites.hpp"
#include "window.hpp"
#include "mash.hpp"
#include "templates.hpp"
#include "vertex.hpp"
#include "shader.hpp"
#include "texturegen.hpp"
#include "texture.hpp"

void onGlfwError(int error, const char *descr) {
    LOGE << "[GLFW] Code: " << error << ", Description: " << descr;
}

int main() {
    Utilites::LogHelper::Instance()->Initialize(plog::debug);
    glfwSetErrorCallback(onGlfwError);
    if(!glfwInit()) {
        return 1;
    }

    Window::Ref window{ new Window{ {1024, 600}, "OpenGLRem" } }; // windowed screen
    // Window::Ref window{ new Window{ glfwGetPrimaryMonitor(), "OpenGLRem" } }; // fullscreen
    if(!window->isActive()) {
        return 2;
    }
    const float scaleCoef{ window->height() / static_cast<float>(window->width()) };
    LOGI << "[main] scaleCoef coef: " << scaleCoef;

    if(auto rc{ glewInit() }; GL_NO_ERROR != rc) {
        LOGE << "[GLEW] Cannot initialize GLEW: " << rc;
        return 3;
    }

    std::shared_ptr<Shader> shader{ new Shader {
        "resources/shaders/vs.glsl",
        "resources/shaders/fs.glsl"
    } };

    if(!shader->Valide()) {
        auto error{ shader->GetLastError() };
        LOGE << "[main] Cannot create shader: " << error->what;
        return error->code;
    }

    TemplateGenerator::Template triangleTemplate {
        TemplateGenerator::Generate(TemplateType::SQUARE, 5)
    };
    Mash triangle(triangleTemplate.first, triangleTemplate.second, shader);

    shader->Use();
    std::vector<Texture::Ref> textures {
        TextureGenerator::Gen("resources/textures/texture_0.jpeg", shader),
        TextureGenerator::Gen("resources/textures/texture_1.png", shader)
    };

    glm::vec4 bgcolor{.3f, .2f, .4f, 1.0f};
    float mixValue{ 0.8f };
    int texId{ 0 };
    float angle{ 0.0f };
    float xAngle{ 0.0f };
    float yAngle{ 0.0f };

    while(window->isActive()) {
        window->poll_events();
        window->clear(bgcolor);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame(); 

        shader->Use();
        glm::mat4 transformation{ 1.0f };
        transformation = glm::rotate(transformation, angle, glm::vec3{0, 0, 1});
        transformation = glm::rotate(transformation, xAngle, glm::vec3{1, 0, 0});
        transformation = glm::rotate(transformation, yAngle, glm::vec3{0, 1, 0});
        transformation = glm::scale(transformation, glm::vec3{scaleCoef, 1, 0});
        shader->Set("transform", transformation);
        shader->Set("mix_value", mixValue);
        shader->Set("tex_id", texId);
        shader->UnUse();

        textures[texId]->Bind();
        triangle.Bind();
        triangle.Draw();
        triangle.Unbind();
        textures[texId]->Unbind();

        ImGui::Begin("Settings");
            ImGui::TextWrapped("Shader settings:");
            ImGui::SliderFloat("Texture mix value", &mixValue, 0.0f, 1.0f);
            ImGui::SliderAngle("Rotation", &angle);
            ImGui::SliderAngle("X", &xAngle);
            ImGui::SliderAngle("Y", &yAngle);
            ImGui::Separator();
            ImGui::TextWrapped("Global settings:");
            ImGui::ColorEdit3("Clear color", &bgcolor[0]);
            ImGui::Separator();
            ImGui::TextWrapped("Information:");
            ImGui::TextWrapped("Application average %.3f ms/frame (%.1f FPS)",
                               1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Begin("Textures");
            for(size_t i = 0; i < textures.size(); ++i) {
                auto id{ reinterpret_cast<ImTextureID>(textures[i]->id()) };
                if(ImGui::ImageButton(id, ImVec2{128, 128}, ImVec2{0, 1}, ImVec2{1, 0})) {
                    LOGI << "[Main] Update texture id: " << i;
                    texId = i;
                }
            }
        ImGui::End();

        Utilites::LogHelper::Instance()->Visualizer()->Draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window->update();
    }
}
