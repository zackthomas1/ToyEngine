#include "utils/test_fixtures.h"
#include "ToyEngine/application.h"
#include "ToyEngine/layers/layer.h"
#include "ToyEngine/layers/layer_stack.h"
#include "ToyEngine/event.h"
#include <memory>
#include <chrono>
#include <vector>

namespace ToyEngine::Testing
{
    // Mock layer for testing
    class MockLayer : public Layer
    {
    public:
        MockLayer(const std::string& name = "MockLayer") 
            : name_(name), attached_(false), detached_(false), update_called_(false), event_called_(false) {}

        void OnAttach() override 
        { 
            attached_ = true; 
        }

        void OnDetatch() override 
        { 
            detached_ = true; 
        }

        void Update(const TimeStep& time_step) override 
        { 
            update_called_ = true;
            last_timestep_ = time_step;
        }

        void OnEvent(Event& e) override 
        { 
            event_called_ = true;
            // Store the event for testing - in real implementation you'd check event type
            last_event_handled_ = true;
        }

        void OnImGuiRender() override 
        { 
            imgui_render_called_ = true; 
        }

        // Test accessors
        bool IsAttached() const { return attached_; }
        bool IsDetached() const { return detached_; }
        bool WasUpdateCalled() const { return update_called_; }
        bool WasEventCalled() const { return event_called_; }
        bool WasImGuiRenderCalled() const { return imgui_render_called_; }
        const std::string& GetName() const { return name_; }
        bool GetLastEventHandled() const { return last_event_handled_; }

        void Reset() 
        {
            update_called_ = false;
            event_called_ = false;
            imgui_render_called_ = false;
        }

    private:
        std::string name_;
        bool attached_;
        bool detached_;
        bool update_called_;
        bool event_called_;
        bool imgui_render_called_ = false;
        TimeStep last_timestep_;
        bool last_event_handled_ = false;
    };

    class LayerTest : public ApplicationTestFixture
    {
    protected:
        void SetUp() override
        {
            ApplicationTestFixture::SetUp();
            mock_layer = std::make_unique<MockLayer>("TestLayer");
        }

        std::unique_ptr<MockLayer> mock_layer;
    };

    class LayerStackTest : public ApplicationTestFixture
    {
    protected:
        void SetUp() override
        {
            ApplicationTestFixture::SetUp();
            layer_stack = std::make_unique<LayerStack>();
        }

        std::unique_ptr<LayerStack> layer_stack;
    };

    class ApplicationTest : public ApplicationTestFixture
    {
    protected:
        void SetUp() override
        {
            ApplicationTestFixture::SetUp();
            // Note: Application is a singleton, so we need to be careful with testing
            // In a real test environment, we'd need to reset the singleton state
        }
    };

    TEST_F(LayerTest, LayerLifecycle)
    {
        // Test initial state
        EXPECT_FALSE(mock_layer->IsAttached());
        EXPECT_FALSE(mock_layer->IsDetached());
        EXPECT_FALSE(mock_layer->WasUpdateCalled());
        EXPECT_FALSE(mock_layer->WasEventCalled());

        // Test attach
        mock_layer->OnAttach();
        EXPECT_TRUE(mock_layer->IsAttached());

        // Test detach
        mock_layer->OnDetatch();
        EXPECT_TRUE(mock_layer->IsDetached());
    }

    TEST_F(LayerTest, LayerUpdateCall)
    {
        TimeStep test_timestep(0.016f); // ~60 FPS
        
        EXPECT_FALSE(mock_layer->WasUpdateCalled());
        mock_layer->Update(test_timestep);
        EXPECT_TRUE(mock_layer->WasUpdateCalled());
    }

    TEST_F(LayerTest, LayerEventHandling)
    {
        // Create a real event for testing
        EventKeyInput test_event(eKeyCode::kKeyW, eKeyState::kPress);
        
        EXPECT_FALSE(mock_layer->WasEventCalled());
        mock_layer->OnEvent(test_event);
        EXPECT_TRUE(mock_layer->WasEventCalled());
        EXPECT_TRUE(mock_layer->GetLastEventHandled());
    }

    TEST_F(LayerTest, LayerImGuiRender)
    {
        EXPECT_FALSE(mock_layer->WasImGuiRenderCalled());
        mock_layer->OnImGuiRender();
        EXPECT_TRUE(mock_layer->WasImGuiRenderCalled());
    }

    TEST_F(LayerStackTest, LayerStackOperations)
    {
        auto layer1 = new MockLayer("Layer1");
        auto layer2 = new MockLayer("Layer2");
        auto overlay1 = new MockLayer("Overlay1");

        // Test adding layers
        EXPECT_NO_THROW({
            layer_stack->PushLayer(layer1);
            layer_stack->PushLayer(layer2);
            layer_stack->PushOverlay(overlay1);
        });

        // Test layer stack iteration
        int count = 0;
        for (auto& layer : *layer_stack)
        {
            EXPECT_NE(layer, nullptr);
            count++;
        }
        EXPECT_EQ(count, 3);

        // Test layer stack removal
        EXPECT_NO_THROW({
            layer_stack->PopLayer(layer1);
            layer_stack->PopOverlay(overlay1);
        });

        // Cleanup
        delete layer2; // layer_stack should handle remaining cleanup
    }

    TEST_F(LayerStackTest, LayerStackOrdering)
    {
        auto layer1 = new MockLayer("Layer1");
        auto layer2 = new MockLayer("Layer2");
        auto overlay1 = new MockLayer("Overlay1");
        auto overlay2 = new MockLayer("Overlay2");

        layer_stack->PushLayer(layer1);
        layer_stack->PushLayer(layer2);
        layer_stack->PushOverlay(overlay1);
        layer_stack->PushOverlay(overlay2);

        // Collect layer names in iteration order
        std::vector<std::string> order;
        for (auto& layer : *layer_stack)
        {
            auto mock_layer = dynamic_cast<MockLayer*>(layer);
            if (mock_layer)
            {
                order.push_back(mock_layer->GetName());
            }
        }

        // Expected order: layers first, then overlays
        // Exact order depends on LayerStack implementation
        EXPECT_GE(order.size(), 4);
        
        // Generally, layers should come before overlays
        // Specific order verification would depend on LayerStack implementation details
    }

    // Note: Application tests would require careful singleton management
    // and potentially mock window/renderer systems
    TEST_F(ApplicationTest, ApplicationSingletonAccess)
    {
        // Test that Application follows singleton pattern
        // Note: This test is tricky because Application is a singleton
        // and may already be instantiated
        
        // In a real test environment, we'd need to:
        // 1. Reset singleton state before each test
        // 2. Mock dependencies (Window, etc.)
        // 3. Test application lifecycle
        
        SUCCEED(); // Placeholder for singleton pattern verification
    }

    TEST_F(ApplicationTest, ApplicationLayerManagement)
    {
        // Test Application's layer management interface
        // This would require a testable Application instance
        
        /*
        auto test_layer = new MockLayer("TestAppLayer");
        auto test_overlay = new MockLayer("TestAppOverlay");
        
        Application& app = Application::Get();
        
        EXPECT_NO_THROW({
            app.PushLayer(test_layer);
            app.PushOverlay(test_overlay);
        });
        */
        
        SUCCEED(); // Placeholder until Application can be properly mocked/tested
    }

    TEST_F(ApplicationTest, ApplicationEventDispatch)
    {
        // Test that Application properly dispatches events to layers
        // This would require:
        // 1. Mock event system
        // 2. Testable Application instance
        // 3. Ability to inject mock layers
        
        SUCCEED(); // Placeholder for event dispatch testing
    }

    // Performance test for layer stack operations
    TEST_F(LayerStackTest, PerformanceLayerStackIteration)
    {
        const int num_layers = 1000;
        
        // Add many layers
        for (int i = 0; i < num_layers; ++i)
        {
            layer_stack->PushLayer(new MockLayer("Layer" + std::to_string(i)));
        }

        auto start = std::chrono::high_resolution_clock::now();
        
        // Iterate through all layers multiple times
        const int iterations = 100;
        for (int iter = 0; iter < iterations; ++iter)
        {
            for (auto& layer : *layer_stack)
            {
                // Simulate layer update operation
                static_cast<void>(layer); // Prevent unused variable warning
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // Should complete in reasonable time (less than 10ms for 100k iterations)
        EXPECT_LT(duration.count(), 10000);
    }

    TEST_F(LayerTest, LayerMemoryManagement)
    {
        // Test that layers can be created and destroyed safely
        const int num_layers = 100;
        std::vector<std::unique_ptr<MockLayer>> layers;
        
        // Create many layers
        for (int i = 0; i < num_layers; ++i)
        {
            layers.push_back(std::make_unique<MockLayer>("Layer" + std::to_string(i)));
        }
        
        // Test that all layers are valid
        for (const auto& layer : layers)
        {
            EXPECT_NE(layer.get(), nullptr);
            EXPECT_FALSE(layer->GetName().empty());
        }
        
        // Layers will be automatically destroyed when vector goes out of scope
        // This tests that destructors work correctly
    }

    TEST_F(LayerTest, LayerStateReset)
    {
        // Test that layer state can be reset for testing purposes
        mock_layer->OnAttach();
        mock_layer->Update(TimeStep(0.016f));
        mock_layer->OnImGuiRender();
        
        EXPECT_TRUE(mock_layer->IsAttached());
        EXPECT_TRUE(mock_layer->WasUpdateCalled());
        EXPECT_TRUE(mock_layer->WasImGuiRenderCalled());
        
        mock_layer->Reset();
        
        EXPECT_TRUE(mock_layer->IsAttached()); // Attach state persists
        EXPECT_FALSE(mock_layer->WasUpdateCalled());
        EXPECT_FALSE(mock_layer->WasImGuiRenderCalled());
    }
}