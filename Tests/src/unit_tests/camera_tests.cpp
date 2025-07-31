#include "utils/test_fixtures.h"
#include "ToyEngine/renderer/camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

namespace ToyEngine::Testing
{
    class CameraTest : public RendererTestFixture
    {
    protected:
        void SetUp() override
        {
            RendererTestFixture::SetUp();
            
            // Default camera properties for testing
            default_props = CameraProps();
        }

        CameraProps default_props;
    };

    TEST_F(CameraTest, DefaultConstructor)
    {
        Camera camera;
        
        // Test that default camera is created with reasonable defaults
        auto view_matrix = camera.GetViewMatrix();
        auto projection_matrix = camera.GetProjectionMatrix();
        
        EXPECT_FALSE(view_matrix == glm::mat4(0.0f));
        EXPECT_FALSE(projection_matrix == glm::mat4(0.0f));
    }

    TEST_F(CameraTest, PropertiesConstructor)
    {
        Camera camera(default_props);
        
        // Test that camera properties are properly set
        EXPECT_EQ(camera.GetPosition(), default_props.position);
        EXPECT_EQ(camera.GetFront(), default_props.front);
        EXPECT_FLOAT_EQ(camera.GetFOV(), default_props.fov);
        EXPECT_FLOAT_EQ(camera.GetNearPlane(), default_props.znear);
        EXPECT_FLOAT_EQ(camera.GetFarPlane(), default_props.zfar);
    }

    TEST_F(CameraTest, ViewMatrixCalculation)
    {
        glm::vec3 position(0.0f, 0.0f, 3.0f);
        glm::vec3 target(0.0f, 0.0f, 0.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        
        default_props.position = position;
        Camera camera(default_props);
        
        auto view_matrix = camera.GetViewMatrix();
        
        // The view matrix should not be zero
        EXPECT_FALSE(view_matrix == glm::mat4(0.0f));
        
        // Test that the view matrix transforms world coordinates correctly
        glm::vec4 world_point(0.0f, 0.0f, 0.0f, 1.0f);
        glm::vec4 view_point = view_matrix * world_point;
        
        // Point at origin should be in front of camera (negative Z in view space)
        EXPECT_LT(view_point.z, 0.0f);
    }

    TEST_F(CameraTest, ProjectionMatrixPerspective)
    {
        default_props.type = eCameraType::kFlyCamera; // Assuming perspective
        default_props.fov = 45.0f;
        default_props.znear = 0.1f;
        default_props.zfar = 100.0f;
        
        Camera camera(default_props);
        camera.SetAspectRatio(16.0f / 9.0f);
        
        auto projection_matrix = camera.GetProjectionMatrix();
        
        // Test that projection matrix is not zero
        EXPECT_FALSE(projection_matrix == glm::mat4(0.0f));
        
        // Test that the projection matrix correctly transforms coordinates
        // Point at near plane should be mapped to near Z in NDC
        glm::vec4 near_point(0.0f, 0.0f, -default_props.znear, 1.0f);
        glm::vec4 projected = projection_matrix * near_point;
        projected /= projected.w; // Perspective divide
        
        // Should be close to -1 in NDC Z
        EXPECT_NEAR(projected.z, -1.0f, 0.1f);
    }

    TEST_F(CameraTest, MovementOperations)
    {
        Camera camera(default_props);
        glm::vec3 initial_position = camera.GetPosition();
        
        // Test forward movement
        camera.ProcessMovement(eCameraMovement::kForward, 1.0f);
        glm::vec3 new_position = camera.GetPosition();
        
        EXPECT_NE(initial_position, new_position);
        
        // Movement should be in the direction of camera front
        glm::vec3 movement = new_position - initial_position;
        glm::vec3 front = camera.GetFront();
        
        // Dot product should be positive (same direction)
        EXPECT_GT(glm::dot(movement, front), 0.0f);
    }

    TEST_F(CameraTest, MouseLookUpdate)
    {
        Camera camera(default_props);
        glm::vec3 initial_front = camera.GetFront();
        
        // Simulate mouse movement
        float x_offset = 10.0f;
        float y_offset = 5.0f;
        
        camera.ProcessMouseMovement(x_offset, y_offset);
        glm::vec3 new_front = camera.GetFront();
        
        // Front vector should have changed
        EXPECT_NE(initial_front, new_front);
        
        // Front vector should still be normalized
        float length = glm::length(new_front);
        EXPECT_NEAR(length, 1.0f, 0.001f);
    }

    TEST_F(CameraTest, FOVZoom)
    {
        Camera camera(default_props);
        float initial_fov = camera.GetFOV();
        
        // Test zoom in (negative scroll)
        camera.ProcessMouseScroll(-1.0f);
        EXPECT_LT(camera.GetFOV(), initial_fov);
        
        // Test zoom out (positive scroll)
        camera.ProcessMouseScroll(2.0f);
        EXPECT_GT(camera.GetFOV(), initial_fov);
        
        // FOV should be clamped to reasonable bounds
        camera.ProcessMouseScroll(-100.0f); // Extreme zoom in
        EXPECT_GE(camera.GetFOV(), 1.0f);
        
        camera.ProcessMouseScroll(200.0f); // Extreme zoom out
        EXPECT_LE(camera.GetFOV(), 180.0f);
    }

    TEST_F(CameraTest, AspectRatioUpdate)
    {
        Camera camera(default_props);
        
        float aspect1 = 16.0f / 9.0f;
        float aspect2 = 4.0f / 3.0f;
        
        camera.SetAspectRatio(aspect1);
        auto proj1 = camera.GetProjectionMatrix();
        
        camera.SetAspectRatio(aspect2);
        auto proj2 = camera.GetProjectionMatrix();
        
        // Projection matrices should be different
        EXPECT_NE(proj1, proj2);
    }

    // Performance test for camera matrix calculations
    TEST_F(CameraTest, PerformanceMatrixCalculations)
    {
        Camera camera(default_props);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Perform many matrix calculations
        const int iterations = 10000;
        for (int i = 0; i < iterations; ++i)
        {
            camera.GetViewMatrix();
            camera.GetProjectionMatrix();
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // Should complete in reasonable time (less than 100ms for 10k iterations)
        EXPECT_LT(duration.count(), 100000);
    }
}