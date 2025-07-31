#include "utils/test_fixtures.h"
#include "ToyEngine/core.h"
#include "ToyEngine/enum.h"

namespace ToyEngine::Testing
{
    class BasicSystemTest : public ToyEngineTestBase
    {
    protected:
        void SetUp() override
        {
            ToyEngineTestBase::SetUp();
        }
    };

    TEST_F(BasicSystemTest, CoreTypesAvailable)
    {
        // Test that core type aliases are available
        Vector<int> test_vector;
        test_vector.push_back(1);
        test_vector.push_back(2);
        test_vector.push_back(3);
        
        EXPECT_EQ(test_vector.size(), 3);
        EXPECT_EQ(test_vector[0], 1);
        EXPECT_EQ(test_vector[1], 2);
        EXPECT_EQ(test_vector[2], 3);
    }

    TEST_F(BasicSystemTest, SmartPointerTypes)
    {
        // Test Ref (shared_ptr) type
        auto shared_int = MakeRef<int>(42);
        EXPECT_NE(shared_int, nullptr);
        EXPECT_EQ(*shared_int, 42);
        
        Ref<int> another_ref = shared_int;
        EXPECT_EQ(shared_int.use_count(), 2);
        
        // Test Scope (unique_ptr) type
        auto unique_int = MakeScope<int>(24);
        EXPECT_NE(unique_int, nullptr);
        EXPECT_EQ(*unique_int, 24);
    }

    TEST_F(BasicSystemTest, EnumTypes)
    {
        // Test that enum types are accessible
        eKeyCode escape_key = eKeyCode::kKeyEscape;
        EXPECT_EQ(static_cast<int>(escape_key), 256);
        
        eCameraMovement forward = eCameraMovement::kForward;
        EXPECT_EQ(forward, eCameraMovement::kForward);
        
        eKeyState pressed = eKeyState::kPress;
        EXPECT_EQ(static_cast<int>(pressed), 1);
    }

    TEST_F(BasicSystemTest, CompilationTest)
    {
        // This test mainly ensures that our test setup compiles correctly
        // with all the necessary headers and dependencies
        SUCCEED();
    }
}